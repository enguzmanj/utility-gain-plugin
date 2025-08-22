# Utility Gain Plugin (JUCE)

A minimal JUCE audio plug-in/standalone with:
- **Gain** (db, smoothed), **Bypass** (xfade), **Input/Output meters**
- CMake + FetchContent + XCode

## Requirements
- CMake 3.24+
- JUCE fetched automatically (pinned via FetchContent)

## Configure & Build (XCode project)
```bash
cmake -S . -B build -G Xcode
cmake --build build --config Debug
```

## Run (Standalone)
- Built app: ```build/UtilityGain_artefacts/Debug/Standalone/UtilityGain.app```

## Install (VST3)
- Built binary: ```build/UtilityGain_artefacts/Debug/VST3/UtilityGain.vst3```
- User install path (Mac): ```~/Library/Audio/Plug-Ins/VST3/```
- Install after building (Mac):
```bash
mkdir -p ~/Library/Audio/Plug-Ins/VST3
cp -R build/UtilityGain_artefacts/Release/VST3/UtilityGain.vst3 \
      ~/Library/Audio/Plug-Ins/VST3/
```

