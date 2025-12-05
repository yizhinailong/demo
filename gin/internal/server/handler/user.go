package handler

import (
	"net/http"

	"github.com/gin-gonic/gin"

	"github.com/yizhinailong/demo/gin/internal/model"
	"github.com/yizhinailong/demo/gin/internal/service"

	_ "github.com/go-sql-driver/mysql"

	router "github.com/yizhinailong/demo/gin/internal/server"
)

type UserHandler struct {
	userService service.UserServiceInterface
}

func init() {
	// Initialize repository and service
	userService := service.NewUserService()

	// Register handler with initialized service
	router.Register(&UserHandler{userService: userService})
}

func (h *UserHandler) RegisterRoutes(r *gin.Engine) {
	group := r.Group("/users")
	{
		group.POST("/create", h.CreateUser)
		group.GET("/get", h.GetUser)
	}
}

type CreateUserRequest struct {
	Username string `json:"username"`
	Email    string `json:"email"`
	Database string `json:"database"`
}

func (h *UserHandler) CreateUser(c *gin.Context) {
	var resquest CreateUserRequest
	if err := c.ShouldBindJSON(&resquest); err != nil {
		c.JSON(http.StatusBadRequest, gin.H{"error": err.Error()})
		return
	}

	input := &service.CreateUserInput{
		Username: resquest.Username,
		Email:    resquest.Email,
		Database: resquest.Database,
	}

	if user, err := h.userService.CreateUser(c, input); err != nil {
		c.JSON(http.StatusInternalServerError, gin.H{"error": err.Error()})
	} else {
		c.JSON(http.StatusOK, gin.H{"status": "user created", "userID": user.ID})
	}
}

type GetUserRequest struct {
	ID       int64  `json:"id"`
	Database string `json:"database"`
}

type GetUserResponse struct {
	Message string      `json:"message"`
	User    *model.User `json:"user"`
}

func (h *UserHandler) GetUser(c *gin.Context) {
	var resquest GetUserRequest
	if err := c.ShouldBindJSON(&resquest); err != nil {
		response := GetUserResponse{
			Message: err.Error(),
			User:    nil,
		}
		c.JSON(http.StatusBadRequest, response)
		return
	}

	input := &service.GetUserInput{
		ID:       resquest.ID,
		Database: resquest.Database,
	}

	if user, err := h.userService.GetUser(c, input); err != nil {
		response := GetUserResponse{
			Message: err.Error(),
			User:    nil,
		}
		c.JSON(http.StatusInternalServerError, response)
	} else {
		response := GetUserResponse{
			Message: "user found",
			User:    user,
		}
		c.JSON(http.StatusOK, response)
	}
}
