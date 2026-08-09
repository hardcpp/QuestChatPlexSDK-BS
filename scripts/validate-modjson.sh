#!/usr/bin/env bash
exit
set -euo pipefail

script_dir="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd -P)"
project_root="$(cd -- "$script_dir/.." && pwd -P)"
mod_file="$project_root/mod.json"
template_file="$project_root/mod.template.json"
schema_url="https://raw.githubusercontent.com/Lauriethefish/QuestPatcher.QMod/refs/heads/main/QuestPatcher.QMod/Resources/qmod.schema.json"

cd -- "$project_root"

if [[ ! -f "$mod_file" ]]; then
    if [[ -f "$template_file" ]]; then
        if command -v qpm-rust >/dev/null 2>&1; then
            qpm-rust qmod build
        elif command -v qpm >/dev/null 2>&1; then
            qpm qmod build
        else
            printf 'Error: qpm is required to generate mod.json from mod.template.json\n' >&2
            exit 1
        fi
    else
        printf 'Error: mod.json and mod.template.json were not present\n' >&2
        exit 1
    fi
fi

printf 'Creating qmod from mod.json\n'
command -v curl >/dev/null 2>&1 || { printf 'Error: curl is required to download the qmod schema\n' >&2; exit 1; }
command -v python3 >/dev/null 2>&1 || { printf 'Error: Python 3 and jsonschema are required to validate mod.json\n' >&2; exit 1; }

schema_file="$(mktemp)"
trap 'rm -f -- "$schema_file"' EXIT
curl -fsSL "$schema_url" -o "$schema_file"

printf 'Validating mod.json...\n'
python3 - "$mod_file" "$schema_file" <<'PY'
import json
import sys

try:
    import jsonschema
except ImportError:
    print("Error: install the Python 'jsonschema' package to validate mod.json", file=sys.stderr)
    raise SystemExit(1)

mod_path, schema_path = sys.argv[1:]
try:
    with open(mod_path, encoding="utf-8") as mod_stream:
        instance = json.load(mod_stream)
    with open(schema_path, encoding="utf-8") as schema_stream:
        schema = json.load(schema_stream)
except (OSError, json.JSONDecodeError) as error:
    print(f"Error: {error}", file=sys.stderr)
    raise SystemExit(1)

validator_class = jsonschema.validators.validator_for(schema)
try:
    validator_class.check_schema(schema)
except jsonschema.SchemaError as error:
    print(f"Error: invalid qmod schema: {error.message}", file=sys.stderr)
    raise SystemExit(1)

validator = validator_class(schema, format_checker=validator_class.FORMAT_CHECKER)
errors = sorted(validator.iter_errors(instance), key=lambda error: list(error.absolute_path))
if errors:
    print("Error: mod.json is not valid", file=sys.stderr)
    for error in errors:
        location = "$"
        for part in error.absolute_path:
            location += f"[{part}]" if isinstance(part, int) else f".{part}"
        print(f"  {location}: {error.message}", file=sys.stderr)
    raise SystemExit(1)
PY
