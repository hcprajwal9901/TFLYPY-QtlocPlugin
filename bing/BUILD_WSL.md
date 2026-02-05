# Building Bing Plugin in WSL Ubuntu

## Prerequisites

### 1. Open WSL Ubuntu Terminal

In Windows, open **PowerShell** or **Command Prompt** and run:

```powershell
wsl
```

Or search for "Ubuntu" in Windows Start menu.

---

### 2. Install Qt 5.15 and Build Tools

```bash
# Update package list
sudo apt update

# Install Qt 5.15 development packages
sudo apt install -y \
    qtbase5-dev \
    qtdeclarative5-dev \
    qtlocation5-dev \
    qtpositioning5-dev \
    qtbase5-private-dev \
    build-essential \
    git

# Verify installation
qmake --version
```

**Expected output:**
```
QMake version 3.1
Using Qt version 5.15.x in /usr/lib/x86_64-linux-gnu
```

---

## Access Windows Files from WSL

Your Windows `E:\` drive is mounted at `/mnt/e/` in WSL.

```bash
# Navigate to your project
cd /mnt/e/TiHAN/qtlocation-cpp/bing

# Verify you're in the right place
ls -la
# Should show: bingplugin.cpp, bingmappingengine.cpp, bing.pro, etc.
```

---

## Build the Plugin

### Option 1: Using the Build Script

```bash
cd /mnt/e/TiHAN/qtlocation-cpp/bing

# Make script executable
chmod +x build_linux.sh

# Run build
./build_linux.sh
```

### Option 2: Manual Build

```bash
cd /mnt/e/TiHAN/qtlocation-cpp/bing

# Clean previous build
make clean 2>/dev/null || true
rm -f Makefile

# Generate Makefile
qmake bing.pro

# Build
make -j$(nproc)

# Install
sudo make install
```

---

## Build Output

**Plugin location:**
```
/usr/lib/x86_64-linux-gnu/qt5/plugins/geoservices/libqtgeoservices_bing.so
```

**Verify:**
```bash
ls -lh /usr/lib/x86_64-linux-gnu/qt5/plugins/geoservices/libqtgeoservices_bing.so
```

**Expected:**
```
-rwxr-xr-x 1 root root 45K Feb  5 10:50 libqtgeoservices_bing.so
```

---

## Test the Plugin in WSL

### 1. Install X Server for Windows (Optional - for GUI)

To run GUI apps from WSL, you need an X server on Windows.

**Option A: Install VcXsrv (Recommended)**

1. Download from: https://sourceforge.net/projects/vcxsrv/
2. Install and run **XLaunch**
3. Select: "Multiple windows", "Start no client", **Disable access control**

**Option B: Use WSLg (Windows 11 only)**

If you have Windows 11, WSLg is built-in - skip this step!

### 2. Set DISPLAY Variable

```bash
# For VcXsrv (Windows 10)
export DISPLAY=$(cat /etc/resolv.conf | grep nameserver | awk '{print $2}'):0.0

# For WSLg (Windows 11)
# No need to set DISPLAY - it's automatic
```

### 3. Test with QML

```bash
cd /mnt/e/TiHAN/qtlocation-cpp/bing

# Install qmlscene if not present
sudo apt install -y qml-module-qtquick2 qml-module-qtquick-window2 \
    qml-module-qtlocation qml-module-qtpositioning

# Run test
QT_DEBUG_PLUGINS=1 qmlscene test_bing.qml
```

**Expected console output:**
```
QFactoryLoader::QFactoryLoader() looking at "/usr/lib/.../libqtgeoservices_bing.so"
loaded library ".../libqtgeoservices_bing.so"
qml: Plugin name: bing
qml: Supported map types: 2
qml:   [0] Bing Satellite
qml:   [1] Bing Hybrid
qml: Active map type: Bing Satellite
```

**Expected GUI:**
- Window opens with Bing satellite map
- Tiles load from internet
- Status overlay shows "✓ Plugin Working"

---

## Test Without GUI (Console Only)

If you don't want to set up X server, verify the plugin loads correctly:

```bash
cd /mnt/e/TiHAN/qtlocation-cpp/bing

# Create minimal test
cat > test_console.qml << 'EOF'
import QtQuick 2.15
import QtLocation 5.15
import QtPositioning 5.15

Item {
    Plugin {
        id: bingPlugin
        name: "bing"
        
        Component.onCompleted: {
            console.log("=== PLUGIN TEST ===")
            console.log("Plugin name:", name)
            console.log("Plugin loaded successfully")
        }
    }

    Map {
        id: map
        plugin: bingPlugin
        
        Component.onCompleted: {
            console.log("Map types:", supportedMapTypes.length)
            for (var i = 0; i < supportedMapTypes.length; i++) {
                console.log("  [" + i + "]", supportedMapTypes[i].name)
            }
            
            if (supportedMapTypes.length > 0) {
                console.log("✅ SUCCESS: Plugin working!")
                Qt.quit()
            } else {
                console.error("❌ FAILED: No map types!")
                Qt.quit()
            }
        }
    }
}
EOF

