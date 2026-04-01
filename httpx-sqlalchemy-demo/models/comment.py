from sqlalchemy.orm import Mapped, mapped_column
from models.base import Base


class CommentModel(Base):
    __tablename__ = "comment_model"

    id: Mapped[int] = mapped_column(primary_key=True, autoincrement=True)
    post_id: Mapped[int]
    comment_id: Mapped[int]
    name: Mapped[str]
    email: Mapped[str]
    body: Mapped[str]
