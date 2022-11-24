cmake_minimum_required(VERSION 3.0)
project(tracy_profiler LANGUAGES CXX)
add_library(tracy_profiler SHARED TracyClient.cpp)
target_include_directories(tracy_profiler PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

if (PLATFORM_WIN64)
add_custom_command(TARGET tracy_profiler POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_LINKER_FILE:tracy_profiler> ${ELECTRON_RUNTIME_LIB_DIR}/$<TARGET_LINKER_FILE_NAME:tracy_profiler>
)
endif ()
