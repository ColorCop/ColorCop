# ![ColorCop Icon](https://raw.githubusercontent.com/ColorCop/ColorCop-website/main/images/ccicon.png) ColorCop

A lightweight Windows color picker built with Microsoft Foundation Classes (MFC).

[![Build Status](https://github.com/ColorCop/ColorCop/actions/workflows/build.yml/badge.svg)](https://github.com/ColorCop/ColorCop/actions)
[![Release](https://img.shields.io/github/v/release/ColorCop/ColorCop)](https://github.com/ColorCop/ColorCop/releases)
[![Downloads](https://img.shields.io/github/downloads/ColorCop/ColorCop/total.svg)](https://github.com/ColorCop/ColorCop/releases)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Winget](https://img.shields.io/badge/Winget-colorcop-blue?logo=microsoft)](https://github.com/microsoft/winget-pkgs/tree/master/manifests/c/ColorCop/ColorCop)

## About

ColorCop is a Windows desktop utility for identifying and converting screen colors. It provides color values in multiple formats, supports magnified sampling, and remains a lightweight tool for web designers, developers, and digital artists.

This repository contains the source code, build system, tests, and release automation for ColorCop.

If you are looking for the latest installer, screenshots, or end-user documentation, visit:

**https://colorcop.net**

## Quick Start for Developers

### Prerequisites

- Windows 10 or Windows 11
- Visual Studio 2022
- MFC components installed
- MSVC toolset

### Clone the Repository

```sh
git clone https://github.com/ColorCop/ColorCop.git
cd ColorCop
```

### Build in Visual Studio

1. Open `ColorCop.sln`
2. Select one of:
   - `Debug | Win32`
   - `Release | Win32`
3. Build the solution

A placeholder `version.h` is included so the solution builds cleanly in Visual Studio.

## Testing

Run the portable unit test suite:

```sh
just test
```

This configures the CMake test build, compiles the Catch2 test executable, and runs the test suite.

## Linting

Run static analysis and style checks:

```sh
just lint
```

This runs:

- cpplint
- cppcheck

## Project Structure

| Path | Description |
|--------|-------------|
| `ColorCop/` | Main MFC application |
| `ColorCopTests/` | Catch2 unit tests |
| `.github/workflows/` | CI/CD workflows |
| `Res/` | Application resources |

## Contributing

Contributions are welcome.

Please see [CONTRIBUTING.md](CONTRIBUTING.md) for:

- Development setup
- Architecture notes
- Testing requirements
- Pull request guidelines
- Release procedures

## Security

See [SECURITY.md](SECURITY.md).

## License

ColorCop is licensed under the MIT License.

See [LICENSE.txt](LICENSE.txt) for details.

## Troubleshooting

### Build Errors

Ensure MFC components are installed for the active MSVC toolset used by Visual Studio.
