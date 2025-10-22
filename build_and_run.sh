#!/bin/bash

# 第一引数が指定されていればそれを使う
# 無ければカレントディレクトリ
SRC_DIR="${1:-$(pwd)}"

# 出力ファイル
OUT="$SRC_DIR/sample"

# コンパイル（ワイルドカードはクォートで囲まない）
g++ -o "$OUT" $SRC_DIR/*.cpp
if [ $? -eq 0 ]; then
    echo "Build successful. Running..."
    "$OUT"
else
    echo "Build failed."
fi
