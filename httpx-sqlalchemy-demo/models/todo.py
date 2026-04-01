from sqlalchemy.orm import Mapped, mapped_column
from models.base import Base


class TodoModel(Base):
    __tablename__ = "todo_model"

    id: Mapped[int] = mapped_column(primary_key=True, autoincrement=True)
    todo_id: Mapped[int]
    title: Mapped[str]
    completed: Mapped[bool]
    user_id: Mapped[int]
