# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.8

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


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
CMAKE_COMMAND = /home/nhannt/.local/share/umake/ide/clion/bin/cmake/bin/cmake

# The command to remove a file.
RM = /home/nhannt/.local/share/umake/ide/clion/bin/cmake/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/nhannt/CLionProjects/Chatwork

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/nhannt/CLionProjects/Chatwork/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/gtk_test.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/gtk_test.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/gtk_test.dir/flags.make

CMakeFiles/gtk_test.dir/main.c.o: CMakeFiles/gtk_test.dir/flags.make
CMakeFiles/gtk_test.dir/main.c.o: ../main.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/nhannt/CLionProjects/Chatwork/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/gtk_test.dir/main.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/gtk_test.dir/main.c.o   -c /home/nhannt/CLionProjects/Chatwork/main.c

CMakeFiles/gtk_test.dir/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/gtk_test.dir/main.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/nhannt/CLionProjects/Chatwork/main.c > CMakeFiles/gtk_test.dir/main.c.i

CMakeFiles/gtk_test.dir/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/gtk_test.dir/main.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/nhannt/CLionProjects/Chatwork/main.c -o CMakeFiles/gtk_test.dir/main.c.s

CMakeFiles/gtk_test.dir/main.c.o.requires:

.PHONY : CMakeFiles/gtk_test.dir/main.c.o.requires

CMakeFiles/gtk_test.dir/main.c.o.provides: CMakeFiles/gtk_test.dir/main.c.o.requires
	$(MAKE) -f CMakeFiles/gtk_test.dir/build.make CMakeFiles/gtk_test.dir/main.c.o.provides.build
.PHONY : CMakeFiles/gtk_test.dir/main.c.o.provides

CMakeFiles/gtk_test.dir/main.c.o.provides.build: CMakeFiles/gtk_test.dir/main.c.o


# Object files for target gtk_test
gtk_test_OBJECTS = \
"CMakeFiles/gtk_test.dir/main.c.o"

# External object files for target gtk_test
gtk_test_EXTERNAL_OBJECTS =

gtk_test: CMakeFiles/gtk_test.dir/main.c.o
gtk_test: CMakeFiles/gtk_test.dir/build.make
gtk_test: CMakeFiles/gtk_test.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/nhannt/CLionProjects/Chatwork/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable gtk_test"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/gtk_test.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/gtk_test.dir/build: gtk_test

.PHONY : CMakeFiles/gtk_test.dir/build

CMakeFiles/gtk_test.dir/requires: CMakeFiles/gtk_test.dir/main.c.o.requires

.PHONY : CMakeFiles/gtk_test.dir/requires

CMakeFiles/gtk_test.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/gtk_test.dir/cmake_clean.cmake
.PHONY : CMakeFiles/gtk_test.dir/clean

CMakeFiles/gtk_test.dir/depend:
	cd /home/nhannt/CLionProjects/Chatwork/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/nhannt/CLionProjects/Chatwork /home/nhannt/CLionProjects/Chatwork /home/nhannt/CLionProjects/Chatwork/cmake-build-debug /home/nhannt/CLionProjects/Chatwork/cmake-build-debug /home/nhannt/CLionProjects/Chatwork/cmake-build-debug/CMakeFiles/gtk_test.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/gtk_test.dir/depend

