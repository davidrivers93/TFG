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
include trabajo/CMakeFiles/dorsal_new.dir/depend.make

# Include the progress variables for this target.
include trabajo/CMakeFiles/dorsal_new.dir/progress.make

# Include the compile flags for this target's objects.
include trabajo/CMakeFiles/dorsal_new.dir/flags.make

trabajo/CMakeFiles/dorsal_new.dir/dorsal_new.cpp.o: trabajo/CMakeFiles/dorsal_new.dir/flags.make
trabajo/CMakeFiles/dorsal_new.dir/dorsal_new.cpp.o: /home/david/Desktop/tfg/tfg/fuentes/trabajo/dorsal_new.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/david/Desktop/tfg/tfg/build_ubuntu/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object trabajo/CMakeFiles/dorsal_new.dir/dorsal_new.cpp.o"
	cd /home/david/Desktop/tfg/tfg/build_ubuntu/trabajo && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/dorsal_new.dir/dorsal_new.cpp.o -c /home/david/Desktop/tfg/tfg/fuentes/trabajo/dorsal_new.cpp

trabajo/CMakeFiles/dorsal_new.dir/dorsal_new.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/dorsal_new.dir/dorsal_new.cpp.i"
	cd /home/david/Desktop/tfg/tfg/build_ubuntu/trabajo && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/david/Desktop/tfg/tfg/fuentes/trabajo/dorsal_new.cpp > CMakeFiles/dorsal_new.dir/dorsal_new.cpp.i

trabajo/CMakeFiles/dorsal_new.dir/dorsal_new.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/dorsal_new.dir/dorsal_new.cpp.s"
	cd /home/david/Desktop/tfg/tfg/build_ubuntu/trabajo && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/david/Desktop/tfg/tfg/fuentes/trabajo/dorsal_new.cpp -o CMakeFiles/dorsal_new.dir/dorsal_new.cpp.s

trabajo/CMakeFiles/dorsal_new.dir/dorsal_new.cpp.o.requires:
.PHONY : trabajo/CMakeFiles/dorsal_new.dir/dorsal_new.cpp.o.requires

trabajo/CMakeFiles/dorsal_new.dir/dorsal_new.cpp.o.provides: trabajo/CMakeFiles/dorsal_new.dir/dorsal_new.cpp.o.requires
	$(MAKE) -f trabajo/CMakeFiles/dorsal_new.dir/build.make trabajo/CMakeFiles/dorsal_new.dir/dorsal_new.cpp.o.provides.build
.PHONY : trabajo/CMakeFiles/dorsal_new.dir/dorsal_new.cpp.o.provides

trabajo/CMakeFiles/dorsal_new.dir/dorsal_new.cpp.o.provides.build: trabajo/CMakeFiles/dorsal_new.dir/dorsal_new.cpp.o

# Object files for target dorsal_new
dorsal_new_OBJECTS = \
"CMakeFiles/dorsal_new.dir/dorsal_new.cpp.o"

# External object files for target dorsal_new
dorsal_new_EXTERNAL_OBJECTS =

