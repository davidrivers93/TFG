# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

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

# The program to use to edit the cache.
CMAKE_EDIT_COMMAND = /opt/local/bin/ccmake

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/David/Desktop/ProyectosC++/TFG/fuentes

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/David/Desktop/ProyectosC++/TFG/build_mac

# Include any dependencies generated for this target.
include trabajo/CMakeFiles/dorsales.dir/depend.make

# Include the progress variables for this target.
include trabajo/CMakeFiles/dorsales.dir/progress.make

# Include the compile flags for this target's objects.
include trabajo/CMakeFiles/dorsales.dir/flags.make

trabajo/CMakeFiles/dorsales.dir/dorsales.cpp.o: trabajo/CMakeFiles/dorsales.dir/flags.make
trabajo/CMakeFiles/dorsales.dir/dorsales.cpp.o: /Users/David/Desktop/ProyectosC++/TFG/fuentes/trabajo/dorsales.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /Users/David/Desktop/ProyectosC++/TFG/build_mac/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object trabajo/CMakeFiles/dorsales.dir/dorsales.cpp.o"
	cd /Users/David/Desktop/ProyectosC++/TFG/build_mac/trabajo && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/dorsales.dir/dorsales.cpp.o -c /Users/David/Desktop/ProyectosC++/TFG/fuentes/trabajo/dorsales.cpp

trabajo/CMakeFiles/dorsales.dir/dorsales.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/dorsales.dir/dorsales.cpp.i"
	cd /Users/David/Desktop/ProyectosC++/TFG/build_mac/trabajo && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /Users/David/Desktop/ProyectosC++/TFG/fuentes/trabajo/dorsales.cpp > CMakeFiles/dorsales.dir/dorsales.cpp.i

trabajo/CMakeFiles/dorsales.dir/dorsales.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/dorsales.dir/dorsales.cpp.s"
	cd /Users/David/Desktop/ProyectosC++/TFG/build_mac/trabajo && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /Users/David/Desktop/ProyectosC++/TFG/fuentes/trabajo/dorsales.cpp -o CMakeFiles/dorsales.dir/dorsales.cpp.s

trabajo/CMakeFiles/dorsales.dir/dorsales.cpp.o.requires:
.PHONY : trabajo/CMakeFiles/dorsales.dir/dorsales.cpp.o.requires

trabajo/CMakeFiles/dorsales.dir/dorsales.cpp.o.provides: trabajo/CMakeFiles/dorsales.dir/dorsales.cpp.o.requires
	$(MAKE) -f trabajo/CMakeFiles/dorsales.dir/build.make trabajo/CMakeFiles/dorsales.dir/dorsales.cpp.o.provides.build
.PHONY : trabajo/CMakeFiles/dorsales.dir/dorsales.cpp.o.provides

trabajo/CMakeFiles/dorsales.dir/dorsales.cpp.o.provides.build: trabajo/CMakeFiles/dorsales.dir/dorsales.cpp.o

# Object files for target dorsales
dorsales_OBJECTS = \
"CMakeFiles/dorsales.dir/dorsales.cpp.o"

# External object files for target dorsales
dorsales_EXTERNAL_OBJECTS =

