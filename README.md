# ColorCop

A Windows-based color picker utility built with Microsoft Foundation Classes (MFC).

## About this repository

This GitHub repository contains the source code for Color Cop.
It is intended for **developers and contributors** who want to build, debug, or improve the application.

If you just want to **download and use Color Cop**, please visit the official website: [colorcop.net](https://colorcop.net)

The website provides the latest stable installer and user-focused information.
This repository is focused on development, issue tracking, and contribution workflows.

## Quick start for developers

If you want to build and run Color Cop from source:

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

## Building

  MSBuild.exe ColorCop.sln /p:Configuration=Release /p:Platform=Win32

### Build Configurations

- **Debug**: Development build with debugging symbols
- **Release**: Optimized production build

## Release Workflow

ColorCop uses **semantic version tags** (e.g., `v5.5.2`) to drive the release pipeline.
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
- `colorspace.cpp/h` - Color conversion utilities
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
