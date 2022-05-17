sudo pip3 install setuptools
sudo pip3 install conan
conan profile new default --detect
conan profile update settings.compiler.libcxx=libstdc++11 default