trabajo/dorsal_new: trabajo/CMakeFiles/dorsal_new.dir/dorsal_new.cpp.o
trabajo/dorsal_new: trabajo/CMakeFiles/dorsal_new.dir/build.make
trabajo/dorsal_new: /usr/lib/x86_64-linux-gnu/libtiff.so
trabajo/dorsal_new: /usr/lib/x86_64-linux-gnu/libjpeg.so
trabajo/dorsal_new: /usr/lib/x86_64-linux-gnu/libSM.so
trabajo/dorsal_new: /usr/lib/x86_64-linux-gnu/libICE.so
trabajo/dorsal_new: /usr/lib/x86_64-linux-gnu/libX11.so
trabajo/dorsal_new: /usr/lib/x86_64-linux-gnu/libXext.so
trabajo/dorsal_new: trabajo/libutilities.a
trabajo/dorsal_new: trabajo/libfunctions.a
trabajo/dorsal_new: /usr/lib/x86_64-linux-gnu/libopencv_videostab.so.2.4.8
trabajo/dorsal_new: /usr/lib/x86_64-linux-gnu/libopencv_video.so.2.4.8
trabajo/dorsal_new: /usr/lib/x86_64-linux-gnu/libopencv_ts.so.2.4.8
trabajo/dorsal_new: /usr/lib/x86_64-linux-gnu/libopencv_superres.so.2.4.8
trabajo/dorsal_new: /usr/lib/x86_64-linux-gnu/libopencv_stitching.so.2.4.8
trabajo/dorsal_new: /usr/lib/x86_64-linux-gnu/libopencv_photo.so.2.4.8
trabajo/dorsal_new: /usr/lib/x86_64-linux-gnu/libopencv_ocl.so.2.4.8
trabajo/dorsal_new: /usr/lib/x86_64-linux-gnu/libopencv_objdetect.so.2.4.8
trabajo/dorsal_new: /usr/lib/x86_64-linux-gnu/libopencv_ml.so.2.4.8
trabajo/dorsal_new: /usr/lib/x86_64-linux-gnu/libopencv_legacy.so.2.4.8
trabajo/dorsal_new: /usr/lib/x86_64-linux-gnu/libopencv_imgproc.so.2.4.8
trabajo/dorsal_new: /usr/lib/x86_64-linux-gnu/libopencv_highgui.so.2.4.8
trabajo/dorsal_new: /usr/lib/x86_64-linux-gnu/libopencv_gpu.so.2.4.8
trabajo/dorsal_new: /usr/lib/x86_64-linux-gnu/libopencv_flann.so.2.4.8
trabajo/dorsal_new: /usr/lib/x86_64-linux-gnu/libopencv_features2d.so.2.4.8
trabajo/dorsal_new: /usr/lib/x86_64-linux-gnu/libopencv_core.so.2.4.8
trabajo/dorsal_new: /usr/lib/x86_64-linux-gnu/libopencv_contrib.so.2.4.8
trabajo/dorsal_new: /usr/lib/x86_64-linux-gnu/libopencv_calib3d.so.2.4.8
trabajo/dorsal_new: /usr/lib/x86_64-linux-gnu/libopencv_photo.so.2.4.8
trabajo/dorsal_new: /usr/lib/x86_64-linux-gnu/libopencv_legacy.so.2.4.8
trabajo/dorsal_new: /usr/lib/x86_64-linux-gnu/libopencv_video.so.2.4.8
trabajo/dorsal_new: /usr/lib/x86_64-linux-gnu/libopencv_objdetect.so.2.4.8
trabajo/dorsal_new: /usr/lib/x86_64-linux-gnu/libopencv_ml.so.2.4.8
trabajo/dorsal_new: /usr/lib/x86_64-linux-gnu/libopencv_calib3d.so.2.4.8
trabajo/dorsal_new: /usr/lib/x86_64-linux-gnu/libopencv_features2d.so.2.4.8
trabajo/dorsal_new: /usr/lib/x86_64-linux-gnu/libopencv_highgui.so.2.4.8
trabajo/dorsal_new: /usr/lib/x86_64-linux-gnu/libopencv_imgproc.so.2.4.8
trabajo/dorsal_new: /usr/lib/x86_64-linux-gnu/libopencv_flann.so.2.4.8
trabajo/dorsal_new: /usr/lib/x86_64-linux-gnu/libopencv_core.so.2.4.8
trabajo/dorsal_new: trabajo/CMakeFiles/dorsal_new.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable dorsal_new"
	cd /home/david/Desktop/tfg/tfg/build_ubuntu/trabajo && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/dorsal_new.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
trabajo/CMakeFiles/dorsal_new.dir/build: trabajo/dorsal_new
.PHONY : trabajo/CMakeFiles/dorsal_new.dir/build

# Object files for target dorsal_new
dorsal_new_OBJECTS = \
"CMakeFiles/dorsal_new.dir/dorsal_new.cpp.o"

# External object files for target dorsal_new
dorsal_new_EXTERNAL_OBJECTS =

