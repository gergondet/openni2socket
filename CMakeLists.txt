## CMake file for plugin_object_tracker
###
###    From the off-tree build directory, invoke:
###	 cmake <PATH_TO_vision_server>
###
###
###   - Feb 2010: Initial version <druon@lirmm.fr>
###
###
###############################################################################


cmake_minimum_required (VERSION 2.6)

project (openni2socket)

set( CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} /lib /usr/lib /usr/local/lib )
set( CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} /usr/include /usr/local/include )

set( CMAKE_MODULE_PATH  ${CMAKE_MODULE_PATH}  
            "${PROJECT_SOURCE_DIR}/cmake" )

set ( INSTALL_BIN_PATH bin )
set ( INSTALL_LIB_PATH lib )
set ( INSTALL_AR_PATH  lib )
set ( INSTALL_HEADERS_PATH include )

# Make relative paths absolute (needed later on)

foreach(p LIB BIN HEADERS AR)
        set(var INSTALL_${p}_PATH)
        if(NOT IS_ABSOLUTE "${${var}}")
                set(${var} "${CMAKE_INSTALL_PREFIX}/${${var}}")
        endif()
endforeach()

### Search for Lib/Headers

find_package( configparser )
find_package( vision )
find_package( visionsystem )
find_package( Boost 1.40.0 COMPONENTS system thread filesystem)
find_package( OpenCV REQUIRED )

#
### Set include directories 
        
include_directories( "${CMAKE_CURRENT_SOURCE_DIR}/include"
	             "${CMAKE_CURRENT_BINARY_DIR}"
		     ${VISION_INCLUDE_DIRS} 
		     ${VISIONSYSTEM_INCLUDE_DIRS} 
             ${Boost_INCLUDE_DIRS}
		     )

link_directories( "${VISIONSYSTEM_LIBRARY_DIRS}" "${VISION_LIBRARY_DIR}" ${INSTALL_LIB_PATH}/vs_plugins)

### Go to subdirs

SET( openni2socket_lib_src 
src/openni2socket.cpp
)

SET( openni2socket_lib_hdr
src/openni2socket.h
)

add_library( "openni2socket" SHARED ${openni2socket_lib_src} ${openni2socket_lib_hdr} )

target_link_libraries( "openni2socket"
                     stream2socket
					 ${VISIONSYSTEM_LIBRARIES}
				     ${VISION_LIBRARIES} 
                    ${CONFIGPARSER_LIBRARIES} 
                    ${OpenCV_LIBS}
                    )

### INSTAll

install( TARGETS "openni2socket" LIBRARY DESTINATION
     ${INSTALL_LIB_PATH}  COMPONENT main )

