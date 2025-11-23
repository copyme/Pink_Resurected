cmake_minimum_required(VERSION 3.10)

project(pink_clean C CXX)

# ==============================================================================
# 1. SYSTEM CONFIGURATION
# ==============================================================================
if(CYGWIN)
    add_compile_definitions(PC UNIXIO CYGWIN)
elseif(UNIX)
    add_compile_definitions(UNIXIO)
elseif(WIN32)
    add_compile_definitions(DOSIO)
endif()

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    add_compile_definitions(MC_64_BITS)
endif()

option(BUILD_SHARED_LIBS "Build using shared libraries" ON)
option(PINK_WITH_TIFF "Build with TIFF support" ON)


if(UNIX)
    add_compile_definitions(_FILE_OFFSET_BITS=64)
    add_compile_definitions(_LARGEFILE_SOURCE)

    # This tells the compiler: "Enable POSIX 2008 features" (includes fseeko)
    add_compile_definitions(_POSIX_C_SOURCE=200809L)
    # Enable standard Linux/BSD legacy definitions (struct timezone)
    add_compile_definitions(_DEFAULT_SOURCE)
endif()


# ==============================================================================
# 2. BUILD LIBRARY
# ==============================================================================
file(GLOB PINK_LIB_SOURCES
    "src/lib/*.c"
    "src/lib/*.cxx"
    "src/lib/*.cpp"
)


add_library(pink ${PINK_LIB_SOURCES})

if(UNIX AND NOT APPLE)
    target_link_options(pink PRIVATE "-Wl,--allow-multiple-definition")
endif()

target_include_directories(pink PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/include>
    $<INSTALL_INTERFACE:include>
)

target_link_libraries(pink PRIVATE m)

if(PINK_WITH_TIFF)

    find_package(Tiff 4.7 QUIET)

    if(NOT Tiff_FOUND AND NOT TIFF_FOUND)
        find_package(TIFF REQUIRED)
    endif()

    target_compile_definitions(pink PUBLIC HAVE_TIFF_LIB)

    if(TARGET TIFF::TIFF)
        target_link_libraries(pink PUBLIC TIFF::TIFF)
    elseif(TARGET Tiff::Tiff)
        target_link_libraries(pink PUBLIC Tiff::Tiff)
    else()

        # Resolve Includes
        if(DEFINED TIFF_INCLUDE_DIRS)
            target_include_directories(pink PUBLIC ${TIFF_INCLUDE_DIRS})
        elseif(DEFINED Tiff_INCLUDE_DIRS)
            target_include_directories(pink PUBLIC ${Tiff_INCLUDE_DIRS})
        elseif(DEFINED TIFF_INCLUDE_DIR)
            target_include_directories(pink PUBLIC ${TIFF_INCLUDE_DIR})
        endif()

        # Resolve Libraries
        if(DEFINED TIFF_LIBRARIES)
            target_link_libraries(pink PUBLIC ${TIFF_LIBRARIES})
        elseif(DEFINED Tiff_LIBRARIES)
            target_link_libraries(pink PUBLIC ${Tiff_LIBRARIES})
        elseif(DEFINED TIFF_LIBRARY)
            target_link_libraries(pink PUBLIC ${TIFF_LIBRARY})
        endif()
    endif()
endif()

set_property(TARGET pink PROPERTY POSITION_INDEPENDENT_CODE ON)

# ==============================================================================
# 3. BUILD TOOLS
# ==============================================================================
file(GLOB TOOL_GROUPS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}/src/tools ${CMAKE_CURRENT_SOURCE_DIR}/src/tools/*)

foreach(GROUP_DIR ${TOOL_GROUPS})
    if(IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/src/tools/${GROUP_DIR}")
        string(TOUPPER "${GROUP_DIR}" UPPER_GROUP)

        set(DEFAULT_STATE OFF)
        if("${UPPER_GROUP}" STREQUAL "ARITH")
            set(DEFAULT_STATE ON)
        endif()

        option(BUILD_GROUP_${UPPER_GROUP} "Build tools in ${GROUP_DIR}" ${DEFAULT_STATE})

        if(BUILD_GROUP_${UPPER_GROUP})
            message(STATUS "  [+] Group ${GROUP_DIR}: ENABLED")

            file(GLOB GROUP_SOURCES
                "src/tools/${GROUP_DIR}/*.c"
                "src/tools/${GROUP_DIR}/*.cxx"
            )

            foreach(TOOL_SRC ${GROUP_SOURCES})
                get_filename_component(TOOL_NAME ${TOOL_SRC} NAME_WE)
                add_executable(${TOOL_NAME} ${TOOL_SRC})

                # --- HYBRID LINKING TRICK ---
                # Even if the tool is a .c file, we force the LINKER to be the C++ linker.
                # This ensures that when it links against libpink.so, it pulls in the
                # C++ runtime (libstdc++) required by the internal C++ parts of libpink.
                set_target_properties(${TOOL_NAME} PROPERTIES LINKER_LANGUAGE CXX)

                set_target_properties(${TOOL_NAME} PROPERTIES
                    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/tools/${GROUP_DIR}"
                )

                target_link_libraries(${TOOL_NAME} PRIVATE pink m)

                if(PINK_WITH_TIFF)
                    target_compile_definitions(${TOOL_NAME} PRIVATE HAVE_TIFF_LIB)
                endif()
            endforeach()
        endif()
    endif()
endforeach()
