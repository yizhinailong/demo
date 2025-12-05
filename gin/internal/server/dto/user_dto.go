package dto

import "github.com/yizhinailong/demo/gin/internal/model"

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

type GetUserRequest struct {
	ID       int64  `json:"id"`
	Database string `json:"database"`
}

type GetUserResponse struct {
	Status  int         `json:"status"`
	Message string      `json:"message"`
	User    *model.User `json:"user"`
}
