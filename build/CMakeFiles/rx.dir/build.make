# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.10

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/cancam/workspace/ceng513-project/final_software_stack_GroupC

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/cancam/workspace/ceng513-project/final_software_stack_GroupC/build

# Include any dependencies generated for this target.
include CMakeFiles/rx.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/rx.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/rx.dir/flags.make

CMakeFiles/rx.dir/src/ofdm_rx.c.o: CMakeFiles/rx.dir/flags.make
CMakeFiles/rx.dir/src/ofdm_rx.c.o: ../src/ofdm_rx.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/cancam/workspace/ceng513-project/final_software_stack_GroupC/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/rx.dir/src/ofdm_rx.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/rx.dir/src/ofdm_rx.c.o   -c /home/cancam/workspace/ceng513-project/final_software_stack_GroupC/src/ofdm_rx.c

CMakeFiles/rx.dir/src/ofdm_rx.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/rx.dir/src/ofdm_rx.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/cancam/workspace/ceng513-project/final_software_stack_GroupC/src/ofdm_rx.c > CMakeFiles/rx.dir/src/ofdm_rx.c.i

CMakeFiles/rx.dir/src/ofdm_rx.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/rx.dir/src/ofdm_rx.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/cancam/workspace/ceng513-project/final_software_stack_GroupC/src/ofdm_rx.c -o CMakeFiles/rx.dir/src/ofdm_rx.c.s

CMakeFiles/rx.dir/src/ofdm_rx.c.o.requires:

.PHONY : CMakeFiles/rx.dir/src/ofdm_rx.c.o.requires

CMakeFiles/rx.dir/src/ofdm_rx.c.o.provides: CMakeFiles/rx.dir/src/ofdm_rx.c.o.requires
	$(MAKE) -f CMakeFiles/rx.dir/build.make CMakeFiles/rx.dir/src/ofdm_rx.c.o.provides.build
.PHONY : CMakeFiles/rx.dir/src/ofdm_rx.c.o.provides

CMakeFiles/rx.dir/src/ofdm_rx.c.o.provides.build: CMakeFiles/rx.dir/src/ofdm_rx.c.o


CMakeFiles/rx.dir/src/config.c.o: CMakeFiles/rx.dir/flags.make
CMakeFiles/rx.dir/src/config.c.o: ../src/config.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/cancam/workspace/ceng513-project/final_software_stack_GroupC/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/rx.dir/src/config.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/rx.dir/src/config.c.o   -c /home/cancam/workspace/ceng513-project/final_software_stack_GroupC/src/config.c

CMakeFiles/rx.dir/src/config.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/rx.dir/src/config.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/cancam/workspace/ceng513-project/final_software_stack_GroupC/src/config.c > CMakeFiles/rx.dir/src/config.c.i

CMakeFiles/rx.dir/src/config.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/rx.dir/src/config.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/cancam/workspace/ceng513-project/final_software_stack_GroupC/src/config.c -o CMakeFiles/rx.dir/src/config.c.s

CMakeFiles/rx.dir/src/config.c.o.requires:

.PHONY : CMakeFiles/rx.dir/src/config.c.o.requires

CMakeFiles/rx.dir/src/config.c.o.provides: CMakeFiles/rx.dir/src/config.c.o.requires
	$(MAKE) -f CMakeFiles/rx.dir/build.make CMakeFiles/rx.dir/src/config.c.o.provides.build
.PHONY : CMakeFiles/rx.dir/src/config.c.o.provides

CMakeFiles/rx.dir/src/config.c.o.provides.build: CMakeFiles/rx.dir/src/config.c.o


CMakeFiles/rx.dir/src/transceive.c.o: CMakeFiles/rx.dir/flags.make
CMakeFiles/rx.dir/src/transceive.c.o: ../src/transceive.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/cancam/workspace/ceng513-project/final_software_stack_GroupC/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object CMakeFiles/rx.dir/src/transceive.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/rx.dir/src/transceive.c.o   -c /home/cancam/workspace/ceng513-project/final_software_stack_GroupC/src/transceive.c

CMakeFiles/rx.dir/src/transceive.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/rx.dir/src/transceive.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/cancam/workspace/ceng513-project/final_software_stack_GroupC/src/transceive.c > CMakeFiles/rx.dir/src/transceive.c.i

CMakeFiles/rx.dir/src/transceive.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/rx.dir/src/transceive.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/cancam/workspace/ceng513-project/final_software_stack_GroupC/src/transceive.c -o CMakeFiles/rx.dir/src/transceive.c.s