# Run test
QT_DEBUG_PLUGINS=1 qmlscene test_console.qml
```

**Expected output:**
```
=== PLUGIN TEST ===
Plugin name: bing
Plugin loaded successfully
Map types: 2
  [0] Bing Satellite
  [1] Bing Hybrid
✅ SUCCESS: Plugin working!
```

---

## Configure Tile Caching in WSL

```bash
# Set cache environment variables
export QT_LOCATION_TILECACHE_SIZE=2048
export QT_LOCATION_TILECACHE_DIRECTORY=$HOME/.cache/QtLocation

# Create cache directory
mkdir -p $HOME/.cache/QtLocation

# Run with caching
QT_LOCATION_TILECACHE_SIZE=2048 \
QT_LOCATION_TILECACHE_DIRECTORY=$HOME/.cache/QtLocation \
qmlscene test_bing.qml
```

**Verify cache:**
```bash
# After navigating the map
du -sh $HOME/.cache/QtLocation
ls -lh $HOME/.cache/QtLocation/QtLocation/bing/
```

---

## Copy Plugin to Windows (Optional)

If you want to use the Linux `.so` file on a real Linux system:

```bash
# Copy from WSL to Windows desktop
cp /usr/lib/x86_64-linux-gnu/qt5/plugins/geoservices/libqtgeoservices_bing.so \
   /mnt/c/Users/$USER/Desktop/
```

Then transfer to your Linux machine.

---

## Troubleshooting

### Issue: `qmake: command not found`

**Solution:**
```bash
sudo apt update
sudo apt install -y qtbase5-dev
```

### Issue: `fatal error: QtLocation/private/...`

**Solution:**
```bash
sudo apt install -y qtbase5-private-dev qtlocation5-dev
```

### Issue: `cannot open shared object file: libQt5Location.so.5`

**Solution:**
```bash
sudo apt install -y libqt5location5
```

### Issue: GUI doesn't open

**Solution:**

1. **Check DISPLAY variable:**
```bash
echo $DISPLAY
# Should show: <IP>:0.0 or :0
```

2. **Test X server:**
```bash
sudo apt install -y x11-apps
xeyes
# Should show a window with eyes following cursor
```

3. **Restart VcXsrv** with "Disable access control" checked

### Issue: Plugin not found

**Solution:**
```bash
# Check plugin exists
ls -lh /usr/lib/x86_64-linux-gnu/qt5/plugins/geoservices/

# Check permissions
sudo chmod 755 /usr/lib/x86_64-linux-gnu/qt5/plugins/geoservices/libqtgeoservices_bing.so

# Verify metadata
strings /usr/lib/x86_64-linux-gnu/qt5/plugins/geoservices/libqtgeoservices_bing.so | grep bing
```

---

## Quick Start Script

Save this as `build_and_test_wsl.sh`:

```bash
#!/bin/bash
set -e

echo "=== WSL Bing Plugin Build & Test ==="

# Navigate to project
cd /mnt/e/TiHAN/qtlocation-cpp/bing

# Clean
echo "Cleaning..."
make clean 2>/dev/null || true
rm -f Makefile

# Build
echo "Building..."
qmake bing.pro
make -j$(nproc)

# Install
echo "Installing..."
sudo make install

# Verify
PLUGIN_PATH=/usr/lib/x86_64-linux-gnu/qt5/plugins/geoservices/libqtgeoservices_bing.so
if [ -f "$PLUGIN_PATH" ]; then
    echo "✅ Plugin built successfully:"
    ls -lh "$PLUGIN_PATH"
else
    echo "❌ Build failed!"
    exit 1
fi

# Test (console only)
echo ""
echo "Testing plugin..."
cat > /tmp/test_plugin.qml << 'EOF'
import QtQuick 2.15
import QtLocation 5.15
Item {
    Plugin { id: p; name: "bing" }
    Map {
        plugin: p
        Component.onCompleted: {
            console.log("Map types:", supportedMapTypes.length)
            for (var i = 0; i < supportedMapTypes.length; i++)
                console.log("  [" + i + "]", supportedMapTypes[i].name)
            Qt.quit()
        }
    }
}
EOF

qmlscene /tmp/test_plugin.qml 2>&1 | grep -E "(Map types|Bing)"

echo ""
echo "=== Build complete ==="
echo "To test with GUI: qmlscene test_bing.qml"
```

**Usage:**
```bash
chmod +x build_and_test_wsl.sh
./build_and_test_wsl.sh
```

---

## Summary

| Step | Command |
|------|---------|
| **1. Open WSL** | `wsl` in PowerShell |
| **2. Install Qt** | `sudo apt install qtbase5-dev qtlocation5-dev qtpositioning5-dev build-essential` |
| **3. Navigate** | `cd /mnt/e/TiHAN/qtlocation-cpp/bing` |
| **4. Build** | `qmake bing.pro && make -j$(nproc)` |
| **5. Install** | `sudo make install` |
| **6. Test** | `QT_DEBUG_PLUGINS=1 qmlscene test_bing.qml` |

**Output:** `/usr/lib/x86_64-linux-gnu/qt5/plugins/geoservices/libqtgeoservices_bing.so`

The plugin is now built for Linux and can be tested in WSL or copied to a real Linux system!
