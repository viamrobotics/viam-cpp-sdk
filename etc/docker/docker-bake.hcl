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

variable "LLVM_VERSION" {
  default = "19"
}

variable "GRPC_VERSION" {
  default = "v1.62.1"
}

// Native arch throughout. No QEMU emulation.
variable "ARCH" {
  default = "amd64"
}

group "default" {
  targets = ["system"]
}

// Internal stage only; not published. Holds everything common to the strategies
// stacked on top of it. `system` inherits this and points `target` at its own
// Dockerfile stage.
target "base" {
  dockerfile = "Dockerfile"
  context    = "."
  target     = "base"

  args = {
    LLVM_VERSION = "${LLVM_VERSION}"
    GRPC_VERSION = "${GRPC_VERSION}"
  }

  // amd64 / arm64 built on native runners; one manifest list per cell.
  platforms = ["linux/${ARCH}"]
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
      { distro = "debian", version = "bullseye" },
      { distro = "debian", version = "sid" },
      { distro = "ubuntu", version = "focal" },
      { distro = "ubuntu", version = "jammy" },
    ]
  }

  args = {
    DISTRO  = cell.distro
    VERSION = cell.version
  }

  tags = [
    "${REGISTRY}/cpp-sdk-system:${cell.distro}-${cell.version}",
  ]

  labels = {
    "viam.cellAxis.distro"   = cell.distro
    "viam.cellAxis.version"  = cell.version
    "viam.cellAxis.arch"     = "${ARCH}"
    "viam.cellAxis.strategy" = "system"
  }
}
