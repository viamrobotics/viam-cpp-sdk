#!/usr/bin/env bash
#
# conan strategy: python3 + conan in an isolated venv, plus a single jinja2
# default profile.
#
# The venv keeps us off PEP-668 "externally managed" distros' toes. We source
# its activate script so conan lands on PATH.

set -euo pipefail
[[ "${DEBUG:-}" ]] && set -x

# shellcheck disable=SC1091
. "$(dirname "$0")/lib/common.sh"

# venv (not global pip): managed-environment distros forbid a global install.
apt_install python3 python3-venv

python3 -m venv /opt/conan-venv
# Upgrade pip first: the pip bundled on older distros (e.g. bullseye's 20.3) ships
# the legacy resolver, which crashes resolving conan's deps (KeyError: 'markupsafe').
/opt/conan-venv/bin/pip install --no-cache-dir --upgrade pip
/opt/conan-venv/bin/pip install --no-cache-dir "conan~=2.0"

# Source the venv activate for conan on PATH; profile.d carries it to runtime shells.
echo '. /opt/conan-venv/bin/activate' > /etc/profile.d/conan-venv.sh
# shellcheck disable=SC1091
. /opt/conan-venv/bin/activate

# Install the single jinja2 default profile.
conan_home="$(conan config home)"
mkdir -p "${conan_home}/profiles"
install -m 0644 "$(dirname "$0")/conan/default.jinja" "${conan_home}/profiles/default"

conan --version
