#/usr/bin/env bash

set -eu

SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
PROJECT_ROOT="$(dirname "$SCRIPT_DIR")"

pushd "$PROJECT_ROOT"

RELATIVE_OUTPUT="etc/docs/api/current"

mkdir -p "$RELATIVE_OUTPUT"

doxygen Doxyfile

popd

echo 
echo "Doxygen output has been saved to $PROJECT_ROOT/$RELATIVE_OUTPUT"

