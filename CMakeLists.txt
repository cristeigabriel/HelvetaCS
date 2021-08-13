cmake_minimum_required(VERSION 3.21)
project(Helveta)

file(GLOB_RECURSE SRC
# Custom WinApi
"${PROJECT_SOURCE_DIR}/Vendor/WinApi/*.cc",

# ImGui
"${PROJECT_SOURCE_DIR}/Vendor/ImGui/*.cpp",

# Drawing
"${PROJECT_SOURCE_DIR}/Drawing/*.cc",

# Netvars
"${PROJECT_SOURCE_DIR}/Netvars/*.cc",

# Entities
"${PROJECT_SOURCE_DIR}/Entities/*.cc",

# Console
"${PROJECT_SOURCE_DIR}/Console/*.cc",

# Memory
"${PROJECT_SOURCE_DIR}/Dll/*.cc",

# Features
"${PROJECT_SOURCE_DIR}/Features/*.cc",

# Hooks
"${PROJECT_SOURCE_DIR}/Hooks/*.cc",

"${PROJECT_SOURCE_DIR}/*.cc"
)

add_library(${PROJECT_NAME} SHARED ${SRC})
link_directories("$ENV{DXSDK_DIR}/Lib/x86")
include_directories("$ENV{DXSDK_DIR}/Include")
find_library(D3D9_LIBRARY d3d9.lib PATHS "$ENV{DXSDK_DIR}/Lib/x86")  
find_library(D3DX9_LIBRARY d3dx9.lib PATHS "$ENV{DXSDK_DIR}/Lib/x86")
target_link_libraries(${PROJECT_NAME} "${D3D9_LIBRARY}" "${D3DX9_LIBRARY}")

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_EXTENSIONS OFF)
string(REGEX REPLACE "/GR" "/GR-" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
string(REGEX REPLACE "/EHsc" "/EHs-c-" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
add_definitions(-D_HAS_EXCEPTIONS=0) # for STL
add_definitions(/MP) # multi-processor compilation
