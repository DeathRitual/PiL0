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

# The program to use to edit the cache.
CMAKE_EDIT_COMMAND = /usr/bin/ccmake

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/nirvash/Projects/PiL0

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/nirvash/Projects/PiL0/build

# Include any dependencies generated for this target.
include CMakeFiles/PiL0.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/PiL0.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/PiL0.dir/flags.make

CMakeFiles/PiL0.dir/tokdef.o: CMakeFiles/PiL0.dir/flags.make
CMakeFiles/PiL0.dir/tokdef.o: ../tokdef.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/nirvash/Projects/PiL0/build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object CMakeFiles/PiL0.dir/tokdef.o"
	/usr/bin/gcc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/PiL0.dir/tokdef.o   -c /home/nirvash/Projects/PiL0/tokdef.c

CMakeFiles/PiL0.dir/tokdef.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/PiL0.dir/tokdef.i"
	/usr/bin/gcc  $(C_DEFINES) $(C_FLAGS) -E /home/nirvash/Projects/PiL0/tokdef.c > CMakeFiles/PiL0.dir/tokdef.i

CMakeFiles/PiL0.dir/tokdef.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/PiL0.dir/tokdef.s"
	/usr/bin/gcc  $(C_DEFINES) $(C_FLAGS) -S /home/nirvash/Projects/PiL0/tokdef.c -o CMakeFiles/PiL0.dir/tokdef.s

CMakeFiles/PiL0.dir/tokdef.o.requires:
.PHONY : CMakeFiles/PiL0.dir/tokdef.o.requires

CMakeFiles/PiL0.dir/tokdef.o.provides: CMakeFiles/PiL0.dir/tokdef.o.requires
	$(MAKE) -f CMakeFiles/PiL0.dir/build.make CMakeFiles/PiL0.dir/tokdef.o.provides.build
.PHONY : CMakeFiles/PiL0.dir/tokdef.o.provides

CMakeFiles/PiL0.dir/tokdef.o.provides.build: CMakeFiles/PiL0.dir/tokdef.o

CMakeFiles/PiL0.dir/lexer.o: CMakeFiles/PiL0.dir/flags.make
CMakeFiles/PiL0.dir/lexer.o: ../lexer.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/nirvash/Projects/PiL0/build/CMakeFiles $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object CMakeFiles/PiL0.dir/lexer.o"
	/usr/bin/gcc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/PiL0.dir/lexer.o   -c /home/nirvash/Projects/PiL0/lexer.c

CMakeFiles/PiL0.dir/lexer.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/PiL0.dir/lexer.i"
	/usr/bin/gcc  $(C_DEFINES) $(C_FLAGS) -E /home/nirvash/Projects/PiL0/lexer.c > CMakeFiles/PiL0.dir/lexer.i

CMakeFiles/PiL0.dir/lexer.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/PiL0.dir/lexer.s"
	/usr/bin/gcc  $(C_DEFINES) $(C_FLAGS) -S /home/nirvash/Projects/PiL0/lexer.c -o CMakeFiles/PiL0.dir/lexer.s

CMakeFiles/PiL0.dir/lexer.o.requires:
.PHONY : CMakeFiles/PiL0.dir/lexer.o.requires

CMakeFiles/PiL0.dir/lexer.o.provides: CMakeFiles/PiL0.dir/lexer.o.requires
	$(MAKE) -f CMakeFiles/PiL0.dir/build.make CMakeFiles/PiL0.dir/lexer.o.provides.build
.PHONY : CMakeFiles/PiL0.dir/lexer.o.provides

CMakeFiles/PiL0.dir/lexer.o.provides.build: CMakeFiles/PiL0.dir/lexer.o

# Object files for target PiL0
PiL0_OBJECTS = \
"CMakeFiles/PiL0.dir/tokdef.o" \
"CMakeFiles/PiL0.dir/lexer.o"

# External object files for target PiL0
PiL0_EXTERNAL_OBJECTS =

PiL0: CMakeFiles/PiL0.dir/tokdef.o
PiL0: CMakeFiles/PiL0.dir/lexer.o
PiL0: CMakeFiles/PiL0.dir/build.make
PiL0: CMakeFiles/PiL0.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking C executable PiL0"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/PiL0.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/PiL0.dir/build: PiL0
.PHONY : CMakeFiles/PiL0.dir/build

CMakeFiles/PiL0.dir/requires: CMakeFiles/PiL0.dir/tokdef.o.requires
CMakeFiles/PiL0.dir/requires: CMakeFiles/PiL0.dir/lexer.o.requires
.PHONY : CMakeFiles/PiL0.dir/requires

CMakeFiles/PiL0.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/PiL0.dir/cmake_clean.cmake
.PHONY : CMakeFiles/PiL0.dir/clean

CMakeFiles/PiL0.dir/depend:
	cd /home/nirvash/Projects/PiL0/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/nirvash/Projects/PiL0 /home/nirvash/Projects/PiL0 /home/nirvash/Projects/PiL0/build /home/nirvash/Projects/PiL0/build /home/nirvash/Projects/PiL0/build/CMakeFiles/PiL0.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/PiL0.dir/depend

