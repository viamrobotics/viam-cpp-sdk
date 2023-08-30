# See if the user provided a libviam_rust_utils in the root of the
# tree. If not, try to download the latest.
#
# TODO: Fixup runtime paths at install time.
# TODO: This should be pushed down into a tool
#
# TODO: Eventually, `rust_utils` should be its own package that can be
# pulled in via `find_package`, and this whole thing can be removed.
#
# TODO: When this is removed, also remove the
# `target_link_directories` call down in src/CMakeLists.txt, as it
# will no longer be needed.

set(viam_rust_utils_file ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_SHARED_LIBRARY_PREFIX}viam_rust_utils${CMAKE_SHARED_LIBRARY_SUFFIX})

file(GLOB viam_rust_utils_files ${PROJECT_SOURCE_DIR}/${CMAKE_SHARED_LIBRARY_PREFIX}viam_rust_utils*${CMAKE_SHARED_LIBRARY_SUFFIX})

if (viam_rust_utils_files)
  list(LENGTH viam_rust_utils_files num_viam_rust_utils_files)
  if (num_viam_rust_utils_files GREATER 1)
    message(FATAL_ERROR "Found multiple viam_rust_utils libraries in source tree root, cannot proceed")
  endif()
  FILE(COPY_FILE
    ${viam_rust_utils_files}
    ${viam_rust_utils_file}
    ONLY_IF_DIFFERENT
  )
else()
  set(lvru_system_name ${CMAKE_SYSTEM_NAME})
  if (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    set(lvru_system_name "macosx")
  endif()
  file(
    DOWNLOAD
    https://github.com/viamrobotics/rust-utils/releases/latest/download/${CMAKE_SHARED_LIBRARY_PREFIX}viam_rust_utils-${lvru_system_name}_${CMAKE_SYSTEM_PROCESSOR}${CMAKE_SHARED_LIBRARY_SUFFIX}
    ${viam_rust_utils_file}
    STATUS lvru_status
  )
  list(GET lvru_status 0 lvru_status_code)
  list(GET lvru_status 1 lvru_status_string)

  if(NOT lvru_status_code EQUAL 0)
    message(FATAL_ERROR "No local viam_rust_utils found and failed to download: ${lvru_status_string}")
  endif()

endif()

add_library(viam_rust_utils SHARED IMPORTED)

target_link_directories(viam_rust_utils
  INTERFACE
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>"
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_LIBDIR}>"
)

set_property(TARGET viam_rust_utils PROPERTY IMPORTED_LOCATION ${viam_rust_utils_file})

install(
  IMPORTED_RUNTIME_ARTIFACTS viam_rust_utils
  LIBRARY COMPONENT viam-cpp-sdk_runtime
)
