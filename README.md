# lumen-cappuccino game

```bash
# to build from source, you need cmake>=3.23 and conan

# for debug build
conan install . --output-folder=build --build=missing -s "&:build_type=Debug" -s build_type=Release
cmake --preset conan-debug
cmake --build --preset conan-debug

# for release build
conan install . --output-folder=build --build=missing
cmake --preset conan-release
cmake --build --preset conan-release

# to run the game
./build/build/Release/src/lumen-cappucino
# or on wayland gnome if no borders
SDL_VIDEODRIVER=x11 ./build/build/Release/src/lumen-cappucino

# for testing
ctest --test-dir build/build/Release/src



```
