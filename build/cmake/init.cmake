cmake_minimum_required( VERSION 3.16 FATAL_ERROR )

if( ${CMAKE_BINARY_DIR} STREQUAL ${CMAKE_SOURCE_DIR} )
   message( FATAL_ERROR "'CMAKE_BINARY_DIR' and 'CMAKE_SOURCE_DIR' cannot have the same value." )
endif( )



include( ${FENIX_SUBDIR}/constants.cmake )
include( ${FENIX_SUBDIR}/functions/message.cmake )
include( ${FENIX_SUBDIR}/functions/base.cmake )
include( ${FENIX_SUBDIR}/configuration.cmake )
include( ${FENIX_SUBDIR}/compiler.cmake )
include( ${FENIX_SUBDIR}/functions/helpers.cmake )
include( ${FENIX_SUBDIR}/functions/generators.cmake )
include( ${FENIX_SUBDIR}/functions/debug.cmake )



###########################################################################################
#                                                                                         #
#                             Compilation include directories                             #
#                                                                                         #
###########################################################################################
include_directories( "$ENV{HOME}/.local/include" )



###########################################################################################
#                                                                                         #
#                                Linkage include directories                              #
#                                                                                         #
###########################################################################################
link_directories( "/usr/local/lib/" )
link_directories( "$ENV{HOME}/.local/lib" )



###########################################################################################
#                                                                                         #
#                                      Debug messages                                     #
#                                                                                         #
###########################################################################################
# print_environment_variables( )
# print_cmake_variables( )
# print_configuration( )
# print_variables( )



###########################################################################################
#                                                                                         #
#                                    Getting timestamps                                   #
#                                                                                         #
###########################################################################################
# string( TIMESTAMP START_TIME "%Y.%m.%d-%H:%M:%S" UTC )
string( TIMESTAMP START_TIME "%s" )
print_variable( START_TIME )

# string( TIMESTAMP END_TIME "%Y.%m.%d-%H:%M:%S" UTC )
string( TIMESTAMP END_TIME "%s" )
print_variable( END_TIME )

math( EXPR ELAPSED_TIME "${END_TIME} - ${START_TIME}" )
msg_wrn( "Elapsed time: ${ELAPSED_TIME} seconds" )
