from   conans       import ConanFile, CMake, tools
from   conans.tools import download, unzip
import os

class Project(ConanFile):
    name            = "Potato ImGui"
    description     = "Conan package for Potato ImGui."
    version         = "0.0.1"                
    url             = "https://github.com/AlbertoCejas/PotatoEngine"
    settings        = "arch", "build_type", "compiler", "os"
    generators      = "cmake"
    requires        = (("glfw/3.3.4"))

    def imports(self):
       self.copy("*.dylib*", dst="", src="lib")
       self.copy("*.dll"   , dst="", src="bin")

    def source(self):
        zip_name = "%s.zip" % self.version
        download ("%s/archive/%s" % (self.url, zip_name), zip_name, verify=False)
        unzip    (zip_name)
        os.unlink(zip_name)

    def build(self):
        cmake = CMake(self)
        self.run("cmake %s-%s %s" % (self.name, self.version, cmake.command_line))
        self.run("cmake --build . %s" % cmake.build_config)

    def package(self):
        self.copy("*.dylib*", dst="lib", keep_path=False)
        self.copy("*.so*"   , dst="lib", keep_path=False)
        self.copy("*.dll"   , dst="bin", keep_path=False)
        self.copy("*.exe"   , dst="bin", keep_path=False)