CMakeFiles/rx.dir/src/transceive.c.o.requires:

.PHONY : CMakeFiles/rx.dir/src/transceive.c.o.requires

CMakeFiles/rx.dir/src/transceive.c.o.provides: CMakeFiles/rx.dir/src/transceive.c.o.requires
	$(MAKE) -f CMakeFiles/rx.dir/build.make CMakeFiles/rx.dir/src/transceive.c.o.provides.build
.PHONY : CMakeFiles/rx.dir/src/transceive.c.o.provides

CMakeFiles/rx.dir/src/transceive.c.o.provides.build: CMakeFiles/rx.dir/src/transceive.c.o


CMakeFiles/rx.dir/src/utils.c.o: CMakeFiles/rx.dir/flags.make
CMakeFiles/rx.dir/src/utils.c.o: ../src/utils.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/cancam/workspace/ceng513-project/final_software_stack_GroupC/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building C object CMakeFiles/rx.dir/src/utils.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/rx.dir/src/utils.c.o   -c /home/cancam/workspace/ceng513-project/final_software_stack_GroupC/src/utils.c

CMakeFiles/rx.dir/src/utils.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/rx.dir/src/utils.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/cancam/workspace/ceng513-project/final_software_stack_GroupC/src/utils.c > CMakeFiles/rx.dir/src/utils.c.i

CMakeFiles/rx.dir/src/utils.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/rx.dir/src/utils.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/cancam/workspace/ceng513-project/final_software_stack_GroupC/src/utils.c -o CMakeFiles/rx.dir/src/utils.c.s

CMakeFiles/rx.dir/src/utils.c.o.requires:

.PHONY : CMakeFiles/rx.dir/src/utils.c.o.requires

CMakeFiles/rx.dir/src/utils.c.o.provides: CMakeFiles/rx.dir/src/utils.c.o.requires
	$(MAKE) -f CMakeFiles/rx.dir/build.make CMakeFiles/rx.dir/src/utils.c.o.provides.build
.PHONY : CMakeFiles/rx.dir/src/utils.c.o.provides

CMakeFiles/rx.dir/src/utils.c.o.provides.build: CMakeFiles/rx.dir/src/utils.c.o


# Object files for target rx
rx_OBJECTS = \
"CMakeFiles/rx.dir/src/ofdm_rx.c.o" \
"CMakeFiles/rx.dir/src/config.c.o" \
"CMakeFiles/rx.dir/src/transceive.c.o" \
"CMakeFiles/rx.dir/src/utils.c.o"

# External object files for target rx
rx_EXTERNAL_OBJECTS =

rx: CMakeFiles/rx.dir/src/ofdm_rx.c.o
rx: CMakeFiles/rx.dir/src/config.c.o
rx: CMakeFiles/rx.dir/src/transceive.c.o
rx: CMakeFiles/rx.dir/src/utils.c.o
rx: CMakeFiles/rx.dir/build.make
rx: CMakeFiles/rx.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/cancam/workspace/ceng513-project/final_software_stack_GroupC/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Linking C executable rx"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/rx.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/rx.dir/build: rx

.PHONY : CMakeFiles/rx.dir/build

CMakeFiles/rx.dir/requires: CMakeFiles/rx.dir/src/ofdm_rx.c.o.requires
CMakeFiles/rx.dir/requires: CMakeFiles/rx.dir/src/config.c.o.requires
CMakeFiles/rx.dir/requires: CMakeFiles/rx.dir/src/transceive.c.o.requires
CMakeFiles/rx.dir/requires: CMakeFiles/rx.dir/src/utils.c.o.requires

.PHONY : CMakeFiles/rx.dir/requires

CMakeFiles/rx.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/rx.dir/cmake_clean.cmake
.PHONY : CMakeFiles/rx.dir/clean

CMakeFiles/rx.dir/depend:
	cd /home/cancam/workspace/ceng513-project/final_software_stack_GroupC/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/cancam/workspace/ceng513-project/final_software_stack_GroupC /home/cancam/workspace/ceng513-project/final_software_stack_GroupC /home/cancam/workspace/ceng513-project/final_software_stack_GroupC/build /home/cancam/workspace/ceng513-project/final_software_stack_GroupC/build /home/cancam/workspace/ceng513-project/final_software_stack_GroupC/build/CMakeFiles/rx.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/rx.dir/depend

