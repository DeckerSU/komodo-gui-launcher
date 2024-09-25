# Komodo Launcher

A simple cross-platform Qt application that allows users to launch `komodod` or `komodo-qt` with specified assetchain parameters. The application downloads the latest `assetchains.json` from the KomodoPlatform repository, parses the available assetchains, and provides a user-friendly interface to select and launch them with the appropriate parameters.

<img src="https://github.com/user-attachments/assets/e3d2a879-a709-43fa-84a8-750b02e1674c" width="50%">

## Build Instructions

### Prerequisites

- **Qt 5.15.x**: Ensure you have Qt 5.15.x installed.
- **CMake** or **qmake**: For building the application.

### Building with qmake

```bash
mkdir build
cd build
qmake ../LauncherApp.pro
make
```

### Building with cmake
```bash
mkdir build
cd build
cmake ..
make
```

### Build with Automake

Ensure you have `autoconf`, `automake`, `libtool`, `pkg-config`, and Qt development packages installed.

```bash
./autogen.sh
make
```
