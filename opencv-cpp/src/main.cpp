#include <opencv2/dnn.hpp>
#include <opencv2/opencv.hpp>

import std;

constexpr int   INPUT_SIZE     = 640;   // 模型输入图像尺寸
constexpr float CONF_THRESHOLD = 0.25F; // 置信度阈值：低于此值的检测结果将被过滤
constexpr float NMS_THRESHOLD  = 0.45F; // 非极大值抑制（NMS）阈值：用于去除重叠的检测框

namespace {

    // 检测结果结构体：包含类别ID、置信度和边界框
    struct Detection {
        int      class_id;
        float    confidence;
        cv::Rect bbox;
    };

    // COCO 数据集的 80 个类别名称（编译期常量，零堆分配）
    constexpr auto& coco_classes() {
        static constexpr std::array classes = {
            "person",
            "bicycle",
            "car",
            "motorcycle",
            "airplane",
            "bus",
            "train",
            "truck",
            "boat",
            "traffic light",
            "fire hydrant",
            "stop sign",
            "parking meter",
            "bench",
            "bird",
            "cat",
            "dog",
            "horse",
            "sheep",
            "cow",
            "elephant",
            "bear",
            "zebra",
            "giraffe",
            "backpack",
            "umbrella",
            "handbag",
            "tie",
            "suitcase",
            "frisbee",
            "skis",
            "snowboard",
            "sports ball",
            "kite",
            "baseball bat",
            "baseball glove",
            "skateboard",
            "surfboard",
            "tennis racket",
            "bottle",
            "wine glass",
            "cup",
            "fork",
            "knife",
            "spoon",
            "bowl",
            "banana",
            "apple",
            "sandwich",
            "orange",
            "broccoli",
            "carrot",
            "hot dog",
            "pizza",
            "donut",
            "cake",
            "chair",
            "couch",
            "potted plant",
            "bed",
            "dining table",
            "toilet",
            "tv",
            "laptop",
            "mouse",
            "remote",
            "keyboard",
            "cell phone",
            "microwave",
            "oven",
            "toaster",
            "sink",
            "refrigerator",
            "book",
            "clock",
            "vase",
            "scissors",
            "teddy bear",
            "hair drier",
            "toothbrush"
        };
        return classes;
    }

    // 在图像上绘制检测框和类别标签
    void draw(cv::Mat& img, const std::vector<Detection>& dets) {
        const auto& classes = coco_classes();
        for (const auto& d : dets) {
            if (d.class_id < 0 || d.class_id >= static_cast<int>(classes.size())) {
                continue;
            }
            // 绘制绿色边界框
            cv::rectangle(img, d.bbox, { 0, 255, 0 }, 2);
            // 生成标签文本，格式为 "类别名: 置信度"
            auto label    = std::format("{}: {:.2f}", classes.at(d.class_id), d.confidence);
            int  baseline = 0;
            auto sz       = cv::getTextSize(label, cv::FONT_HERSHEY_SIMPLEX, 0.6, 1, &baseline);
            int  top      = std::max(d.bbox.y, sz.height);
            // 绘制标签背景矩形（填充绿色）
            cv::rectangle(
                img,
                { d.bbox.x, top - sz.height },
                { d.bbox.x + sz.width, top + baseline },
                { 0, 255, 0 },
                cv::FILLED
            );
            // 在背景上绘制黑色标签文字
            cv::putText(img, label, { d.bbox.x, top }, cv::FONT_HERSHEY_SIMPLEX, 0.6, { 0, 0, 0 }, 1);
        }
    }

    // 从 ONNX 文件加载 YOLO 模型，使用 OpenCV 后端在 CPU 上运行
    auto load_model(const std::string& path) -> cv::dnn::Net {
        auto net = cv::dnn::readNetFromONNX(path);
        net.setPreferableBackend(cv::dnn::DNN_BACKEND_OPENCV);
        net.setPreferableTarget(cv::dnn::DNN_TARGET_CPU);
        return net;
    }

