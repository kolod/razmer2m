# Toolchain file for AVR-GCC on Windows
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR avr)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# Path to AVR-GCC toolchain
if(NOT AVR_TOOLCHAIN_ROOT)
    if(WIN32)
        find_program(_AVR_GCC_EXE 
            NAMES avr-gcc.exe 
            PATHS
                "C:/Tools/avr-gcc-15.1_2025-07-12_mingw32/bin"
                "C:/Program Files (x86)/Arduino/hardware/tools/avr/bin"
                "C:/Program Files (x86)/Arduino/hardware/tools/avr/bin/avr-gcc"
                "/usr/local/avr/bin"
                "/opt/avr-gcc/bin"
                ENV AVR_GCC_PATH
            OPTIONAL
            NO_CACHE
        )
    else()
        find_program(_AVR_GCC_EXE 
            NAMES avr-gcc 
            PATHS
                "/usr/local/avr/bin"
                "/opt/avr-gcc/bin"
                "/usr/bin"
                ENV AVR_GCC_PATH
            OPTIONAL
            NO_CACHE
        )
    endif()
    if(_AVR_GCC_EXE)
        get_filename_component(_AVR_GCC_BIN_DIR "${_AVR_GCC_EXE}" DIRECTORY)
        get_filename_component(_AVR_GCC_ROOT "${_AVR_GCC_BIN_DIR}" DIRECTORY)
        set(_AVR_TOOLCHAIN_DEFAULT "${_AVR_GCC_ROOT}")
    else()
        message(FATAL_ERROR "AVR-GCC toolchain not found. Please set AVR_TOOLCHAIN_ROOT or AVR_GCC_PATH environment variable or make avr-gcc available in your PATH.")
    endif()
else()
    set(_AVR_TOOLCHAIN_DEFAULT "${AVR_TOOLCHAIN_ROOT}")
endif()

# You can set the AVR_TOOLCHAIN_ROOT variable to point to your AVR-GCC installation
# or set path in AVR_GCC_PATH environment variable or use the default path
set(AVR_TOOLCHAIN_ROOT "${_AVR_TOOLCHAIN_DEFAULT}" CACHE PATH "Path to AVR-GCC toolchain root directory")

# Compiler and toolchain executables
if(WIN32)
    set(CMAKE_C_COMPILER   "${AVR_TOOLCHAIN_ROOT}/bin/avr-gcc.exe")
    set(CMAKE_CXX_COMPILER "${AVR_TOOLCHAIN_ROOT}/bin/avr-g++.exe")
    set(CMAKE_ASM_COMPILER "${AVR_TOOLCHAIN_ROOT}/bin/avr-gcc.exe")
    set(CMAKE_AR           "${AVR_TOOLCHAIN_ROOT}/bin/avr-ar.exe")
    set(CMAKE_RANLIB       "${AVR_TOOLCHAIN_ROOT}/bin/avr-ranlib.exe")
    set(CMAKE_OBJCOPY      "${AVR_TOOLCHAIN_ROOT}/bin/avr-objcopy.exe")
    set(CMAKE_OBJDUMP      "${AVR_TOOLCHAIN_ROOT}/bin/avr-objdump.exe")
    set(CMAKE_SIZE         "${AVR_TOOLCHAIN_ROOT}/bin/avr-size.exe")
else()
    set(CMAKE_C_COMPILER   "${AVR_TOOLCHAIN_ROOT}/bin/avr-gcc")
    set(CMAKE_CXX_COMPILER "${AVR_TOOLCHAIN_ROOT}/bin/avr-g++")
    set(CMAKE_ASM_COMPILER "${AVR_TOOLCHAIN_ROOT}/bin/avr-gcc")
    set(CMAKE_AR           "${AVR_TOOLCHAIN_ROOT}/bin/avr-ar")
    set(CMAKE_RANLIB       "${AVR_TOOLCHAIN_ROOT}/bin/avr-ranlib")
    set(CMAKE_OBJCOPY      "${AVR_TOOLCHAIN_ROOT}/bin/avr-objcopy")
    set(CMAKE_OBJDUMP      "${AVR_TOOLCHAIN_ROOT}/bin/avr-objdump")
    set(CMAKE_SIZE         "${AVR_TOOLCHAIN_ROOT}/bin/avr-size")
