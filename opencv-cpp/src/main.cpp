#include <opencv2/dnn.hpp>
#include <opencv2/opencv.hpp>

import std;

constexpr int INPUT_SIZE = 640;
constexpr float CONF_THRESHOLD = 0.25F;
constexpr float NMS_THRESHOLD = 0.45F;

struct Detection {
    int class_id;
    float confidence;
    cv::Rect bbox;
};

static const std::vector<std::string> COCO_CLASSES = {
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

static auto draw(cv::Mat& img, const std::vector<Detection>& dets) -> void {
    std::ranges::for_each(dets, [&](const Detection& d) {
        if (d.class_id < 0 || d.class_id >= static_cast<int>(COCO_CLASSES.size())) {
            return;
        }
        cv::rectangle(img, d.bbox, { 0, 255, 0 }, 2);
        std::string label = std::format("{}: {:.2f}", COCO_CLASSES[d.class_id], d.confidence);
        int baseline = 0;
        auto sz = cv::getTextSize(label, cv::FONT_HERSHEY_SIMPLEX, 0.6, 1, &baseline);
        int top = std::max(d.bbox.y, sz.height);
        cv::rectangle(
            img,
            { d.bbox.x, top - sz.height },
            { d.bbox.x + sz.width, top + baseline },
            { 0, 255, 0 },
            cv::FILLED
        );
        cv::putText(img, label, { d.bbox.x, top }, cv::FONT_HERSHEY_SIMPLEX, 0.6, { 0, 0, 0 }, 1);
    });
}

auto main() -> int {
    // Load model
    auto net = cv::dnn::readNetFromONNX("data/yolo26n.onnx");
    net.setPreferableBackend(cv::dnn::DNN_BACKEND_OPENCV);
    net.setPreferableTarget(cv::dnn::DNN_TARGET_CPU);

    // Load image
    cv::Mat image = cv::imread("data/bus.jpg");
    if (image.empty()) {
        std::println("Failed to load image.");
        return -1;
    }
    const int orig_w = image.cols;
    const int orig_h = image.rows;

    // Create blob: resize to 640x640, normalize to [0,1], BGR->RGB
    cv::Mat blob = cv::dnn::blobFromImage(image, 1.0 / 255.0, { INPUT_SIZE, INPUT_SIZE }, {}, true, false);
    net.setInput(blob);

    // Forward
    std::vector<cv::Mat> outputs;
    net.forward(outputs, net.getUnconnectedOutLayersNames());

    // YOLOv8/v11 output: [1, 4+num_classes, num_predictions]
    auto& out = outputs[0];
    const int num_channels = out.size[1];
    const int num_preds = out.size[2];
    const int num_classes = num_channels - 4;

    const float x_scale = static_cast<float>(orig_w) / INPUT_SIZE;
    const float y_scale = static_cast<float>(orig_h) / INPUT_SIZE;

    std::vector<cv::Rect> boxes;
    std::vector<float> confs;
    std::vector<int> cls_ids;

    for (int i = 0; i < num_preds; ++i) {
        // Find best class
        float best_score = 0;
        int best_cls = 0;
        for (int c = 0; c < num_classes; ++c) {
            float s = out.at<float>(0, 4 + c, i);
            if (s > best_score) {
                best_score = s;
                best_cls = c;
            }
        }
        if (best_score < CONF_THRESHOLD) {
            continue;
        }

        // cx, cy, w, h -> x, y, w, h (scaled to original image)
        float cx = out.at<float>(0, 0, i);
        float cy = out.at<float>(0, 1, i);
        float w = out.at<float>(0, 2, i);
        float h = out.at<float>(0, 3, i);

        int left = static_cast<int>((cx - (w / 2)) * x_scale);
        int top = static_cast<int>((cy - (h / 2)) * y_scale);
        int width = static_cast<int>(w * x_scale);
        int height = static_cast<int>(h * y_scale);

        left = std::clamp(left, 0, orig_w - 1);
        top = std::clamp(top, 0, orig_h - 1);
        width = std::min(width, orig_w - left);
        height = std::min(height, orig_h - top);

        boxes.emplace_back(left, top, width, height);
        confs.push_back(best_score);
        cls_ids.push_back(best_cls);
    }

    // NMS
    std::vector<int> indices;
    cv::dnn::NMSBoxes(boxes, confs, CONF_THRESHOLD, NMS_THRESHOLD, indices);

    std::vector<Detection> detections;
    detections.reserve(indices.size());
    for (int idx : indices) {
        detections.push_back({ .class_id = cls_ids[idx], .confidence = confs[idx], .bbox = boxes[idx] });
    }

    std::println("Detected {} objects:", detections.size());
    for (const auto& d : detections) {
        std::println(
            "  {} ({:.2f}): [{}, {}, {}, {}]",
            COCO_CLASSES[d.class_id],
            d.confidence,
            d.bbox.x,
            d.bbox.y,
            d.bbox.width,
            d.bbox.height
        );
    }

    draw(image, detections);
    cv::imshow("YOLO Detection", image);
    cv::waitKey(0);

    return 0;
}
