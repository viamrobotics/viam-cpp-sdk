// Canonical declaration of the cell surface.
//
// Matrix dimensions: (distro, version) pair and arch. The `system` strategy is
// built on top of the internal `base` target, which it inherits. Adding a distro
// or arch is a change to THIS file only; workflow YAML stays untouched.
//
// Every target carries viam.cellAxis.* labels exposing cell coordinates as
// machine-readable metadata; `docker buildx bake --print` is the contract by
// which the matrix-generator job consumes them.

variable "REGISTRY" {
  default = "ghcr.io/viamrobotics"
}

// Per-run image version, YYYY-MM.NN (see image-version.sh). Empty for local
// builds: only the floating `:<codename>` tag is produced.
variable "IMAGE_VERSION" {
  default = ""
}

// Git revision the images were built from.
variable "REVISION" {
  default = ""
}

// Labels shared by every published cell. The OCI version/revision labels let
// `docker inspect` on a floating tag reveal which build it currently points at.
function "cell_labels" {
  params = [cell, strategy]
  result = merge(
    {
      "viam.cellAxis.distro"   = cell.distro
      "viam.cellAxis.version"  = cell.version
      "viam.cellAxis.strategy" = strategy
    },
    IMAGE_VERSION != "" ? { "org.opencontainers.image.version" = IMAGE_VERSION } : {},
    REVISION != "" ? { "org.opencontainers.image.revision" = REVISION } : {},
  )
}

// Floating codename tag plus, when versioned, the immutable `<codename>-<version>`
// tag. Both live in the cell's repository ${REGISTRY}/cpp-sdk-<strategy>-<distro>.
function "cell_tags" {
  params = [cell, strategy]
  result = concat(
    ["${REGISTRY}/cpp-sdk-${strategy}-${cell.distro}:${cell.version}"],
    IMAGE_VERSION != "" ? ["${REGISTRY}/cpp-sdk-${strategy}-${cell.distro}:${cell.version}-${IMAGE_VERSION}"] : [],
  )
}

group "default" {
  targets = ["system", "conan"]
}

// Holds everything common to the strategies stacked on top of it.
// `system` inherits this and points `target` at its own Dockerfile stage.
target "base" {
  dockerfile = "Dockerfile"
  context    = "."
  target     = "base"

  // Full matrix; slice to a native arch with --set '*.platform=linux/<arch>'.
  platforms = ["linux/amd64", "linux/arm64", "linux/armhf"]
}

target "system" {
  inherits = ["base"]
  name     = "system-${cell.distro}-${cell.version}"
  target   = "system"

  // (distro, version) cells in the `system` strategy. Drop a cell here to drop
  // it from this strategy without touching any other matrix.
  matrix = {
    cell = [
      { distro = "debian", version = "bookworm" },
      { distro = "debian", version = "trixie" },
      { distro = "debian", version = "sid" },
      { distro = "ubuntu", version = "focal" },
      { distro = "ubuntu", version = "jammy" },
      { distro = "ubuntu", version = "noble" },
      { distro = "ubuntu", version = "resolute" },
    ]
  }

  args = {
    DISTRO  = cell.distro
    VERSION = cell.version
  }

  tags   = cell_tags(cell, "system")
  labels = cell_labels(cell, "system")
}

target "conan" {
  inherits = ["base"]
  name     = "conan-${cell.distro}-${cell.version}"
  target   = "conan"

  // (distro, version) cells in the `conan` strategy. Independent of `system`'s
  // matrix: drop a cell here without touching the system matrix, and vice versa.
  matrix = {
    cell = [
      { distro = "debian", version = "bookworm" },
      { distro = "debian", version = "trixie" },
      { distro = "debian", version = "sid" },
      { distro = "ubuntu", version = "focal" },
      { distro = "ubuntu", version = "jammy" },
      { distro = "ubuntu", version = "noble" },
      { distro = "ubuntu", version = "resolute" },
    ]
  }

  args = {
    DISTRO  = cell.distro
    VERSION = cell.version
  }

  tags   = cell_tags(cell, "conan")
  labels = cell_labels(cell, "conan")
}
