# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

_No unreleased changes yet._

## [5.5.14] - 2026-06-10

### Added

- Dark mode support with owner‑drawn controls and a themed status bar for improved low‑light usability.

### Changed

- Modernized hex‑case normalization and limited UI updates to the hex field to reduce unnecessary redraws.
- Unified sampling‑mode switching logic and corrected flag checks for more predictable eyedropper behavior.
- Centralized eyedropper cursor selection and removed redundant `SetIcon` calls.
- Replaced `UpdateData(TRUE)` with `GetDlgItemInt` in RGB/CMYK handlers for clearer, safer input parsing.
- Replaced legacy `RangeCheck` with `std::clamp` and removed obsolete wrap‑around logic.
- Moved the entire project into the `ColorCop/` subdirectory to simplify build and packaging workflows.

### Fixed

- Corrected CMYK output to use a proper 0–100% range and clarified RGB inversion logic.

### Security

- Enabled Control Flow Guard (CFG), SafeSEH, SDL checks, and Spectre mitigations for both Debug and Release builds.

### CI / Packaging

- Updated Chocolatey workflow paths to reflect the new `ColorCop/` directory structure.

## [5.5.13] - 2026-05-28

### Added

- Display HSV values via a dynamic multiline tooltip when hovering the color preview.

### Changed

- Refactored `SetupSystemMenu` for cleaner formatting and modern pointer usage.

### Fixed

- Corrected collapsed-dialog sizing by switching to `AdjustWindowRectEx`.

### Security

- Enabled ASLR and DEP for Win32 builds to harden executable security.

---

## [5.5.12] - 2026-05-23

### Fixed

- Bringing a second instance to the foreground now activates the existing window instead of showing an error.
- Corrected `CString` TRACE logging.
- Restored consistent Always-on-Top behavior and improved z-order handling.
- Corrected color-mode switching logic.
- Clamped zoom level on load and replaced remaining raw `HWND` usage with `GetSafeHwnd`.
- Added empty JSON body to prevent HTTP 411 errors during Store submissions.

### Changed

- Normalized numeric types, removed narrowing conversions, and centralized default values.
- Simplified build output and removed noisy version logging.
- Centralized magnifier defaults and removed redundant clamping.
- Cleaned up CMYK and WebSafe logic, replaced C-style casts, and clarified grayscale comments.
- Streamlined dialog state resets and reduced variable scope.

### Added

- Improved Store submission diagnostics with full HTTP status and raw response output.
- Draft Microsoft Store publishing workflow.

---

## [5.5.11] - 2026-05-09

### Added

- Portable mode (`/portable`) to store settings next to the executable.

---

## [5.5.7] - 2026-04-11

### Changed

- Modernized codebase (C++20, Unicode safety).
- Improved UI and system tray behavior.

### Fixed

- Corrected CMYK color math.

---

## [5.5.6] - 2026-03-19

### Changed

- Installer now bundles the VC++ 2015–2022 runtime.

---

## [5.5.5] - 2026-03-14

### Added

- Modern application manifest with DPI awareness.

---

## [5.5.3] - 2026-02-05

### Added

- Official Chocolatey installer support.

---

## [5.5.2] - 2026-01-31

### Fixed

- Inno Setup configuration issues.

---

## [5.5.1] - 2025-12-01

### Added

- First release from GitHub repository.
- Switched to HTML-based help system for improved compatibility and accessibility.
- Pressing F1 now opens the new HTML help interface.

---

## [5.4.5] - 2007-10-24

### Added
- Clarion Hex mode.
- Arrow key support while eyedropping or magnifying.
- Multi‑pixel average sampling (3×3 up to 31×31).

### Changed
- Began CMYK support.

---

## [5.4.2] - 2006-09-19

### Added
- “Put cursor over Eyedropper on startup” option.
- Digital signature.

### Fixed
- Minimize‑on‑startup flashing behavior.

---

## [5.4.1] - 2006-09-15

### Changed
- Settings files moved to the Application Data directory.

---

## [5.4] - 2006-09-03

### Added
- New application icon by Raul Matei.

### Changed
- Updated location of `.dat` and `.bmp` files.
- Updated domain used in URLs.

---

## [5.3] - 2003-02-23

### Added
- Crosshair cursor mode.
- WebSafe color detection.
- Relative position measurements.
- Magnify‑while‑eyedropping.
- Multiple instance support.
- RGB integer mode.
- RGB float mode.

---

## [5.2] - 2002-11-23

### Added
- Convert to Grayscale.
- Ability to click inside the magnifier to pick that color.

---

## [5.1.3] - 2002-02-16

### Added
- Persistent magnifier bitmap.
- Shift+F10 opens the options menu.
- Options menu added to the system menu for keyboard access.

---

## [5.0.1] - 2001-07-28

### Added
- Windows Help File (F1).

### Fixed
- Plus/minus button sensitivity.
- About box static text issues.

---

## [5.0] - 2001-07-22

### Added
- Magnifier with 1×–16× zoom.
- 3×3 and 5×5 average sampling.
- Snap to WebSafe.
- Minimize to system tray.
- Visual Basic, Visual C++, and PowerBuilder Hex color support.
- Mouse wheel support.
- EasyMove window dragging.
- Uppercase/lowercase Hex toggle.
- Collapsible dialog.
- Color history.
- Complementary color palette.

---

## [4.0] - 2000-05-17

### Added
- Dropdown menus.
- Delphi Hex code support.
- Reverse Color option.
- Random Color option.
- Toggleable Always on Top.

### Fixed
- Minimize bug causing the app to reopen stuck when previously closed minimized.

---

## [3.6] - 2000-02-20

### Added
- HEX → RGB conversion.
- Link to Color Cop website in About dialog.
- Persistent settings.

---

## [3.5] - 1999-11-11

### Added
- Eyedropper support.
- Always on Top.
- Remember settings for next run.

### Changed
- Dialog significantly reduced in size.

---

## [3.0] - 1999-10-01

### Changed
- Fonts changed back to MS Sans Serif.
- Enlarged edit boxes for oversized fonts.

### Fixed
- “Over 255” RGB input bug.

---

## [2.2] - 1999-09-24

### Added

- Auto Copy to Clipboard.

---

## [2.0] - 1999-08-22

### Added

- Color preview.

---

## [1.0] - 1999-08-15

### Added

- Custom Color Picker.
