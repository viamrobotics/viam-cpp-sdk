# Vision Service SDK + Template Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Add `viam::sdk::Vision` (a full peer SDK class to `Motion`/`MLModelService`) covering all 9 proto RPCs, with client/server, mock, unit tests, and the auto-generated module-generator template.

**Architecture:** Mirror the layout of the `mlmodel` service (public abstract base + nested POD payload types under `src/viam/sdk/services/`, paired with `services/private/{vision,vision_client,vision_server}.cpp`). Reuse `Camera::raw_image` / `Camera::point_cloud` for image-shaped payloads by lifting Camera's inline proto-conv into a new shared private header `common/private/raw_image.{hpp,cpp}`. Tests use the shared `viamsdk_test` library + `viamcppsdk_add_boost_test` macro.

**Tech Stack:** C++17, gRPC, Protobuf, Boost.Test, CMake. Existing helpers: `make_client_helper` (in `viam/sdk/common/client_helper.hpp`), `make_service_helper` (in `viam/sdk/common/private/service_helper.hpp`), `to_proto`/`from_proto` for `ProtoStruct`.

**Spec:** `docs/superpowers/specs/2026-05-08-vision-service-sdk-and-template-design.md`

---

## File Structure

**New files:**
- `src/viam/sdk/common/private/raw_image.hpp` — shared `Camera::raw_image` ↔ `viam::component::camera::v1::Image` proto-conv declarations
- `src/viam/sdk/common/private/raw_image.cpp` — implementations lifted from `camera_client.cpp` / `camera_server.cpp`
- `src/viam/sdk/services/vision.hpp` — public abstract `viam::sdk::Vision` class + nested payload structs
- `src/viam/sdk/services/vision.cpp` — `api()`, `API::traits<Vision>`, `Vision(name)` ctor
- `src/viam/sdk/services/private/vision.hpp` — internal proto-conv helper declarations (in `viam::sdk::impl::vision`)
- `src/viam/sdk/services/private/vision.cpp` — proto ↔ POD conversion impls
- `src/viam/sdk/services/private/vision_client.hpp` — `class VisionClient : public Vision`
- `src/viam/sdk/services/private/vision_client.cpp` — gRPC client implementations
- `src/viam/sdk/services/private/vision_server.hpp` — `class VisionServer`
- `src/viam/sdk/services/private/vision_server.cpp` — gRPC server implementations
- `src/viam/sdk/tests/mocks/mock_vision.hpp` — `class MockVision : public Vision`
- `src/viam/sdk/tests/mocks/mock_vision.cpp` — implementations
- `src/viam/sdk/tests/test_vision.cpp` — Boost.Test suite

**Modified files:**
- `src/viam/sdk/components/private/camera_client.cpp` — call new shared `raw_image` helper
- `src/viam/sdk/components/private/camera_server.cpp` — call new shared `raw_image` helper (only if it currently has inline conversion; verify)
- `src/viam/sdk/CMakeLists.txt` — add new sources + install new public header `vision.hpp`
- `src/viam/sdk/tests/CMakeLists.txt` — add `mocks/mock_vision.cpp` to `viamsdk_test` `target_sources`; append `viamcppsdk_add_boost_test(test_vision.cpp)`

---

## Phase 1: Foundation

These tasks set up scaffolding so per-RPC TDD becomes possible. Each task ends with the project still building.

### Task 1: Lift Camera's `raw_image` proto-conv into a shared header

**Why first:** Vision's `capture_all_from_camera` returns a `viam::component::camera::v1::Image`, the same proto type Camera already converts inline. We lift this conversion before adding Vision so Vision can call the shared helper instead of duplicating ~30 LoC.

**Files:**
- Create: `src/viam/sdk/common/private/raw_image.hpp`, `src/viam/sdk/common/private/raw_image.cpp`
- Modify: `src/viam/sdk/components/private/camera_client.cpp` (currently inlines conversion at lines 29–37)
- Modify: `src/viam/sdk/components/private/camera_server.cpp` (verify whether it has parallel inline conversion; if so, migrate it too)
- Modify: `src/viam/sdk/CMakeLists.txt`

- [ ] **Step 1: Read camera_server.cpp to confirm whether it has parallel inline conversion of `raw_image` ↔ proto Image**

```bash
grep -n "raw_image\|::Image" src/viam/sdk/components/private/camera_server.cpp
```

Expected: identifies any inline conversion. Note line numbers; you'll either migrate them in Step 6 or skip if camera_server doesn't return `Image` protos.

- [ ] **Step 2: Create `raw_image.hpp` with the helper declarations**

```cpp
// src/viam/sdk/common/private/raw_image.hpp
// Copyright 2026 Viam Inc.
// Apache-2.0 license header (copy from any existing services/private/*.hpp)

#pragma once

#include <viam/api/component/camera/v1/camera.pb.h>
#include <viam/sdk/components/camera.hpp>

namespace viam {
namespace sdk {
namespace impl {

/// Convert a proto Image into a Camera::raw_image (mime_type, bytes, source_name).
Camera::raw_image from_proto(const ::viam::component::camera::v1::Image& proto);

/// Populate a proto Image from a Camera::raw_image.
void to_proto(const Camera::raw_image& image, ::viam::component::camera::v1::Image* out);

}  // namespace impl
}  // namespace sdk
}  // namespace viam
```

- [ ] **Step 3: Create `raw_image.cpp` with implementations**

```cpp
// src/viam/sdk/common/private/raw_image.cpp
#include <viam/sdk/common/private/raw_image.hpp>

namespace viam {
namespace sdk {
namespace impl {

Camera::raw_image from_proto(const ::viam::component::camera::v1::Image& proto) {
    Camera::raw_image out;
    const auto& bytes_str = proto.image();
    out.bytes.assign(bytes_str.begin(), bytes_str.end());
    out.mime_type = proto.mime_type();
    out.source_name = proto.source_name();
    return out;
}

void to_proto(const Camera::raw_image& image, ::viam::component::camera::v1::Image* out) {
    out->set_mime_type(image.mime_type);
    out->set_source_name(image.source_name);
    out->set_image(std::string(image.bytes.begin(), image.bytes.end()));
}

}  // namespace impl
}  // namespace sdk
}  // namespace viam
```

- [ ] **Step 4: Add the new sources to `src/viam/sdk/CMakeLists.txt`**

Find the `viamsdk` target's source list (the same place that lists `services/mlmodel.cpp`, etc.). Add alphabetically:

```cmake
common/private/raw_image.cpp
```

- [ ] **Step 5: Build to confirm the new translation unit compiles standalone**

```bash
cmake --build build --target viamsdk
```

Expected: success. If you don't have a build dir yet, see "Build setup" at the bottom of this plan.

- [ ] **Step 6: Migrate `camera_client.cpp`'s inline `raw_image` build (lines 29–37) to call the new helper**

In `from_proto(GetImagesResponse)`:

```cpp
// Before (lines 28–37):
std::vector<Camera::raw_image> images;
for (const auto& img : proto.images()) {
    Camera::raw_image raw_image;
    std::string img_string = img.image();
    const std::vector<unsigned char> bytes(img_string.begin(), img_string.end());
    raw_image.bytes = bytes;
    raw_image.mime_type = img.mime_type();
    raw_image.source_name = img.source_name();
    images.push_back(raw_image);
}

// After:
std::vector<Camera::raw_image> images;
images.reserve(proto.images_size());
for (const auto& img : proto.images()) {
    images.push_back(from_proto(img));   // resolves to impl::from_proto via using-declaration at line 23
}
```

