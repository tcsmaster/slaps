cmake_minimum_required(VERSION 3.12)

project(slaps)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(ORT_ROOT "$ENV{HOME}/onnxruntime")
set(ORT_BUILD_DIR "${ORT_ROOT}/build/Linux/Release")
set(OV_ROOT "/opt/intel/openvino")

add_library(glad STATIC external/glad/src/glad.c)
target_include_directories(glad PUBLIC external/glad/include)

find_package(OpenGL REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
if(UNIX)
    set(OpenCV_DIR "$ENV{HOME}/build")
elseif(WIN32)
    # vcpkg supplies OpenCV_DIR automatically via the toolchain file — nothing needed here
endif()
find_package(OpenCV REQUIRED)
find_package(OpenVINO REQUIRED)


add_executable(${PROJECT_NAME}
    src/main_2.cpp
    src/PoseEstimator.cpp
    src/particle.cpp
    src/renderer.cpp
    src/application.cpp
    src/camera.cpp
)
# Stop GLFW from pulling in the system GL headers, which clash with glad
target_compile_definitions(${PROJECT_NAME} PRIVATE GLFW_INCLUDE_NONE)
# --- ONNX Runtime include paths ---
target_include_directories(${PROJECT_NAME} PRIVATE
    ${ORT_ROOT}/include
    ${ORT_ROOT}/include/onnxruntime/core/session
    ${ORT_ROOT}/include/onnxruntime/core/providers/openvino
)
 
# --- ONNX Runtime shared library ---
find_library(ONNXRUNTIME_LIB
    NAMES onnxruntime
    PATHS ${ORT_BUILD_DIR}
    NO_DEFAULT_PATH
)
 
if(NOT ONNXRUNTIME_LIB)
    message(FATAL_ERROR "libonnxruntime.so not found in ${ORT_BUILD_DIR} — check ORT_ROOT/ORT_BUILD_DIR paths")
endif()
set(DEPS glad glfw glm::glm OpenGL::GL openvino::runtime)

target_link_libraries(${PROJECT_NAME} PRIVATE ${DEPS})
target_link_libraries(glad PRIVATE ${CMAKE_DL_LIBS})
target_include_directories(${PROJECT_NAME} PRIVATE ${OpenCV_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE ${OpenCV_LIBS})
target_link_libraries(${PROJECT_NAME} PRIVATE ${ONNXRUNTIME_LIB})
 
# --- Make sure the OpenVINO env (setupvars.sh) is sourced before building/running ---
# This just double-checks OV_ROOT looks sane; it does not replace sourcing setupvars.sh.
if(NOT EXISTS "${OV_ROOT}/setupvars.sh")
    message(WARNING "OpenVINO setupvars.sh not found at ${OV_ROOT} — did you source it and set OV_ROOT correctly?")
endif()
 
# --- Embed the rpath so the app finds libonnxruntime.so at runtime without needing LD_LIBRARY_PATH ---
set_target_properties(${PROJECT_NAME} PROPERTIES
    BUILD_RPATH "${ORT_BUILD_DIR}"
    INSTALL_RPATH "${ORT_BUILD_DIR}"
)
 