trabajo/dorsales: trabajo/CMakeFiles/dorsales.dir/dorsales.cpp.o
trabajo/dorsales: trabajo/CMakeFiles/dorsales.dir/build.make
trabajo/dorsales: /opt/local/lib/libtiff.dylib
trabajo/dorsales: /opt/local/lib/libjpeg.dylib
trabajo/dorsales: /opt/local/lib/libSM.dylib
trabajo/dorsales: /opt/local/lib/libICE.dylib
trabajo/dorsales: /opt/local/lib/libX11.dylib
trabajo/dorsales: /opt/local/lib/libXext.dylib
trabajo/dorsales: trabajo/libutilities.a
trabajo/dorsales: trabajo/libfunctions.a
trabajo/dorsales: /opt/local/lib/libopencv_videostab.2.4.11.dylib
trabajo/dorsales: /opt/local/lib/libopencv_video.2.4.11.dylib
trabajo/dorsales: /opt/local/lib/libopencv_ts.a
trabajo/dorsales: /opt/local/lib/libopencv_superres.2.4.11.dylib
trabajo/dorsales: /opt/local/lib/libopencv_stitching.2.4.11.dylib
trabajo/dorsales: /opt/local/lib/libopencv_photo.2.4.11.dylib
trabajo/dorsales: /opt/local/lib/libopencv_objdetect.2.4.11.dylib
trabajo/dorsales: /opt/local/lib/libopencv_nonfree.2.4.11.dylib
trabajo/dorsales: /opt/local/lib/libopencv_ml.2.4.11.dylib
trabajo/dorsales: /opt/local/lib/libopencv_legacy.2.4.11.dylib
trabajo/dorsales: /opt/local/lib/libopencv_imgproc.2.4.11.dylib
trabajo/dorsales: /opt/local/lib/libopencv_highgui.2.4.11.dylib
trabajo/dorsales: /opt/local/lib/libopencv_gpu.2.4.11.dylib
trabajo/dorsales: /opt/local/lib/libopencv_flann.2.4.11.dylib
trabajo/dorsales: /opt/local/lib/libopencv_features2d.2.4.11.dylib
trabajo/dorsales: /opt/local/lib/libopencv_core.2.4.11.dylib
trabajo/dorsales: /opt/local/lib/libopencv_contrib.2.4.11.dylib
trabajo/dorsales: /opt/local/lib/libopencv_calib3d.2.4.11.dylib
trabajo/dorsales: /opt/local/lib/libopencv_nonfree.2.4.11.dylib
trabajo/dorsales: /opt/local/lib/libopencv_gpu.2.4.11.dylib
trabajo/dorsales: /opt/local/lib/libopencv_photo.2.4.11.dylib
trabajo/dorsales: /opt/local/lib/libopencv_objdetect.2.4.11.dylib
trabajo/dorsales: /opt/local/lib/libopencv_legacy.2.4.11.dylib
trabajo/dorsales: /opt/local/lib/libopencv_video.2.4.11.dylib
trabajo/dorsales: /opt/local/lib/libopencv_ml.2.4.11.dylib
trabajo/dorsales: /opt/local/lib/libopencv_calib3d.2.4.11.dylib
trabajo/dorsales: /opt/local/lib/libopencv_features2d.2.4.11.dylib
trabajo/dorsales: /opt/local/lib/libopencv_highgui.2.4.11.dylib
trabajo/dorsales: /opt/local/lib/libopencv_imgproc.2.4.11.dylib
trabajo/dorsales: /opt/local/lib/libopencv_flann.2.4.11.dylib
trabajo/dorsales: /opt/local/lib/libopencv_core.2.4.11.dylib
trabajo/dorsales: trabajo/CMakeFiles/dorsales.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable dorsales"
	cd /Users/David/Desktop/ProyectosC++/TFG/build_mac/trabajo && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/dorsales.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
trabajo/CMakeFiles/dorsales.dir/build: trabajo/dorsales
.PHONY : trabajo/CMakeFiles/dorsales.dir/build

trabajo/CMakeFiles/dorsales.dir/requires: trabajo/CMakeFiles/dorsales.dir/dorsales.cpp.o.requires
.PHONY : trabajo/CMakeFiles/dorsales.dir/requires

trabajo/CMakeFiles/dorsales.dir/clean:
	cd /Users/David/Desktop/ProyectosC++/TFG/build_mac/trabajo && $(CMAKE_COMMAND) -P CMakeFiles/dorsales.dir/cmake_clean.cmake
.PHONY : trabajo/CMakeFiles/dorsales.dir/clean

trabajo/CMakeFiles/dorsales.dir/depend:
	cd /Users/David/Desktop/ProyectosC++/TFG/build_mac && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/David/Desktop/ProyectosC++/TFG/fuentes /Users/David/Desktop/ProyectosC++/TFG/fuentes/trabajo /Users/David/Desktop/ProyectosC++/TFG/build_mac /Users/David/Desktop/ProyectosC++/TFG/build_mac/trabajo /Users/David/Desktop/ProyectosC++/TFG/build_mac/trabajo/CMakeFiles/dorsales.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : trabajo/CMakeFiles/dorsales.dir/depend

