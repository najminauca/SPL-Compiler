cmake_minimum_required(VERSION 3.5)
project(spl C)

set(CMAKE_C_STANDARD 11)
if(NOT CMAKE_BUILD_TYPE)    # Default to Debug build
    set(CMAKE_BUILD_TYPE Debug)
endif()
add_compile_options(-Wall -Wextra)

# Automatically fix bison and flex path for xcode's bundled bison and flex: https://stackoverflow.com/a/59567896
if (CMAKE_HOST_SYSTEM_NAME MATCHES "Darwin")
    execute_process(
            COMMAND brew --prefix bison
            RESULT_VARIABLE BREW_BISON
            OUTPUT_VARIABLE BREW_BISON_PREFIX
            OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    if (BREW_BISON EQUAL 0 AND EXISTS "${BREW_BISON_PREFIX}")
        message(STATUS "Found Bison keg installed by Homebrew at ${BREW_BISON_PREFIX}")
        set(BISON_EXECUTABLE "${BREW_BISON_PREFIX}/bin/bison")
    endif()

    execute_process(
            COMMAND brew --prefix flex
            RESULT_VARIABLE BREW_FLEX
            OUTPUT_VARIABLE BREW_FLEX_PREFIX
            OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    if (BREW_FLEX EQUAL 0 AND EXISTS "${BREW_FLEX_PREFIX}")
        message(STATUS "Found Flex keg installed by Homebrew at ${BREW_FLEX_PREFIX}")
        set(FLEX_EXECUTABLE "${BREW_FLEX_PREFIX}/bin/flex")
    endif()
endif()

# Create directories for generated sources inside the build directory
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/generated-sources/phases/_01_scanner)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/generated-sources/phases/_02_03_parser)

include_directories(src)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/generated-sources)

FIND_PACKAGE(BISON REQUIRED)
FIND_PACKAGE(FLEX REQUIRED)

bison_target(PARSER
        ${CMAKE_CURRENT_SOURCE_DIR}/src/phases/_02_03_parser/parser.y
        ${CMAKE_CURRENT_BINARY_DIR}/generated-sources/phases/_02_03_parser/parser.c
        COMPILE_FLAGS --debug
        VERBOSE
        )

flex_target(SCANNER
        ${CMAKE_CURRENT_SOURCE_DIR}/src/phases/_01_scanner/scanner.flex
        ${CMAKE_CURRENT_BINARY_DIR}/generated-sources/phases/_01_scanner/scanner.c)

add_flex_bison_dependency(SCANNER PARSER)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_executable(
        spl
        ${BISON_PARSER_OUTPUTS}
        ${FLEX_SCANNER_OUTPUTS}
        src/absyn/expressions.c
        src/absyn/global_declarations.c
        src/absyn/lists.c
        src/absyn/parameter_declaration.c
        src/absyn/position.c
        src/absyn/program.c
        src/absyn/statements.c
        src/absyn/type_expressions.c
        src/absyn/variable_declaration.c
        src/absyn/variables.c
        src/absyn/printing/printing_helpers.c
        src/phases/_04a_tablebuild/tablebuild.c
        src/phases/_04b_semant/procedurebodycheck.c
        src/phases/_05_varalloc/varalloc.c
        src/phases/_05_varalloc/stack_layout.c
        src/phases/_06_codegen/codegen.c
        src/phases/_06_codegen/codeprint.c
        src/main.c
        src/command_line_options.c
        src/table/identifier.c
        src/table/table.c
        src/table/entry.c
        src/types/types.c
        src/util/errors.c
        src/util/memory.c
        src/util/ascii_table.c
        src/util/string_ops.c
        src/absyn/absyn.c)

add_custom_target( _generated_source_folder_dummy_target
        COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/generated-sources/phases/_01_scanner
        COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/generated-sources/phases/_02_03_parser
        )

add_dependencies(spl _generated_source_folder_dummy_target)