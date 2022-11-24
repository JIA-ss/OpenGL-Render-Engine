cmake_minimum_required(VERSION 3.0)

project("OpenGL_Study")

set(LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libs)
set(OUTPUT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/build/output)
set(GLAD_DIR ${LIB_DIR}/GLAD)
set(GLFW_DIR ${LIB_DIR}/GLFW)
set(LIBUV_PATH ${LIB_DIR}/libuv)
set(ASSIMP_PATH ${LIB_DIR}/assimp)
set(JSON_PATH ${LIB_DIR}/json)
set(TRACY_PATH ${LIB_DIR}/tracy_profiler)


set(ALL_LIBS 
    GLAD
    uv
    uv_a
    #nlohmann_json
    OpenGL_Study
    tracy_profiler
)


set(TRACY_PROFILER_MACRO "-DTRACY_ENABLE -DTracyGpuCollect -DTRACY_ON_DEMAND -DTRACY_MANUAL_LIFETIME -DTRACY_DELAYED_INIT -DTRACY_NO_FRAME_IMAGE -DTRACY_NO_SYSTEM_TRACING")

set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DDEBUG")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -D_DEBUG ${TRACY_PROFILER_MACRO}")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D_DEBUG ${TRACY_PROFILER_MACRO}")


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


include(createSrcGroups.cmake)

FILE(
    GLOB_RECURSE MAIN_SRC_FILES
    
    #include files
    ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/include/*.cpp

    #source files
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
)

createSrcGroups(MAIN_SRC_FILES)

add_subdirectory(${LIBUV_PATH})
add_subdirectory(${JSON_PATH})
add_subdirectory(${TRACY_PATH})
target_compile_definitions(tracy_profiler PRIVATE TRACY_EXPORTS)

add_library(GLAD ${GLAD_DIR}/src/glad.c)

add_definitions(-D_RESOURCE_PATH_=\"${CMAKE_CURRENT_SOURCE_DIR}/resources\")

add_executable(OpenGL_Study ${MAIN_SRC_FILES})

target_link_libraries(OpenGL_Study GLAD glfw3 opengl32.lib uv uv_a assimp-vc142-mtd nlohmann_json tracy_profiler)
target_link_directories(OpenGL_Study
    PUBLIC ${GLFW_DIR}/lib-vc2019/
    PUBLIC ${ASSIMP_PATH}/lib/
)


target_include_directories(GLAD PUBLIC ${GLAD_DIR}/include)

target_include_directories(OpenGL_Study 
    PUBLIC ${LIB_DIR}
    PUBLIC ${GLAD_DIR}/include
    PUBLIC ${GLFW_DIR}/include
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
    PUBLIC ${LIB_DIR}/glm
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src
    PUBLIC ${LIBUV_PATH}/include
    PUBLIC ${ASSIMP_PATH}/include
    PRIVATE ${JSON_PATH}/include
    PUBLIC ${TRACY_PATH}
)

target_compile_definitions(OpenGL_Study PRIVATE STB_IMAGE_IMPLEMENTATION)


foreach(EACH_LIB ${ALL_LIBS})
set_target_properties(${EACH_LIB} PROPERTIES DEBUG_POSTFIX "_d")
set_target_properties(${EACH_LIB} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIR}/runtime)
set_target_properties(${EACH_LIB} PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${OUTPUT_DIR}/library)
set_target_properties(${EACH_LIB} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${OUTPUT_DIR}/archive)
endforeach(EACH_LIB ${ALL_LIBS})


add_custom_command(TARGET OpenGL_Study POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${ASSIMP_PATH}/lib"
        $<TARGET_FILE_DIR:OpenGL_Study>)