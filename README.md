# ![ColorCop Icon](https://raw.githubusercontent.com/ColorCop/ColorCop-website/main/images/ccicon.png) ColorCop

A Windows-based color picker utility built with Microsoft Foundation Classes (MFC).

[![Build Status](https://github.com/ColorCop/ColorCop/actions/workflows/build.yml/badge.svg)](https://github.com/ColorCop/ColorCop/actions)
[![Release](https://img.shields.io/github/v/release/ColorCop/ColorCop)](https://github.com/ColorCop/ColorCop/releases)
[![Downloads](https://img.shields.io/github/downloads/ColorCop/ColorCop/total.svg)](https://github.com/ColorCop/ColorCop/releases)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Winget](https://img.shields.io/badge/Winget-colorcop-blue?logo=microsoft)](https://github.com/microsoft/winget-pkgs/tree/master/manifests/c/ColorCop/ColorCop)

## About this repository

This GitHub repository contains the source code for Color Cop.
It is intended for **developers and contributors** who want to build, debug, or improve the application.

If you just want to **download and use Color Cop**, visit the official website: [colorcop.net](https://colorcop.net), which provides the latest stable installer and user-focused information.

This repository is focused on development, issue tracking, and contribution workflows.

## Quick start for developers

1. **Install prerequisites**
   - Windows 10/11
   - Visual Studio 2022 with:
     - MSVC v143 compiler toolset
     - Windows 10/11 SDK
     - MFC for latest v143 build tools (x86 & x64)

2. **Clone the repository**

   ```sh
   git clone https://github.com/ColorCop/ColorCop.git
   cd ColorCop

### Building in Visual Studio

1. Open `ColorCop.sln` in **Visual Studio 2022**.
2. Select the desired configuration from the toolbar:
   - **Debug | Win32**
   - **Release | Win32**
3. Build the solution via **Build → Build Solution**.

A placeholder `version.h` is included in the repository so the project builds cleanly in the IDE.
When using the command‑line `build.cmd` script, this file is automatically regenerated with version information from Git tags.

### Build Configurations

- **Debug** — Development build with debugging symbols and no optimizations
- **Release** — Optimized production build suitable for distribution

### Command-line build and versioning

ColorCop includes a `build.cmd` script for reproducible Release builds.

- **Version source:** The script reads the latest Git tag using
  `git describe --tags --abbrev=0`. Tags are expected to follow semantic
  versioning, typically `vMAJOR.MINOR.PATCH` (for example, `v5.5.2`).

- **Fallback behavior:** If no tags are found (for example, in forks or
  shallow clones), the script falls back to `0.0.0`.

- **Generated files:**
  - `ColorCop\version.h` — contains `VER_FILEVERSION`, `VER_PRODUCTVERSION`,
    and related string macros derived from the Git tag.
  - `ColorCop\installer\version.iss` — contains `AppVersionStr` for the
    installer, also derived from the Git tag.

- **Build invocation:** The script locates a C++‑capable `MSBuild.exe` via
  `vswhere` and runs a `Rebuild` of the `Release | Win32` configuration.

## Release Workflow

ColorCop uses Git tags (e.g., `v5.5.2`) to drive the release pipeline.
Pushing a new tag triggers GitHub Actions to:

- Build the application
- Update version metadata
- Publish a new GitHub Release with compiled binaries

> GitHub does **not** support a “post‑release” event.
> The following steps must be run **after** the GitHub Release has been created.

### Post‑release actions

After a new release is published on GitHub, two manual follow‑up workflows must be run:

#### 1. Publish to Chocolatey

1. Go to **Actions → Publish Chocolatey Package**
2. Click **Run workflow**
3. Enter the release tag (e.g., `v5.5.2`)
4. Run the workflow to publish the updated Chocolatey package

#### 2. Publish to WinGet

1. Go to **Actions → Publish to WinGet**
2. Click **Run workflow**
3. Enter the same release tag (e.g., `v5.5.2`)
4. Run the workflow to submit the updated WinGet manifest

### Listing existing tags

```sh
git tag --list
```

## Development

### Project Structure

- `ColorCop.cpp/h` - Main application class
- `ColorCopDlg.cpp/h` - Main dialog implementation
- `SystemTray.cpp/h` - System tray functionality
- `Res/` - Application resources and icons

## Code Linting

This project uses [cpplint](https://github.com/cpplint/cpplint) to enforce C++ style guidelines.

**Install:**

```sh
pip install cpplint
```

**Run linting:**

```sh
mise lint
```

### Contributing

1. Fork the repository
2. Create a feature branch
3. Make your changes
4. Test thoroughly on Windows 10/11
5. Submit a pull request

## License

ColorCop is licensed under the **MIT License**.

See [LICENSE.txt](LICENSE.txt) for the full license text.

## Troubleshooting

**Build Errors:**

- Ensure MFC components are installed in Visual Studio 2022
- Verify Windows SDK is installed
- Check that platform toolset is set to v143

**Runtime Issues:**

- Install Visual C++ Redistributable for Visual Studio 2022