If `camera_server.cpp` had a parallel inline conversion (per Step 1), migrate it the same way using `to_proto(raw_image, *out)`.

- [ ] **Step 7: Add `#include <viam/sdk/common/private/raw_image.hpp>` to camera_client.cpp (and camera_server.cpp if migrated)**

- [ ] **Step 8: Build and run camera tests**

```bash
cmake --build build --target test_camera
./build/src/viam/sdk/tests/test_camera
```

Expected: all camera tests pass (no regressions from the lift).

- [ ] **Step 9: Commit**

```bash
git add src/viam/sdk/common/private/raw_image.{hpp,cpp} \
        src/viam/sdk/components/private/camera_client.cpp \
        src/viam/sdk/CMakeLists.txt
# Add camera_server.cpp if migrated
git commit -m "refactor: lift Camera raw_image proto-conv into common/private

Extracts the field-by-field Camera::raw_image <-> proto Image conversion
inlined in camera_client.cpp into shared helpers in
common/private/raw_image.{hpp,cpp}, so the upcoming Vision service can
reuse the conversion for capture_all_from_camera. No behavior change."
```

---

### Task 2: Create the public `Vision` class skeleton

**Why before client/server:** Both depend on the abstract base + nested payload types being defined.

**Files:**
- Create: `src/viam/sdk/services/vision.hpp`, `src/viam/sdk/services/vision.cpp`
- Modify: `src/viam/sdk/CMakeLists.txt`

- [ ] **Step 1: Read the existing `motion.hpp` (or `navigation.hpp`) to model the file shape**

```bash
sed -n '1,40p' src/viam/sdk/services/motion.hpp
```

Note: license header, `#pragma once`, includes (`viam/sdk/common/proto_value.hpp`, `viam/sdk/services/service.hpp`), namespace structure, `class Motion : public Service`, the `API::traits<Motion>` specialization at the bottom.

- [ ] **Step 2: Create `src/viam/sdk/services/vision.hpp` with the full public API**

Key shape (license header omitted for brevity — copy from `motion.hpp`):

```cpp
#pragma once

#include <string>
#include <vector>

#include <boost/optional.hpp>

#include <viam/sdk/common/proto_value.hpp>
#include <viam/sdk/components/camera.hpp>
#include <viam/sdk/services/service.hpp>
#include <viam/sdk/spatialmath/geometry.hpp>

namespace viam {
namespace sdk {

/// @class Vision vision.hpp "services/vision.hpp"
/// @brief A vision service that runs detectors / classifiers / segmenters.
class Vision : public Service {
   public:
    API api() const override;

    /// Result of a single detection.
    /// Pixel and normalized bbox fields are presence-tracked separately so that
    /// implementations can return either (or both, or neither).
    struct detection {
        boost::optional<double> x_min;
        boost::optional<double> y_min;
        boost::optional<double> x_max;
        boost::optional<double> y_max;
        boost::optional<std::string> class_name;
        boost::optional<double> confidence;
        boost::optional<std::int64_t> x_min_normalized;
        boost::optional<std::int64_t> y_min_normalized;
        boost::optional<std::int64_t> x_max_normalized;
        boost::optional<std::int64_t> y_max_normalized;
    };

    struct classification {
        std::string class_name;
        double confidence;
    };

    /// Reuses Camera::point_cloud (mime_type + bytes).
    using point_cloud = Camera::point_cloud;
    /// Reuses Camera::raw_image (mime_type + bytes + source_name).
    using raw_image = Camera::raw_image;

    /// A detected/segmented object together with its constituent geometries.
    /// NOTE: the field name `point_cloud` shadows the type name `point_cloud`.
    /// This is intentional and matches the proto field; flag in PR description.
    struct point_cloud_object {
        point_cloud point_cloud;
        std::vector<GeometryConfig> geometries;
    };

    struct properties {
        bool classifications_supported;
        bool detections_supported;
        bool object_point_clouds_supported;
    };

    struct capture_options {
        bool return_image = false;
        bool return_detections = false;
        bool return_classifications = false;
        bool return_object_point_clouds = false;
    };

    struct capture_all_result {
        boost::optional<raw_image> image;
        std::vector<detection> detections;
        std::vector<classification> classifications;
        std::vector<point_cloud_object> objects;
        ProtoStruct extra;
    };

    // === RPC virtuals (=0) — each has a no-extra inline overload. ===

    inline std::vector<detection> get_detections_from_camera(const std::string& camera_name) {
        return get_detections_from_camera(camera_name, {});
    }
    virtual std::vector<detection> get_detections_from_camera(const std::string& camera_name,
                                                              const ProtoStruct& extra) = 0;

    inline std::vector<detection> get_detections(const raw_image& image) {
        return get_detections(image, {});
    }
    virtual std::vector<detection> get_detections(const raw_image& image,
                                                  const ProtoStruct& extra) = 0;

    inline std::vector<classification> get_classifications_from_camera(
        const std::string& camera_name, int count) {
        return get_classifications_from_camera(camera_name, count, {});
    }
    virtual std::vector<classification> get_classifications_from_camera(
        const std::string& camera_name, int count, const ProtoStruct& extra) = 0;

    inline std::vector<classification> get_classifications(const raw_image& image, int count) {
        return get_classifications(image, count, {});
    }
    virtual std::vector<classification> get_classifications(const raw_image& image,
                                                             int count,
                                                             const ProtoStruct& extra) = 0;

    inline std::vector<point_cloud_object> get_object_point_clouds(
        const std::string& camera_name, const std::string& mime_type) {
        return get_object_point_clouds(camera_name, mime_type, {});
    }
    virtual std::vector<point_cloud_object> get_object_point_clouds(
        const std::string& camera_name, const std::string& mime_type,
        const ProtoStruct& extra) = 0;

    inline struct properties get_properties() {
        return get_properties({});
    }
    virtual struct properties get_properties(const ProtoStruct& extra) = 0;

    inline capture_all_result capture_all_from_camera(const std::string& camera_name,
                                                       const capture_options& opts) {
        return capture_all_from_camera(camera_name, opts, {});
    }
    virtual capture_all_result capture_all_from_camera(const std::string& camera_name,
                                                       const capture_options& opts,
                                                       const ProtoStruct& extra) = 0;

    virtual ProtoStruct do_command(const ProtoStruct& command) = 0;
    virtual ProtoStruct get_status() = 0;

   protected:
    explicit Vision(std::string name);
};

template <>
struct API::traits<Vision> {
    static API api();
};

bool operator==(const Vision::detection& lhs, const Vision::detection& rhs);
bool operator==(const Vision::classification& lhs, const Vision::classification& rhs);
bool operator==(const Vision::point_cloud_object& lhs, const Vision::point_cloud_object& rhs);
bool operator==(const Vision::properties& lhs, const Vision::properties& rhs);

}  // namespace sdk
}  // namespace viam
```

- [ ] **Step 3: Create `src/viam/sdk/services/vision.cpp`**

