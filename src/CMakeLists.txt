# CMakeLists.txt
file(GLOB_RECURSE SRC_FILES "**/*.cpp")

add_library(MSMLibrary ${SRC_FILES})