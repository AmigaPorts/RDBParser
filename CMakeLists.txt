cmake_minimum_required(VERSION 3.17)
project(RDBParser C CXX)

set(CMAKE_DEBUG_POSTFIX _d)

set(BIN_DIR "bin" CACHE STRING "Binary output directory")

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/${BIN_DIR})

# Second, for multi-config builds (e.g. msvc)
foreach( OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES} )
	string( TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG )
	set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${PROJECT_SOURCE_DIR}/lib )
	set( CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${PROJECT_SOURCE_DIR}/lib )
	set( CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${PROJECT_SOURCE_DIR}/${BIN_DIR} )
endforeach( OUTPUTCONFIG CMAKE_CONFIGURATION_TYPES )

link_directories(${PROJECT_SOURCE_DIR}/lib)

# Adhere to GNU filesystem layout conventions
include(GNUInstallDirs)

# Lowercase project name for binaries and packaging
string(TOLOWER ${PROJECT_NAME} PROJECT_NAME_LOWER)

# Additional CMake modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/cmake)

# Version number in format X.YY.ZZ
set(VER_X 0)
set(VER_Y 0)
set(VER_Z 1)
set(VER_EXTRA "test" CACHE STRING "Extra version")

set(VER_FULL "${VER_X}.${VER_Y}.${VER_Z}-${VER_EXTRA}")
set(VER_AMIGA "${VER_X}.${VER_Y}${VER_Z}")

# Build date Information
string(TIMESTAMP VER_YEAR "%Y")
string(TIMESTAMP VER_MONTH "%m")
string(TIMESTAMP VER_DAY "%d")
string(REGEX REPLACE "^0+" "" VER_MONTH "${VER_MONTH}")
string(REGEX REPLACE "^0+" "" VER_DAY "${VER_DAY}")

set(CREDITS "Marlon Beijer (marlon@amigadev.com)")


STRING(REGEX REPLACE " " "-" VER_CPACK ${VER_FULL})

# Generate version header from the above
configure_file(
		${PROJECT_SOURCE_DIR}/include/IConfig.h.in
		${PROJECT_BINARY_DIR}/include/IConfig.h
)

if(NOT AMIGA)
	file(DOWNLOAD "https://raw.githubusercontent.com/aros-development-team/AROS/master/compiler/include/devices/hardblocks.h" "${PROJECT_BINARY_DIR}/include/devices/hardblocks.h")
	file(WRITE "${PROJECT_BINARY_DIR}/include/exec/types.h" "#ifndef EXEC_TYPES_H\n#define EXEC_TYPES_H\n#include <cstdio>\n#include <cstring>\n#include <cerrno>\n#include <cstdlib>\n#include <netinet/in.h>\n#include <vector>\n\n#define ULONG uint32_t\n#define LONG int32_t\n#define UBYTE unsigned char\n#endif")
endif()

set(CMAKE_CXX_STANDARD 20)

option(NOSTATIC "Don't compile as a static runtime." OFF)
if(NOSTATIC)
	message("Don't compile as a static runtime")
	set(BUILD_SHARED_LIBS TRUE CACHE BOOL "-" FORCE)
else()
	message("Compile as static runtime")
	add_definitions(-DSTATICLIB)
	set(BUILD_SHARED_LIBS FALSE CACHE BOOL "-" FORCE)
endif()


# Packaging
if(APPLE)
	set(CPACK_GENERATOR DragNDrop)
	set(CPACK_DMG_VOLUME_NAME "${PROJECT_NAME} ${VER_FULL}")
	set(
			CPACK_DMG_DS_STORE_SETUP_SCRIPT
			${PROJECT_SOURCE_DIR}/resources/packaging/osx/DMGSetup.scpt
	)
	set(
			CPACK_DMG_BACKGROUND_IMAGE
			${PROJECT_SOURCE_DIR}/resources/packaging/osx/DMGBackground.tif
	)
elseif(WIN32)
	set(CPACK_GENERATOR "ZIP")
elseif(AMIGA)
	set(CPACK_GENERATOR LHA)
else()
	set(CPACK_GENERATOR TGZ)
endif()

set(CPACK_PACKAGE_NAME ${PROJECT_NAME_LOWER})
set(CPACK_PACKAGE_VENDOR "Graal Reborn Team")
set(CPACK_PACKAGE_CONTACT "graal.in")
set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${VER_CPACK}")
set(CPACK_PACKAGE_VERSION_MAJOR ${VER_X})
set(CPACK_PACKAGE_VERSION_MINOR ${VER_Y})
set(CPACK_PACKAGE_VERSION_PATCH ${VER_Z}${VER_EXTRA})
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Graal Online v1.411 to v5.002 compatible server")
#set(CPACK_PACKAGE_DESCRIPTION_FILE ${PROJECT_SOURCE_DIR}/README.md)
#set(CPACK_RESOURCE_FILE_README ${PROJECT_SOURCE_DIR}/README.md)
#set(CPACK_RESOURCE_FILE_LICENSE ${PROJECT_SOURCE_DIR}/LICENSE.md)
set(CPACK_SOURCE_GENERATOR TGZ)
set(CPACK_SOURCE_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${VER_CPACK}-src")
set(CPACK_SOURCE_IGNORE_FILES "/build/;/.bzr/;~$;${CPACK_SOURCE_IGNORE_FILES}")
include(CPack)

# Prefer static linkage
if(NOT NOSTATIC)
	set(SUFFIXES_ORIG ${CMAKE_FIND_LIBRARY_SUFFIXES})
	set(CMAKE_FIND_LIBRARY_SUFFIXES .a ${CMAKE_FIND_LIBRARY_SUFFIXES})
endif()

# Restore library suffixes
if(NOT NOSTATIC)
	set(CMAKE_FIND_LIBRARY_SUFFIXES ${SUFFIXES_ORIG})
endif()

include_directories("${PROJECT_SOURCE_DIR}/include" "${PROJECT_BINARY_DIR}/include")
add_executable(${PROJECT_NAME} src/main.cpp include/main.h)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
