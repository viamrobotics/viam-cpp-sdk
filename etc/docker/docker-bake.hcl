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
      { distro = "debian", version = "bullseye" },
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

  tags = [
    "${REGISTRY}/cpp-sdk-system-${cell.distro}:${cell.version}",
  ]

  labels = {
    "viam.cellAxis.distro"   = cell.distro
    "viam.cellAxis.version"  = cell.version
    "viam.cellAxis.strategy" = "system"
  }
}

target "conan" {
  inherits = ["base"]
  name     = "conan-${cell.distro}-${cell.version}"
  target   = "conan"

  // (distro, version) cells in the `conan` strategy. Independent of `system`'s
  // matrix: drop a cell here without touching the system matrix, and vice versa.
  matrix = {
    cell = [
      { distro = "debian", version = "bullseye" },
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

  tags = [
    "${REGISTRY}/cpp-sdk-conan-${cell.distro}:${cell.version}",
  ]

  labels = {
    "viam.cellAxis.distro"   = cell.distro
    "viam.cellAxis.version"  = cell.version
    "viam.cellAxis.strategy" = "conan"
  }
}
