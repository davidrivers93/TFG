# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.2

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list

# Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /opt/local/bin/cmake

# The command to remove a file.
RM = /opt/local/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/David/Desktop/ProyectosC++/TFG/fuentes

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/David/Desktop/ProyectosC++/TFG/build_mac

# Include any dependencies generated for this target.
include P1/CMakeFiles/cimg_compare_images.dir/depend.make

# Include the progress variables for this target.
include P1/CMakeFiles/cimg_compare_images.dir/progress.make

# Include the compile flags for this target's objects.
include P1/CMakeFiles/cimg_compare_images.dir/flags.make

P1/CMakeFiles/cimg_compare_images.dir/cimg_compare_images.cpp.o: P1/CMakeFiles/cimg_compare_images.dir/flags.make
P1/CMakeFiles/cimg_compare_images.dir/cimg_compare_images.cpp.o: /Users/David/Desktop/ProyectosC++/TFG/fuentes/P1/cimg_compare_images.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /Users/David/Desktop/ProyectosC++/TFG/build_mac/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object P1/CMakeFiles/cimg_compare_images.dir/cimg_compare_images.cpp.o"
	cd /Users/David/Desktop/ProyectosC++/TFG/build_mac/P1 && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/cimg_compare_images.dir/cimg_compare_images.cpp.o -c /Users/David/Desktop/ProyectosC++/TFG/fuentes/P1/cimg_compare_images.cpp

P1/CMakeFiles/cimg_compare_images.dir/cimg_compare_images.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/cimg_compare_images.dir/cimg_compare_images.cpp.i"
	cd /Users/David/Desktop/ProyectosC++/TFG/build_mac/P1 && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /Users/David/Desktop/ProyectosC++/TFG/fuentes/P1/cimg_compare_images.cpp > CMakeFiles/cimg_compare_images.dir/cimg_compare_images.cpp.i

P1/CMakeFiles/cimg_compare_images.dir/cimg_compare_images.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/cimg_compare_images.dir/cimg_compare_images.cpp.s"
	cd /Users/David/Desktop/ProyectosC++/TFG/build_mac/P1 && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /Users/David/Desktop/ProyectosC++/TFG/fuentes/P1/cimg_compare_images.cpp -o CMakeFiles/cimg_compare_images.dir/cimg_compare_images.cpp.s

P1/CMakeFiles/cimg_compare_images.dir/cimg_compare_images.cpp.o.requires:
.PHONY : P1/CMakeFiles/cimg_compare_images.dir/cimg_compare_images.cpp.o.requires

P1/CMakeFiles/cimg_compare_images.dir/cimg_compare_images.cpp.o.provides: P1/CMakeFiles/cimg_compare_images.dir/cimg_compare_images.cpp.o.requires
	$(MAKE) -f P1/CMakeFiles/cimg_compare_images.dir/build.make P1/CMakeFiles/cimg_compare_images.dir/cimg_compare_images.cpp.o.provides.build
.PHONY : P1/CMakeFiles/cimg_compare_images.dir/cimg_compare_images.cpp.o.provides

P1/CMakeFiles/cimg_compare_images.dir/cimg_compare_images.cpp.o.provides.build: P1/CMakeFiles/cimg_compare_images.dir/cimg_compare_images.cpp.o

# Object files for target cimg_compare_images
cimg_compare_images_OBJECTS = \
"CMakeFiles/cimg_compare_images.dir/cimg_compare_images.cpp.o"

# External object files for target cimg_compare_images
cimg_compare_images_EXTERNAL_OBJECTS =

P1/cimg_compare_images: P1/CMakeFiles/cimg_compare_images.dir/cimg_compare_images.cpp.o
P1/cimg_compare_images: P1/CMakeFiles/cimg_compare_images.dir/build.make
P1/cimg_compare_images: /opt/local/lib/libtiff.dylib
P1/cimg_compare_images: /opt/local/lib/libjpeg.dylib
P1/cimg_compare_images: /opt/local/lib/libSM.dylib
P1/cimg_compare_images: /opt/local/lib/libICE.dylib
P1/cimg_compare_images: /opt/local/lib/libX11.dylib
P1/cimg_compare_images: /opt/local/lib/libXext.dylib
P1/cimg_compare_images: P1/CMakeFiles/cimg_compare_images.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable cimg_compare_images"
	cd /Users/David/Desktop/ProyectosC++/TFG/build_mac/P1 && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/cimg_compare_images.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
P1/CMakeFiles/cimg_compare_images.dir/build: P1/cimg_compare_images
.PHONY : P1/CMakeFiles/cimg_compare_images.dir/build

P1/CMakeFiles/cimg_compare_images.dir/requires: P1/CMakeFiles/cimg_compare_images.dir/cimg_compare_images.cpp.o.requires
.PHONY : P1/CMakeFiles/cimg_compare_images.dir/requires

P1/CMakeFiles/cimg_compare_images.dir/clean:
	cd /Users/David/Desktop/ProyectosC++/TFG/build_mac/P1 && $(CMAKE_COMMAND) -P CMakeFiles/cimg_compare_images.dir/cmake_clean.cmake
.PHONY : P1/CMakeFiles/cimg_compare_images.dir/clean

P1/CMakeFiles/cimg_compare_images.dir/depend:
	cd /Users/David/Desktop/ProyectosC++/TFG/build_mac && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/David/Desktop/ProyectosC++/TFG/fuentes /Users/David/Desktop/ProyectosC++/TFG/fuentes/P1 /Users/David/Desktop/ProyectosC++/TFG/build_mac /Users/David/Desktop/ProyectosC++/TFG/build_mac/P1 /Users/David/Desktop/ProyectosC++/TFG/build_mac/P1/CMakeFiles/cimg_compare_images.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : P1/CMakeFiles/cimg_compare_images.dir/depend

