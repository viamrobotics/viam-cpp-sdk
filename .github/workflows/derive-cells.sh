#!/usr/bin/env bash
# Derive the docker base-image cell matrix from the bake file.
#
# Reads the cell surface straight out of docker-bake.hcl via `bake --print`
# (the documented contract) and emits one JSON object per published cell target
# (system-* and conan-*):
#   { target, tags, image }
# `tags` are the published multi-arch references the merge job creates (the
# floating codename tag, plus the `<codename>-<version>` tag when IMAGE_VERSION
# is set in the environment); `image` is the repository without a tag (the
# per-cell arch digests are pushed under it).
#
# Writes `cells=<json>` to $GITHUB_OUTPUT when set, and always echoes the
# pretty-printed matrix to stdout for the run log.
#
# Usage: derive-cells.sh [bake-file]   (default: docker-bake.hcl)
set -euo pipefail

bake_file="${1:-docker-bake.hcl}"

cells=$(docker buildx bake -f "$bake_file" --print \
  | jq -c '[
      .target
      | to_entries[]
      | select(.key | test("^(system|conan)-"))
      | { target: .key, tags: .value.tags, image: (.value.tags[0] | split(":")[0]) }
    ]')

if [ -n "${GITHUB_OUTPUT:-}" ]; then
  echo "cells=$cells" >> "$GITHUB_OUTPUT"
fi

# Debug/log visibility
echo "$cells" | jq .
