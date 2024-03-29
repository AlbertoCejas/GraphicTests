from   conans       import ConanFile, CMake, tools
from   conans.tools import download, unzip
import os

class Project(ConanFile):
    name            = "Potato Engine"
    description     = "Conan package for Potato Engine."
    version         = "0.0.1"                
    url             = "https://github.com/AlbertoCejas/PotatoEngine"
    settings        = "arch", "build_type", "compiler", "os"
    generators      = "cmake"
    requires        = (("nlohmann_json/3.9.1"),
                       ("entt/3.5.2"),
                       ("glfw/3.3.6"),
                       ("glm/0.9.9.8"))
    options         = {"shared": [True, False]} 
    default_options = "shared=False"

    def imports(self):
       self.copy("*.dylib*", dst="", src="lib")
       self.copy("*.dll"   , dst="", src="bin")

    def source(self):
        zip_name = "%s.zip" % self.version
        download ("%s/archive/%s" % (self.url, zip_name), zip_name, verify=False)
        unzip    (zip_name)
        os.unlink(zip_name)

    def build(self):
        cmake          = CMake(self)
        shared_options = "-DBUILD_SHARED_LIBS=ON" if self.options.shared else "-DBUILD_SHARED_LIBS=OFF"
        self.run("cmake %s-%s %s %s" % (self.name, self.version, cmake.command_line, shared_options))
        self.run("cmake --build . %s" % cmake.build_config)

    def package(self):
        include_folder = "%s-%s/include" % (self.name, self.version)
        self.copy("*.h"     , dst="include", src=include_folder)
        self.copy("*.hpp"   , dst="include", src=include_folder)
        self.copy("*.inl"   , dst="include", src=include_folder)
        self.copy("*.dylib*", dst="lib"    , keep_path=False   )
        self.copy("*.lib"   , dst="lib"    , keep_path=False   )
        self.copy("*.so*"   , dst="lib"    , keep_path=False   )
        self.copy("*.dll"   , dst="bin"    , keep_path=False   )

    def package_info(self):
        self.cpp_info.libs = [self.name]
        if self.settings.os == "Windows":
            if not self.options.shared:
                self.cpp_info.defines.append("%s_STATIC" % self.name.upper())
