cmake_minimum_required (VERSION 3.8)
project (gorshochek CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
if(APPLE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall \
                                            -fparse-all-comments \
                                            -fno-strict-aliasing \
                                            -fno-rtti \
                                            -mmacosx-version-min=10.15")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall \
                                            -fparse-all-comments \
                                            -fno-strict-aliasing \
                                            -fno-rtti")
endif()

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

# The clang package doesn't appear to provide a version
find_package(Clang REQUIRED CONFIG)
message(STATUS "Using ClangConfig.cmake in: ${CLANG_CMAKE_DIR}")

include_directories(${LLVM_INCLUDE_DIRS} ${CLANG_INCLUDE_DIRS})
if(LLVM_BUILD_MAIN_SRC_DIR)
    include_directories(${LLVM_BUILD_MAIN_SRC_DIR}/tools/clang/include)
    include_directories(${LLVM_BUILD_BINARY_DIR}/tools/clang/include)
endif()
link_directories(${LLVM_LIBRARY_DIRS})
add_definitions(${LLVM_DEFINITIONS})

add_definitions(
        -D__STDC_LIMIT_MACROS
        -D__STDC_CONSTANT_MACROS
)

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
        src/transformations/io-transformations/PrintfToCoutTransformation.cpp
        src/transformations/io-transformations/IOUtils.cpp
        src/transformations/ForToWhileTransformation.cpp
        src/transformations/WhileToForTransformation.cpp
        )

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
