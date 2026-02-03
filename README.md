# ColorCop

A Windows-based color picker utility built with Microsoft Foundation Classes (MFC).

## Installation

Color Cop is available as a simple Windows installer for every release.

You can download the latest version from our [GitHub releases](https://github.com/ColorCop/ColorCop/releases), or install it using [winget](https://docs.microsoft.com/en-us/windows/package-manager/winget/):

```sh
winget install JayPrall.ColorCop
```

## Features

- System tray color picker for Windows
- Real-time color sampling from anywhere on screen
- Multiple color format support (RGB, HEX, HSV)

## Requirements

- **Windows 10/11** (x86/x64)
- **Visual Studio 2022** with the following components:
  - MSVC v143 compiler toolset
  - Windows 10/11 SDK
  - MFC for latest v143 build tools (x86 & x64)

## Building

```sh
MSBuild.exe ColorCop.sln /p:Configuration=Release /p:Platform=Win32
```

### Build Configurations

- **Debug**: Development build with debugging symbols
- **Release**: Optimized production build

## Release Workflow

ColorCop uses Git tags to trigger automated GitHub releases and publish compiled binaries.

### Listing existing tags

```sh
git tag --list
```

## Development

### Project Structure

- `ColorCop.cpp/h` - Main application class
- `ColorCopDlg.cpp/h` - Main dialog implementation
- `SystemTray.cpp/h` - System tray functionality
- `colorspace.cpp/h` - Color conversion utilities
- `Res/` - Application resources and icons

## Code Linting

This project uses `[cpplint](https://github.com/cpplint/cpplint)` to enforce C++ style guidelines.

**Install:**

```sh
pip install cpplint
```

**Run linting:**

```sh
./lint.sh
```

### Contributing

1. Fork the repository
2. Create a feature branch
3. Make your changes
4. Test thoroughly on Windows 10/11
5. Submit a pull request

## License

See [LICENSE.TXT](LICENSE.TXT) for details.

## Troubleshooting

**Build Errors:**

- Ensure MFC components are installed in Visual Studio 2022
- Verify Windows SDK is installed
- Check that platform toolset is set to v143

**Runtime Issues:**

- Install Visual C++ Redistributable for Visual Studio 2022
