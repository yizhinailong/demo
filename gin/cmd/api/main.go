package main

import (
	"log/slog"

	"github.com/gin-gonic/gin"

	"github.com/yizhinailong/demo/gin/internal/config"
	"github.com/yizhinailong/demo/gin/internal/server/middleware"

	_ "github.com/yizhinailong/demo/gin/internal/server/handler"

	router "github.com/yizhinailong/demo/gin/internal/server"
)

func main() {
	cfg := config.GetConfig()

	r := gin.New()
	middleware.Use(r)
	router.SetupRoutes(r)

	// slog.Info("Web server listening address http://localhost:" + cfg.Server.Port)

	if err := r.Run(":" + cfg.Server.Port); err != nil {
		slog.Error("Error starting server error " + err.Error())
	}
}
