import asyncio
import time
import httpx
import sqlalchemy

from typing import Any
from sqlalchemy import insert
from sqlalchemy.orm import Session
from sqlalchemy.ext.asyncio import (
    AsyncEngine,
    AsyncSession,
    create_async_engine,
    async_sessionmaker,
)

from models import (
    Base,
    ResponseModel,
    CommentModel,
    AlbumModel,
    PhotoModel,
    TodoModel,
    UserModel,
)


BASE_URL: str = "https://jsonplaceholder.typicode.com"

# --- 同步引擎 ---
sync_engine: sqlalchemy.Engine = sqlalchemy.create_engine("sqlite:///demo_sync.db", echo=False)

# --- 异步引擎 ---
async_engine: AsyncEngine = create_async_engine("sqlite+aiosqlite:///demo_async.db", echo=False)
async_session: async_sessionmaker[AsyncSession] = async_sessionmaker(
    async_engine, expire_on_commit=False
)

# --- 端点配置 ---
# field_map 只需列出 JSON key → DB 列名 不同的字段，相同名称的自动透传
ENDPOINT_CONFIG: list[dict[str, Any]] = [
    {
        "path": "/posts",
        "model": ResponseModel,
        "field_map": {"id": "response_id", "userId": "user_id"},
    },
    {
        "path": "/comments",
        "model": CommentModel,
        "field_map": {"postId": "post_id", "id": "comment_id"},
    },
    {
        "path": "/albums",
        "model": AlbumModel,
        "field_map": {"id": "album_id", "userId": "user_id"},
    },
    {
        "path": "/photos",
        "model": PhotoModel,
        "field_map": {"albumId": "album_id", "id": "photo_id", "thumbnailUrl": "thumbnail_url"},
    },
    {
        "path": "/todos",
        "model": TodoModel,
        "field_map": {"id": "todo_id", "userId": "user_id"},
    },
    {
        "path": "/users",
        "model": UserModel,
        "field_map": {"id": "user_id"},
    },
]


def remap_rows(
    raw_rows: list[dict[str, Any]],
    field_map: dict[str, str],
) -> list[dict[str, Any]]:
    """将 JSON key 映射为 DB 列名，未在 field_map 中出现的 key 保持原名。"""
    return [
        {field_map.get(k, k): v for k, v in row.items()}
        for row in raw_rows
    ]


# ==================== 同步模式 ====================
def sync_mode() -> float:
    Base.metadata.create_all(sync_engine)

    start: float = time.perf_counter()

    raw_data: dict[str, list[dict[str, Any]]] = {}
    for config in ENDPOINT_CONFIG:
        response: httpx.Response = httpx.get(f"{BASE_URL}{config['path']}", timeout=30)
        raw_data[config["path"]] = response.json()

    with Session(sync_engine) as session:
        for config in ENDPOINT_CONFIG:
            rows = remap_rows(raw_data[config["path"]], config["field_map"])
            if rows:
                session.execute(insert(config["model"]), rows)
        session.commit()

    elapsed: float = time.perf_counter() - start
    print(f"[同步模式] 耗时: {elapsed:.4f} 秒")
    return elapsed


# ==================== 异步模式 ====================
async def async_mode() -> float:
    async with async_engine.begin() as conn:
        await conn.run_sync(Base.metadata.create_all)

    start: float = time.perf_counter()

    async with httpx.AsyncClient(timeout=30) as client:
        responses: list[httpx.Response] = await asyncio.gather(
            *[client.get(f"{BASE_URL}{cfg['path']}") for cfg in ENDPOINT_CONFIG]
        )

    raw_data: list[list[dict[str, Any]]] = [r.json() for r in responses]

    async with async_session() as session:
        for config, raw in zip(ENDPOINT_CONFIG, raw_data):
            rows = remap_rows(raw, config["field_map"])
            if rows:
                await session.execute(insert(config["model"]), rows)
        await session.commit()

    elapsed: float = time.perf_counter() - start
    print(f"[异步模式] 耗时: {elapsed:.4f} 秒")
    return elapsed


async def main() -> None:
    print("=" * 40)
    sync_time: float = sync_mode()
    async_time: float = await async_mode()
    print("=" * 40)
    print(f"差异: 异步比同步快 {sync_time - async_time:.4f} 秒")


if __name__ == "__main__":
    asyncio.run(main())
