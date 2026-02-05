#!/bin/bash
set -e

echo "=== Google Maps Plugin Build Script (Linux) ==="

# Clean previous build
echo "Cleaning previous build..."
make clean 2>/dev/null || true
rm -f Makefile .qmake.stash

# Generate Makefile
echo "Generating Makefile..."
qmake google.pro

# Build
echo "Building plugin..."
make -j$(nproc)

# Check if build succeeded
if [ -f "libqtgeoservices_google.so" ]; then
    echo "✅ Build successful!"
    ls -lh libqtgeoservices_google.so
    
    echo ""
    echo "To install:"
    echo "  sudo cp libqtgeoservices_google.so /usr/lib/x86_64-linux-gnu/qt5/plugins/geoservices/"
    echo ""
    echo "To test:"
    echo "  QT_DEBUG_PLUGINS=1 qmlscene preview.qml"
else
    echo "❌ Build failed!"
    exit 1
fi
