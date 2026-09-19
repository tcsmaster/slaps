#!/bin/bash
set -e

cd "$(dirname "${BASH_SOURCE[0]}")"

verify_build_folder_exists() {
  if [ ! -d "build" ]; then
    mkdir build
  fi
}

source /opt/intel/openvino/setupvars.sh

verify_build_folder_exists

# Navigate into the build folder
pushd build
# Request that CMake configure itself based on what it finds in the parent folder.
echo "Configuring CMake with Ninja ..."
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_CXX_FLAGS="-g" -G Ninja ..

# Start the build process.
echo "Building project with Ninja ..."
ninja
popd
