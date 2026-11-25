# ColorCopTests

This project contains the automated unit tests for ColorCop.

## Purpose

ColorCop is primarily an MFC desktop application. Most UI code depends on
Windows dialogs, controls, GDI objects, and message handling, which are
difficult to unit test directly.

To enable automated testing, ColorCop gradually extracts pure, portable
logic from the UI layer into standalone headers and helper functions.
Those components can then be tested independently using Catch2.

## Architecture

```text
ColorCop UI (MFC)
       │
       ▼
Pure utility functions
       │
       ▼
Catch2 unit tests
```

Examples of testable components include:

- Color conversions
- Sampling calculations
- Color history management
- Web-safe color calculations

## Design Guidelines

When adding new tests:

1. Prefer testing pure functions.
2. Avoid dependencies on MFC controls or window handles.
3. Keep tests deterministic.
4. Prefer extracting reusable logic from dialog code over mocking MFC.
5. Add regression tests for bug fixes when practical.

## Running Tests

### Visual Studio

Build and run:

```text
ColorCopTests
```

### CMake

```bash
cmake -S . -B build
cmake --build build
ctest --test-dir build
```

### just

```bash
just test
```

## Why Not Test The Dialog Directly?

The goal is to validate ColorCop behavior without requiring:

- a running desktop session
- user interaction
- Windows message pumps
- MFC dialog initialization

This keeps tests fast, reliable, and portable.

As more logic is extracted from the MFC layer, additional functionality can
be covered by automated tests.
