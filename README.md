# WinRed

Basic red team techniques for Windows.

## Requirements

Visual Studio 2017 or later, or the equivalent Build Tools for Visual Studio. In
both cases, select the "Desktop development with C++" option during install.

## Build

```bash
cmake --preset x64-release
cmake --preset --build x64-release
```

In the preceding replace the preset as appropriate:

* x64-debug
* x86-{release,debug}