```cpp
#include <viam/sdk/services/vision.hpp>

namespace viam {
namespace sdk {

API Vision::api() const {
    return API::get<Vision>();
}

API API::traits<Vision>::api() {
    return API(kRDK, kService, "vision");
}

Vision::Vision(std::string name) : Service(std::move(name)) {}

bool operator==(const Vision::detection& a, const Vision::detection& b) {
    return a.x_min == b.x_min && a.y_min == b.y_min && a.x_max == b.x_max && a.y_max == b.y_max &&
           a.class_name == b.class_name && a.confidence == b.confidence &&
           a.x_min_normalized == b.x_min_normalized && a.y_min_normalized == b.y_min_normalized &&
           a.x_max_normalized == b.x_max_normalized && a.y_max_normalized == b.y_max_normalized;
}

bool operator==(const Vision::classification& a, const Vision::classification& b) {
    return a.class_name == b.class_name && a.confidence == b.confidence;
}

bool operator==(const Vision::point_cloud_object& a, const Vision::point_cloud_object& b) {
    if (a.point_cloud.mime_type != b.point_cloud.mime_type) return false;
    if (a.point_cloud.pc != b.point_cloud.pc) return false;
    return a.geometries == b.geometries;
}

bool operator==(const Vision::properties& a, const Vision::properties& b) {
    return a.classifications_supported == b.classifications_supported &&
           a.detections_supported == b.detections_supported &&
           a.object_point_clouds_supported == b.object_point_clouds_supported;
}

}  // namespace sdk
}  // namespace viam
```

NOTE: if `GeometryConfig::operator==` is not yet defined, either define it (matching the other types in `geometry.hpp`) or replace the geometries comparison with a manual elementwise check. Verify with `grep "operator==.*GeometryConfig" src/viam/sdk/spatialmath/geometry.{hpp,cpp}`.

- [ ] **Step 4: Verify `kRDK`, `kService`, `Service`, `API`, `API::traits` are reachable through the existing includes**

```bash
grep -n "kRDK\|kService" src/viam/sdk/services/mlmodel.cpp
```

If `mlmodel.cpp` references them without extra includes, your `vision.cpp` is fine. If extra includes are present in `mlmodel.cpp`, mirror them.

- [ ] **Step 5: Add `services/vision.cpp` to `viamsdk` target in `src/viam/sdk/CMakeLists.txt`**

Add alphabetically next to `services/mlmodel.cpp`.

- [ ] **Step 6: Add `../../viam/sdk/services/vision.hpp` to the public-header install list**

Find the `install(FILES ...)` block listing `motion.hpp`, `navigation.hpp`, etc., and add `vision.hpp` alphabetically.

- [ ] **Step 7: Build**

```bash
cmake --build build --target viamsdk
```

Expected: success. The class is abstract — no instantiation tests yet. If you get errors about `boost::optional` formatting, add `#include <boost/optional/optional_io.hpp>` only in the test files, not the public header.

- [ ] **Step 8: Commit**

```bash
git add src/viam/sdk/services/vision.{hpp,cpp} src/viam/sdk/CMakeLists.txt
git commit -m "feat(sdk): add Vision service public abstract class

Adds viam::sdk::Vision with all 9 RPC virtuals and nested POD payload
types (detection, classification, point_cloud_object, properties,
capture_options, capture_all_result). Mirrors the structure of
viam::sdk::Motion / MLModelService. Client and server impls follow
in subsequent commits."
```

---

### Task 3: Create proto-conv helpers (private)

**Why now:** Client and server both depend on these conversions. Implementing them once first means client+server tasks become straightforward.

**Files:**
- Create: `src/viam/sdk/services/private/vision.hpp`, `src/viam/sdk/services/private/vision.cpp`
- Modify: `src/viam/sdk/CMakeLists.txt`

- [ ] **Step 1: Create `services/private/vision.hpp` with helper declarations**

```cpp
#pragma once

#include <viam/api/common/v1/common.pb.h>
#include <viam/api/service/vision/v1/vision.pb.h>
#include <viam/sdk/services/vision.hpp>

namespace viam {
namespace sdk {
namespace impl {
namespace vision {

::viam::service::vision::v1::Detection      to_proto(const Vision::detection&);
Vision::detection                            from_proto(const ::viam::service::vision::v1::Detection&);

::viam::service::vision::v1::Classification to_proto(const Vision::classification&);
Vision::classification                       from_proto(const ::viam::service::vision::v1::Classification&);

void to_proto(const Vision::point_cloud_object&, ::viam::common::v1::PointCloudObject* out);
Vision::point_cloud_object                   from_proto(const ::viam::common::v1::PointCloudObject&);

void to_proto(const Vision::properties&,
              ::viam::service::vision::v1::GetPropertiesResponse* out);
Vision::properties                           from_proto(const ::viam::service::vision::v1::GetPropertiesResponse&);

}  // namespace vision
}  // namespace impl
}  // namespace sdk
}  // namespace viam
```

`capture_all_result` is converted inline in client.cpp / server.cpp (it pulls in too many other helpers to be a useful free function); `do_command` and `get_status` use shared infra (Task 5+).

- [ ] **Step 2: Write a unit test for `Detection` round-trip** — first failing test

In `src/viam/sdk/tests/test_vision.cpp` (create this file now even though Phase 2 fills it; just put the case below in it):

```cpp
#define BOOST_TEST_MODULE vision

#include <boost/test/included/unit_test.hpp>

#include <viam/sdk/services/private/vision.hpp>
#include <viam/sdk/services/vision.hpp>

namespace vimpl = viam::sdk::impl::vision;
namespace vpb = viam::service::vision::v1;
using viam::sdk::Vision;

BOOST_AUTO_TEST_SUITE(vision_proto_conv)

BOOST_AUTO_TEST_CASE(detection_round_trip_pixel_only) {
    Vision::detection d;
    d.x_min = 1.0; d.y_min = 2.0; d.x_max = 3.0; d.y_max = 4.0;
    d.class_name = std::string("cat");
    d.confidence = 0.9;

    const vpb::Detection proto = vimpl::to_proto(d);
    const Vision::detection back = vimpl::from_proto(proto);

    BOOST_TEST(back == d);
    BOOST_TEST(!back.x_min_normalized.has_value());
}

BOOST_AUTO_TEST_CASE(detection_round_trip_normalized_only) {
    Vision::detection d;
    d.x_min_normalized = 10; d.y_min_normalized = 20;
    d.x_max_normalized = 30; d.y_max_normalized = 40;
    d.class_name = std::string("dog");
    d.confidence = 0.7;

    const vpb::Detection proto = vimpl::to_proto(d);
    const Vision::detection back = vimpl::from_proto(proto);

    BOOST_TEST(back == d);
    BOOST_TEST(!back.x_min.has_value());
}

BOOST_AUTO_TEST_CASE(detection_round_trip_no_bbox) {
    Vision::detection d;
    d.class_name = std::string("unknown");
    d.confidence = 0.1;

    const vpb::Detection proto = vimpl::to_proto(d);
    const Vision::detection back = vimpl::from_proto(proto);

    BOOST_TEST(back == d);
    BOOST_TEST(!back.x_min.has_value());
    BOOST_TEST(!back.x_min_normalized.has_value());
}

BOOST_AUTO_TEST_SUITE_END()
```

