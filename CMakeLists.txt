cmake_minimum_required( VERSION 3.16 FATAL_ERROR )

if( ${CMAKE_BINARY_DIR} STREQUAL ${CMAKE_SOURCE_DIR} )
   message( FATAL_ERROR "'CMAKE_BINARY_DIR' and 'CMAKE_SOURCE_DIR' cannot have the same value." )
endif( )



set( FENIX_SUBDIR "./build/cmake/" )
include( ${FENIX_SUBDIR}/init.cmake )



fenix_project( TARGET fenix PROJECT FENIX )

fenix_add_subdirectory( sources )

fenix_get_all_targets( TARGETS )
generate_dependencies_graph( ${PROJECT_GEN_DIR} ${CMAKE_PROJECT_NAME} "${TARGETS}" )
