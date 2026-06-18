# lumen-cappuccino game

```bash
# to build from source, you need cmake>=3.23 and conan
conan install . --output-folder=build --build=missing
cmake --preset conan-release 
cmake --build --preset conan-release  #--config Debug|Release
./build/build/Release/src/lumen-cappucino
ctest --test-dir build/build/Release/src
```