Wire `test_vision.cpp` into `tests/CMakeLists.txt` now: append `viamcppsdk_add_boost_test(test_vision.cpp)` at the bottom of the existing list. Don't add the mock yet — there are no tests using it.

- [ ] **Step 3: Run the test to confirm it fails to compile**

```bash
cmake --build build --target test_vision
```

Expected: link/compile error referencing `vimpl::to_proto(detection)` etc. — these don't exist yet.

- [ ] **Step 4: Implement `to_proto` / `from_proto` for `Detection` in `services/private/vision.cpp`**

Inspect the proto fields with:

```bash
grep -nE "set_x_min|x_min\(\)|has_x_min|x_min_normalized" \
  src/viam/api/service/vision/v1/vision.pb.h | head -40
```

Check whether the optional fields use `optional double x_min = N` (proto3 with `optional`, generates `has_x_min()` + `_internal_x_min()`) or proto3 default semantics. Implement `to_proto` to only call setters for engaged optionals; implement `from_proto` to read with `has_*` guards.

```cpp
// services/private/vision.cpp (skeleton)
#include <viam/sdk/services/private/vision.hpp>

namespace viam {
namespace sdk {
namespace impl {
namespace vision {

namespace vpb = ::viam::service::vision::v1;

vpb::Detection to_proto(const Vision::detection& d) {
    vpb::Detection out;
    if (d.x_min) out.set_x_min(*d.x_min);
    if (d.y_min) out.set_y_min(*d.y_min);
    if (d.x_max) out.set_x_max(*d.x_max);
    if (d.y_max) out.set_y_max(*d.y_max);
    if (d.x_min_normalized) out.set_x_min_normalized(*d.x_min_normalized);
    if (d.y_min_normalized) out.set_y_min_normalized(*d.y_min_normalized);
    if (d.x_max_normalized) out.set_x_max_normalized(*d.x_max_normalized);
    if (d.y_max_normalized) out.set_y_max_normalized(*d.y_max_normalized);
    if (d.class_name) out.set_class_name(*d.class_name);
    if (d.confidence) out.set_confidence(*d.confidence);
    return out;
}

Vision::detection from_proto(const vpb::Detection& p) {
    Vision::detection out;
    if (p.has_x_min()) out.x_min = p.x_min();
    if (p.has_y_min()) out.y_min = p.y_min();
    if (p.has_x_max()) out.x_max = p.x_max();
    if (p.has_y_max()) out.y_max = p.y_max();
    if (p.has_x_min_normalized()) out.x_min_normalized = p.x_min_normalized();
    if (p.has_y_min_normalized()) out.y_min_normalized = p.y_min_normalized();
    if (p.has_x_max_normalized()) out.x_max_normalized = p.x_max_normalized();
    if (p.has_y_max_normalized()) out.y_max_normalized = p.y_max_normalized();
    if (p.has_class_name()) out.class_name = p.class_name();
    if (p.has_confidence()) out.confidence = p.confidence();
    return out;
}

// (other conversions added in subsequent steps)

}}}}  // namespace
```

Add `services/private/vision.cpp` to `viamsdk` target in `src/viam/sdk/CMakeLists.txt`.

- [ ] **Step 5: Run tests; expect Detection cases to pass**

```bash
cmake --build build --target test_vision
./build/src/viam/sdk/tests/test_vision --run_test=vision_proto_conv/detection_round_trip_pixel_only
./build/src/viam/sdk/tests/test_vision --run_test=vision_proto_conv/detection_round_trip_normalized_only
./build/src/viam/sdk/tests/test_vision --run_test=vision_proto_conv/detection_round_trip_no_bbox
```

Expected: all three pass.

- [ ] **Step 6: Add round-trip cases for `Classification`, then implement; run; pass**

Test (append to `test_vision.cpp`):

```cpp
BOOST_AUTO_TEST_CASE(classification_round_trip) {
    Vision::classification c{"cat", 0.95};
    const vpb::Classification proto = vimpl::to_proto(c);
    const Vision::classification back = vimpl::from_proto(proto);
    BOOST_TEST(back == c);
}
```

Implement in `services/private/vision.cpp`:

```cpp
vpb::Classification to_proto(const Vision::classification& c) {
    vpb::Classification out;
    out.set_class_name(c.class_name);
    out.set_confidence(c.confidence);
    return out;
}

Vision::classification from_proto(const vpb::Classification& p) {
    return Vision::classification{p.class_name(), p.confidence()};
}
```

Build, run, expect pass.

- [ ] **Step 7: Add round-trip cases for `properties` and `point_cloud_object`, implement, pass**

For `point_cloud_object`, the proto type is `viam::common::v1::PointCloudObject`. Inspect with:

```bash
grep -nE "(set_point_cloud|point_cloud\(\)|geometries|add_geometries)" \
  src/viam/api/common/v1/common.pb.h | head -20
```

The proto stores point cloud bytes in a `bytes point_cloud` field (no mime type at the proto level — Vision passes `mime_type` separately as the `mime_type` request field; the response `PointCloudObject` only carries bytes + geometries). Set `point_cloud_object::point_cloud.mime_type` to the request mime_type when converting (responsibility lives in the client/server layer, not the helper). The helper only converts the proto's bytes into `point_cloud_object::point_cloud.pc` and the proto's geometries into `std::vector<GeometryConfig>`.

For converting `repeated Geometry geometries` ↔ `std::vector<GeometryConfig>`, look for an existing helper:

```bash
grep -rn "to_repeated_field\|from_repeated_field\|GeometryConfig.*from_proto" src/viam/sdk/spatialmath/ src/viam/sdk/components/ 2>/dev/null | head
```

Use whatever exists. If nothing fits, do a manual loop.

Test cases to add:

```cpp
BOOST_AUTO_TEST_CASE(properties_round_trip) {
    Vision::properties p{true, false, true};
    vpb::GetPropertiesResponse proto;
    vimpl::to_proto(p, &proto);
    BOOST_TEST(vimpl::from_proto(proto) == p);
}

BOOST_AUTO_TEST_CASE(point_cloud_object_round_trip_no_geometries) {
    Vision::point_cloud_object o;
    o.point_cloud.mime_type = "application/pcd";
    o.point_cloud.pc = {0xDE, 0xAD, 0xBE, 0xEF};
    // geometries empty
    ::viam::common::v1::PointCloudObject proto;
    vimpl::to_proto(o, &proto);
    auto back = vimpl::from_proto(proto);
    // Note: mime_type round-trip is the caller's responsibility (client/server),
    // so don't assert on it here. Only assert bytes + geometries.
    BOOST_TEST(back.point_cloud.pc == o.point_cloud.pc);
    BOOST_TEST(back.geometries.empty());
}
```

Add `point_cloud_object_round_trip_with_geometries` if `GeometryConfig` is comparable / constructible from a known fixture (skip if too painful — geometry round-trip is exercised by camera tests already).

- [ ] **Step 8: Commit**

