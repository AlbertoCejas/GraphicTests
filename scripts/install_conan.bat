pip3 install setuptools
pip3 install conan
conan profile new default --detect
conan profile update settings.compiler.libcxx=libstdc++11 default

pause