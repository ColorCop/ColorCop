# Color Cop Installer

This folder contains the Inno Setup script used to build the Color Cop
Windows installer.

## How it works

- The installer runs in **admin mode** because Color Cop installs into
  **Program Files (x86)**.
- It bundles the **VC++ 2015-2022 runtime** (`vc_redist.x86.exe`).
- The script checks the official registry key to see if the runtime is
  already installed.
- If missing, the runtime is installed silently; otherwise it is skipped.

## Building

1. Install Inno Setup 6.x.
2. Place `vc_redist.x86.exe` in this directory.
3. Build `colorcop.iss`.
4. The output installer is written to `../Output/`.

## Files

- `colorcop.iss` - main installer script
- `version.iss` - version metadata
- `vc_redist.x86.exe` - bundled runtime (not committed)
