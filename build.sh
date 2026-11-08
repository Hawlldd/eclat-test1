#!/bin/bash
set -e

SRC_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_DIR=$(cat /var/tmp/eclat-intro-build-path 2>/dev/null || echo "")

if [ -z "$BUILD_DIR" ] || [ ! -d "$BUILD_DIR" ]; then
    echo "Compilando pokeemerald..."
    cd "$SRC_DIR"
    make -j$(nproc) emerald
    cp pokeemerald.gba eclat-intro-teste.gba
    echo "ROM gerada com sucesso em eclat-intro-teste.gba!"
    exit 0
fi

echo "Sincronizando arquivos modificados..."
rsync -rtuv --include='*/' \
  --include='*.json' --include='*.bin' --include='*.c' --include='*.h' \
  --include='*.inc' --include='*.s' --include='*.party' --include='*.png' \
  --include='*.pal' --exclude='*' \
  "$SRC_DIR/data/" "$BUILD_DIR/data/"

rsync -rtuv "$SRC_DIR/src/" "$BUILD_DIR/src/"
rsync -rtuv "$SRC_DIR/include/" "$BUILD_DIR/include/"
rsync -rtuv "$SRC_DIR/asm/" "$BUILD_DIR/asm/"
rsync -rtuv "$SRC_DIR/graphics/" "$BUILD_DIR/graphics/"

cd "$BUILD_DIR"
echo "Compilando pokeemerald..."
make -j$(nproc) emerald

cp pokeemerald.gba "$SRC_DIR/eclat-intro-teste.gba"
echo ""
echo "=== SUCESSO! ==="
echo "ROM atualizada: $SRC_DIR/eclat-intro-teste.gba"
