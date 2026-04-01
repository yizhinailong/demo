from sqlalchemy.orm import Mapped, mapped_column
from models.base import Base


class AlbumModel(Base):
    __tablename__ = "album_model"

    id: Mapped[int] = mapped_column(primary_key=True, autoincrement=True)
    album_id: Mapped[int]
    title: Mapped[str]
    user_id: Mapped[int]
