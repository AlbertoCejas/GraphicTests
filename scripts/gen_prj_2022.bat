cd ..
mkdir build

cmake -S . -B build -G "Visual Studio 17 2022" -DCMAKE_CONFIGURATION_TYPES="Debug;Release;RelWithDebInfo"

cd scripts
