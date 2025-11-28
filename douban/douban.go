package main

import (
	"encoding/json"
	"fmt"
	"log"
	"net/http"
	"os"
	"strings"
	"time"

	"github.com/PuerkitoBio/goquery"
)

// Movie 定义电影数据结构
type Movie struct {
	Rank     string `json:"排名"`
	Title    string `json:"电影名"`
	Link     string `json:"链接"`
	Rating   string `json:"评分"`
	Evaluate string `json:"评价人数"`
	Quote    string `json:" quote"`
}

func main() {
	start := time.Now()
	var movies []Movie
	ch := make(chan []Movie, 10) // 缓冲channel

	// 并发爬取10页，每页25部电影
	for i := range 10 {
		url := fmt.Sprintf("https://movie.douban.com/top250?start=%d&filter=", i*25)
		go fetchPage(url, ch)
		time.Sleep(300 * time.Millisecond) // 限速，防止被封
	}

	// 收集结果
	for i := range 10 {
		pageMovies := <-ch
		movies = append(movies, pageMovies...)
		fmt.Printf("已收集第 %d 页数据\n", i+1)
	}

	// 保存为JSON文件
	saveToJSON(movies)
	fmt.Printf("✅ 爬取完成！共 %d 部电影，耗时: %v\n", len(movies), time.Since(start))
}

// fetchPage 爬取单页数据
func fetchPage(url string, ch chan<- []Movie) {
	var movies []Movie

	// 创建请求并设置反爬Header
	client := &http.Client{Timeout: 10 * time.Second}
	req, _ := http.NewRequest("GET", url, nil)
	req.Header.Set("User-Agent", "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36")

	resp, err := client.Do(req)
	if err != nil {
		log.Printf("请求失败 %s: %v", url, err)
		ch <- movies
		return
	}
	defer resp.Body.Close()

	// 使用goquery解析HTML
	doc, err := goquery.NewDocumentFromReader(resp.Body)
	if err != nil {
		log.Printf("解析失败: %v", err)
		ch <- movies
		return
	}

	// 提取每部电影信息
	doc.Find("ol.grid_view li").Each(func(i int, s *goquery.Selection) {
		movie := Movie{
			Rank:     s.Find("div.pic em").Text(),
			Title:    strings.TrimSpace(s.Find("div.info span.title").First().Text()),
			Link:     s.Find("div.info a").AttrOr("href", ""),
			Rating:   s.Find("div.star span.rating_num").Text(),
			Evaluate: s.Find("div.star span:last-child").Text(),
			Quote:    strings.TrimSpace(s.Find("p.quote span").Text()),
		}
		movies = append(movies, movie)
	})

	ch <- movies
}

// saveToJSON 保存数据到文件
func saveToJSON(movies []Movie) {
	file, _ := os.Create("out/douban_top250.json")
	defer file.Close()

	encoder := json.NewEncoder(file)
	encoder.SetIndent("", "  ")
	if err := encoder.Encode(movies); err != nil {
		log.Printf("保存失败: %v", err)
	}
}
