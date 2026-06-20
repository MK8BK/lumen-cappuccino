cmake_minimum_required(VERSION 3.23)
project(lumen_cappuccino CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

message(STATUS "CMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}")


# thanks ConanCenter
find_package(SDL3 REQUIRED)
find_package(SDL3_image REQUIRED)
find_package(SDL3_mixer REQUIRED)
find_package(SDL3_ttf REQUIRED CONFIG)
find_package(spdlog REQUIRED)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

enable_testing()

add_library(strict_compile_flags INTERFACE)
if(MSVC)
  target_compile_options(strict_compile_flags INTERFACE
    /W4
    /WX
  )
else()
  target_compile_options(strict_compile_flags INTERFACE
    -Wall
    -Wextra
    -Werror
  )
endif()


function(add_gnu_flag_target target flag)
  add_library(${target} INTERFACE)
  if(NOT MSVC)
    target_compile_options(${target} INTERFACE ${flag})
    target_link_options(${target} INTERFACE ${flag})
  endif()
endfunction()

add_gnu_flag_target(profiling_flags -pg)
add_gnu_flag_target(debugging_flags -g)

add_subdirectory(src)

##try this if version mismatch problem
# target_link_libraries(YOUR_TARGET openssl::openssl)
##### try this if problem with ttf
# find_package(SDL3_ttf REQUIRED CONFIG)
