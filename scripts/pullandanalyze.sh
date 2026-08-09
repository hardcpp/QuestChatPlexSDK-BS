#!/usr/bin/env bash

set -euo pipefail

script_dir="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd -P)"
project_root="$(cd -- "$script_dir/.." && pwd -P)"

if [[ "${1:-}" == "-help" || "${1:-}" == "--help" || "${1:-}" == "-h" ]]; then
    printf 'Usage: pullandanalyze.sh TOMBSTONE_INDEX\n'
    exit 0
fi

if (($# != 1)); then
    printf 'Usage: pullandanalyze.sh TOMBSTONE_INDEX\n' >&2
    exit 2
fi

ndk_path="${ANDROID_NDK_HOME:-}"
[[ -n "$ndk_path" ]] || { printf 'Error: ANDROID_NDK_HOME is not set\n' >&2; exit 1; }
stack_script="$ndk_path/ndk-stack"
[[ -x "$stack_script" ]] || { printf 'Error: ndk-stack was not found at %s\n' "$stack_script" >&2; exit 1; }

cd -- "$project_root"
filename="tombstone_0${1}"
adb pull "/storage/emulated/0/Android/data/com.beatgames.beatsaber/files/$filename"
"$stack_script" -sym "$project_root/build/debug/" < "$filename" > "${filename}_unstripped.txt"

if command -v code >/dev/null 2>&1; then
    code "${filename}_unstripped.txt"
else
    printf 'Created %s (the code command was not found, so it was not opened)\n' "${filename}_unstripped.txt"
fi
