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
include trabajo/CMakeFiles/bib_generator_cpp.dir/depend.make

# Include the progress variables for this target.
include trabajo/CMakeFiles/bib_generator_cpp.dir/progress.make

# Include the compile flags for this target's objects.
include trabajo/CMakeFiles/bib_generator_cpp.dir/flags.make

# Object files for target bib_generator_cpp
bib_generator_cpp_OBJECTS =

# External object files for target bib_generator_cpp
bib_generator_cpp_EXTERNAL_OBJECTS =

trabajo/bib_generator_cpp: trabajo/CMakeFiles/bib_generator_cpp.dir/build.make
trabajo/bib_generator_cpp: /opt/local/lib/libtiff.dylib
trabajo/bib_generator_cpp: /opt/local/lib/libjpeg.dylib
trabajo/bib_generator_cpp: /opt/local/lib/libSM.dylib
trabajo/bib_generator_cpp: /opt/local/lib/libICE.dylib
trabajo/bib_generator_cpp: /opt/local/lib/libX11.dylib
trabajo/bib_generator_cpp: /opt/local/lib/libXext.dylib
trabajo/bib_generator_cpp: trabajo/libutilities.a
trabajo/bib_generator_cpp: trabajo/libfunctions.a
trabajo/bib_generator_cpp: /opt/local/lib/libopencv_videostab.2.4.11.dylib
trabajo/bib_generator_cpp: /opt/local/lib/libopencv_ts.a
trabajo/bib_generator_cpp: /opt/local/lib/libopencv_superres.2.4.11.dylib
trabajo/bib_generator_cpp: /opt/local/lib/libopencv_stitching.2.4.11.dylib
trabajo/bib_generator_cpp: /opt/local/lib/libopencv_contrib.2.4.11.dylib
trabajo/bib_generator_cpp: /opt/local/lib/libopencv_nonfree.2.4.11.dylib
trabajo/bib_generator_cpp: /opt/local/lib/libopencv_gpu.2.4.11.dylib
trabajo/bib_generator_cpp: /opt/local/lib/libopencv_photo.2.4.11.dylib
trabajo/bib_generator_cpp: /opt/local/lib/libopencv_objdetect.2.4.11.dylib
trabajo/bib_generator_cpp: /opt/local/lib/libopencv_legacy.2.4.11.dylib
trabajo/bib_generator_cpp: /opt/local/lib/libopencv_video.2.4.11.dylib
trabajo/bib_generator_cpp: /opt/local/lib/libopencv_ml.2.4.11.dylib
trabajo/bib_generator_cpp: /opt/local/lib/libopencv_calib3d.2.4.11.dylib
trabajo/bib_generator_cpp: /opt/local/lib/libopencv_features2d.2.4.11.dylib
trabajo/bib_generator_cpp: /opt/local/lib/libopencv_highgui.2.4.11.dylib
trabajo/bib_generator_cpp: /opt/local/lib/libopencv_imgproc.2.4.11.dylib
trabajo/bib_generator_cpp: /opt/local/lib/libopencv_flann.2.4.11.dylib
trabajo/bib_generator_cpp: /opt/local/lib/libopencv_core.2.4.11.dylib
trabajo/bib_generator_cpp: trabajo/CMakeFiles/bib_generator_cpp.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable bib_generator_cpp"
	cd /Users/David/Desktop/ProyectosC++/TFG/build_mac/trabajo && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/bib_generator_cpp.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
trabajo/CMakeFiles/bib_generator_cpp.dir/build: trabajo/bib_generator_cpp
.PHONY : trabajo/CMakeFiles/bib_generator_cpp.dir/build

trabajo/CMakeFiles/bib_generator_cpp.dir/requires:
.PHONY : trabajo/CMakeFiles/bib_generator_cpp.dir/requires

trabajo/CMakeFiles/bib_generator_cpp.dir/clean:
	cd /Users/David/Desktop/ProyectosC++/TFG/build_mac/trabajo && $(CMAKE_COMMAND) -P CMakeFiles/bib_generator_cpp.dir/cmake_clean.cmake
.PHONY : trabajo/CMakeFiles/bib_generator_cpp.dir/clean

trabajo/CMakeFiles/bib_generator_cpp.dir/depend:
	cd /Users/David/Desktop/ProyectosC++/TFG/build_mac && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/David/Desktop/ProyectosC++/TFG/fuentes /Users/David/Desktop/ProyectosC++/TFG/fuentes/trabajo /Users/David/Desktop/ProyectosC++/TFG/build_mac /Users/David/Desktop/ProyectosC++/TFG/build_mac/trabajo /Users/David/Desktop/ProyectosC++/TFG/build_mac/trabajo/CMakeFiles/bib_generator_cpp.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : trabajo/CMakeFiles/bib_generator_cpp.dir/depend

