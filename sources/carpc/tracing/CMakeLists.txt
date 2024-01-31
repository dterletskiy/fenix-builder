cmake_minimum_required( VERSION 3.16 FATAL_ERROR )

fenix_project( TARGET tracing PROJECT TRACING )



###########################################################################################
#                                                                                         #
#                             Compilation include directories                             #
#                                                                                         #
###########################################################################################
include_directories( ${PROJECT_SOURCE_DIR}/api )



###########################################################################################
#                                                                                         #
#                                Linkage include directories                              #
#                                                                                         #
###########################################################################################



###########################################################################################
#                                                                                         #
#                                   Compile definitions                                   #
#                                                                                         #
###########################################################################################



###########################################################################################
#                                                                                         #
#                                      Code generation                                    #
#                                                                                         #
###########################################################################################
find_files_by_ext( PLANTUML_FILES ${PROJECT_SOURCE_DIR} "${EXTENTIONS_PLANTUML}" )
generate_plantumls( "${PLANTUML_FILES}" PROJECT_GEN_PLANTUML_FILES )



###########################################################################################
#                                                                                         #
#                                   Build source file list                                #
#                                                                                         #
###########################################################################################
find_files_by_ext( PROJECT_SOURCE_FILES ${PROJECT_SOURCE_DIR} "${EXTENTIONS_CPP_SRC}" )



###########################################################################################
#                                                                                         #
#                                      Debug messages                                     #
#                                                                                         #
###########################################################################################
print_variables( )



add_library(
      ${PROJECT_TARGET_NAME} SHARED
      ${PROJECT_SOURCE_FILES}
   )

if( DLT_TRACE )
   target_link_libraries( ${PROJECT_TARGET_NAME} dlt )
endif( )
if( ${OS_TARGET} EQUAL ${OS_ANDROID} )
   target_link_libraries( ${PROJECT_TARGET_NAME} log )
endif( )

add_custom_target( "${PROJECT_TARGET_NAME}_documentation" ALL
      COMMENT "cmake ${PROJECT_TARGET_NAME}_documentation"
      DEPENDS ${PROJECT_GEN_PLANTUML_FILES}
   )

add_dependencies( ${PROJECT_TARGET_NAME} "${PROJECT_TARGET_NAME}_documentation" )

install( TARGETS ${PROJECT_TARGET_NAME} DESTINATION lib )
install( DIRECTORY ${PROJECT_SOURCE_DIR}/api/ DESTINATION include )
if( DLT_TRACE )
   install( FILES ${PROJECT_SOURCE_DIR}/dlt.conf DESTINATION etc )
endif( )



add_custom_target( deploy_${PROJECT_TARGET_NAME}
   DEPENDS ${PROJECT_TARGET_NAME}
   COMMENT "Deploying the project"
)

file( MAKE_DIRECTORY ${CMAKE_INSTALL_PREFIX}/lib/ )
add_custom_command(
   TARGET deploy_${PROJECT_TARGET_NAME} POST_BUILD
   COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${PROJECT_TARGET_NAME}> ${CMAKE_INSTALL_PREFIX}/lib/
   COMMENT "Copying library articats"
)

file( MAKE_DIRECTORY ${CMAKE_INSTALL_PREFIX}/include/ )
add_custom_command(
   TARGET deploy_${PROJECT_TARGET_NAME} POST_BUILD
   COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/api ${CMAKE_INSTALL_PREFIX}/include/
   COMMENT "Copying include artifacts"
)

file( MAKE_DIRECTORY ${CMAKE_INSTALL_PREFIX}/etc/ )
add_custom_command(
   TARGET deploy_${PROJECT_TARGET_NAME} POST_BUILD
   COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/dlt.conf ${CMAKE_INSTALL_PREFIX}/etc/
   COMMENT "Copying configuration articats"
)
