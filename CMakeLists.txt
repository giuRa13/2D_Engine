cmake_minimum_required(VERSION 3.26.0)
project(base LANGUAGES C CXX)
#if(POLICY CMP0072)
#    cmake_policy(SET CMP0072 NEW)
#else(NOT POLICY CMP0072)
#    message(STATUS "Error")
#endif(POLICY CMP0072)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
#set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/)

option(ENABLE_WARNINGS "Enable to add warnings to a target." ON)
option(ENABLE_WARNINGS_AS_ERRORS "Enable to treat warnings as errors." OFF)
option(ENABLE_CLANG_TIDY "Enable to add clang tidy." ON)
option(ENABLE_CLANG_FORMAT "Enable to add clang-format." ON)

#include(Warnings)
#include(ConfigSafeGuards)

set (VCPKG_DIR "/home/giuli/vcpkg")
include(${VCPKG_DIR}/scripts/buildsystems/vcpkg.cmake)

find_package(glad CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
#find_package(assimp CONFIG REQUIRED)
find_package(EnTT CONFIG REQUIRED)
#find_package(SDL2 CONFIG REQUIRED)
#find_package(SDL2_image CONFIG REQUIRED)
#find_package(SDL2_mixer CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(imguizmo CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(Stb REQUIRED)
find_package(yaml-cpp CONFIG REQUIRED)
find_package(nfd CONFIG REQUIRED)
#find_package(fmt CONFIG REQUIRED)


#add_library(external_lib STATIC external/lodepng.cpp)
#target_include_directories(external_lib PUBLIC
#    external/
#)

add_executable(base
    editor/EditorLayer.cpp
    editor/EditorApp.cpp
    editor/FileDialog.cpp
    editor/panels/SceneHierarchyPanel.cpp

    sandbox/SandBox.cpp
    sandbox/SandBox2D.cpp

    src/application.cpp
    src/Log.cpp
    src/Window.cpp
    src/WindowGLFW.cpp
    src/WindowInput.cpp

    src/layers/Layer.cpp
    src/layers/LayerStack.cpp
    src/layers/ImGuiLayer.cpp

    src/renderer/Shader.cpp
    src/renderer/RendererAPI.cpp
    src/renderer/RenderCommand.cpp
    src/renderer/Renderer.cpp
    src/renderer/Buffer.cpp
    src/renderer/VertexArray.cpp
    src/renderer/OrthographicCamera.cpp
    src/renderer/OrthoCameraController.cpp
    src/renderer/Texture.cpp
    src/renderer/SubTexture2D.cpp
    src/renderer/Renderer2D.cpp
    src/renderer/Framebuffer.cpp

    src/renderer/OpenGL/OpenGLContext.cpp
    src/renderer/OpenGL/OpenGLBuffer.cpp
    src/renderer/OpenGL/OpenGLVertexArray.cpp
    src/renderer/OpenGL/OpenGLRendererAPI.cpp
    src/renderer/OpenGL/OpenGLShader.cpp
    src/renderer/OpenGL/OpenGLTexture.cpp
    src/renderer/OpenGL/OpenGLFramebuffer.cpp

    src/scene/Scene.cpp
    src/scene/Entity.cpp
    src/scene/SceneCamera.cpp
    src/scene/SceneSerializer.cpp
    src/scene/Math.cpp
)
    

add_compile_definitions(
        PROJECT_PATH="${CMAKE_SOURCE_DIR}"
)

#include(cmake/AddResources.cmake)

target_include_directories(${PROJECT_NAME} PUBLIC
    src
    sandbox
    editor
    ${SIMPLEINI_INCLUDE_DIRS}
)

target_link_libraries(${PROJECT_NAME} PRIVATE glad::glad)
target_link_libraries(${PROJECT_NAME} PRIVATE glm::glm)
target_link_libraries(${PROJECT_NAME} PRIVATE glfw)
#target_link_libraries(${PROJECT_NAME} PRIVATE assimp::assimp)
target_link_libraries(${PROJECT_NAME} PRIVATE EnTT::EnTT)
#target_link_libraries(${PROJECT_NAME} PRIVATE SDL2::SDL2)
#target_link_libraries(${PROJECT_NAME} PRIVATE SDL2_image)
#target_link_libraries(${PROJECT_NAME} PRIVATE SDL2_mixer)
target_link_libraries(${PROJECT_NAME} PRIVATE imgui::imgui)
target_link_libraries(${PROJECT_NAME} PRIVATE imguizmo::imguizmo)
target_link_libraries(${PROJECT_NAME} PRIVATE spdlog::spdlog)
#target_link_libraries(${PROJECT_NAME} PRIVATE external_lib)
#target_link_libraries(${PROJECT_NAME} PRIVATE fmt::fmt-header-only)
target_link_libraries(${PROJECT_NAME} PRIVATE yaml-cpp::yaml-cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE nfd::nfd)




target_compile_options(
    base PRIVATE $<$<COMPILE_LANGUAGE:CXX>:${CXX_COMPILE_FLAGS}>)