cmake_minimum_required(VERSION 3.29)

project(lambdax)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(LLVM 18.1 REQUIRED CONFIG)

execute_process(
        COMMAND ${LLVM_TOOLS_BINARY_DIR}/llvm-config --system-libs --libnames
        OUTPUT_VARIABLE LLVM_LIBNAMES
        OUTPUT_STRIP_TRAILING_WHITESPACE
)

separate_arguments(LLVM_LIBNAMES_LIST NATIVE_COMMAND ${LLVM_LIBNAMES})
separate_arguments(LLVM_DEFINITIONS_LIST NATIVE_COMMAND ${LLVM_DEFINITIONS})

file(GLOB_RECURSE sources "src/lib/*.cpp" "include/*.hpp")
add_library(lambdax STATIC ${sources})
target_include_directories(lambdax PUBLIC "include" ${LLVM_INCLUDE_DIRS})
target_compile_definitions(lambdax PUBLIC ${LLVM_DEFINITIONS_LIST})
target_link_directories(lambdax PUBLIC ${LLVM_LIBRARY_DIR})
target_link_libraries(lambdax PUBLIC ${LLVM_LIBNAMES_LIST})

add_executable(lx "src/lang/main.cpp")
target_link_libraries(lx PRIVATE lambdax)

if (CMAKE_BUILD_TYPE STREQUAL "Debug" OR CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
    add_custom_command(TARGET lx POST_BUILD
            COMMAND ${CMAKE_OBJCOPY} --only-keep-debug $<TARGET_FILE:lx> $<TARGET_FILE:lx>.dbg
            COMMAND ${CMAKE_OBJCOPY} --strip-debug $<TARGET_FILE:lx>
            COMMAND ${CMAKE_OBJCOPY} --add-gnu-debuglink=$<TARGET_FILE:lx>.dbg $<TARGET_FILE:lx>
    )
    install(FILES $<TARGET_FILE:lx>.dbg DESTINATION bin)
endif ()

install(TARGETS lambdax lx)
