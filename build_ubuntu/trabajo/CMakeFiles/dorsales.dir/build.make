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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/david/Desktop/tfg/tfg/fuentes

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/david/Desktop/tfg/tfg/build_ubuntu

# Include any dependencies generated for this target.
include trabajo/CMakeFiles/dorsales.dir/depend.make

# Include the progress variables for this target.
include trabajo/CMakeFiles/dorsales.dir/progress.make

# Include the compile flags for this target's objects.
include trabajo/CMakeFiles/dorsales.dir/flags.make

trabajo/CMakeFiles/dorsales.dir/dorsales.cpp.o: trabajo/CMakeFiles/dorsales.dir/flags.make
trabajo/CMakeFiles/dorsales.dir/dorsales.cpp.o: /home/david/Desktop/tfg/tfg/fuentes/trabajo/dorsales.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/david/Desktop/tfg/tfg/build_ubuntu/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object trabajo/CMakeFiles/dorsales.dir/dorsales.cpp.o"
	cd /home/david/Desktop/tfg/tfg/build_ubuntu/trabajo && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/dorsales.dir/dorsales.cpp.o -c /home/david/Desktop/tfg/tfg/fuentes/trabajo/dorsales.cpp

trabajo/CMakeFiles/dorsales.dir/dorsales.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/dorsales.dir/dorsales.cpp.i"
	cd /home/david/Desktop/tfg/tfg/build_ubuntu/trabajo && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/david/Desktop/tfg/tfg/fuentes/trabajo/dorsales.cpp > CMakeFiles/dorsales.dir/dorsales.cpp.i

trabajo/CMakeFiles/dorsales.dir/dorsales.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/dorsales.dir/dorsales.cpp.s"
	cd /home/david/Desktop/tfg/tfg/build_ubuntu/trabajo && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/david/Desktop/tfg/tfg/fuentes/trabajo/dorsales.cpp -o CMakeFiles/dorsales.dir/dorsales.cpp.s

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
trabajo/dorsales: /usr/lib/x86_64-linux-gnu/libtiff.so
trabajo/dorsales: /usr/lib/x86_64-linux-gnu/libjpeg.so
trabajo/dorsales: /usr/lib/x86_64-linux-gnu/libSM.so
trabajo/dorsales: /usr/lib/x86_64-linux-gnu/libICE.so
trabajo/dorsales: /usr/lib/x86_64-linux-gnu/libX11.so
trabajo/dorsales: /usr/lib/x86_64-linux-gnu/libXext.so
trabajo/dorsales: trabajo/libutilities.a
trabajo/dorsales: trabajo/libfunctions.a
trabajo/dorsales: /usr/local/lib/libopencv_videostab.so.3.0.0
trabajo/dorsales: /usr/local/lib/libopencv_videoio.so.3.0.0
trabajo/dorsales: /usr/local/lib/libopencv_video.so.3.0.0
trabajo/dorsales: /usr/local/lib/libopencv_ts.a
trabajo/dorsales: /usr/local/lib/libopencv_superres.so.3.0.0
trabajo/dorsales: /usr/local/lib/libopencv_stitching.so.3.0.0
trabajo/dorsales: /usr/local/lib/libopencv_shape.so.3.0.0
trabajo/dorsales: /usr/local/lib/libopencv_photo.so.3.0.0
trabajo/dorsales: /usr/local/lib/libopencv_objdetect.so.3.0.0
trabajo/dorsales: /usr/local/lib/libopencv_ml.so.3.0.0
trabajo/dorsales: /usr/local/lib/libopencv_imgproc.so.3.0.0
trabajo/dorsales: /usr/local/lib/libopencv_imgcodecs.so.3.0.0
trabajo/dorsales: /usr/local/lib/libopencv_highgui.so.3.0.0
trabajo/dorsales: /usr/local/lib/libopencv_hal.a
trabajo/dorsales: /usr/local/lib/libopencv_flann.so.3.0.0
trabajo/dorsales: /usr/local/lib/libopencv_features2d.so.3.0.0
trabajo/dorsales: /usr/local/lib/libopencv_core.so.3.0.0
trabajo/dorsales: /usr/local/lib/libopencv_calib3d.so.3.0.0
trabajo/dorsales: /usr/local/lib/libopencv_features2d.so.3.0.0
trabajo/dorsales: /usr/local/lib/libopencv_ml.so.3.0.0
trabajo/dorsales: /usr/local/lib/libopencv_highgui.so.3.0.0
trabajo/dorsales: /usr/local/lib/libopencv_videoio.so.3.0.0
trabajo/dorsales: /usr/local/lib/libopencv_imgcodecs.so.3.0.0
trabajo/dorsales: /usr/local/lib/libopencv_flann.so.3.0.0
trabajo/dorsales: /usr/local/lib/libopencv_video.so.3.0.0
trabajo/dorsales: /usr/local/lib/libopencv_imgproc.so.3.0.0
trabajo/dorsales: /usr/local/lib/libopencv_core.so.3.0.0
trabajo/dorsales: /usr/local/lib/libopencv_hal.a
trabajo/dorsales: /usr/local/share/OpenCV/3rdparty/lib/libippicv.a
trabajo/dorsales: trabajo/CMakeFiles/dorsales.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable dorsales"
	cd /home/david/Desktop/tfg/tfg/build_ubuntu/trabajo && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/dorsales.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
trabajo/CMakeFiles/dorsales.dir/build: trabajo/dorsales
.PHONY : trabajo/CMakeFiles/dorsales.dir/build

trabajo/CMakeFiles/dorsales.dir/requires: trabajo/CMakeFiles/dorsales.dir/dorsales.cpp.o.requires
.PHONY : trabajo/CMakeFiles/dorsales.dir/requires

trabajo/CMakeFiles/dorsales.dir/clean:
	cd /home/david/Desktop/tfg/tfg/build_ubuntu/trabajo && $(CMAKE_COMMAND) -P CMakeFiles/dorsales.dir/cmake_clean.cmake
.PHONY : trabajo/CMakeFiles/dorsales.dir/clean

trabajo/CMakeFiles/dorsales.dir/depend:
	cd /home/david/Desktop/tfg/tfg/build_ubuntu && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/david/Desktop/tfg/tfg/fuentes /home/david/Desktop/tfg/tfg/fuentes/trabajo /home/david/Desktop/tfg/tfg/build_ubuntu /home/david/Desktop/tfg/tfg/build_ubuntu/trabajo /home/david/Desktop/tfg/tfg/build_ubuntu/trabajo/CMakeFiles/dorsales.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : trabajo/CMakeFiles/dorsales.dir/depend
