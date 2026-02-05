# Bing Maps Viewer - Build & Run Instructions

## Overview
This is a production-ready Qt Quick application that displays Bing Maps with a persistent 2GB tile cache.

## Features
- ✅ 2048 MB (2 GB) persistent disk cache
- ✅ Offline map support for cached areas
- ✅ Satellite and Hybrid map types
- ✅ Cache directory: `C:\QtLocationCache`
- ✅ Automatic cache management (LRU eviction)

## Build Instructions

### Using Qt Creator
1. Open `bing_viewer.pro` in Qt Creator
2. Select Qt 5.15.2 MSVC 2019 64-bit kit
3. Click **Build** → **Build Project "bing_viewer"**
4. Click **Run** to launch

### Using Command Line
```cmd
cd e:\TiHAN\qtlocation-cpp\bing_viewer

REM Configure Qt environment
set QTDIR=C:\Qt\5.15.2\msvc2019_64
set PATH=%QTDIR%\bin;%PATH%

REM Generate Makefile
qmake bing_viewer.pro

REM Build
nmake release

REM Run
release\bing_viewer.exe
```

## File Structure
```
bing_viewer/
├── main.cpp           # Application entry point with cache config
├── preview.qml        # Map UI with cache status display
├── qml.qrc           # QML resource file
├── bing_viewer.pro   # qmake project file
└── README.md         # This file
```

## How It Works

### Cache Configuration
The cache is configured in `main.cpp` **before** any QtLocation usage:

```cpp
qputenv("QT_LOCATION_TILECACHE_SIZE", "2048");
qputenv("QT_LOCATION_TILECACHE_DIRECTORY", "C:/QtLocationCache");
```

### Cache Behavior
- **Online**: Tiles downloaded and cached automatically
- **Offline**: Cached tiles load instantly
- **Size limit**: Old tiles pruned when 2GB limit reached
- **Persistence**: Cache survives application restarts

## Testing Cache

### Test 1: Verify Cache Creation
1. Run the application
2. Navigate around the map (pan/zoom)
3. Check `C:\QtLocationCache` directory
4. **Expected**: Folder contains cached tile files

### Test 2: Offline Mode
1. Run with internet, navigate map extensively
2. Close application
3. **Disconnect internet**
4. Run application again
5. Navigate to previously viewed areas
6. **Expected**: Tiles load instantly from cache

### Test 3: Cache Size Limit
1. Navigate extensively to load many tiles
2. Check cache directory size: `dir C:\QtLocationCache`
3. **Expected**: Size approaches but doesn't exceed ~2GB

## Cache Location

**Default**: `C:\QtLocationCache`

**To change**: Edit `main.cpp` line:
```cpp
QString cacheDir = "C:/QtLocationCache";  // Change this path
```

## Troubleshooting

### No tiles loading
- Check internet connection
- Verify plugin DLL exists: `C:\Qt\5.15.2\msvc2019_64\plugins\geoservices\qtgeoservices_bing.dll`
- Check console output for errors

### Cache not persisting
- Verify cache directory exists and is writable
- Check console output for cache configuration messages
- Ensure environment variables are set before QtLocation usage

### Application won't build
- Verify Qt 5.15.2 MSVC 2019 x64 is installed
- Check that `qmake` and `nmake` are in PATH
- Ensure MSVC 2019 compiler is available

## Console Output

Expected output on startup:
```
Created cache directory: C:/QtLocationCache
Tile cache configured:
  Size: 2048 MB
  Directory: C:/QtLocationCache
Plugin loaded: bing
Map initialized
Plugin: bing
Supported map types: 2
  [0] Bing Satellite
  [1] Bing Hybrid
Active map type: Bing Satellite
Application started successfully
```

## Production Deployment

To deploy the application:

1. Build in Release mode
2. Copy executable: `release\bing_viewer.exe`
3. Run `windeployqt` to bundle dependencies:
   ```cmd
   windeployqt release\bing_viewer.exe --qmldir .
   ```
4. Copy Bing plugin DLL to deployment folder:
   ```cmd
   mkdir release\plugins\geoservices
   copy C:\Qt\5.15.2\msvc2019_64\plugins\geoservices\qtgeoservices_bing.dll release\plugins\geoservices\
   ```
5. Distribute the `release` folder

## Notes

- Cache is shared across all QtLocation providers
- Cache uses LRU (Least Recently Used) eviction
- No plugin code modifications required
- Cache configuration follows QGroundControl architecture