endif()

# No standard system paths
set(CMAKE_FIND_ROOT_PATH "${AVR_TOOLCHAIN_ROOT}")

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

# Set the default C and C++ standards
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Set the default C and C++ flags
add_compile_options(-g)                         # Enable debugging information
add_compile_options(-Wall)                      # Enable all warnings
add_compile_options(-Wextra)                    # Enable extra warnings
add_compile_options(-Wpedantic)                 # Enable pedantic warnings
add_compile_options(-Wshadow)                   # Enable warnings about shadowed variables
add_compile_options(-Wsign-conversion)          # Enable warnings about sign conversions
add_compile_options(-Wnull-dereference)         # Enable warnings about null pointer dereferences
add_compile_options(-Wunreachable-code)         # Enable warnings about unreachable code
add_compile_options(-Wuninitialized)            # Enable warnings about uninitialized variables
add_compile_options(-Wfloat-equal)              # Enable warnings about floating point equality
add_compile_options(-Wno-main)                  # Suppress '::main' must return 'int' warning
add_compile_options(-Wno-error=narrowing)       # Do not treat narrowing warnings as errors
add_compile_options(-fno-exceptions)            # Disable exception handling
add_compile_options(-fno-threadsafe-statics)    # Disable thread-safe initialization of static variables
add_compile_options(-flto)                      # Enable Link Time Optimization
add_compile_options(-fpermissive)               # Allow some non-conforming code (mainly for C++)
add_compile_options(-ffunction-sections)        # Place each function in its own section
add_compile_options(-fdata-sections)            # Place each data item in its own section
add_compile_options(-fdiagnostics-color=always) # Enable colored diagnostics
add_compile_options(-fno-strict-aliasing)       # Disable strict aliasing rules
add_compile_options(-fno-common)                # Do not allow common symbols
add_compile_options(-ffreestanding)             # Indicate that the code is freestanding

add_link_options(-Wl,--gc-sections)             # Enable garbage collection of unused sections
add_link_options(-Wl,--relax)                   # Relax linking constraints


# Add include directories
include_directories(${AVR_TOOLCHAIN_ROOT}/avr/include)

# Macro to add a custom target for printing the size of the built ELF file
macro(target_add_size target)
    add_custom_command(
        TARGET ${target}
        POST_BUILD
        COMMAND ${CMAKE_SIZE} $<TARGET_FILE:${target}>
        COMMENT "Printing size of $<TARGET_FILE:${target}>"
        VERBATIM
    )
endmacro()

# Macro to add a custom target for converting ELF to Intel HEX using objcopy
macro(target_create_hex target)
    set(_HEX $<TARGET_FILE_DIR:${target}>/$<TARGET_FILE_BASE_NAME:${target}>.hex)
    set_target_properties(${target} PROPERTIES HEX_FILE "${_HEX}")

    add_custom_command(
        TARGET ${target}
        POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:${target}> ${_HEX}
        COMMENT "Converting $<TARGET_FILE:${target}> to Intel HEX format"
        VERBATIM
    )

endmacro()

# Macro to add a custom target for converting ELF to raw binary using objcopy
macro(target_create_binary target)
    set(_BIN $<TARGET_FILE_DIR:${target}>/$<TARGET_FILE_BASE_NAME:${target}>.bin)
    set_target_properties(${target} PROPERTIES BINARY_FILE "${_BIN}")

    add_custom_command(
        TARGET ${target}
        POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${target}> ${_BIN}
        COMMENT "Generating raw binary file: ${_BIN}"
        VERBATIM
    )

endmacro()

# Macro to add a post-build command for generating a listing file (assembly + source)
macro(target_create_listing target)
    set(_LST $<TARGET_FILE_DIR:${target}>/$<TARGET_FILE_BASE_NAME:${target}>.lst)
    set_property(TARGET ${target} PROPERTY LISTING_FILE "${_LST}")

    add_custom_command(
        TARGET ${target} 
        POST_BUILD
        COMMAND ${CMAKE_OBJDUMP} -d -S $<TARGET_FILE:${target}> > ${_LST}
        COMMENT "Generating listing file: ${_LST}"
        VERBATIM
    )
endmacro()
