#!/usr/bin/env bash

set -euo pipefail

script_dir="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd -P)"
project_root="$(cd -- "$script_dir/.." && pwd -P)"
self_only=false
all=false
custom=""
log_file=""
exclude_header=false
show_help=false

usage() {
    if [[ "$exclude_header" == false ]]; then
        cat <<'EOF'
"Start-Logging" - Logs Beat Saber using adb logcat

Arguments:
EOF
    fi
    cat <<'EOF'
  -self, --self          Only show this mod and crash logs
  -all, --all            Show all logs from the Beat Saber process
  -custom, --custom REGEX
                        Add a custom pattern; AndroidRuntime and CRASH are always included
  -file, --file FILE     Save output under the scripts folder
  -help, --help          Show this help
EOF
}

while (($#)); do
    case "$1" in
        -self|--self) self_only=true; shift ;;
        -all|--all) all=true; shift ;;
        -custom|--custom)
            (($# >= 2)) || { printf 'Missing custom pattern\n' >&2; exit 2; }
            custom="$2"
            shift 2
            ;;
        -custom:*|--custom=*)
            custom="${1#*:}"
            [[ "$1" == --custom=* ]] && custom="${1#*=}"
            shift
            ;;
        -file|--file)
            (($# >= 2)) || { printf 'Missing output file\n' >&2; exit 2; }
            log_file="$2"
            shift 2
            ;;
        -file:*|--file=*)
            log_file="${1#*:}"
            [[ "$1" == --file=* ]] && log_file="${1#*=}"
            shift
            ;;
        -excludeHeader|--exclude-header) exclude_header=true; shift ;;
        -help|--help|-h) show_help=true; shift ;;
        *) printf 'Unknown argument: %s\n' "$1" >&2; usage >&2; exit 2 ;;
    esac
done

if [[ "$show_help" == true ]]; then
    usage
    exit 0
fi

cd -- "$project_root"

output_file=""
if [[ -n "$log_file" ]]; then
    if [[ "$log_file" = /* ]]; then
        output_file="$log_file"
    else
        output_file="$script_dir/$log_file"
    fi
fi

if [[ "$all" == true ]]; then
    beat_saber_pid=""
    for _ in 0 1 2 3; do
        beat_saber_pid="$(adb shell pidof com.beatgames.beatsaber 2>/dev/null | tr -d '\r')"
        [[ -n "$beat_saber_pid" ]] && break
        sleep 0.1
    done

    if [[ -z "$beat_saber_pid" ]]; then
        printf 'Could not connect to adb or find Beat Saber, exiting...\n' >&2
        exit 1
    fi

    printf 'Logging using command: adb logcat --pid %s\n' "$beat_saber_pid"
    if [[ -n "$output_file" ]]; then
        adb logcat --pid "$beat_saber_pid" > "$output_file"
    else
        adb logcat --pid "$beat_saber_pid"
    fi
    exit
fi

pattern=""
if [[ "$self_only" == true ]]; then
    command -v jq >/dev/null 2>&1 || { printf 'Error: jq is required for --self\n' >&2; exit 1; }
    mod_name="$(jq -er '.name' "$project_root/mod.json")"
    pattern+="$mod_name|"
fi
[[ -n "$custom" ]] && pattern+="$custom|"
[[ -n "$pattern" ]] || pattern='QuestHook|modloader|scotland2|ChatPlexSDK|'
pattern+='AndroidRuntime|CRASH'

printf 'Logging using command: adb logcat | grep -E %q\n' "$pattern"
if [[ -n "$output_file" ]]; then
    adb logcat | grep --line-buffered -E "$pattern" > "$output_file"
else
    adb logcat | grep --line-buffered -E "$pattern"
fi