trabajo/CMakeFiles/CMakeRelink.dir/dorsal_new: trabajo/CMakeFiles/dorsal_new.dir/dorsal_new.cpp.o
trabajo/CMakeFiles/CMakeRelink.dir/dorsal_new: trabajo/CMakeFiles/dorsal_new.dir/build.make
trabajo/CMakeFiles/CMakeRelink.dir/dorsal_new: /usr/lib/x86_64-linux-gnu/libtiff.so
trabajo/CMakeFiles/CMakeRelink.dir/dorsal_new: /usr/lib/x86_64-linux-gnu/libjpeg.so
trabajo/CMakeFiles/CMakeRelink.dir/dorsal_new: /usr/lib/x86_64-linux-gnu/libSM.so
trabajo/CMakeFiles/CMakeRelink.dir/dorsal_new: /usr/lib/x86_64-linux-gnu/libICE.so
trabajo/CMakeFiles/CMakeRelink.dir/dorsal_new: /usr/lib/x86_64-linux-gnu/libX11.so
trabajo/CMakeFiles/CMakeRelink.dir/dorsal_new: /usr/lib/x86_64-linux-gnu/libXext.so
trabajo/CMakeFiles/CMakeRelink.dir/dorsal_new: trabajo/libutilities.a
trabajo/CMakeFiles/CMakeRelink.dir/dorsal_new: trabajo/libfunctions.a
trabajo/CMakeFiles/CMakeRelink.dir/dorsal_new: /usr/lib/x86_64-linux-gnu/libopencv_videostab.so.2.4.8
trabajo/CMakeFiles/CMakeRelink.dir/dorsal_new: /usr/lib/x86_64-linux-gnu/libopencv_video.so.2.4.8
trabajo/CMakeFiles/CMakeRelink.dir/dorsal_new: /usr/lib/x86_64-linux-gnu/libopencv_ts.so.2.4.8
trabajo/CMakeFiles/CMakeRelink.dir/dorsal_new: /usr/lib/x86_64-linux-gnu/libopencv_superres.so.2.4.8
trabajo/CMakeFiles/CMakeRelink.dir/dorsal_new: /usr/lib/x86_64-linux-gnu/libopencv_stitching.so.2.4.8
trabajo/CMakeFiles/CMakeRelink.dir/dorsal_new: /usr/lib/x86_64-linux-gnu/libopencv_photo.so.2.4.8
trabajo/CMakeFiles/CMakeRelink.dir/dorsal_new: /usr/lib/x86_64-linux-gnu/libopencv_ocl.so.2.4.8
trabajo/CMakeFiles/CMakeRelink.dir/dorsal_new: /usr/lib/x86_64-linux-gnu/libopencv_objdetect.so.2.4.8
trabajo/CMakeFiles/CMakeRelink.dir/dorsal_new: /usr/lib/x86_64-linux-gnu/libopencv_ml.so.2.4.8
trabajo/CMakeFiles/CMakeRelink.dir/dorsal_new: /usr/lib/x86_64-linux-gnu/libopencv_legacy.so.2.4.8
trabajo/CMakeFiles/CMakeRelink.dir/dorsal_new: /usr/lib/x86_64-linux-gnu/libopencv_imgproc.so.2.4.8
trabajo/CMakeFiles/CMakeRelink.dir/dorsal_new: /usr/lib/x86_64-linux-gnu/libopencv_highgui.so.2.4.8
trabajo/CMakeFiles/CMakeRelink.dir/dorsal_new: /usr/lib/x86_64-linux-gnu/libopencv_gpu.so.2.4.8
trabajo/CMakeFiles/CMakeRelink.dir/dorsal_new: /usr/lib/x86_64-linux-gnu/libopencv_flann.so.2.4.8
trabajo/CMakeFiles/CMakeRelink.dir/dorsal_new: /usr/lib/x86_64-linux-gnu/libopencv_features2d.so.2.4.8
trabajo/CMakeFiles/CMakeRelink.dir/dorsal_new: /usr/lib/x86_64-linux-gnu/libopencv_core.so.2.4.8
trabajo/CMakeFiles/CMakeRelink.dir/dorsal_new: /usr/lib/x86_64-linux-gnu/libopencv_contrib.so.2.4.8
trabajo/CMakeFiles/CMakeRelink.dir/dorsal_new: /usr/lib/x86_64-linux-gnu/libopencv_calib3d.so.2.4.8
trabajo/CMakeFiles/CMakeRelink.dir/dorsal_new: /usr/lib/x86_64-linux-gnu/libopencv_photo.so.2.4.8
trabajo/CMakeFiles/CMakeRelink.dir/dorsal_new: /usr/lib/x86_64-linux-gnu/libopencv_legacy.so.2.4.8
trabajo/CMakeFiles/CMakeRelink.dir/dorsal_new: /usr/lib/x86_64-linux-gnu/libopencv_video.so.2.4.8
trabajo/CMakeFiles/CMakeRelink.dir/dorsal_new: /usr/lib/x86_64-linux-gnu/libopencv_objdetect.so.2.4.8
trabajo/CMakeFiles/CMakeRelink.dir/dorsal_new: /usr/lib/x86_64-linux-gnu/libopencv_ml.so.2.4.8
trabajo/CMakeFiles/CMakeRelink.dir/dorsal_new: /usr/lib/x86_64-linux-gnu/libopencv_calib3d.so.2.4.8
trabajo/CMakeFiles/CMakeRelink.dir/dorsal_new: /usr/lib/x86_64-linux-gnu/libopencv_features2d.so.2.4.8
trabajo/CMakeFiles/CMakeRelink.dir/dorsal_new: /usr/lib/x86_64-linux-gnu/libopencv_highgui.so.2.4.8
trabajo/CMakeFiles/CMakeRelink.dir/dorsal_new: /usr/lib/x86_64-linux-gnu/libopencv_imgproc.so.2.4.8
trabajo/CMakeFiles/CMakeRelink.dir/dorsal_new: /usr/lib/x86_64-linux-gnu/libopencv_flann.so.2.4.8
trabajo/CMakeFiles/CMakeRelink.dir/dorsal_new: /usr/lib/x86_64-linux-gnu/libopencv_core.so.2.4.8
trabajo/CMakeFiles/CMakeRelink.dir/dorsal_new: trabajo/CMakeFiles/dorsal_new.dir/relink.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable CMakeFiles/CMakeRelink.dir/dorsal_new"
	cd /home/david/Desktop/tfg/tfg/build_ubuntu/trabajo && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/dorsal_new.dir/relink.txt --verbose=$(VERBOSE)

