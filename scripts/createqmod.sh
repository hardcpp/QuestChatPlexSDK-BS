#!/usr/bin/env bash

set -euo pipefail

script_dir="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd -P)"
project_root="$(cd -- "$script_dir/.." && pwd -P)"
qmod_name=""
clean=false

usage() {
    cat <<'EOF'
"CreateQmod" - Packages mod.json and its files into a .qmod archive

Usage: createqmod.sh [QMOD_NAME] [-clean]

Arguments:
  -qmodname, --qmod-name NAME  Set the output name (defaults to mod.json's name)
  -clean, --clean              Build a fresh archive instead of updating an old one
  -help, --help                Show this help
EOF
}

while (($#)); do
    case "$1" in
        -qmodname|--qmod-name)
            (($# >= 2)) || { printf 'Missing qmod name\n' >&2; exit 2; }
            qmod_name="$2"
            shift 2
            ;;
        -qmodname:*|--qmod-name=*)
            qmod_name="${1#*:}"
            [[ "$1" == --qmod-name=* ]] && qmod_name="${1#*=}"
            shift
            ;;
        -clean|--clean) clean=true; shift ;;
        -help|--help|-h) usage; exit 0 ;;
        -*) printf 'Unknown argument: %s\n' "$1" >&2; usage >&2; exit 2 ;;
        *)
            [[ -z "$qmod_name" ]] || { printf 'Unexpected argument: %s\n' "$1" >&2; exit 2; }
            qmod_name="$1"
            shift
            ;;
    esac
done

cd -- "$project_root"

for tool in jq zip; do
    command -v "$tool" >/dev/null 2>&1 || { printf 'Error: %s is required\n' "$tool" >&2; exit 1; }
done

mod_file="$project_root/mod.json"
[[ -f "$mod_file" ]] || { printf 'Error: mod.json was not found\n' >&2; exit 1; }

if [[ -z "$qmod_name" ]]; then
    qmod_name="$(jq -er '.name' "$mod_file")"
fi

files=("$mod_file")
cover="$(jq -r '.coverImage // empty' "$mod_file")"
if [[ -n "$cover" && -f "$project_root/$cover" ]]; then
    files+=("$project_root/$cover")
fi

mapfile -t packaged_files < <(jq -r '(.modFiles // [])[], (.lateModFiles // [])[], (.libraryFiles // [])[]' "$mod_file")
for file_name in "${packaged_files[@]}"; do
    path="$project_root/build/$file_name"
    [[ -f "$path" ]] || path="$project_root/extern/libs/$file_name"
    if [[ ! -f "$path" ]]; then
        printf 'Error: could not find dependency: %s\n' "$path" >&2
        exit 1
    fi
    files+=("$path")
done

[[ "$clean" == true ]] && printf 'Making clean Qmod\n'

output="$project_root/${qmod_name}.qmod"
temp_dir="$(mktemp -d)"
trap 'rm -rf -- "$temp_dir"' EXIT
temp_archive="$temp_dir/archive.zip"

if [[ "$clean" == false && -f "$output" ]]; then
    cp -- "$output" "$temp_archive"
fi

if [[ -f "$temp_archive" ]]; then
    zip -j -u "$temp_archive" "${files[@]}"
else
    zip -j "$temp_archive" "${files[@]}"
fi

mv -- "$temp_archive" "$output"
printf 'Created %s\n' "$output"
