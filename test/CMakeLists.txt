cmake_minimum_required( VERSION 2.8 )

include_directories( ${CMAKE_CURRENT_SOURCE_DIR} )

set( MAIN_SOURCES main.c )

set( MAIN_EXE_NAME cgltf_test )
add_executable( ${MAIN_EXE_NAME} ${MAIN_SOURCES} )
set_property(TARGET ${MAIN_EXE_NAME} PROPERTY C_STANDARD 99)

target_link_libraries( ${MAIN_EXE_NAME} )

install( TARGETS ${MAIN_EXE_NAME}
	RUNTIME DESTINATION bin )

