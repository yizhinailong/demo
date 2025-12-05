package service

import (
	"context"
	"fmt"
	"regexp"
	"sync"

	"github.com/yizhinailong/demo/gin/internal/model"
	"github.com/yizhinailong/demo/gin/internal/repository"
)

// UserServiceInterface defines the interface for user service operations
type UserServiceInterface interface {
	GetUser(ctx context.Context, input *GetUserInput) (*model.User, error)
	CreateUser(ctx context.Context, input *CreateUserInput) (*model.User, error)
}

type UserService struct {
	mysqlRepo    repository.UserRepository
	postgresRepo repository.UserRepository
	cache        sync.Map
}

// CreateUserInput 创建用户输入（与 model 分离）
type CreateUserInput struct {
	Username string
	Email    string
	Database string
}

type GetUserInput struct {
	ID       int64
	Database string
}

func NewUserService() *UserService {
	return &UserService{
		mysqlRepo:    repository.NewUserMySQLRepository(),
		postgresRepo: repository.NewUserPostgresRepository(),
	}
}

func (s *UserService) getUserRepo(dbType string) repository.UserRepository {
	switch dbType {
	case "mysql":
		return s.mysqlRepo
	case "postgres":
		return s.postgresRepo
	default:
		return s.mysqlRepo // default to MySQL
	}
}

// GetUser gets a user from the default database (MySQL)
func (s *UserService) GetUser(ctx context.Context, input *GetUserInput) (*model.User, error) {
	dbType := input.Database
	if dbType == "" {
		dbType = "mysql"
	}

	// 1. 先查缓存
	if cached, ok := s.cache.Load(input.ID); ok {
		return cached.(*model.User), nil
	}

	// 2. 缓存未命中，查数据库
	repo := s.getUserRepo(dbType)
	if repo == nil {
		return nil, fmt.Errorf("database connection not available for %s", dbType)
	}

	user, err := repo.GetByID(ctx, input.ID)
	if err != nil {
		return nil, err
	}

	// 3. 写入缓存
	s.cache.Store(input.ID, user)

	return user, nil
}

func validateEmail(email string) error {
	if email == "" {
		return fmt.Errorf("邮箱不能为空")
	}

	// 使用正则表达式验证邮箱格式
	pattern := `^\w+([-+.]\w+)*@\w+([-.]\w+)*\.\w+([-.]\w+)*$`
	matched, err := regexp.MatchString(pattern, email)
	if err != nil {
		return fmt.Errorf("邮箱验证出错: %v", err)
	}

	if !matched {
		return fmt.Errorf("邮箱格式不正确")
	}

	return nil
}

// CreateUser creates a user in the specified database (from input)
func (s *UserService) CreateUser(ctx context.Context, input *CreateUserInput) (*model.User, error) {
	dbType := input.Database
	if dbType == "" {
		dbType = "mysql"
	}

	// 1. 业务验证：邮箱格式、用户名长度等
	if err := validateEmail(input.Email); err != nil {
		return nil, fmt.Errorf("邮箱验证失败: %w", err)
	}

	if len(input.Username) < 3 {
		return nil, fmt.Errorf("用户名长度至少3个字符")
	}

	// 2. 构造模型
	user := &model.User{
		Username: input.Username,
		Email:    input.Email,
	}

	// 3. 调用 Repository 持久化
	repo := s.getUserRepo(dbType)
	if repo == nil {
		return nil, fmt.Errorf("database connection not available for %s", dbType)
	}

	if err := repo.Create(ctx, user); err != nil {
		return nil, fmt.Errorf("创建用户失败: %w", err)
	}

	// 4. 返回结果（ID 已由 Repository 填充）
	return user, nil
}
