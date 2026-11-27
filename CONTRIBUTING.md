# Contributing to ColorCop

Thank you for your interest in improving ColorCop.

This document describes the development workflow, testing expectations, architecture goals, and release procedures used by the project.

## Development Setup

### Requirements

- Windows 10 or Windows 11
- Visual Studio 2022
- MFC components installed
- Git

### Clone

```sh
git clone https://github.com/ColorCop/ColorCop.git
cd ColorCop
```

### Build

Open `ColorCop.sln` and build:

- `Debug | Win32`
- `Release | Win32`

The primary supported target is currently **Win32 (x86)**.

## Architecture

ColorCop is an MFC application.

When practical, new logic should be implemented as portable C++ modules that can be tested independently of the UI.

Examples include:

- `ColorHistory`
- `SamplingBounds`
- `WebSafe`

Portable logic should generally:

- Avoid MFC dependencies
- Avoid UI dependencies
- Be suitable for unit testing in `ColorCopTests`

This approach makes the code easier to maintain, test, and modernize over time.

```text
ColorCop UI (MFC)
        │
        ▼
Portable C++ Modules
        │
        ▼
Catch2 Tests
```

## Testing

Before submitting a pull request, run:

```sh
just test
```

and

```sh
just lint
```

Verify that:

- All tests pass
- Linting succeeds
- ColorCop launches correctly
- Core color-picking functionality still works

## Pull Requests

Please keep pull requests focused and easy to review.

Recommended workflow:

1. Fork the repository
2. Create a feature branch
3. Implement the change
4. Run tests and linting
5. Submit a pull request

When possible:

- Reference related issues
- Describe the user-visible impact
- Include screenshots for UI changes
- Keep commits logically grouped

## Reporting Bugs

Please include:

- ColorCop version
- Windows version
- Steps to reproduce
- Expected behavior
- Actual behavior
- Screenshots if applicable

For startup failures, Event Viewer details are especially helpful:

- Exception code
- Faulting module
- Faulting module version

## Versioning

ColorCop versions are derived from Git tags.

Expected format:

```text
vMAJOR.MINOR.PATCH
```

Example:

```text
v5.5.20
```

## Build Script

The `build.cmd` script performs reproducible release builds.

### Version Source

```sh
git describe --tags --abbrev=0
```

If no tags are available, the build falls back to:

```text
0.0.0
```

### Generated Files

The build process generates:

```text
ColorCop/version.h
ColorCop/installer/version.iss
```

### Build Invocation

The script locates `MSBuild.exe` using `vswhere` and performs a:

```text
Release | Win32
```

rebuild.

## Release Process

ColorCop releases are driven by Git tags.

Creating and pushing a new tag:

```sh
git tag v5.5.20
git push origin v5.5.20
```

triggers GitHub Actions to:

1. Build ColorCop
2. Generate version metadata
3. Publish a GitHub Release

## Post-Release Tasks

GitHub does not provide a post-release event, so the following workflows must be run manually after the GitHub Release is published.

### Publish Chocolatey Package

1. Open **Actions**
2. Select **Publish Chocolatey Package**
3. Click **Run workflow**
4. Enter the release tag
5. Run the workflow

### Publish WinGet Package

1. Open **Actions**
2. Select **Publish to WinGet**
3. Click **Run workflow**
4. Enter the release tag
5. Run the workflow

## Useful Commands

List all tags:

```sh
git tag --list
```

Run tests:

```sh
just test
```

Run linting:

```sh
just lint
```
