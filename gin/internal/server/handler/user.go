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
	Name     string `json:"name"`
	Email    string `json:"email"`
	Database string `json:"database"`
}

type CreateUserResponse struct {
	Status  int    `json:"status"`
	Message string `json:"message"`
	ID      int64  `json:"id"`
}

func (h *UserHandler) CreateUser(c *gin.Context) {
	var resquest CreateUserRequest
	if err := c.ShouldBindJSON(&resquest); err != nil {
		c.JSON(http.StatusBadRequest, CreateUserResponse{
			Status:  http.StatusBadRequest,
			Message: err.Error(),
			ID:      -1,
		})
		return
	}

	input := &service.CreateUserInput{
		Name:     resquest.Name,
		Email:    resquest.Email,
		Database: resquest.Database,
	}

	if user, err := h.userService.CreateUser(c, input); err != nil {
		c.JSON(http.StatusInternalServerError, CreateUserResponse{
			Status:  http.StatusInternalServerError,
			Message: err.Error(),
			ID:      -1,
		})
	} else {
		c.JSON(http.StatusOK, CreateUserResponse{
			Status:  http.StatusOK,
			Message: "user successfully created",
			ID:      user.ID,
		})
	}
}

type GetUserRequest struct {
	ID       int64  `json:"id"`
	Database string `json:"database"`
}

type GetUserResponse struct {
	Status  int         `json:"status"`
	Message string      `json:"message"`
	User    *model.User `json:"user"`
}

func (h *UserHandler) GetUser(c *gin.Context) {
	var resquest GetUserRequest
	if err := c.ShouldBindJSON(&resquest); err != nil {
		c.JSON(http.StatusBadRequest, GetUserResponse{
			Status:  http.StatusBadRequest,
			Message: err.Error(),
			User:    nil,
		})
		return
	}

	input := &service.GetUserInput{
		ID:       resquest.ID,
		Database: resquest.Database,
	}

	if user, err := h.userService.GetUser(c, input); err != nil {
		c.JSON(http.StatusInternalServerError, GetUserResponse{
			Status:  http.StatusInternalServerError,
			Message: err.Error(),
			User:    nil,
		})
	} else {
		c.JSON(http.StatusOK, GetUserResponse{
			Status:  http.StatusOK,
			Message: "user found",
			User:    user,
		})
	}
}
