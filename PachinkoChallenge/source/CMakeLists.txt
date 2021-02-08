# To build a release version on Linux, from within this project folder
# mkdir release
# cd release
# cmake -DCMAKE_BUILD_TYPE=Release ..
# make

# Add the game level executable files
add_executable(
    ${PROJECT_NAME}
    game/game.cpp
    game/main.cpp )

if(${CMAKE_SYSTEM_PROCESSOR} MATCHES "^arm")
    # List all the libraries to link against
    target_link_libraries(
        ${PROJECT_NAME}
        ${CMAKE_BINARY_DIR}/library/liblibrary.a
        ${CMAKE_BINARY_DIR}/angelscript/libangelscript.a
        ${CMAKE_BINARY_DIR}/Box2D/libBox2D.a
        ${CMAKE_BINARY_DIR}/bulletPhysics/libbulletPhysics.a
        ${Vulkan_LIBRARY} 
        /usr/lib/arm-linux-gnueabihf/libSDL2.so
        /usr/lib/arm-linux-gnueabihf/libSDL2_mixer.so )
else()
    # List all the libraries to link against
    # These need to be listed in order of dependency
    target_link_libraries(
        ${PROJECT_NAME}
        ${CMAKE_BINARY_DIR}/library/liblibrary.a
        ${CMAKE_BINARY_DIR}/angelscript/libangelscript.a
        ${CMAKE_BINARY_DIR}/Box2D/libBox2D.a
        ${CMAKE_BINARY_DIR}/bulletPhysics/libbulletPhysics.a
        ${Vulkan_LIBRARY}
        ${SDL2_LIBRARY}
        ${SDL2MIXER_LIBRARY} )
endif()

install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_BINARY_DIR})
