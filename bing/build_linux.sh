#!/bin/bash
set -e

echo "=== Building Bing QtLocation Plugin for Linux ==="

# Clean previous build
echo "Cleaning..."
make clean 2>/dev/null || true
rm -f Makefile

# Generate Makefile
echo "Running qmake..."
qmake bing.pro

# Build
echo "Building..."
make -j$(nproc)

# Install
echo "Installing..."
sudo make install

# Verify
PLUGIN_PATH=$(qmake -query QT_INSTALL_PLUGINS)/geoservices/libqtgeoservices_bing.so
if [ -f "$PLUGIN_PATH" ]; then
    echo "✅ Plugin installed successfully:"
    ls -lh "$PLUGIN_PATH"
else
    echo "❌ Plugin installation failed!"
    exit 1
fi

echo "=== Build complete ==="
echo ""
echo "To test the plugin:"
echo "  QT_DEBUG_PLUGINS=1 qmlscene test_bing.qml"