```bash
git add src/viam/sdk/services/private/vision.{hpp,cpp} \
        src/viam/sdk/tests/test_vision.cpp \
        src/viam/sdk/tests/CMakeLists.txt \
        src/viam/sdk/CMakeLists.txt
git commit -m "feat(sdk): add Vision proto-conv helpers + round-trip tests

Adds free-function to_proto/from_proto helpers for Detection,
Classification, properties, and point_cloud_object in
viam::sdk::impl::vision. Tests cover engaged/unengaged optional bbox
fields on Detection, classification basics, properties, and
point_cloud_object bytes."
```

---

### Task 4: Create `VisionClient` skeleton + smoke test

**Why before per-RPC tests:** The mock+test infra references the client; we need the client to exist (even if methods throw) for the build graph to compile.

**Files:**
- Create: `src/viam/sdk/services/private/vision_client.hpp`, `src/viam/sdk/services/private/vision_client.cpp`
- Modify: `src/viam/sdk/CMakeLists.txt`

- [ ] **Step 1: Read `mlmodel_client.hpp` + `mlmodel_client.cpp` to understand the registration pattern**

In particular note:
- `using interface_type = MLModelService;` and `using service_type = ...;` typedefs at the top of the class
- `using MLModelService::infer;` lines that pull the no-extra overload into the client's namespace
- The constructor signature `(std::string name, const ViamChannel& channel)`
- The `make_client_helper(this, *stub_, &service_type::StubInterface::Method).invoke(...)` pattern for simple RPCs (used in `get_status` at the bottom of `mlmodel_client.cpp`)
- The static registration block at the bottom of `mlmodel_client.cpp` (look for any `Registry` calls). If none exists in `mlmodel_client.cpp` directly, search for where `MLModelService` is registered:

```bash
grep -rn "MLModelService\|MLModelServiceClient" src/viam/sdk/registry/ src/viam/sdk/robot/ 2>/dev/null | head
```

Replicate that registration shape for `Vision` / `VisionClient`.

- [ ] **Step 2: Create `vision_client.hpp`**

```cpp
#pragma once

#include <viam/api/service/vision/v1/vision.grpc.pb.h>

#include <viam/sdk/rpc/dial.hpp>
#include <viam/sdk/services/vision.hpp>

namespace viam {
namespace sdk {
namespace impl {

class VisionClient : public Vision {
   public:
    using interface_type = Vision;
    using service_type = ::viam::service::vision::v1::VisionService;

    VisionClient(std::string name, const ViamChannel& channel);

    const ViamChannel& channel() const { return *channel_; }

    std::vector<Vision::detection> get_detections_from_camera(const std::string& camera_name,
                                                              const ProtoStruct& extra) override;
    std::vector<Vision::detection> get_detections(const Vision::raw_image& image,
                                                  const ProtoStruct& extra) override;
    std::vector<Vision::classification> get_classifications_from_camera(
        const std::string& camera_name, int count, const ProtoStruct& extra) override;
    std::vector<Vision::classification> get_classifications(const Vision::raw_image& image,
                                                             int count,
                                                             const ProtoStruct& extra) override;
    std::vector<Vision::point_cloud_object> get_object_point_clouds(
        const std::string& camera_name, const std::string& mime_type,
        const ProtoStruct& extra) override;
    Vision::properties get_properties(const ProtoStruct& extra) override;
    Vision::capture_all_result capture_all_from_camera(const std::string& camera_name,
                                                       const Vision::capture_options& opts,
                                                       const ProtoStruct& extra) override;
    ProtoStruct do_command(const ProtoStruct& command) override;
    ProtoStruct get_status() override;

    using Vision::get_detections_from_camera;
    using Vision::get_detections;
    using Vision::get_classifications_from_camera;
    using Vision::get_classifications;
    using Vision::get_object_point_clouds;
    using Vision::get_properties;
    using Vision::capture_all_from_camera;

   private:
    std::unique_ptr<service_type::StubInterface> stub_;
    const ViamChannel* channel_;
};

}  // namespace impl
}  // namespace sdk
}  // namespace viam
```

- [ ] **Step 3: Create `vision_client.cpp` with stub bodies that throw `std::runtime_error("not implemented")`**

Constructor:

```cpp
VisionClient::VisionClient(std::string name, const ViamChannel& channel)
    : Vision(std::move(name)),
      stub_(service_type::NewStub(channel.channel())),
      channel_(&channel) {}
```

Each override: `throw std::runtime_error("not implemented");`. We fill these in Phase 2.

- [ ] **Step 4: Add the static registration block matching mlmodel's**

Copy whatever pattern Step 1 surfaced. Likely a `static auto _registered = ...;` line near the bottom of `vision_client.cpp` registering `Vision::api()` → factory producing `VisionClient`.

- [ ] **Step 5: Add `services/private/vision_client.cpp` to `viamsdk` target in CMakeLists.txt**

- [ ] **Step 6: Build**

```bash
cmake --build build --target viamsdk
```

Expected: success.

- [ ] **Step 7: Commit**

```bash
git add src/viam/sdk/services/private/vision_client.{hpp,cpp} src/viam/sdk/CMakeLists.txt
git commit -m "feat(sdk): add VisionClient skeleton (all methods throw)

Adds the Vision gRPC client class with all 9 RPC overrides as
not-implemented stubs and the channel-side registration. RPC bodies
are filled in subsequent commits."
```

---

### Task 5: Create `VisionServer` skeleton

**Files:**
- Create: `src/viam/sdk/services/private/vision_server.hpp`, `src/viam/sdk/services/private/vision_server.cpp`
- Modify: `src/viam/sdk/CMakeLists.txt`

- [ ] **Step 1: Read `mlmodel_server.{hpp,cpp}` for the structure**

Key: `class MLModelServiceServer : public ResourceServer, public viam::service::mlmodel::v1::MLModelService::Service` — multiple inheritance from `ResourceServer` and the gRPC `::Service`. Each RPC override uses `make_service_helper<MLModelService>("...", this, context, request)([&](auto& helper, auto& service) { ... })`.

- [ ] **Step 2: Create `vision_server.hpp`**

```cpp
#pragma once

#include <viam/api/service/vision/v1/vision.grpc.pb.h>

#include <viam/sdk/resource/resource_server.hpp>
#include <viam/sdk/services/vision.hpp>

namespace viam {
namespace sdk {
namespace impl {

class VisionServer : public ResourceServer,
                     public ::viam::service::vision::v1::VisionService::Service {
   public:
    explicit VisionServer(std::shared_ptr<ResourceManager> manager);

    ::grpc::Status GetDetectionsFromCamera(::grpc::ServerContext*,
                                           const ::viam::service::vision::v1::GetDetectionsFromCameraRequest*,
                                           ::viam::service::vision::v1::GetDetectionsFromCameraResponse*) noexcept override;
    ::grpc::Status GetDetections(::grpc::ServerContext*,
                                 const ::viam::service::vision::v1::GetDetectionsRequest*,
                                 ::viam::service::vision::v1::GetDetectionsResponse*) noexcept override;
    ::grpc::Status GetClassificationsFromCamera(::grpc::ServerContext*,
                                                const ::viam::service::vision::v1::GetClassificationsFromCameraRequest*,
                                                ::viam::service::vision::v1::GetClassificationsFromCameraResponse*) noexcept override;
    ::grpc::Status GetClassifications(::grpc::ServerContext*,
                                      const ::viam::service::vision::v1::GetClassificationsRequest*,
                                      ::viam::service::vision::v1::GetClassificationsResponse*) noexcept override;
    ::grpc::Status GetObjectPointClouds(::grpc::ServerContext*,
                                        const ::viam::service::vision::v1::GetObjectPointCloudsRequest*,
                                        ::viam::service::vision::v1::GetObjectPointCloudsResponse*) noexcept override;
    ::grpc::Status GetProperties(::grpc::ServerContext*,
                                 const ::viam::service::vision::v1::GetPropertiesRequest*,
                                 ::viam::service::vision::v1::GetPropertiesResponse*) noexcept override;
    ::grpc::Status CaptureAllFromCamera(::grpc::ServerContext*,
                                        const ::viam::service::vision::v1::CaptureAllFromCameraRequest*,
                                        ::viam::service::vision::v1::CaptureAllFromCameraResponse*) noexcept override;
    ::grpc::Status DoCommand(::grpc::ServerContext*,
                             const ::viam::common::v1::DoCommandRequest*,
                             ::viam::common::v1::DoCommandResponse*) noexcept override;
    ::grpc::Status GetStatus(::grpc::ServerContext*,
                             const ::viam::common::v1::GetStatusRequest*,
                             ::viam::common::v1::GetStatusResponse*) noexcept override;
};

}  // namespace impl
}  // namespace sdk
}  // namespace viam
```

