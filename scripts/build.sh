#!/usr/bin/env bash

set -euo pipefail

script_dir="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd -P)"
project_root="$(cd -- "$script_dir/.." && pwd -P)"
clean=false

usage() {
    cat <<'EOF'
"Build" - Compiles your mod into a ".so" or ".a" library

Arguments:
  -clean, --clean  Delete the build folder before compiling
  -help, --help    Show this help
EOF
}

while (($#)); do
    case "$1" in
        -clean|--clean) clean=true ;;
        -help|--help|-h) usage; exit 0 ;;
        *) printf 'Unknown argument: %s\n' "$1" >&2; usage >&2; exit 2 ;;
    esac
    shift
done

cd -- "$project_root"

if [[ "$clean" == true && -d "$project_root/build" ]]; then
    rm -rf -- "$project_root/build"
fi

mkdir -p -- "$project_root/build"
cmake -G Ninja -DCMAKE_BUILD_TYPE=RelWithDebInfo -B "$project_root/build"
cmake --build "$project_root/build" --parallel 12
