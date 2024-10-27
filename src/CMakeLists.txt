set(SOURCES
    main.cpp
    Grid.h
    IDrawable.h
    IPhysicsObject.h
    MessageBox.h MessageBox.cpp
    Particle.h Particle.cpp
    ParticleGrid.h ParticleGrid.cpp
    Renderer.h Renderer.cpp
)

add_executable(${CMAKE_PROJECT_NAME} MACOSX_BUNDLE ${SOURCES})
# add_executable(${CMAKE_PROJECT_NAME} WIN32 ${SOURCES})

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE SDL3::SDL3)
target_precompile_headers(${CMAKE_PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/pch.h)
