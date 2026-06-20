# lumen-cappuccino game

```bash
# to build from source, you need cmake>=3.23 and conan

# for debug build, feel free to create a new conan debug profile
conan install . --output-folder=build --build=missing -s "&:build_type=Debug" -s build_type=Release
cmake --preset conan-debug # or possibly some other name for your debug build
cmake --build --preset conan-debug # or possibly some other name for your debug build

# for release build
conan install . --output-folder=build --build=missing
cmake --preset conan-release # or possibly some other name for your release build
cmake --build --preset conan-release # or possibly some other name for your release build

# to run the game
cd assets

../build/build/Release/src/lumen-cappucino
# or on wayland gnome if no borders
SDL_VIDEODRIVER=x11 ../build/build/Release/src/lumen_cappuccino

```
