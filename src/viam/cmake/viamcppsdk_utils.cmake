function(viamcppsdk_add_boost_test SOURCE_FILE_NAME)
  get_filename_component(TEST_EXECUTABLE_NAME ${SOURCE_FILE_NAME} NAME_WE)

  add_executable(${TEST_EXECUTABLE_NAME} ${SOURCE_FILE_NAME})
  target_link_libraries(${TEST_EXECUTABLE_NAME} viamsdk_test)

  file(READ "${SOURCE_FILE_NAME}" SOURCE_FILE_CONTENTS)
  string(
    REGEX MATCHALL "BOOST_AUTO_TEST_CASE\\( *([A-Za-z_0-9]+) *\\)"
    FOUND_TESTS ${SOURCE_FILE_CONTENTS})

  foreach(HIT ${FOUND_TESTS})
    string(REGEX REPLACE ".*\\( *([A-Za-z_0-9]+) *\\).*" "\\1" TEST_NAME ${HIT})

    add_test(
      NAME "${TEST_EXECUTABLE_NAME}.${TEST_NAME}"
      COMMAND ${TEST_EXECUTABLE_NAME} --catch_system_error=yes)
  endforeach()
endfunction()

function(viamcppsdk_link_viam_api TARGET_LIBRARY)
  if (BUILD_SHARED_LIBS)
    target_link_libraries(${TARGET_LIBRARY} ${ARGV1} viam-cpp-sdk::viamapi)
  else()
    target_link_libraries(${TARGET_LIBRARY} ${ARGV1} "$<LINK_LIBRARY:WHOLE_ARCHIVE,viam-cpp-sdk::viamapi>")
  endif()
endfunction()

# Look for the `buf` command in the usual places, and use it if
# found. If we can't find it, try to download it and use that.
#
# TODO: File an upstream issue with `buf.build` to add
# `find_package` support for it, then use it.
#
function(viamcppsdk_get_buf)
  if (BUF_COMMAND)
    return()
  endif()

  find_program(BUF_COMMAND buf)
  if (BUF_COMMAND)
    return()
  endif()

  set(HOST_SYSTEM_PROCESSOR_FOR_BUF_DOWNLOAD ${CMAKE_HOST_SYSTEM_PROCESSOR})
  if (CMAKE_HOST_WIN32)
    if (HOST_SYSTEM_PROCESSOR_FOR_BUF_DOWNLOAD STREQUAL "AMD64")
      set(HOST_SYSTEM_PROCESSOR_FOR_BUF_DOWNLOAD x86_64)
    elseif (HOST_SYSTEM_PROCESSOR_FOR_BUF_DOWNLOAD STREQUAL "ARM64")
      set(HOST_SYSTEM_PROCESSOR_FOR_BUF_DOWNLOAD arm64)
    else()
      message(FATAL_ERROR "Unknown Windows platform to correct buf download URL: ${HOST_SYSTEM_PROCESSOR_FOR_BUF_DOWNLOAD}")
    endif()
    set (_buf_exe_suffix ".exe")
  endif()

  set(BUF_DOWNLOAD_URL https://github.com/bufbuild/buf/releases/latest/download/buf-${CMAKE_HOST_SYSTEM_NAME}-${HOST_SYSTEM_PROCESSOR_FOR_BUF_DOWNLOAD}${_buf_exe_suffix})

  file(
    DOWNLOAD
    ${BUF_DOWNLOAD_URL}
    ${CMAKE_CURRENT_BINARY_DIR}/buf_latest${_buf_exe_suffix}
    STATUS buf_status
  )

  list(GET buf_status 0 buf_status_code)
  list(GET buf_status 1 buf_status_string)

  if(NOT buf_status_code EQUAL 0)
    message(FATAL_ERROR "No local `buf` program found (try setting PATH?) and failed to download: ${buf_status_string} for ${BUF_DOWNLOAD_URL}")
  endif()

  set(BUF_COMMAND ${CMAKE_CURRENT_BINARY_DIR}/buf_latest${_buf_exe_suffix} CACHE INTERNAL "buf command")
  file(CHMOD ${BUF_COMMAND} PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE)
endfunction()
