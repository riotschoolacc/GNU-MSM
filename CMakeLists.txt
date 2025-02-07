# CMakeLists.txt
cmake_minimum_required(VERSION 3.25)
project(MSM)

set(CMAKE_CXX_STANDARD 17)

include_directories(${PROJECT_SOURCE_DIR}/include)

add_subdirectory(src)
add_executable(MySingingMonsters)
file(GLOB SOURCES "src/*.cpp")

target_sources(MySingingMonsters PRIVATE ${SOURCES})

find_package(SDL2 REQUIRED)
find_package(SDL2_mixer REQUIRED)
find_package(libavif REQUIRED)

# yeah, a workaround
find_package(PkgConfig REQUIRED)
pkg_check_modules(OpenGLES2 REQUIRED glesv2)

include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_MIXER_INCLUDE_DIRS} ${OpenGLES2_INCLUDE_DIRS})

set(LIBRARIES ${SDL2_LIBRARIES} ${SDL2_MIXER_LIBRARIES} ${OpenGLES2_LIBRARIES} avif)

target_link_libraries(MySingingMonsters PRIVATE ${LIBRARIES})