    // 执行目标检测：预处理图像、模型推理、后处理（含 NMS）
    auto detect(cv::dnn::Net& net, const cv::Mat& image) -> std::vector<Detection> {
        const int orig_w = image.cols;
        const int orig_h = image.rows;

        // 将图像转换为模型输入格式：缩放到 INPUT_SIZE，归一化到 [0,1]，BGR 转 RGB
        cv::Mat blob     = cv::dnn::blobFromImage(image, 1.0 / 255.0, { INPUT_SIZE, INPUT_SIZE }, {}, true, false);
        net.setInput(blob);

        // 执行前向推理，获取输出层结果
        std::vector<cv::Mat> outputs;
        net.forward(outputs, net.getUnconnectedOutLayersNames());

        // YOLOv8/v11 输出格式: [1, 4+num_classes, num_predictions]
        auto&     out         = outputs[0];
        const int num_preds   = out.size[2];
        const int num_classes = out.size[1] - 4;

        // 计算从模型输入尺寸到原始图像尺寸的缩放比例
        const float x_scale   = static_cast<float>(orig_w) / INPUT_SIZE;
        const float y_scale   = static_cast<float>(orig_h) / INPUT_SIZE;

        std::vector<cv::Rect> boxes;
        std::vector<float>    confs;
        std::vector<int>      cls_ids;

        // 遍历所有预测结果，筛选置信度高于阈值的检测
        for (int i = 0; i < num_preds; ++i) {
            // 找到得分最高的类别
            int   best_cls   = 0;
            float best_score = 0;
            for (int c = 0; c < num_classes; ++c) {
                float s = out.at<float>(0, 4 + c, i);
                if (s > best_score) {
                    best_score = s;
                    best_cls   = c;
                }
            }
            if (best_score < CONF_THRESHOLD) {
                continue;
            }

            // 提取边界框中心点坐标和宽高
            const float cx   = out.at<float>(0, 0, i);
            const float cy   = out.at<float>(0, 1, i);
            const float w    = out.at<float>(0, 2, i);
            const float h    = out.at<float>(0, 3, i);

            // 将中心坐标转换为左上角坐标，并按缩放比例映射回原始图像尺寸
            const int left   = std::clamp(static_cast<int>((cx - (w / 2)) * x_scale), 0, orig_w - 1);
            const int top    = std::clamp(static_cast<int>((cy - (h / 2)) * y_scale), 0, orig_h - 1);
            const int width  = std::min(static_cast<int>(w * x_scale), orig_w - left);
            const int height = std::min(static_cast<int>(h * y_scale), orig_h - top);

            boxes.emplace_back(left, top, width, height);
            confs.push_back(best_score);
            cls_ids.push_back(best_cls);
        }

        // 执行非极大值抑制（NMS），去除重叠的检测框
        std::vector<int> indices;
        cv::dnn::NMSBoxes(boxes, confs, CONF_THRESHOLD, NMS_THRESHOLD, indices);

        // 收集 NMS 后保留的检测结果
        std::vector<Detection> detections;
        detections.reserve(indices.size());
        for (int idx : indices) {
            detections.push_back({ .class_id = cls_ids[idx], .confidence = confs[idx], .bbox = boxes[idx] });
        }
        return detections;
    }

    // 在终端打印检测结果：类别名称、置信度和边界框坐标
    void print_results(const std::vector<Detection>& detections) {
        const auto& classes = coco_classes();
        std::println("Detected {} objects:", detections.size());
        for (const auto& d : detections) {
            std::println(
                "  {} ({:.2f}): [{}, {}, {}, {}]",
                classes.at(d.class_id),
                d.confidence,
                d.bbox.x,
                d.bbox.y,
                d.bbox.width,
                d.bbox.height
            );
        }
    }

} // namespace

int main() {
    // 加载 ONNX 模型
    auto net      = load_model("data/yolo26n.onnx");

    // 读取待检测的图像
    cv::Mat image = cv::imread("data/bus.jpg");
    if (image.empty()) {
        std::println("Failed to load image.");
        return -1;
    }

    // 执行目标检测并打印结果
    auto detections = detect(net, image);
    print_results(detections);

    // 在图像上绘制检测结果并显示窗口
    draw(image, detections);
    cv::imshow("YOLO Detection", image);
    cv::waitKey(0);

    return 0;
}
