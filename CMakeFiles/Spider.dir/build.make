# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.11

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
CMAKE_COMMAND = /usr/local/bin/cmake

# The command to remove a file.
RM = /usr/local/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/spider

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/spider

# Include any dependencies generated for this target.
include CMakeFiles/Spider.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/Spider.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Spider.dir/flags.make

CMakeFiles/Spider.dir/spider.c.o: CMakeFiles/Spider.dir/flags.make
CMakeFiles/Spider.dir/spider.c.o: spider.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/spider/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/Spider.dir/spider.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/Spider.dir/spider.c.o   -c /home/spider/spider.c

CMakeFiles/Spider.dir/spider.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/Spider.dir/spider.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/spider/spider.c > CMakeFiles/Spider.dir/spider.c.i

CMakeFiles/Spider.dir/spider.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/Spider.dir/spider.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/spider/spider.c -o CMakeFiles/Spider.dir/spider.c.s

# Object files for target Spider
Spider_OBJECTS = \
"CMakeFiles/Spider.dir/spider.c.o"

# External object files for target Spider
Spider_EXTERNAL_OBJECTS =

Spider: CMakeFiles/Spider.dir/spider.c.o
Spider: CMakeFiles/Spider.dir/build.make
Spider: CMakeFiles/Spider.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/spider/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable Spider"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Spider.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Spider.dir/build: Spider

.PHONY : CMakeFiles/Spider.dir/build

CMakeFiles/Spider.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Spider.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Spider.dir/clean

CMakeFiles/Spider.dir/depend:
	cd /home/spider && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/spider /home/spider /home/spider /home/spider /home/spider/CMakeFiles/Spider.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/Spider.dir/depend

