cmake_minimum_required(VERSION 3.20)

project(SpaceShooter3D LANGUAGES CXX)

# Toolchain notes:
# - On Windows, use the MSVC toolchain in CLion for easiest DirectX linking.
# - Requires legacy DirectX 9 SDK (June 2010) or the Windows 10 SDK components that still provide these libs.
# - If you use MinGW, you may need to adjust the library names/availability.

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Source files
file(GLOB_RECURSE SPACE_SHOOTER_SOURCES
    CONFIGURE_DEPENDS
    "src/*.cpp"
)


add_executable(${PROJECT_NAME}
    ${SPACE_SHOOTER_SOURCES}
)

# Copy entire `Media` folder to the executable output directory after build
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:${PROJECT_NAME}>/Media
  COMMAND ${CMAKE_COMMAND} -E copy_directory
          ${CMAKE_CURRENT_SOURCE_DIR}/Media $<TARGET_FILE_DIR:${PROJECT_NAME}>/Media
  COMMENT "Copying `Media` folder to output directory"
)

target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Common Windows definitions (the original code targets Win32/DirectX)
if (WIN32)
    target_compile_definitions(${PROJECT_NAME}
        PRIVATE
            WIN32
            _WINDOWS
            _CRT_SECURE_NO_WARNINGS
    )

    # Link against DirectX 9, DirectInput, XInput, and common Win32 libs
    # Adjust as needed depending on your installed SDKs
    target_link_libraries(${PROJECT_NAME}
        PRIVATE
            d3d9
            d3dx9
            dinput8
            dxguid
            xinput9_1_0
            winmm
            user32
            gdi32
            ole32
            oleaut32
            advapi32
            shell32
            uuid
    )
endif()