# Rule to relink during preinstall.
trabajo/CMakeFiles/dorsal_new.dir/preinstall: trabajo/CMakeFiles/CMakeRelink.dir/dorsal_new
.PHONY : trabajo/CMakeFiles/dorsal_new.dir/preinstall

trabajo/CMakeFiles/dorsal_new.dir/requires: trabajo/CMakeFiles/dorsal_new.dir/dorsal_new.cpp.o.requires
.PHONY : trabajo/CMakeFiles/dorsal_new.dir/requires

trabajo/CMakeFiles/dorsal_new.dir/clean:
	cd /home/david/Desktop/tfg/tfg/build_ubuntu/trabajo && $(CMAKE_COMMAND) -P CMakeFiles/dorsal_new.dir/cmake_clean.cmake
.PHONY : trabajo/CMakeFiles/dorsal_new.dir/clean

trabajo/CMakeFiles/dorsal_new.dir/depend:
	cd /home/david/Desktop/tfg/tfg/build_ubuntu && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/david/Desktop/tfg/tfg/fuentes /home/david/Desktop/tfg/tfg/fuentes/trabajo /home/david/Desktop/tfg/tfg/build_ubuntu /home/david/Desktop/tfg/tfg/build_ubuntu/trabajo /home/david/Desktop/tfg/tfg/build_ubuntu/trabajo/CMakeFiles/dorsal_new.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : trabajo/CMakeFiles/dorsal_new.dir/depend
