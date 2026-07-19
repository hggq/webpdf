#!/usr/bin/env bash
#
# brew.sh - install the webpdf library via Homebrew (macOS / Linuxbrew).
#
# It installs from the local formula (webpdf.rb) in this repository, so you can
# test the formula before publishing it to a tap.
#
# Usage:
#   ./brew.sh                 # build + install from the local formula
#   ./brew.sh --HEAD          # build from the current git HEAD
#   ./brew.sh audit           # run `brew audit --strict` on the formula
#   ./brew.sh uninstall       # remove the installed keg
#
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
FORMULA="${SCRIPT_DIR}/webpdf.rb"

if ! command -v brew >/dev/null 2>&1; then
    echo "error: Homebrew is not installed. See https://brew.sh" >&2
    exit 1
fi

if [[ ! -f "${FORMULA}" ]]; then
    echo "error: formula not found: ${FORMULA}" >&2
    exit 1
fi

cmd="${1:-install}"

case "${cmd}" in
    install)
        # --build-from-source compiles locally instead of fetching a bottle.
        brew install --build-from-source "${FORMULA}"
        echo
        echo "webpdf installed. Headers: $(brew --prefix)/include/webpdf"
        echo "Link with: -lwebpdf -lz   (or use find_package(webpdf) in CMake)"
        ;;
    --HEAD)
        brew install --HEAD --build-from-source "${FORMULA}"
        ;;
    audit)
        brew audit --strict --new-formula "${FORMULA}" || brew audit --strict "${FORMULA}"
        ;;
    uninstall)
        brew uninstall webpdf
        ;;
    *)
        echo "usage: $0 [install|--HEAD|audit|uninstall]" >&2
        exit 2
        ;;
esac