(Confirm exact request/response message names by grepping the proto header — this list is best-effort from the proto class names already verified.)

- [ ] **Step 3: Create `vision_server.cpp` with stub overrides returning `::grpc::Status(::grpc::UNIMPLEMENTED, "not yet")` for each**

Keep the constructor body simple: `: ResourceServer(std::move(manager)) {}`.

- [ ] **Step 4: Add registration matching `MLModelServiceServer`**

```bash
grep -rn "MLModelServiceServer" src/viam/sdk/registry/ src/viam/sdk/rpc/ src/viam/sdk/robot/ 2>/dev/null | head
```

Replicate for Vision.

- [ ] **Step 5: Add to CMakeLists, build, commit**

```bash
cmake --build build --target viamsdk
git add src/viam/sdk/services/private/vision_server.{hpp,cpp} src/viam/sdk/CMakeLists.txt
git commit -m "feat(sdk): add VisionServer skeleton (all RPCs UNIMPLEMENTED)"
```

---

### Task 6: Create `MockVision` skeleton + wire into test library

**Files:**
- Create: `src/viam/sdk/tests/mocks/mock_vision.hpp`, `src/viam/sdk/tests/mocks/mock_vision.cpp`
- Modify: `src/viam/sdk/tests/CMakeLists.txt`

- [ ] **Step 1: Read `mocks/mock_motion.{hpp,cpp}` to model the shape**

Note: simple inheritance, public canned-data members, methods record the last call's args + return canned data.

- [ ] **Step 2: Create `mock_vision.hpp`**

```cpp
#pragma once

#include <string>
#include <vector>

#include <viam/sdk/services/vision.hpp>

namespace viam {
namespace sdk {

class MockVision : public Vision {
   public:
    explicit MockVision(std::string name = "mock-vision");

    // Canned responses (set per-test).
    std::vector<detection> canned_detections;
    std::vector<classification> canned_classifications;
    std::vector<point_cloud_object> canned_objects;
    properties canned_properties{false, false, false};
    capture_all_result canned_capture_all;
    ProtoStruct canned_status;

    // Last-call records (used to assert request fields round-trip).
    std::string last_camera_name;
    int last_count = 0;
    std::string last_mime_type;
    raw_image last_image;
    capture_options last_capture_options;
    ProtoStruct last_extra;
    ProtoStruct last_command;

    // Pluggable hook for the exception-mapping test.
    bool throw_on_next_call = false;

    std::vector<detection> get_detections_from_camera(const std::string& camera_name,
                                                      const ProtoStruct& extra) override;
    std::vector<detection> get_detections(const raw_image& image,
                                          const ProtoStruct& extra) override;
    std::vector<classification> get_classifications_from_camera(const std::string& camera_name,
                                                                 int count,
                                                                 const ProtoStruct& extra) override;
    std::vector<classification> get_classifications(const raw_image& image, int count,
                                                     const ProtoStruct& extra) override;
    std::vector<point_cloud_object> get_object_point_clouds(const std::string& camera_name,
                                                             const std::string& mime_type,
                                                             const ProtoStruct& extra) override;
    properties get_properties(const ProtoStruct& extra) override;
    capture_all_result capture_all_from_camera(const std::string& camera_name,
                                               const capture_options& opts,
                                               const ProtoStruct& extra) override;
    ProtoStruct do_command(const ProtoStruct& command) override;
    ProtoStruct get_status() override;
};

}  // namespace sdk
}  // namespace viam
```

- [ ] **Step 3: Create `mock_vision.cpp` with straightforward implementations that record args + return canned data**

```cpp
#include "mock_vision.hpp"
#include <viam/sdk/common/exception.hpp>

namespace viam {
namespace sdk {

MockVision::MockVision(std::string name) : Vision(std::move(name)) {}

std::vector<Vision::detection> MockVision::get_detections_from_camera(const std::string& camera_name, const ProtoStruct& extra) {
    if (throw_on_next_call) { throw_on_next_call = false; throw Exception("mock failure"); }
    last_camera_name = camera_name;
    last_extra = extra;
    return canned_detections;
}

// ... one method per virtual following the same pattern ...

ProtoStruct MockVision::do_command(const ProtoStruct& command) {
    if (throw_on_next_call) { throw_on_next_call = false; throw Exception("mock failure"); }
    last_command = command;
    return command;  // echo
}

ProtoStruct MockVision::get_status() {
    if (throw_on_next_call) { throw_on_next_call = false; throw Exception("mock failure"); }
    return canned_status;
}

}  // namespace sdk
}  // namespace viam
```

- [ ] **Step 4: Add `mocks/mock_vision.cpp` to the `target_sources(viamsdk_test PRIVATE ...)` block in `tests/CMakeLists.txt`**

Add alphabetically between `mock_motion.cpp` and `mock_movement_sensor.cpp`.

- [ ] **Step 5: Build the test target**

```bash
cmake --build build --target test_vision
```

Expected: success. Mock is unused by tests so far; it just needs to compile and link.

- [ ] **Step 6: Commit**

```bash
git add src/viam/sdk/tests/mocks/mock_vision.{hpp,cpp} src/viam/sdk/tests/CMakeLists.txt
git commit -m "test(sdk): add MockVision skeleton wired into viamsdk_test"
```

---

## Phase 2: Per-RPC TDD

Each task here implements one RPC end-to-end (proto-conv → server → client → test). Steps follow strict red/green/commit.

### Test infrastructure helper (one-time)

Before the per-RPC tasks, factor a helper at the top of `test_vision.cpp` that stands up a server+client pair backed by a `MockVision`. This avoids repeating boilerplate per case.

- [ ] **Step 1: Add a `vision_test_fixture` to `test_vision.cpp`**

Read how `test_motion.cpp` or `test_mlmodel.cpp` connects a mock-backed in-process server to a client. Mirror the helper. Typical shape:

