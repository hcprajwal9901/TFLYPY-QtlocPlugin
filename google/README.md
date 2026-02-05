# Google Maps QtLocation Plugin

A cross-platform QtLocation geoservices plugin for Google Maps with QGroundControl-style tile architecture.

## Features

✅ **4 Map Types:**
- Google Street Map
- Google Satellite
- Google Terrain
- Google Hybrid

✅ **Cross-Platform:**
- Windows (`.dll`)
- Linux (`.so`)
- macOS (`.dylib`)

✅ **Tile-Based Architecture:**
- XYZ tile system (not quadkey)
- Server sharding: `mt0-mt3.google.com`
- QGroundControl-style URL generation

✅ **Persistent Caching:**
- 2GB tile cache support
- Offline map viewing
- Application-level configuration

---

## Build Instructions

### Windows

```cmd
cd google
qmake google.pro
jom
```

**Output:** `qtgeoservices_google.dll`

**Install:**
```cmd
copy qtgeoservices_google.dll C:\Qt\5.15.2\msvc2019_64\plugins\geoservices\
```

### Linux / WSL Ubuntu

```bash
cd google
qmake google.pro
make -j$(nproc)
sudo make install
```

**Output:** `libqtgeoservices_google.so`

**Install Location:** `/usr/lib/x86_64-linux-gnu/qt5/plugins/geoservices/`

---

## Testing

### Quick Test

```bash
# Windows
qmlscene preview.qml

# Linux
QT_DEBUG_PLUGINS=1 qmlscene preview.qml
```

### Expected Output

```
=== PLUGIN TEST ===
Plugin: google
Map types: 4
  [0] Google Street
  [1] Google Satellite
  [2] Google Terrain
  [3] Google Hybrid
Active map type: Google Street
✓ Plugin Working
```

---

## QML Usage

```qml
import QtQuick 2.15
import QtLocation 5.15
import QtPositioning 5.15

Map {
    plugin: Plugin { name: "google" }
    center: QtPositioning.coordinate(37.7749, -122.4194)
    zoomLevel: 12
    
    // Switch map types
    activeMapType: supportedMapTypes[0] // Street
    activeMapType: supportedMapTypes[1] // Satellite
    activeMapType: supportedMapTypes[2] // Terrain
    activeMapType: supportedMapTypes[3] // Hybrid
}
```

---

## Cache Configuration

Set environment variables **before** creating `QGuiApplication`:

### Windows

```cpp
qputenv("QT_LOCATION_TILECACHE_SIZE", "2048");
qputenv("QT_LOCATION_TILECACHE_DIRECTORY", "C:/QtLocationCache");
```

### Linux

```cpp
qputenv("QT_LOCATION_TILECACHE_SIZE", "2048");
qputenv("QT_LOCATION_TILECACHE_DIRECTORY", "$HOME/.cache/QtLocation");
```

**Cache Location (Auto):**
- Windows: `%LOCALAPPDATA%\<appname>\cache\QtLocation\google\`
- Linux: `~/.cache/<appname>/QtLocation/google/`

---

## Technical Details

### URL Format

```
http://mt{server}.google.com/vt/lyrs={lyrs}&hl=en&x={x}&s={sec1}&y={y}&z={zoom}&s={sec2}
```

- **Server:** `(x + y) % 4` → `mt0`, `mt1`, `mt2`, `mt3`
- **Lyrs:** `m` (street), `s` (satellite), `t,r` (terrain), `y` (hybrid)
- **Security:** Based on "Galileo" string

### Architecture

```
QGeoServiceProviderFactoryGoogle (Plugin Factory)
  └── GoogleMappingEngine (QGeoTiledMappingManagerEngine)
        └── GoogleTileFetcher (QGeoTileFetcher)
              └── GoogleTiledMapReply (QGeoTiledMapReply)
```

---

## Troubleshooting

### Plugin Not Found

```bash
# Check installation
ls /usr/lib/x86_64-linux-gnu/qt5/plugins/geoservices/libqtgeoservices_google.so

# Debug loading
QT_DEBUG_PLUGINS=1 qmlscene preview.qml 2>&1 | grep google
```

### No Tiles Loading

1. Check internet connection
2. Verify URL in debug output: `http://mt*.google.com/vt/...`
3. Test URL in browser
4. Check firewall settings

### Cache Not Working

1. Verify environment variables are set **before** `QGuiApplication`
2. Check cache directory exists and is writable
3. Monitor cache directory size: `du -sh ~/.cache/QtLocation`

---

## License

This plugin is for educational and development purposes. Respect Google Maps Terms of Service.

---

## Credits

Based on QGroundControl's Google Maps provider implementation.
