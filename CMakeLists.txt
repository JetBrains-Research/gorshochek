cmake_minimum_required (VERSION 3.8)
project (gorshochek CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
if(APPLE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-everything \
                                            -Wno-error=unused-command-line-argument \
                                            -fparse-all-comments \
                                            -fno-strict-aliasing \
                                            -fno-rtti \
                                            -mmacosx-version-min=10.15")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-everything  \
                                            -fparse-all-comments \
                                            -fno-strict-aliasing \
                                            -fno-rtti")
endif()

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(LIBRARY_LIST
        clangASTMatchers
        clangDynamicASTMatchers
        clangAST
        clangDriver
        clangLex
        clangParse
        clangBasic
        clangFrontendTool
        clangFrontend
        clangEdit
        clangSerialization
        clangSema
        clangAnalysis
        clangRewrite
        clangRewriteFrontend
        clangTooling
        clangToolingCore
)

find_package(LLVM 9.0.0 REQUIRED CONFIG)
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")
message(STATUS "Current build dir : ${CMAKE_CURRENT_BINARY_DIR}")

include_directories(/usr/local/include/)
list(APPEND CMAKE_PREFIX_PATH "${CMAKE_CURRENT_BINARY_DIR}/clang+llvm-9/lib/cmake/llvm/")
list(APPEND CMAKE_PREFIX_PATH "${CMAKE_CURRENT_BINARY_DIR}/clang+llvm-9/lib/cmake/clang/")
# The clang package doesn't appear to provide a version
find_package(Clang REQUIRED CONFIG)
message(STATUS "Using ClangConfig.cmake in: ${CLANG_CMAKE_DIR}")

message("CLANG STATUS:
  Includes (clang)    ${CLANG_INCLUDE_DIRS}
  Includes (llvm)     ${LLVM_INCLUDE_DIRS}"
)

message("CMAKE BIN: ${CMAKE_BINARY_DIR}")
include_directories("${LLVM_INCLUDE_DIRS};${CLANG_INCLUDE_DIRS}")

if(LLVM_BUILD_MAIN_SRC_DIR)
    include_directories(${LLVM_BUILD_MAIN_SRC_DIR}/tools/clang/include)
    include_directories(${LLVM_BUILD_BINARY_DIR}/tools/clang/include)
endif()
add_definitions(${LLVM_DEFINITIONS})

add_definitions(
        -D__STDC_LIMIT_MACROS
        -D__STDC_CONSTANT_MACROS
)

set(OpenMP_C_FLAGS "-Xpreprocessor -fopenmp -lomp -L/usr/local/opt/libomp/lib -I/usr/local/opt/libomp/include")
set(OpenMP_C_LIB_NAMES "omp")
set(OpenMP_CXX_FLAGS "-Xpreprocessor -fopenmp -lomp -L/usr/local/opt/libomp/lib -I/usr/local/opt/libomp/include")
set(OpenMP_CXX_LIB_NAMES "omp")
set(OpenMP_omp_LIBRARY /usr/local/opt/libomp/lib/libomp.dylib)

set(OMPI_CXX "${CMAKE_CXX_COMPILE}")
set(OMPI_CC "${CMAKE_C_COMPILE}")

find_package(OpenMP REQUIRED)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")

add_executable(${PROJECT_NAME}
        main.cpp
        src/ITransformation.cpp
        src/Utils.cpp
        src/Runner.cpp
        src/TransformationFrontendAction.cpp
        src/TransformationFrontendActionFactory.cpp
        src/transformations/IdentityTransformation.cpp
        src/transformations/AddCommentsTransformation.cpp
        src/transformations/RemoveCommentsTransformation.cpp
        src/transformations/RenameEntitiesTransformation.cpp
        src/transformations/ReorderFuncDeclsTransformation.cpp
        src/transformations/IfElseSwapTransformation.cpp
        src/transformations/input_output/PrintfToCoutTransformation.cpp
        src/transformations/input_output/Utils.cpp
        src/transformations/ForToWhileTransformation.cpp
        src/transformations/WhileToForTransformation.cpp
        src/transformations/renaming/BaseRenameProcessor.cpp
        src/transformations/renaming/HashRenameProcessor.cpp
        src/transformations/renaming/TestRenameProcessor.cpp
        src/transformations/renaming/RandomRenameProcessor.cpp
        src/transformations/renaming/Utils.cpp)

# Installing yaml-cpp

include(FetchContent)
FetchContent_Declare(
        yaml-cpp
        GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git
        GIT_SHALLOW	ON
        GIT_TAG yaml-cpp-0.6.3
)
FetchContent_GetProperties(yaml-cpp)
if(NOT yaml-cpp_POPULATED)
    message(STATUS "Populating yaml-cpp...")
    FetchContent_Populate(yaml-cpp)
    # Add here options for yaml-cpp building
    set(YAML_CPP_BUILD_TESTS OFF)
    add_subdirectory(${yaml-cpp_SOURCE_DIR} ${yaml-cpp_BINARY_DIR})
    message(STATUS "Done.")
endif()

include_directories(BEFORE SYSTEM ${yaml-cpp_SOURCE_DIR} ${yaml-cpp_BINARY_DIR}/include)
target_link_libraries(${PROJECT_NAME} yaml-cpp)

target_link_libraries(${PROJECT_NAME} ${LIBRARY_LIST})

target_link_libraries(${PROJECT_NAME}
        LLVMTransformUtils
        LLVMAnalysis
        LLVMTarget
        LLVMOption # Support
        LLVMObject # BitReader, Core, Support
        LLVMBitReader # Core, Support
        LLVMCore # Support
        LLVMSupport
)
