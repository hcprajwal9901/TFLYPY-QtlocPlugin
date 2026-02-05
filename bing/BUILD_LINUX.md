# Linux Build Guide - Bing QtLocation Plugin

## Prerequisites

### Install Qt 5.15 and Dependencies

```bash
# Ubuntu 20.04 / 22.04
sudo apt update
sudo apt install -y \
    qtbase5-dev \
    qtdeclarative5-dev \
    qtlocation5-dev \
    qtpositioning5-dev \
    qtbase5-private-dev \
    build-essential \
    git

# Verify Qt version
qmake --version
# Should show: QMake version 3.1, Using Qt version 5.15.x
```

---

## Build the Plugin

### 1. Navigate to Plugin Directory

```bash
cd /path/to/qtlocation-cpp/bing
```

### 2. Generate Makefile

```bash
qmake bing.pro
```

**Expected output:**
```
Project MESSAGE: This project is using private headers...
```

### 3. Build the Plugin

```bash
make -j$(nproc)
```

**Expected output:**
```
g++ -c -pipe -O2 -Wall -W -D_REENTRANT -fPIC ...
g++ -Wl,-O1 -shared -o libqtgeoservices_bing.so ...
```

### 4. Install the Plugin

```bash
sudo make install
```

**Installs to:**
```
/usr/lib/x86_64-linux-gnu/qt5/plugins/geoservices/libqtgeoservices_bing.so
```

Or for custom Qt installation:
```
$QTDIR/plugins/geoservices/libqtgeoservices_bing.so
```

---

## Verify Installation

### Check Plugin File

```bash
# System Qt
ls -lh /usr/lib/x86_64-linux-gnu/qt5/plugins/geoservices/libqtgeoservices_bing.so

# Custom Qt
ls -lh $QTDIR/plugins/geoservices/libqtgeoservices_bing.so
```

**Expected:**
```
-rwxr-xr-x 1 root root 45K Feb  5 10:00 libqtgeoservices_bing.so
```

### Verify Plugin Metadata

```bash
strings libqtgeoservices_bing.so | grep -A5 "bing"
```

**Expected output:**
```
bing
Provider
Version
Experimental
OnlineMappingFeature
```

---

## Test the Plugin

### Create Test QML File

**File:** `test_bing.qml`

```qml
import QtQuick 2.15
import QtQuick.Window 2.15
import QtLocation 5.15
import QtPositioning 5.15

Window {
    visible: true
    width: 1024
    height: 768
    title: "Bing Maps Test - Linux"

    Plugin {
        id: bingPlugin
        name: "bing"
        
        Component.onCompleted: {
            console.log("Plugin name:", name)
            console.log("Plugin loaded")
        }
    }

    Map {
        id: map
        anchors.fill: parent
        plugin: bingPlugin
        center: QtPositioning.coordinate(37.7749, -122.4194) // San Francisco
        zoomLevel: 12

        Component.onCompleted: {
            console.log("Map initialized")
            console.log("Plugin:", plugin.name)
            console.log("Supported map types:", supportedMapTypes.length)
            
            for (var i = 0; i < supportedMapTypes.length; i++) {
                console.log("  [" + i + "]", supportedMapTypes[i].name)
            }
            
            if (supportedMapTypes.length > 0) {
                activeMapType = supportedMapTypes[0]
                console.log("Active map type:", activeMapType.name)
            }
        }
        
        Text {
            anchors.centerIn: parent
            text: "Map Types: " + map.supportedMapTypes.length + "\n" +
                  "Active: " + (map.activeMapType ? map.activeMapType.name : "none")
            color: "red"
            font.pixelSize: 24
            font.bold: true
            style: Text.Outline
            styleColor: "white"
        }
    }
}
```

### Run Test with Debug Output

```bash
QT_DEBUG_PLUGINS=1 qmlscene test_bing.qml 2>&1 | grep -i bing
```

**Expected output:**
```
QFactoryLoader::QFactoryLoader() looking at "/usr/lib/.../libqtgeoservices_bing.so"
Found metadata in lib .../libqtgeoservices_bing.so
Got keys from plugin meta data ("bing")
loaded library ".../libqtgeoservices_bing.so"
qml: Plugin name: bing
qml: Supported map types: 2
qml:   [0] Bing Satellite
qml:   [1] Bing Hybrid
```

### Run Test Normally

```bash
qmlscene test_bing.qml
```

**Expected:**
- Window opens showing Bing satellite map
- Tiles load from Bing servers
- Map is interactive (pan/zoom)

---

## Configure Tile Caching

### Set Environment Variables

```bash
export QT_LOCATION_TILECACHE_SIZE=2048
export QT_LOCATION_TILECACHE_DIRECTORY=$HOME/.cache/QtLocation
```

### Run with Caching

```bash
QT_LOCATION_TILECACHE_SIZE=2048 \
QT_LOCATION_TILECACHE_DIRECTORY=$HOME/.cache/QtLocation \
qmlscene test_bing.qml
```

### Verify Cache

```bash
# Navigate the map, then check cache
du -sh $HOME/.cache/QtLocation
ls -lh $HOME/.cache/QtLocation/QtLocation/bing/
```

**Expected:**
- Cache directory created
- Tile files (`.jpg`) appear as you navigate
- Cache size grows up to ~2GB

---

## Troubleshooting

### Plugin Not Found

**Symptom:**
```
qml: Supported map types: 0
```

**Debug:**
```bash
QT_DEBUG_PLUGINS=1 qmlscene test_bing.qml 2>&1 | grep geoservices
```

