cmake_minimum_required(VERSION 3.16)
project(AcidTrip LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)

add_executable(AcidTrip main.cpp)

if(TARGET SDL2::SDL2)
  target_link_libraries(AcidTrip PRIVATE SDL2::SDL2)
elseif(TARGET SDL2::SDL2-static)
  target_link_libraries(AcidTrip PRIVATE SDL2::SDL2-static)
else()
  message(FATAL_ERROR "No SDL2 target found. Ensure SDL2 package exports SDL2::SDL2.")
endif()

target_link_libraries(AcidTrip PRIVATE OpenGL::GL)

target_compile_definitions(AcidTrip PRIVATE SDL_MAIN_HANDLED)

if(WIN32)
  target_compile_definitions(AcidTrip PRIVATE NOMINMAX)
endif()

# Keep shaders next to the executable for runtime loading.
add_custom_command(TARGET AcidTrip POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${CMAKE_CURRENT_SOURCE_DIR}/shader.vert
    ${CMAKE_CURRENT_SOURCE_DIR}/shader.frag
    $<TARGET_FILE_DIR:AcidTrip>
)
