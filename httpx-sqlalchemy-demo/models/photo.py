from sqlalchemy.orm import Mapped, mapped_column
from models.base import Base


class PhotoModel(Base):
    __tablename__ = "photo_model"

    id: Mapped[int] = mapped_column(primary_key=True, autoincrement=True)
    album_id: Mapped[int]
    photo_id: Mapped[int]
    title: Mapped[str]
    url: Mapped[str]
    thumbnail_url: Mapped[str]
