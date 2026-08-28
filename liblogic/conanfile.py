# This file is managed by Conan, contents will be overwritten.
# To keep your changes, remove these comment lines, but the plugin won't be able to modify your requirements

from conan import ConanFile
from conan.tools.cmake import cmake_layout, CMakeToolchain

class ConanApplication(ConanFile):
    package_type = "application"
    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeDeps", "CMakeConfigDeps"

    def layout(self):
        cmake_layout(self)

    def generate(self):
        tc = CMakeToolchain(self)
        tc.user_presets_path = False
        tc.generate()

    def requirements(self):
        requirements = self.conan_data.get('requirements', [])
        for requirement in requirements:
            # override=True for nlohmann_json only: json-schema-validator
            # transitively requires an older nlohmann_json than this
            # project pins directly, and override=True is how Conan is
            # told our version should win that conflict. It still ends up
            # a real, consumable dependency here because
            # json-schema-validator's own (non-override) requirement
            # supplies the real graph edge; override only repins its
            # version.
            self.requires(requirement, override=requirement.startswith("nlohmann_json/"))