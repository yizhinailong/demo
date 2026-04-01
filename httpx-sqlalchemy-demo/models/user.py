from sqlalchemy.orm import Mapped, mapped_column
from models.base import Base


class UserModel(Base):
    __tablename__ = "user_model"

    id: Mapped[int] = mapped_column(primary_key=True, autoincrement=True)
    user_id: Mapped[int]
    name: Mapped[str]
    username: Mapped[str]
    email: Mapped[str]
    phone: Mapped[str]
    website: Mapped[str]
