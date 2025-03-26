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
