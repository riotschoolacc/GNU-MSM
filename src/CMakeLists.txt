# CMakeLists.txt
file(GLOB_RECURSE SRC_FILES "**/*.cpp")
file(GLOB_RECURSE HEADER_FILES "**/*.h")

include_directories(${PROJECT_SOURCE_DIR})

add_library(MSMLibrary ${SRC_FILES} ${HEADER_FILES})
