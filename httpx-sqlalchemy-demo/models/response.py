from sqlalchemy.orm import Mapped, mapped_column
from models.base import Base


class ResponseModel(Base):
    __tablename__ = "response_model"

    id: Mapped[int] = mapped_column(primary_key=True, autoincrement=True)
    response_id: Mapped[int]
    title: Mapped[str]
    body: Mapped[str]
    user_id: Mapped[int]
