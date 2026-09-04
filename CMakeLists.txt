cmake_minimum_required(VERSION 3.25)

project(cpp_unlegacyfier LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_COLOR_DIAGNOSTICS ON)

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    add_compile_options(-fcolor-diagnostics)
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

# Clang lib setup
find_package(LLVM REQUIRED CONFIG)
find_package(Clang REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

include_directories(${LLVM_INCLUDE_DIRS})
include_directories(${CLANG_INCLUDE_DIRS})

separate_arguments(LLVM_DEFINITIONS_LIST NATIVE_COMMAND ${LLVM_DEFINITIONS})
add_definitions(${LLVM_DEFINITIONS_LIST})

if(NOT LLVM_ENABLE_RTTI)
    add_compile_options(-fno-rtti)
endif()

enable_testing()

include(FetchContent)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.14.0 # Use a specific stable release tag
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Core lib
add_library(unlegacifier_core STATIC
    src/transformer/transformer.cpp
    src/transformers/enum_fixer_transformer.cpp
    src/transformers/member_begin_end_fixer_transformer.cpp
    src/transformers/shorter_function_object_transformer.cpp
    src/transformers/add_missing_override_transformer.cpp
    src/util/replacement_map.cpp
    src/frontend/unlegacifier_consumer.cpp
    src/frontend/unlegacifier_frontend.cpp
    src/frontend/unlegacifier_factory.cpp
    src/transformers/class_owned_ptr_to_unique_transformer.cpp
    src/transformers/replace_typedef_with_using_transformer.cpp
    src/frontend/yaml_frontend.cpp
    src/frontend/diff_print_frontend.cpp
    src/frontend/vscode_preview_frontend.cpp
)

target_include_directories(unlegacifier_core PUBLIC include vendor)

target_link_libraries(unlegacifier_core PUBLIC clang-cpp LLVM)

target_compile_definitions(unlegacifier_core PUBLIC
    CLANG_RESOURCE_DIR="${LLVM_LIBRARY_DIR}/clang/${LLVM_VERSION_MAJOR}"
)

# CLI target
add_executable(unlegacifier
    src/main.cpp
)

target_link_libraries(unlegacifier PRIVATE unlegacifier_core)

# Test target
add_executable(unlegacifier_tests test/transformers/enum_fixer_transformer_test.cpp)

target_link_libraries(unlegacifier_tests PUBLIC GTest::gtest_main unlegacifier_core)

include(GoogleTest)

gtest_discover_tests(unlegacifier_tests)