**Check:**
1. Plugin installed: `ls /usr/lib/x86_64-linux-gnu/qt5/plugins/geoservices/`
2. Correct name: Must be `libqtgeoservices_bing.so`
3. Permissions: `chmod 755 libqtgeoservices_bing.so`

### Plugin Loads But No Map Types

**Symptom:**
```
qml: Plugin loaded
qml: Supported map types: 0
```

**Debug:**
```bash
# Check for errors
qmlscene test_bing.qml 2>&1 | grep -i error
```

**Common causes:**
- Missing Qt dependencies
- Incorrect Qt version mismatch
- Plugin built with different Qt than runtime

### Tiles Not Loading

**Symptom:**
- Map shows but tiles don't load

**Check:**
1. Internet connection: `ping ecn.t0.tiles.virtualearth.net`
2. Network errors: Check console output
3. Firewall: Ensure HTTP/HTTPS allowed

### Cache Not Working

**Symptom:**
- Tiles don't persist after restart

**Check:**
1. Environment variables set correctly
2. Cache directory writable: `ls -ld $QT_LOCATION_TILECACHE_DIRECTORY`
3. Disk space available: `df -h`

---

## Cross-Platform `.pro` File

The existing `bing.pro` is already cross-platform:

```qmake
QT += location location-private positioning network

TARGET = qtgeoservices_bing
PLUGIN_TYPE = geoservices
PLUGIN_CLASS_NAME = QGeoServiceProviderFactoryBing
TEMPLATE = lib
CONFIG += plugin

DESTDIR = $$[QT_INSTALL_PLUGINS]/geoservices

HEADERS += \
    bingplugin.h \
    bingmappingengine.h \
    bingtilefetcher.h \
    bingtiledmapreply.h

SOURCES += \
    bingplugin.cpp \
    bingmappingengine.cpp \
    bingtilefetcher.cpp \
    bingtiledmapreply.cpp

OTHER_FILES += \
    bing.json
```

**Key points:**
- `TARGET = qtgeoservices_bing` → Linux adds `lib` prefix automatically
- `DESTDIR = $$[QT_INSTALL_PLUGINS]/geoservices` → Cross-platform Qt variable
- No Windows-specific code paths
- No MSVC-specific flags

---

## Plugin Naming Convention

| Platform | Plugin Name | Location |
|----------|-------------|----------|
| **Windows** | `qtgeoservices_bing.dll` | `C:\Qt\5.15.2\msvc2019_64\plugins\geoservices\` |
| **Linux** | `libqtgeoservices_bing.so` | `/usr/lib/x86_64-linux-gnu/qt5/plugins/geoservices/` |
| **macOS** | `libqtgeoservices_bing.dylib` | `/usr/local/Qt-5.15.2/plugins/geoservices/` |

Qt automatically adds the `lib` prefix on Unix systems.

---

## Production Deployment

### Option 1: System-Wide Installation

```bash
sudo make install
```

**Pros:**
- Available to all Qt applications
- Standard location

**Cons:**
- Requires root access
- May conflict with package manager

### Option 2: Application-Local Plugin

```bash
# Copy plugin to application directory
mkdir -p myapp/plugins/geoservices
cp libqtgeoservices_bing.so myapp/plugins/geoservices/

# Set plugin path in application
export QT_PLUGIN_PATH=/path/to/myapp/plugins
./myapp
```

**Pros:**
- No root required
- Isolated from system Qt

**Cons:**
- Must distribute plugin with app

### Option 3: Custom Qt Installation

```bash
# Build against custom Qt
/opt/Qt/5.15.2/gcc_64/bin/qmake bing.pro
make
make install
```

**Installs to:**
```
/opt/Qt/5.15.2/gcc_64/plugins/geoservices/libqtgeoservices_bing.so
```

---

## Validation Checklist

✅ **Build succeeds without errors**
```bash
make -j$(nproc)
# Exit code: 0
```

✅ **Plugin file exists**
```bash
ls -lh $QTDIR/plugins/geoservices/libqtgeoservices_bing.so
# File exists, ~40-50KB
```

✅ **Plugin metadata correct**
```bash
strings libqtgeoservices_bing.so | grep '"bing"'
# Output: "bing"
```

✅ **Qt detects plugin**
```bash
QT_DEBUG_PLUGINS=1 qmlscene test_bing.qml 2>&1 | grep -c "loaded library.*bing"
# Output: 1
```

✅ **Map types available**
```bash
qmlscene test_bing.qml 2>&1 | grep "Supported map types"
# Output: qml: Supported map types: 2
```

✅ **Tiles load**
- Visual: Map displays satellite imagery
- Console: No network errors

✅ **Cache works**
```bash
# Navigate map, then:
ls $HOME/.cache/QtLocation/QtLocation/bing/*.jpg | wc -l
# Output: > 0
```

✅ **Offline works**
```bash
# Disconnect network, restart app
# Previously viewed areas still display
```

---

## Build Script

**File:** `build_linux.sh`

```bash
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
```

**Usage:**
```bash
chmod +x build_linux.sh
./build_linux.sh
```

---

## Summary

| Aspect | Windows | Linux |
|--------|---------|-------|
| **Build tool** | `nmake` | `make` |
| **Compiler** | MSVC 2019 | GCC/G++ |
| **Plugin name** | `qtgeoservices_bing.dll` | `libqtgeoservices_bing.so` |
| **Install path** | `C:\Qt\...\plugins\geoservices\` | `/usr/lib/.../plugins/geoservices/` |
| **Cache path** | `%LOCALAPPDATA%\...` | `$HOME/.cache/QtLocation` |
| **Build command** | `qmake && nmake` | `qmake && make` |

**No code changes required** - the plugin is already cross-platform!
