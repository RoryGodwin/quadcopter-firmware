# This file sets up pico-sdk-init() which must be called by the main CMakeLists.txt

# Difine PICO_SDK_PATH if not already defined
if (NOT DEFINED PICO_SDK_PATH)
    get_filename_component(PICO_SDK_PATH "$CMAKE_CURRENT_LIST_DIR}/pick-sdk" ABSOLUTE)
endif()
 

# Print path for debug
message("Using PICO SDK from: ${PICO_SDK_PATH}")

# Add the SDK
include("${PICO_SDK_PATH}/pico_sdk_init.cmake")