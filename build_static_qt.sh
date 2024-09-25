#!/bin/bash

# Exit immediately if a command exits with a non-zero status
set -e

QT_VERSION=5.15.15
QT_DIR=qt-everywhere-src-${QT_VERSION}
JOBS=$(nproc)

# Download Qt source code if not already downloaded
if [ ! -f ${QT_DIR}.tar.xz ]; then
    echo "Downloading Qt ${QT_VERSION} source code..."
    wget https://download.qt.io/archive/qt/5.15/${QT_VERSION}/single/qt-everywhere-opensource-src-${QT_VERSION}.tar.xz -O ${QT_DIR}.tar.xz
fi

# Extract source code
if [ ! -d ${QT_DIR} ]; then
    echo "Extracting Qt source code..."
    tar xf ${QT_DIR}.tar.xz
fi

cd ${QT_DIR}

# Create build directory
mkdir -p build && cd build

# Configure Qt for static build
echo "Configuring Qt for static build..."
../configure -prefix $PWD/qtbase \
    -static \
    -release \
    -opensource \
    -confirm-license \
    -nomake examples -nomake tests \
    -skip qtwebengine

# Build and install Qt
echo "Building Qt..."
make -j${JOBS}
make install

echo "Static Qt build completed."
echo "Qt is installed in: $PWD/qtbase"
