# Bing Maps Plugin - Debug Checklist

## Quick Launch

**Run the preview:**
```cmd
cd e:\TiHAN\qtlocation-cpp\bing
run_preview.bat
```

**Or manually:**
```cmd
cd e:\TiHAN\qtlocation-cpp\bing
C:\Qt\5.15.2\msvc2019_64\bin\qmlscene.exe preview.qml
```

## Expected Console Output

If the plugin loads correctly, you should see:
```
Plugin name: bing
Plugin loaded: true
Map plugin: bing
Supported map types: 2
  [0] name: Bing Satellite mapId: 1 style: 2
  [1] name: Bing Hybrid mapId: 2 style: 4
Active map type: Bing Satellite (mapId: 1)
```

## Troubleshooting

### Problem: "No map types available"
**Cause**: Plugin not loaded

**Check:**
1. Verify DLL exists:
   ```cmd
   dir C:\Qt\5.15.2\msvc2019_64\plugins\geoservices\qtgeoservices_bing.dll
   ```

2. Check plugin metadata:
   ```cmd
   type e:\TiHAN\qtlocation-cpp\bing\bing.json
   ```

3. Verify it's in the correct location (not in a subdirectory)

### Problem: Map shows but no tiles load
**Cause**: Network issues or incorrect tile URLs

**Check:**
1. Enable Qt logging:
   ```cmd
   set QT_LOGGING_RULES=qt.location*=true
   qmlscene preview.qml
   ```

2. Look for tile request URLs in console output

3. Verify URLs match pattern:
   ```
   https://ecn.t{0-3}.tiles.virtualearth.net/tiles/{a|h}{quadkey}.jpg?g=2981&mkt=en-US
   ```

### Problem: Blank window or crash
**Cause**: Missing Qt dependencies

**Check:**
1. Verify Qt environment:
   ```cmd
   where qmlscene
   where Qt5Core.dll
   ```

2. Ensure MSVC 2019 x64 runtime is installed

3. Check for missing DLLs using Dependency Walker

## Testing Checklist

- [ ] Plugin loads (check console for "Plugin loaded: true")
- [ ] Two map types available (Satellite and Hybrid)
- [ ] Satellite button switches to aerial imagery
- [ ] Hybrid button switches to hybrid view with labels
- [ ] Tiles load and display correctly
- [ ] Zoom in/out works (mouse wheel)
- [ ] Pan works (click and drag)
- [ ] Debug overlay shows correct information

## Tile URL Examples

**Satellite (mapId=1, type='a'):**
```
https://ecn.t0.tiles.virtualearth.net/tiles/a0313131311.jpg?g=2981&mkt=en-US
```

**Hybrid (mapId=2, type='h'):**
```
https://ecn.t2.tiles.virtualearth.net/tiles/h0313131311.jpg?g=2981&mkt=en-US
```

The quadkey `0313131311` is generated from tile coordinates using bit-interleaving.
