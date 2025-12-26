watchexec \
  --exts cpp,h,cxx,hpp,c,cmake \
  --debounce 500ms \
  --restart \
  --clear \
  --project-origin . \
  --ignore build/** \
  --ignore .git/** \
  --ignore "*.swp" \
  --signal SIGTERM \
  -- bash -c "
    echo '🔄 文件变化，重新构建...' &&
    cmake --build build --config Debug --target all -j 16 &&
    ./build/drogon-demo
  "
