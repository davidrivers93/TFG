# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.2

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

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "E:\Program Files (x86)\CMake\bin\cmake.exe"

# The command to remove a file.
RM = "E:\Program Files (x86)\CMake\bin\cmake.exe" -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = E:\Development\tfg\fuentes

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = E:\Development\tfg\build_win64

# Include any dependencies generated for this target.
include trabajo/CMakeFiles/dorsales.dir/depend.make

# Include the progress variables for this target.
include trabajo/CMakeFiles/dorsales.dir/progress.make

# Include the compile flags for this target's objects.
include trabajo/CMakeFiles/dorsales.dir/flags.make

trabajo/CMakeFiles/dorsales.dir/dorsales.cpp.obj: trabajo/CMakeFiles/dorsales.dir/flags.make
trabajo/CMakeFiles/dorsales.dir/dorsales.cpp.obj: trabajo/CMakeFiles/dorsales.dir/includes_CXX.rsp
trabajo/CMakeFiles/dorsales.dir/dorsales.cpp.obj: E:/Development/tfg/fuentes/trabajo/dorsales.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report E:\Development\tfg\build_win64\CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object trabajo/CMakeFiles/dorsales.dir/dorsales.cpp.obj"
	cd /d E:\Development\tfg\build_win64\trabajo && E:\MinGW\bin\g++.exe   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles\dorsales.dir\dorsales.cpp.obj -c E:\Development\tfg\fuentes\trabajo\dorsales.cpp

trabajo/CMakeFiles/dorsales.dir/dorsales.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/dorsales.dir/dorsales.cpp.i"
	cd /d E:\Development\tfg\build_win64\trabajo && E:\MinGW\bin\g++.exe  $(CXX_DEFINES) $(CXX_FLAGS) -E E:\Development\tfg\fuentes\trabajo\dorsales.cpp > CMakeFiles\dorsales.dir\dorsales.cpp.i

trabajo/CMakeFiles/dorsales.dir/dorsales.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/dorsales.dir/dorsales.cpp.s"
	cd /d E:\Development\tfg\build_win64\trabajo && E:\MinGW\bin\g++.exe  $(CXX_DEFINES) $(CXX_FLAGS) -S E:\Development\tfg\fuentes\trabajo\dorsales.cpp -o CMakeFiles\dorsales.dir\dorsales.cpp.s

trabajo/CMakeFiles/dorsales.dir/dorsales.cpp.obj.requires:
.PHONY : trabajo/CMakeFiles/dorsales.dir/dorsales.cpp.obj.requires

trabajo/CMakeFiles/dorsales.dir/dorsales.cpp.obj.provides: trabajo/CMakeFiles/dorsales.dir/dorsales.cpp.obj.requires
	$(MAKE) -f trabajo\CMakeFiles\dorsales.dir\build.make trabajo/CMakeFiles/dorsales.dir/dorsales.cpp.obj.provides.build
.PHONY : trabajo/CMakeFiles/dorsales.dir/dorsales.cpp.obj.provides

trabajo/CMakeFiles/dorsales.dir/dorsales.cpp.obj.provides.build: trabajo/CMakeFiles/dorsales.dir/dorsales.cpp.obj

# Object files for target dorsales
dorsales_OBJECTS = \
"CMakeFiles/dorsales.dir/dorsales.cpp.obj"

# External object files for target dorsales
dorsales_EXTERNAL_OBJECTS =

trabajo/dorsales.exe: trabajo/CMakeFiles/dorsales.dir/dorsales.cpp.obj
trabajo/dorsales.exe: trabajo/CMakeFiles/dorsales.dir/build.make
trabajo/dorsales.exe: trabajo/libutilities.a
trabajo/dorsales.exe: trabajo/libfunctions.a
trabajo/dorsales.exe: E:/opencv/opencv/build/x64/vc11/lib/opencv_videostab2411d.lib
trabajo/dorsales.exe: E:/opencv/opencv/build/x64/vc11/lib/opencv_ts2411d.lib
trabajo/dorsales.exe: E:/opencv/opencv/build/x64/vc11/lib/opencv_superres2411d.lib
trabajo/dorsales.exe: E:/opencv/opencv/build/x64/vc11/lib/opencv_stitching2411d.lib
trabajo/dorsales.exe: E:/opencv/opencv/build/x64/vc11/lib/opencv_contrib2411d.lib
trabajo/dorsales.exe: E:/opencv/opencv/build/x64/vc11/lib/opencv_nonfree2411d.lib
trabajo/dorsales.exe: E:/opencv/opencv/build/x64/vc11/lib/opencv_ocl2411d.lib
trabajo/dorsales.exe: E:/opencv/opencv/build/x64/vc11/lib/opencv_gpu2411d.lib
trabajo/dorsales.exe: E:/opencv/opencv/build/x64/vc11/lib/opencv_photo2411d.lib
trabajo/dorsales.exe: E:/opencv/opencv/build/x64/vc11/lib/opencv_objdetect2411d.lib
trabajo/dorsales.exe: E:/opencv/opencv/build/x64/vc11/lib/opencv_legacy2411d.lib
trabajo/dorsales.exe: E:/opencv/opencv/build/x64/vc11/lib/opencv_video2411d.lib
trabajo/dorsales.exe: E:/opencv/opencv/build/x64/vc11/lib/opencv_ml2411d.lib
trabajo/dorsales.exe: E:/opencv/opencv/build/x64/vc11/lib/opencv_calib3d2411d.lib
trabajo/dorsales.exe: E:/opencv/opencv/build/x64/vc11/lib/opencv_features2d2411d.lib
trabajo/dorsales.exe: E:/opencv/opencv/build/x64/vc11/lib/opencv_highgui2411d.lib
trabajo/dorsales.exe: E:/opencv/opencv/build/x64/vc11/lib/opencv_imgproc2411d.lib
trabajo/dorsales.exe: E:/opencv/opencv/build/x64/vc11/lib/opencv_flann2411d.lib
trabajo/dorsales.exe: E:/opencv/opencv/build/x64/vc11/lib/opencv_core2411d.lib
trabajo/dorsales.exe: trabajo/CMakeFiles/dorsales.dir/linklibs.rsp
trabajo/dorsales.exe: trabajo/CMakeFiles/dorsales.dir/objects1.rsp
trabajo/dorsales.exe: trabajo/CMakeFiles/dorsales.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable dorsales.exe"
	cd /d E:\Development\tfg\build_win64\trabajo && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\dorsales.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
trabajo/CMakeFiles/dorsales.dir/build: trabajo/dorsales.exe
.PHONY : trabajo/CMakeFiles/dorsales.dir/build

trabajo/CMakeFiles/dorsales.dir/requires: trabajo/CMakeFiles/dorsales.dir/dorsales.cpp.obj.requires
.PHONY : trabajo/CMakeFiles/dorsales.dir/requires

trabajo/CMakeFiles/dorsales.dir/clean:
	cd /d E:\Development\tfg\build_win64\trabajo && $(CMAKE_COMMAND) -P CMakeFiles\dorsales.dir\cmake_clean.cmake
.PHONY : trabajo/CMakeFiles/dorsales.dir/clean

trabajo/CMakeFiles/dorsales.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" E:\Development\tfg\fuentes E:\Development\tfg\fuentes\trabajo E:\Development\tfg\build_win64 E:\Development\tfg\build_win64\trabajo E:\Development\tfg\build_win64\trabajo\CMakeFiles\dorsales.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : trabajo/CMakeFiles/dorsales.dir/depend

