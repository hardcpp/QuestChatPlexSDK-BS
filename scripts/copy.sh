#!/usr/bin/env bash

set -euo pipefail

script_dir="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd -P)"
project_root="$(cd -- "$script_dir/.." && pwd -P)"
clean=false
log=false
use_debug=false
self_only=false
all=false
custom=""
log_file=""

usage() {
    cat <<'EOF'
"Copy" - Builds and copies your mod to your Quest, then starts Beat Saber

Arguments:
  -clean, --clean        Perform a clean build
  -useDebug, --use-debug Copy files from build/debug
  -log, --log            Start logging after launching Beat Saber

Logging arguments:
  -self, --self          Only show this mod and crash logs
  -all, --all            Show all logs from the Beat Saber process
  -custom, --custom REGEX
                          Add a custom logging pattern
  -file, --file FILE     Save logging output under the scripts folder
  -help, --help          Show this help
EOF
}

require_value() {
    if (($# < 2)); then
        printf 'Missing value for %s\n' "$1" >&2
        exit 2
    fi
}

while (($#)); do
    case "$1" in
        -clean|--clean) clean=true; shift ;;
        -log|--log) log=true; shift ;;
        -useDebug|--use-debug) use_debug=true; shift ;;
        -self|--self) self_only=true; shift ;;
        -all|--all) all=true; shift ;;
        -custom|--custom) require_value "$@"; custom="$2"; shift 2 ;;
        -custom:*|--custom=*) custom="${1#*:}"; [[ "$1" == --custom=* ]] && custom="${1#*=}"; shift ;;
        -file|--file) require_value "$@"; log_file="$2"; shift 2 ;;
        -file:*|--file=*) log_file="${1#*:}"; [[ "$1" == --file=* ]] && log_file="${1#*=}"; shift ;;
        -help|--help|-h) usage; exit 0 ;;
        *) printf 'Unknown argument: %s\n' "$1" >&2; usage >&2; exit 2 ;;
    esac
done

cd -- "$project_root"

build_args=()
[[ "$clean" == true ]] && build_args+=(--clean)
if "$script_dir/build.sh" "${build_args[@]}"; then
    :
else
    status=$?
    printf 'Failed to build, exiting...\n' >&2
    exit "$status"
fi

"$script_dir/validate-modjson.sh"

if ! command -v jq >/dev/null 2>&1; then
    printf 'Error: jq is required to read mod.json\n' >&2
    exit 1
fi

mapfile -t mod_files < <(jq -r '(.modFiles // [])[], (.lateModFiles // [])[]' "$project_root/mod.json")
for file_name in "${mod_files[@]}"; do
    source_dir="$project_root/build"
    [[ "$use_debug" == true ]] && source_dir="$project_root/build/debug"
    adb push "$source_dir/$file_name" "/sdcard/ModData/com.beatgames.beatsaber/Modloader/mods/$file_name"
done

"$script_dir/restart-game.sh"

if [[ "$log" == true ]]; then
    adb logcat -c
    logging_args=()
    [[ "$self_only" == true ]] && logging_args+=(--self)
    [[ "$all" == true ]] && logging_args+=(--all)
    [[ -n "$custom" ]] && logging_args+=(--custom "$custom")
    [[ -n "$log_file" ]] && logging_args+=(--file "$log_file")
    "$script_dir/start-logging.sh" "${logging_args[@]}"
fi
