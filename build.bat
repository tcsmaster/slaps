@echo off
setlocal

rem Check that there is a build folder here; create it if not.
if not exist "build" (
    mkdir build
)

rem Navigate into the build folder
pushd build

    rem Request that CMake configure itself based on what it finds in the parent folder.
    echo Configuring CMake with Ninja ...
    cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -G Ninja -DCMAKE_TOOLCHAIN_FILE=C:/Users/csana/vcpkg/scripts/buildsystems/vcpkg.cmake ..
    if errorlevel 1 (
        echo CMake configuration failed.
        popd
        exit /b 1
    )

    rem Start the build process.
    echo Building project with Ninja ...
    ninja
    if errorlevel 1 (
        echo Ninja build failed.
        popd
        exit /b 1
    )

popd

endlocal
