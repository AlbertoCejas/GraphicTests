cd ..
mkdir build

cmake -S . -B build -G "Visual Studio 16 2019" -DCMAKE_CONFIGURATION_TYPES="Debug;Release;RelWithDebInfo"

cd scripts
