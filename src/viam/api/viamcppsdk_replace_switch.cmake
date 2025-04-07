if (NOT SWITCH_REPLACE_PATHS)
    message(FATAL_ERROR "Please provide SWITCH_REPLACE_PATHS argument to switch replace script")
endif()

foreach(SOURCE ${SWITCH_REPLACE_PATHS})
    file(READ "${SOURCE}" _src_text)
    string(REPLACE "namespace switch " "namespace switch_ " _src_text "${_src_text}")
    string(REPLACE "::switch::" "::switch_::" _src_text "${_src_text}")
    file(WRITE "${SOURCE}" "${_src_text}")
endforeach()
