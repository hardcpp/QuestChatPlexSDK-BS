#!/usr/bin/env bash

set -euo pipefail

script_dir="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd -P)"
project_root="$(cd -- "$script_dir/.." && pwd -P)"
log_name="log.log"

usage() {
    cat <<'EOF'
"NDK-Stack" - Processes a tombstone using the debug .so files

Usage: ndk-stack.sh [LOG_NAME]

Arguments:
  -logName, --log-name FILE  Tombstone to process (default: log.log)
  -help, --help              Show this help
EOF
}

while (($#)); do
    case "$1" in
        -logName|--log-name)
            (($# >= 2)) || { printf 'Missing log file name\n' >&2; exit 2; }
            log_name="$2"
            shift 2
            ;;
        -logName:*|--log-name=*)
            log_name="${1#*:}"
            [[ "$1" == --log-name=* ]] && log_name="${1#*=}"
            shift
            ;;
        -help|--help|-h) usage; exit 0 ;;
        -*) printf 'Unknown argument: %s\n' "$1" >&2; usage >&2; exit 2 ;;
        *) log_name="$1"; shift ;;
    esac
done

cd -- "$project_root"

if [[ -f "$project_root/ndkpath.txt" ]]; then
    IFS= read -r ndk_path < "$project_root/ndkpath.txt" || true
    ndk_path="${ndk_path%$'\r'}"
else
    ndk_path="${ANDROID_NDK_HOME:-}"
fi

[[ -n "$ndk_path" ]] || { printf 'Error: set ANDROID_NDK_HOME or create ndkpath.txt\n' >&2; exit 1; }
stack_script="$ndk_path/ndk-stack"
[[ -x "$stack_script" ]] || { printf 'Error: ndk-stack was not found at %s\n' "$stack_script" >&2; exit 1; }
[[ -f "$log_name" ]] || { printf 'Error: log file was not found: %s\n' "$log_name" >&2; exit 1; }

"$stack_script" -sym "$project_root/build/debug/" < "$log_name" > "${log_name}_processed.log"
printf 'Created %s\n' "${log_name}_processed.log"
