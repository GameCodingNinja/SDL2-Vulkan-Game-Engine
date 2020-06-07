# Add the game level executable files
add_executable(
    ${PROJECT_NAME}
    consoleSimpleApp.cpp )

# List all the libraries to link against
# These need to be listed in order of dependency
target_link_libraries(
    ${PROJECT_NAME} )

install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_BINARY_DIR})