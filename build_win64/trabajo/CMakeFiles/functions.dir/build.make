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
CMAKE_SOURCE_DIR = C:\Users\davidrivers93\Desktop\TFG\fuentes

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = C:\Users\davidrivers93\Desktop\TFG\build_win64

# Include any dependencies generated for this target.
include trabajo/CMakeFiles/functions.dir/depend.make

# Include the progress variables for this target.
include trabajo/CMakeFiles/functions.dir/progress.make

# Include the compile flags for this target's objects.
include trabajo/CMakeFiles/functions.dir/flags.make

trabajo/CMakeFiles/functions.dir/functions.cpp.obj: trabajo/CMakeFiles/functions.dir/flags.make
trabajo/CMakeFiles/functions.dir/functions.cpp.obj: trabajo/CMakeFiles/functions.dir/includes_CXX.rsp
trabajo/CMakeFiles/functions.dir/functions.cpp.obj: C:/Users/davidrivers93/Desktop/TFG/fuentes/trabajo/functions.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report C:\Users\davidrivers93\Desktop\TFG\build_win64\CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object trabajo/CMakeFiles/functions.dir/functions.cpp.obj"
	cd /d C:\Users\davidrivers93\Desktop\TFG\build_win64\trabajo && E:\MinGW\bin\g++.exe   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles\functions.dir\functions.cpp.obj -c C:\Users\davidrivers93\Desktop\TFG\fuentes\trabajo\functions.cpp

trabajo/CMakeFiles/functions.dir/functions.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/functions.dir/functions.cpp.i"
	cd /d C:\Users\davidrivers93\Desktop\TFG\build_win64\trabajo && E:\MinGW\bin\g++.exe  $(CXX_DEFINES) $(CXX_FLAGS) -E C:\Users\davidrivers93\Desktop\TFG\fuentes\trabajo\functions.cpp > CMakeFiles\functions.dir\functions.cpp.i

trabajo/CMakeFiles/functions.dir/functions.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/functions.dir/functions.cpp.s"
	cd /d C:\Users\davidrivers93\Desktop\TFG\build_win64\trabajo && E:\MinGW\bin\g++.exe  $(CXX_DEFINES) $(CXX_FLAGS) -S C:\Users\davidrivers93\Desktop\TFG\fuentes\trabajo\functions.cpp -o CMakeFiles\functions.dir\functions.cpp.s

trabajo/CMakeFiles/functions.dir/functions.cpp.obj.requires:
.PHONY : trabajo/CMakeFiles/functions.dir/functions.cpp.obj.requires

trabajo/CMakeFiles/functions.dir/functions.cpp.obj.provides: trabajo/CMakeFiles/functions.dir/functions.cpp.obj.requires
	$(MAKE) -f trabajo\CMakeFiles\functions.dir\build.make trabajo/CMakeFiles/functions.dir/functions.cpp.obj.provides.build
.PHONY : trabajo/CMakeFiles/functions.dir/functions.cpp.obj.provides

trabajo/CMakeFiles/functions.dir/functions.cpp.obj.provides.build: trabajo/CMakeFiles/functions.dir/functions.cpp.obj

# Object files for target functions
functions_OBJECTS = \
"CMakeFiles/functions.dir/functions.cpp.obj"

# External object files for target functions
functions_EXTERNAL_OBJECTS =

trabajo/libfunctions.a: trabajo/CMakeFiles/functions.dir/functions.cpp.obj
trabajo/libfunctions.a: trabajo/CMakeFiles/functions.dir/build.make
trabajo/libfunctions.a: trabajo/CMakeFiles/functions.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX static library libfunctions.a"
	cd /d C:\Users\davidrivers93\Desktop\TFG\build_win64\trabajo && $(CMAKE_COMMAND) -P CMakeFiles\functions.dir\cmake_clean_target.cmake
	cd /d C:\Users\davidrivers93\Desktop\TFG\build_win64\trabajo && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\functions.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
trabajo/CMakeFiles/functions.dir/build: trabajo/libfunctions.a
.PHONY : trabajo/CMakeFiles/functions.dir/build

trabajo/CMakeFiles/functions.dir/requires: trabajo/CMakeFiles/functions.dir/functions.cpp.obj.requires
.PHONY : trabajo/CMakeFiles/functions.dir/requires

trabajo/CMakeFiles/functions.dir/clean:
	cd /d C:\Users\davidrivers93\Desktop\TFG\build_win64\trabajo && $(CMAKE_COMMAND) -P CMakeFiles\functions.dir\cmake_clean.cmake
.PHONY : trabajo/CMakeFiles/functions.dir/clean

trabajo/CMakeFiles/functions.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" C:\Users\davidrivers93\Desktop\TFG\fuentes C:\Users\davidrivers93\Desktop\TFG\fuentes\trabajo C:\Users\davidrivers93\Desktop\TFG\build_win64 C:\Users\davidrivers93\Desktop\TFG\build_win64\trabajo C:\Users\davidrivers93\Desktop\TFG\build_win64\trabajo\CMakeFiles\functions.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : trabajo/CMakeFiles/functions.dir/depend

