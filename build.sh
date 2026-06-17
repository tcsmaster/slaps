#!/bin/bash
verify_build_folder_exists(){
	if [ ! -d "build" ];then
		mkdir build
	fi
}

# Check that there is a build folder here.
verify_build_folder_exists

# Navigate into the build folder
pushd build
    # Request that CMake configure itself based on what it finds in the parent folder.
    echo "Configuring CMake with Ninja ..."
    cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -G Ninja ..
	
    # Start the build process.
    echo "Building project with Ninja ..."
    ninja
popd
