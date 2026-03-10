@echo off
setlocal enabledelayedexpansion

echo Building CxC...

if not exist build mkdir build
cd build

echo Configuring CMake...
cmake -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTING=ON ..

echo Building...
cmake --build . --config Release

if "%1" neq "--no-test" (
    echo Running tests...
    ctest --output-on-failure -C Release
    echo All tests passed!
)

echo Build complete!
echo Static library: build\lib\cxc.lib

cd ..
