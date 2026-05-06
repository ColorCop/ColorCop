#!/usr/bin/env bash
set -euo pipefail

find . \
  -type f \( -name '*.cpp' -o -name '*.h' \) \
  -exec clang-format -i --fallback-style=none {} +

