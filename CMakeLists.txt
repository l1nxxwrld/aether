cmake_minimum_required(VERSION 3.28)

cmake_policy(SET CMP0091 NEW)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
set(BUILD_SHARED_LIBS OFF)

option(AETHER_BUILD_STATIC "Build static" OFF)

project(aether
    LANGUAGES
        C
        CXX
        ASM_MASM
)

include(FetchContent)

message(STATUS "Fetching json (v3.11.3)...")
FetchContent_Declare(json
	GIT_REPOSITORY
		"https://github.com/nlohmann/json.git"
	GIT_TAG
        v3.11.3
)
FetchContent_MakeAvailable(json)

message(STATUS "Fetching minhook (951d13cfe48ecb3a511a9783572df6cd37274f0b)...")
FetchContent_Declare(minhook
	GIT_REPOSITORY
		"https://github.com/TsudaKageyu/minhook.git"
	GIT_TAG
        951d13cfe48ecb3a511a9783572df6cd37274f0b
)
FetchContent_MakeAvailable(minhook)

message(STATUS "Fetching imgui (87f3109c1af835be0e5b4b4410baa1c66ced7b33)...")
FetchContent_Declare(imgui
	GIT_REPOSITORY
		"https://github.com/ocornut/imgui.git"
	GIT_TAG
        87f3109c1af835be0e5b4b4410baa1c66ced7b33
)
FetchContent_MakeAvailable(imgui)

message(STATUS "Fetching freetype (81330e1f8aec49b4ad05c20f4f03497c473e10d0)...")
FetchContent_Declare(freetype
	GIT_REPOSITORY
		"https://github.com/freetype/freetype.git"
	GIT_TAG
        81330e1f8aec49b4ad05c20f4f03497c473e10d0
)
FetchContent_MakeAvailable(freetype)

message(STATUS "Fetching zydis (bffbb610cfea643b98e87658b9058382f7522807)...")
FetchContent_Declare(zydis
	GIT_REPOSITORY
		"https://github.com/zyantific/zydis.git"
	GIT_TAG
        bffbb610cfea643b98e87658b9058382f7522807
)
FetchContent_MakeAvailable(zydis)

FetchContent_Declare(xscan
	GIT_REPOSITORY
		"https://github.com/eliasmoflag/xscan.git"
	GIT_TAG
        9daf95748277d56430ffaf5c3eccc1a4052a8339
)
FetchContent_MakeAvailable(xscan)

message(STATUS "Fetching lua (v5.4.7)...")
FetchContent_Declare(lua
	GIT_REPOSITORY
		"https://github.com/lua/lua.git"
	GIT_TAG
        v5.4.7
)
FetchContent_MakeAvailable(lua)

file(GLOB LUA_SOURCES
    "${lua_SOURCE_DIR}/*.h"
    "${lua_SOURCE_DIR}/*.c"
)
add_library(lua STATIC ${LUA_SOURCES})
add_library(lua::lua ALIAS lua)
target_include_directories(lua PUBLIC "${lua_SOURCE_DIR}")

message(STATUS "Fetching sol2 (v3.3.0)...")
FetchContent_Declare(sol2
	GIT_REPOSITORY
		"https://github.com/ThePhD/sol2.git"
	GIT_TAG
        v3.3.0
)
FetchContent_MakeAvailable(sol2)

file(GLOB IMGUI_SOURCES
    "${imgui_SOURCE_DIR}/*.h"
    "${imgui_SOURCE_DIR}/*.cpp"
)
list(APPEND IMGUI_SOURCES
    "${imgui_SOURCE_DIR}/backends/imgui_impl_win32.h"
    "${imgui_SOURCE_DIR}/backends/imgui_impl_win32.cpp"
    "${imgui_SOURCE_DIR}/backends/imgui_impl_dx11.h"
    "${imgui_SOURCE_DIR}/backends/imgui_impl_dx11.cpp"
)
list(APPEND IMGUI_SOURCES
    "${imgui_SOURCE_DIR}/misc/cpp/imgui_stdlib.h"
    "${imgui_SOURCE_DIR}/misc/cpp/imgui_stdlib.cpp"
)
list(APPEND IMGUI_SOURCES
    "${imgui_SOURCE_DIR}/misc/freetype/imgui_freetype.h"
    "${imgui_SOURCE_DIR}/misc/freetype/imgui_freetype.cpp"
)
add_library(imgui STATIC ${IMGUI_SOURCES})
add_library(imgui::imgui ALIAS imgui)
target_compile_definitions(imgui PUBLIC
    IMGUI_ENABLE_FREETYPE
    IMGUI_DEFINE_MATH_OPERATORS
    IMGUI_USE_WCHAR32
)
target_compile_features(imgui PRIVATE cxx_std_23)
target_link_libraries(imgui PRIVATE
    d3d11
    freetype
)

target_include_directories(imgui PUBLIC
    "${imgui_SOURCE_DIR}"
    "${imgui_SOURCE_DIR}/backends"
    "${imgui_SOURCE_DIR}/misc/cpp"
    "${imgui_SOURCE_DIR}/misc/freetype"
)

file(GLOB_RECURSE SOURCES
    ./src/*.hpp
    ./src/*.cpp
    ./src/*.inc
    ./src/*.asm
    ./src/*.lua
)

if(${AETHER_BUILD_STATIC})
  add_library(aether STATIC ${SOURCES})
  target_compile_definitions(aether PRIVATE AETHER_BUILD_STATIC=1)
else()
  add_library(aether SHARED ${SOURCES})
endif()

target_sources(aether PRIVATE ${SOURCES})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCES})
target_link_libraries(aether PRIVATE
    minhook
    Zydis
    imgui::imgui
    d3d11
    nlohmann_json::nlohmann_json
    xscan::xscan
    lua::lua
    sol2::sol2
)

target_compile_definitions(aether PRIVATE
    NOMINMAX
)

target_compile_features(aether PUBLIC
	cxx_std_23
)

target_compile_options(aether
    PRIVATE
        /bigobj
)

target_compile_options(aether
    PRIVATE
        /GR-
)
