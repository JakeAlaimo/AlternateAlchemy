cmake_minimum_required(VERSION 3.21)
message("Using toolchain file ${CMAKE_TOOLCHAIN_FILE}.")

########################################################################################################################
## Define project
########################################################################################################################
project(AlternateAlchemyNG VERSION 1.2.0 LANGUAGES CXX ASM_MASM)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)

set(sources src/Main.cpp src/Papyrus.cpp src/Patches.cpp src/Hooks.h inc/versionlibdb.h)


########################################################################################################################
## Configure target DLL
########################################################################################################################
find_package(CommonLibSSE CONFIG REQUIRED)
add_commonlibsse_plugin(${PROJECT_NAME} SOURCES ${sources})
target_precompile_headers(${PROJECT_NAME} PRIVATE src/PCH.h)