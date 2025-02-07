# CMakeLists.txt
include_directories(${PROJECT_SOURCE_DIR}/include)

file(GLOB SRC_FILES "*.cpp")

add_library(MSMLibrary ${SRC_FILES})