add_executable(${PROJECT_NAME} main.cpp Controller.cpp Model.cpp
  View.cpp MusicPlayer.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE sdl::sdl SDL3_image::SDL3_image
  SDL3_mixer::SDL3_mixer-static)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/src)

# thanks to the test package of the library
# https://github.com/conan-io/conan-center-index/tree/master/recipes/sdl_ttf/3.x/test_package
if(TARGET SDL3_ttf::SDL3_ttf)
  target_link_libraries(${PROJECT_NAME} PRIVATE SDL3_ttf::SDL3_ttf)
else()
  target_link_libraries(${PROJECT_NAME} PRIVATE SDL3_ttf::SDL3_ttf-static)
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE strict_compile_flags) # always strict
target_link_libraries(${PROJECT_NAME} PRIVATE
  $<$<CONFIG:Debug>:profiling_flags>)
target_link_libraries(${PROJECT_NAME} PRIVATE
  $<$<CONFIG:Debug>:debugging_flags>
)

target_compile_definitions(${PROJECT_NAME} PRIVATE
    $<$<CONFIG:Debug>:LUMEN_LOG>
    $<$<CONFIG:Debug>:LUMEN_STRICT_CHECKS>
)
target_link_libraries(${PROJECT_NAME} PRIVATE
    $<$<CONFIG:Debug>:spdlog::spdlog>
)