```cpp
#include <viam/sdk/tests/mocks/mock_vision.hpp>
// + whatever in-process server bootstrap helpers exist

struct vision_fixture {
    std::shared_ptr<MockVision> mock;
    std::shared_ptr<Vision> client;   // Vision&-shaped client over an in-process channel
    // ... whatever resource_manager / channel scaffolding the peer tests use
};
```

If no shared bootstrap helper exists, look at how `test_mlmodel.cpp` does it (likely uses `mock_robot` + `Server` setup) and copy.

- [ ] **Step 2: Build, ensure helper compiles (no tests using it yet)**

- [ ] **Step 3: Commit (small, isolated)**

```bash
git commit -m "test(sdk): add vision_fixture helper for end-to-end RPC tests"
```

---

### Task 7: Implement `get_properties`

Smallest RPC — no nested types. Tests the round-trip wiring.

- [ ] **Step 1: Add a failing round-trip test**

```cpp
BOOST_AUTO_TEST_CASE(get_properties_round_trip) {
    vision_fixture f;
    f.mock->canned_properties = {true, false, true};
    auto got = f.client->get_properties();
    BOOST_TEST(got == f.mock->canned_properties);
    BOOST_TEST(f.mock->last_extra.empty());
}

BOOST_AUTO_TEST_CASE(get_properties_passes_extra) {
    vision_fixture f;
    f.mock->canned_properties = {false, true, false};
    ProtoStruct extra;
    extra["k"] = ProtoValue("v");
    auto got = f.client->get_properties(extra);
    BOOST_TEST(got == f.mock->canned_properties);
    BOOST_TEST(f.mock->last_extra["k"].get<std::string>() == "v");  // spelling depends on ProtoValue API
}
```

Run: expect failure (client throws "not implemented").

- [ ] **Step 2: Implement `VisionClient::get_properties`** using `make_client_helper`

```cpp
Vision::properties VisionClient::get_properties(const ProtoStruct& extra) {
    return make_client_helper(this, *stub_, &service_type::StubInterface::GetProperties)
        .with_extra(extra)
        .invoke([](auto& response) { return vision::from_proto(response); });
}
```

(Verify `with_extra` is the right helper name by reading `client_helper.hpp` and how `mlmodel_client.cpp:get_status` uses it. Adjust if the API is different.)

- [ ] **Step 3: Implement `VisionServer::GetProperties`**

```cpp
::grpc::Status VisionServer::GetProperties(
    ::grpc::ServerContext* context,
    const vpb::GetPropertiesRequest* request,
    vpb::GetPropertiesResponse* response) noexcept {
    return make_service_helper<Vision>("VisionServer::GetProperties", this, context, request)(
        [&](auto& helper, auto& vs) {
            const auto props = vs->get_properties(helper.getExtra());
            vision::to_proto(props, response);
        });
}
```

- [ ] **Step 4: Run tests, expect pass; commit**

```bash
./build/src/viam/sdk/tests/test_vision --run_test=*get_properties*
git add ... && git commit -m "feat(sdk): implement Vision::get_properties end-to-end"
```

---

### Task 8: Implement `get_status`

