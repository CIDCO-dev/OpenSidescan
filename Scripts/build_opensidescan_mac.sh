#!/bin/bash
# macOS (Apple Silicon) build. Dependency prefixes come from Homebrew keg-only
# formulas (qt@5, opencv@4, eigen@3, libomp) and a source-built mlpack 3.4.2
# (Homebrew only ships header-only mlpack 4, see Scripts/build_mlpack3_mac.sh).
# AppleClang does not search /opt/homebrew by default, so paths are explicit.
# The mlpack 3 include dir comes first so its headers shadow any mlpack 4
# headers present under /opt/homebrew/include; the rpath flag is required
# because libmlpack is linked by bare name and lives outside default paths.

MLPACK3_PREFIX="${MLPACK3_PREFIX:-$HOME/opt/mlpack-3.4.2}"

mkdir -p build/
cd build/
cmake .. -DPROJECT_VERSION=$1 \
	-DCMAKE_PREFIX_PATH="/opt/homebrew/opt/qt@5;/opt/homebrew/opt/opencv@4;/opt/homebrew/opt/libomp;/opt/homebrew/opt/eigen@3" \
	-DCMAKE_CXX_FLAGS="-I$MLPACK3_PREFIX/include -I/opt/homebrew/include" \
	-DCMAKE_EXE_LINKER_FLAGS="-L/opt/homebrew/lib -L$MLPACK3_PREFIX/lib -Wl,-rpath,$MLPACK3_PREFIX/lib"
make -j"$(sysctl -n hw.ncpu)"
