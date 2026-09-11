#!/bin/bash
# Build mlpack 3.4.2 from source (the version OpenSidescan links against;
# Homebrew only ships header-only mlpack 4, which has no library to link).
# Patches applied to the pinned 3.4.2 tree:
# - CMake >= 4 compatibility (policy version minimum + bundled cotire minimum)
# - C++17 standard (current Homebrew armadillo requires C++14; mlpack 3.4.2
#   hardcodes CMAKE_CXX_STANDARD 11)
# Requires Homebrew: boost cereal ensmallen armadillo
set -euo pipefail

VERSION=3.4.2
PREFIX="${MLPACK3_PREFIX:-$HOME/opt/mlpack-3.4.2}"
SRC_DIR="${MLPACK3_SRC_DIR:-$HOME/opt/mlpack3-src}"

if [ -f "$PREFIX/lib/libmlpack.dylib" ]; then
	echo "mlpack $VERSION already built at $PREFIX"
	exit 0
fi

mkdir -p "$SRC_DIR"
cd "$SRC_DIR"
if [ ! -d "mlpack-$VERSION" ]; then
	curl -sL -o mlpack.tar.gz "https://github.com/mlpack/mlpack/archive/refs/tags/$VERSION.tar.gz"
	tar xzf mlpack.tar.gz
fi
cd "mlpack-$VERSION"

sed -i '' 's/cmake_minimum_required(VERSION 2\.8\.12)/cmake_minimum_required(VERSION 3.5)/' CMake/cotire.cmake
sed -i '' 's/set(CMAKE_CXX_STANDARD 11)/set(CMAKE_CXX_STANDARD 17)/' CMakeLists.txt

cmake -S . -B build \
	-DCMAKE_POLICY_VERSION_MINIMUM=3.5 \
	-DCMAKE_BUILD_TYPE=Release \
	-DCMAKE_INSTALL_PREFIX="$PREFIX" \
	-DCMAKE_PREFIX_PATH="/opt/homebrew/opt/libomp" \
	-DCMAKE_CXX_FLAGS="-I/opt/homebrew/opt/libomp/include" \
	-DBUILD_CLI_EXECUTABLES=OFF \
	-DBUILD_TESTS=OFF \
	-DBUILD_PYTHON_BINDINGS=OFF \
	-DBUILD_JULIA_BINDINGS=OFF \
	-DBUILD_GO_BINDINGS=OFF \
	-DBUILD_R_BINDINGS=OFF
cmake --build build -j "$(sysctl -n hw.ncpu)"
cmake --install build