- [ ] **Step 1: Add round-trip test (mirror mlmodel's `get_status` test)**
- [ ] **Step 2: Implement `VisionClient::get_status` using `make_client_helper` + `from_proto(result())`** (same shape as `mlmodel_client.cpp:164`)
- [ ] **Step 3: Implement `VisionServer::GetStatus`** (same shape as `mlmodel_server.cpp:175`)
- [ ] **Step 4: Run, pass, commit**

---

### Task 9: Implement `do_command`

- [ ] **Step 1: Add round-trip test asserting a non-trivial nested ProtoStruct echoes back unchanged**
- [ ] **Step 2: Implement client + server using existing `do_command` helpers**

Look for shared `do_command` machinery:

```bash
grep -rn "DoCommand\|do_command" src/viam/sdk/common/ 2>/dev/null | head
```

There's likely a helper that wraps the entire RPC; if so, both client and server become one-liners. Mirror what `mlmodel_client.cpp` / `mlmodel_server.cpp` do for DoCommand (verify they actually have it; if not, read `motion_*` instead).

- [ ] **Step 3: Run, pass, commit**

---

### Task 10: Implement `get_classifications` + `get_classifications_from_camera`

These share the `Classification` proto-conv (already done in Task 3) and a `count` parameter.

- [ ] **Step 1: Tests**

```cpp
BOOST_AUTO_TEST_CASE(get_classifications_from_camera_round_trip) {
    vision_fixture f;
    f.mock->canned_classifications = {{"cat", 0.9}, {"dog", 0.1}};
    auto got = f.client->get_classifications_from_camera("cam0", 5);
    BOOST_TEST(got.size() == 2u);
    BOOST_TEST(got[0] == f.mock->canned_classifications[0]);
    BOOST_TEST(f.mock->last_camera_name == "cam0");
    BOOST_TEST(f.mock->last_count == 5);
}

BOOST_AUTO_TEST_CASE(get_classifications_round_trip) {
    vision_fixture f;
    f.mock->canned_classifications = {{"box", 0.7}};
    Vision::raw_image img;
    img.mime_type = "image/jpeg";
    img.bytes = {1, 2, 3, 4};
    auto got = f.client->get_classifications(img, 3);
    BOOST_TEST(got.size() == 1u);
    BOOST_TEST(got[0] == f.mock->canned_classifications[0]);
    BOOST_TEST(f.mock->last_image.bytes == img.bytes);
    BOOST_TEST(f.mock->last_image.mime_type == img.mime_type);
    BOOST_TEST(f.mock->last_count == 3);
}
```

- [ ] **Step 2: Implement client methods**

`get_classifications_from_camera`: pack `name`, `count`, optional mime_type if the proto has one (check), `extra`. Invoke stub. Read repeated `Classification` from response, convert each via helper, return vector.

`get_classifications`: same but pack `image` bytes + mime_type (+ width/height — pass 0 if not known; document this in the method's docstring).

- [ ] **Step 3: Implement server methods**

Pull `name`/`count`/`image` out of request, call `vs->get_classifications_*(...)`, pack results.

- [ ] **Step 4: Run, pass, commit**

---

### Task 11: Implement `get_detections` + `get_detections_from_camera`

Shape mirrors Task 10 but with `Detection` (which has the optional bbox fields). Add at least one explicit test for the "neither pixel nor normalized bbox" case round-tripping correctly through the wire.

- [ ] **Step 1: Tests including `detection_no_bbox_round_trip_over_wire`**
- [ ] **Step 2: Implement client + server**
- [ ] **Step 3: Run, pass, commit**

---

### Task 12: Implement `get_object_point_clouds`

- [ ] **Step 1: Test round-trip with empty geometries; another with non-empty geometries (use a fixture from existing camera tests if available)**

Important assertion: the `mime_type` from the request must populate `point_cloud_object::point_cloud.mime_type` on the response side, since the proto doesn't carry it on PointCloudObject itself.

- [ ] **Step 2: Implement client (pack name + mime_type, unpack repeated PointCloudObject, set each result's `point_cloud.mime_type` from the request mime_type)**

- [ ] **Step 3: Implement server (read name + mime_type, call `vs->get_object_point_clouds(...)`, pack each `point_cloud_object` into a proto `PointCloudObject` — using the helper for bytes + geometries)**

- [ ] **Step 4: Run, pass, commit**

---

### Task 13: Implement `capture_all_from_camera`

Largest RPC. Uses everything: image (via `raw_image.hpp` helper from Task 1), detections, classifications, objects, extra.

- [ ] **Step 1: Tests covering**
  - Only `return_image=true` → response has image, all vectors empty
  - Only `return_detections=true` → response has detections, no image
  - All four flags true → all populated
  - `extra` round-trip

- [ ] **Step 2: Implement client**

Build request with name + the four bool flags (or however the proto names them — verify with grep on `CaptureAllFromCameraRequest`). On response, conditionally populate each output member using `has_image()`, vector sizes, etc.

- [ ] **Step 3: Implement server**

Read flags from request, call `vs->capture_all_from_camera(name, opts, extra)`, conditionally populate response fields based on what the user impl returned.

- [ ] **Step 4: Run, pass, commit**

---

### Task 14: Server-side exception → grpc::Status mapping test

- [ ] **Step 1: Add a test that sets `mock->throw_on_next_call = true` before any RPC, asserts the client receives a non-OK `grpc::Status`** (the helper machinery should handle this automatically — confirm by checking what `make_service_helper` does on exception in `service_helper.cpp`)

- [ ] **Step 2: If the helper doesn't already wrap exceptions, add the catch ladder verbatim from `mlmodel_server.cpp`**

- [ ] **Step 3: Run, pass, commit**

---

## Phase 3: Template + finalization

### Task 15: Verify auto-generated module-generator template

The CI workflow `update-templates.yml` runs `cl_gen` against changed `services/*.hpp` files on push to `main`. We want to confirm locally that the template it produces for `vision.hpp` will compile.

- [ ] **Step 1: Read `update-templates.yml` to find the `cl_gen` invocation**

```bash
sed -n '/cl_gen/,/^- /p' .github/workflows/update-templates.yml | head -30
```

- [ ] **Step 2: Run `cl_gen` locally if available**

```bash
which cl_gen
# If not installed locally, follow the workflow's install steps (build from cl_gen source) — or skip this step and rely on CI.
```

If `cl_gen` is available, run it against the build dir and `services/vision.cpp`:

```bash
cl_gen -p ./build -d res/module_generator/_templates src/viam/sdk/services/vision.cpp
```

- [ ] **Step 3: If templates were generated, smoke-build a module from them**

```bash
# Use the resulting vision.{hpp,cpp}.in by templating it (manual sed for {{ }} placeholders) and compile against the installed SDK.
# If this step is too involved for a unit-of-work commit, defer to manual verification post-merge.
```

- [ ] **Step 4: Hand-write fallback (only if Step 2/3 surface a problem)**

Mirror `res/module_generator/_templates/services/mlmodel.{hpp,cpp}.in` exactly, substituting `Vision` for `Mlmodel`:

```cpp
// res/module_generator/_templates/services/vision.hpp.in
#pragma once

#include <viam/sdk/common/proto_value.hpp>
#include <viam/sdk/config/resource.hpp>
#include <viam/sdk/module/service.hpp>
#include <viam/sdk/services/vision.hpp>

namespace {{ .ModuleSnake }} {

class {{ .ModelPascal }} : public viam::sdk::Vision {
public:
    {{ .ModelPascal }}(const viam::sdk::Dependencies& deps, const viam::sdk::ResourceConfig& cfg);
    static std::vector<std::string> validate(const viam::sdk::ResourceConfig& cfg);
};

} // namespace {{ .ModuleSnake }}
```

```cpp
// res/module_generator/_templates/services/vision.cpp.in
#include "{{ .ModelSnake }}.hpp"

#include <stdexcept>

namespace {{ .ModuleSnake }} {

{{ .ModelPascal }}::{{ .ModelPascal }}(const viam::sdk::Dependencies& deps, const viam::sdk::ResourceConfig& cfg)
    : Vision(cfg.name()) {
    throw std::runtime_error("constructor not implemented");
}

std::vector<std::string> {{ .ModelPascal }}::validate(const viam::sdk::ResourceConfig& cfg) {
    throw std::runtime_error("\"validate\" not implemented");
}

} // namespace {{ .ModuleSnake }}
```

- [ ] **Step 5: Commit (only if templates were hand-written)**

```bash
git add res/module_generator/_templates/services/vision.{hpp,cpp}.in
git commit -m "feat(generator): add Vision service module template"
```

If `cl_gen` produced them automatically and they compile, skip this commit — the workflow will produce them via its own automated PR after merge.

---

### Task 16: Final verification

- [ ] **Step 1: Full build**

```bash
cmake --build build
```

- [ ] **Step 2: Full test**

```bash
ctest --test-dir build --output-on-failure
```

Expected: all tests pass, including `test_vision` and unchanged peer tests.

- [ ] **Step 3: Lint**

```bash
clang-format --dry-run --Werror src/viam/sdk/services/vision.{hpp,cpp} \
    src/viam/sdk/services/private/vision*.{hpp,cpp} \
    src/viam/sdk/common/private/raw_image.{hpp,cpp} \
    src/viam/sdk/tests/mocks/mock_vision.{hpp,cpp} \
    src/viam/sdk/tests/test_vision.cpp
clang-tidy -p build src/viam/sdk/services/vision.cpp src/viam/sdk/services/private/vision*.cpp
```

- [ ] **Step 4: Doxygen**

```bash
cmake --build build --target doc 2>&1 | grep -i "warning\|error" | grep -i vision
```

Expected: no Vision-specific warnings.

- [ ] **Step 5: Open PR with description containing**

- Summary of what's added (per spec).
- **Explicit note** about the intentional `point_cloud_object::point_cloud` field/type name shadow — mirrors the proto field; flagged here so reviewers don't take it for an oversight.
- Cross-reference to `docs/superpowers/specs/2026-05-08-vision-service-sdk-and-template-design.md`.
- A note that the module-generator template was either hand-written (Task 15) or will appear via the automated workflow PR after merge.

---

## Build setup (if needed)

If you don't have a build directory yet:

```bash
mkdir -p build && cd build
cmake .. -G Ninja \
  -DVIAMCPPSDK_BUILD_TESTS=ON \
  -DVIAMCPPSDK_BUILD_EXAMPLES=OFF
cd ..
```

For the `cl_gen` smoke test in Task 15, the workflow uses Debian-bookworm + clang-15. If your local environment differs, defer that step to CI.

---

## Notes for the implementer

- **Don't add comments** explaining what code does. Names and signatures should carry the meaning. Only add comments where the *why* is non-obvious — e.g., the `point_cloud_object::point_cloud` shadow gets a comment because it's a deliberate match to the proto field name.
- **Don't introduce new abstractions** beyond what the spec calls for. If you find yourself reaching for a "VisionPayloadConverterFactory" or similar, stop — match the existing service patterns instead.
- **TDD discipline**: red → green → commit, in small slices. Don't write a green test before you've seen it red. Don't bundle multiple RPCs into one commit.
- **Follow existing patterns over instinct.** When in doubt, read what `mlmodel`, `motion`, or `navigation` did and copy the shape. The codebase has strong conventions and the goal is to make Vision look like it always belonged.
