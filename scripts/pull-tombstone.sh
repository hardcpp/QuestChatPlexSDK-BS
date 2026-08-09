#!/usr/bin/env bash

set -euo pipefail

script_dir="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd -P)"
project_root="$(cd -- "$script_dir/.." && pwd -P)"
file_name="RecentCrash.log"
analyze=false

usage() {
    cat <<'EOF'
"Pull-Tombstone" - Pulls the most recent Beat Saber tombstone from your Quest

Arguments:
  -fileName, --file-name FILE  Output file (default: RecentCrash.log)
  -analyze, --analyze          Process the file with ndk-stack afterward
  -help, --help                Show this help
EOF
}

while (($#)); do
    case "$1" in
        -fileName|--file-name)
            (($# >= 2)) || { printf 'Missing output file name\n' >&2; exit 2; }
            file_name="$2"
            shift 2
            ;;
        -fileName:*|--file-name=*)
            file_name="${1#*:}"
            [[ "$1" == --file-name=* ]] && file_name="${1#*=}"
            shift
            ;;
        -analyze|--analyze) analyze=true; shift ;;
        -help|--help|-h) usage; exit 0 ;;
        *) printf 'Unknown argument: %s\n' "$1" >&2; usage >&2; exit 2 ;;
    esac
done

cd -- "$project_root"

current_epoch="$(date +%s)"
recent_epoch=""
recent_tombstone=""

for index in 0 1 2; do
    remote="/sdcard/Android/data/com.beatgames.beatsaber/files/tombstone_0${index}"
    if ! stats="$(adb shell stat "$remote" 2>&1)"; then
        printf 'Failed to inspect tombstone_0%s: %s\n' "$index" "$stats" >&2
        exit 1
    fi

    modified="$(sed -nE 's/^Modify:[[:space:]]+([0-9]{4}-[0-9]{2}-[0-9]{2} [0-9]{2}:[0-9]{2}:[0-9]{2}).*/\1/p' <<< "$stats" | head -n 1)"
    if [[ -z "$modified" ]] || ! modified_epoch="$(date -d "$modified" +%s 2>/dev/null)"; then
        printf 'Failed to read the modification time for tombstone_0%s\n' "$index" >&2
        exit 1
    fi

    delta=$((current_epoch - modified_epoch))
    if ((delta >= 0)); then
        difference=$(((delta + 30) / 60))
    else
        difference=$(((delta - 30) / 60))
    fi

    if ((difference == 1)); then
        printf 'Found tombstone_0%s %s minute ago\n' "$index" "$difference"
    else
        printf 'Found tombstone_0%s %s minutes ago\n' "$index" "$difference"
    fi

    if [[ -z "$recent_epoch" ]] || ((modified_epoch > recent_epoch)); then
        recent_epoch="$modified_epoch"
        recent_tombstone="$index"
    fi
done

printf 'Latest tombstone was tombstone_0%s\n' "$recent_tombstone"
adb pull "/sdcard/Android/data/com.beatgames.beatsaber/files/tombstone_0${recent_tombstone}" "$file_name"

if [[ "$analyze" == true ]]; then
    "$script_dir/ndk-stack.sh" --log-name "$file_name"
fi
