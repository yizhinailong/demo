package handler

import (
	"net/http"

	"github.com/gin-gonic/gin"

	"github.com/yizhinailong/demo/gin/internal/server/dto"
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

func (h *UserHandler) CreateUser(c *gin.Context) {
	var resquest dto.CreateUserRequest
	if err := c.ShouldBindJSON(&resquest); err != nil {
		c.JSON(http.StatusBadRequest, dto.CreateUserResponse{
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
		c.JSON(http.StatusInternalServerError, dto.CreateUserResponse{
			Status:  http.StatusInternalServerError,
			Message: err.Error(),
			ID:      -1,
		})
	} else {
		c.JSON(http.StatusOK, dto.CreateUserResponse{
			Status:  http.StatusOK,
			Message: "user successfully created",
			ID:      user.ID,
		})
	}
}

func (h *UserHandler) GetUser(c *gin.Context) {
	var resquest dto.GetUserRequest
	if err := c.ShouldBindJSON(&resquest); err != nil {
		c.JSON(http.StatusBadRequest, dto.GetUserResponse{
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
		c.JSON(http.StatusInternalServerError, dto.GetUserResponse{
			Status:  http.StatusInternalServerError,
			Message: err.Error(),
			User:    nil,
		})
	} else {
		c.JSON(http.StatusOK, dto.GetUserResponse{
			Status:  http.StatusOK,
			Message: "user found",
			User:    user,
		})
	}
}
