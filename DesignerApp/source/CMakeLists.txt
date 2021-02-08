
# Add the game level executable files
add_executable(
    ${PROJECT_NAME}
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
)

# List all the libraries to link against
# These need to be listed in order of dependency
message(STATUS "Binary Dir: ${CMAKE_BINARY_DIR}")
target_link_libraries(
    ${PROJECT_NAME} PRIVATE Qt${QT_VERSION_MAJOR}::Widgets
    ${CMAKE_BINARY_DIR}/library/liblibrary.a
    ${CMAKE_BINARY_DIR}/angelscript/libangelscript.a
    ${CMAKE_BINARY_DIR}/Box2D/libBox2D.a
    ${CMAKE_BINARY_DIR}/bulletPhysics/libbulletPhysics.a
    ${Vulkan_LIBRARY}
    ${SDL2_LIBRARY}
    ${SDL2MIXER_LIBRARY} )
