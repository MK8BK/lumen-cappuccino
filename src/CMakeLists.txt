
add_executable(ModelTest ModelTest.cpp Model.cpp)
target_link_libraries(ModelTest PRIVATE doctest::doctest)
add_test(NAME ModelTest COMMAND ModelTest)

add_executable(log log.cpp)
target_link_libraries(log PRIVATE spdlog::spdlog)

add_executable(${PROJECT_NAME} main.cpp Controller.cpp Model.cpp View.cpp)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(${PROJECT_NAME} PRIVATE sdl::sdl SDL3_image::SDL3_image)
target_link_libraries(${PROJECT_NAME} PRIVATE strict_compile_flags) # always strict
target_link_libraries(${PROJECT_NAME} PRIVATE profiling_flags)
target_link_libraries(${PROJECT_NAME} PRIVATE optimized2_compile_flags)
target_compile_definitions(${PROJECT_NAME} PRIVATE
    $<$<CONFIG:Debug>:LUMEN_LOG>
    $<$<CONFIG:Debug>:LUMEN_STRICT_CHECKS>
)
target_link_libraries(${PROJECT_NAME} PRIVATE
    $<$<CONFIG:Debug>:spdlog::spdlog>
)




##################### examples
# target_link_libraries(YOUR_TARGET gtest::gtest)
# target_link_libraries(${PROJECT_NAME} SDL3_image::SDL3_image)
# target_link_libraries(${PROJECT_NAME} SDL3_mixer::SDL3_mixer-static)
# target_link_libraries(${PROJECT_NAME} PRIVATE SDL3_ttf::SDL3_ttf)

# if(TARGET SDL3_ttf::SDL3_ttf)
#   target_link_libraries(YOUR_TARGET PRIVATE SDL3_ttf::SDL3_ttf)
# else()
#   target_link_libraries(YOUR_TARGET  PRIVATE SDL3_ttf::SDL3_ttf-static)
# endif()
