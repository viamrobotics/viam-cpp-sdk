#!/usr/bin/env bash
# Derive the docker base-image cell matrix from the bake file.
#
# Reads the cell surface straight out of docker-bake.hcl via `bake --print`
# (the documented contract) and emits one JSON object per system-* target:
#   { target, tag, image }
# `tag` is the published multi-arch reference the merge job creates; `image`
# is that tag with the version stripped (the per-cell repo digests are pushed
# under it).
#
# Writes `cells=<json>` to $GITHUB_OUTPUT when set, and always echoes the
# pretty-printed matrix to stdout for the run log.
#
# Usage: derive-cells.sh [bake-file]   (default: docker-bake.hcl)
set -euo pipefail

bake_file="${1:-docker-bake.hcl}"

cells=$(docker buildx bake -f "$bake_file" system --print \
  | jq -c '[
      .target
      | to_entries[]
      | select(.key | startswith("system-"))
      | { target: .key, tag: (.value.tags[0]), image: (.value.tags[0] | split(":")[0]) }
    ]')

if [ -n "${GITHUB_OUTPUT:-}" ]; then
  echo "cells=$cells" >> "$GITHUB_OUTPUT"
fi

echo "$cells" | jq .
