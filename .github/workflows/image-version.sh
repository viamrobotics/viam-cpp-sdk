#!/usr/bin/env bash
# Compute the image version for one docker-base-images run: YYYY-MM.NN (UTC),
# where NN is 00 for the first publish of the month and increments for each
# later one. Every cell in the run shares it, as the `<codename>-<version>` tag
# and the org.opencontainers.image.version label, so consumers pin one string.
#
# NN comes from tags already in ghcr: the max NN this month across every
# repository the bake file publishes (cpp-sdk-{system,conan}-{ubuntu,debian}),
# plus one.
#
# Writes `version=<value>` to $GITHUB_OUTPUT when set, and echoes it to stdout.
#
# Usage: image-version.sh [bake-file]   (default: docker-bake.hcl)
# Env:   GITHUB_TOKEN  optional; without it ghcr is read anonymously.
set -euo pipefail

bake_file="${1:-docker-bake.hcl}"
month=$(date -u +%Y-%m)

# ghcr repositories, e.g. viamrobotics/cpp-sdk-system-ubuntu (tag stripped)
repositories=$(docker buildx bake -f "$bake_file" --print 2>/dev/null \
  | jq -r '.target[] | select(.tags) | .tags[0] | split(":")[0] | sub("^ghcr\\.io/"; "")' \
  | sort -u)

# List every tag in one ghcr repository, following Link-header pagination.
list_tags() {
  local repository="$1" auth url next body hdr
  hdr=$(mktemp)
  if [ -n "${GITHUB_TOKEN:-}" ]; then
    auth="Bearer $(printf %s "$GITHUB_TOKEN" | base64 | tr -d '\n')"
  else
    auth="Bearer $(curl -fsS "https://ghcr.io/token?scope=repository:${repository}:pull" | jq -r .token)"
  fi
  url="https://ghcr.io/v2/${repository}/tags/list?n=1000"
  while [ -n "$url" ]; do
    # A repository that has never been pushed 404s; treat as no tags.
    body=$(curl -sS -D "$hdr" -H "Authorization: $auth" "$url") || break
    grep -q '^HTTP/[0-9.]* 200' "$hdr" || break
    echo "$body" | jq -r '.tags[]?'
    next=$(grep -i '^link:' "$hdr" | sed -n 's/.*<\([^>]*\)>; *rel="next".*/\1/p')
    url=""
    [ -n "$next" ] && url="https://ghcr.io${next}"
  done
  rm -f "$hdr"
}

max=-1
for repository in $repositories; do
  n=$(list_tags "$repository" | sed -n -E "s/^.*-${month}\.([0-9]{2})$/\1/p" | sort -n | tail -1)
  if [ -n "$n" ] && [ "$((10#$n))" -gt "$max" ]; then
    max=$((10#$n))
  fi
done

version=$(printf '%s.%02d' "$month" $((max + 1)))

if [ -n "${GITHUB_OUTPUT:-}" ]; then
  echo "version=$version" >> "$GITHUB_OUTPUT"
fi
echo "$version"
