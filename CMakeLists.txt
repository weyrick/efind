
cmake_minimum_required(VERSION 2.8)
project (efind)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/modules )

MESSAGE( STATUS "PROJECT_SOURCE_DIR: " ${PROJECT_SOURCE_DIR} )
MESSAGE( STATUS "CMAKE_MODULE_PATH: " ${CMAKE_MODULE_PATH} )
MESSAGE( STATUS "CMAKE_SYSTEM_PROCESSOR: " ${CMAKE_SYSTEM_PROCESSOR} )

include_directories (
                     ${CMAKE_CURRENT_BINARY_DIR}
                     ${CMAKE_CURRENT_SOURCE_DIR}/src
                    )

##### LEXER #####
# note, scanner.c is include in the source repo so that users do not need
# re2c to build. this rule is needed only during development of the scanner
ADD_CUSTOM_COMMAND(
   COMMAND re2c
   ARGS -o ${CMAKE_CURRENT_SOURCE_DIR}/src/scanner.c ${CMAKE_CURRENT_SOURCE_DIR}/src/scanner.re
   DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/scanner.re
   OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/src/scanner.c
)
SET_SOURCE_FILES_PROPERTIES(${CMAKE_CURRENT_SOURCE_DIR}/src/scanner.c GENERATED)

#### PARSER ####

# http://www.cmake.org/pipermail/cmake/2002-September/003028.html

add_executable( lemon-pg
                src/lemon.c )

ADD_CUSTOM_COMMAND(
   # note we do this copy because lemon will output to the source directory instead of the build
   # dir if we don't, so this keeps it tidy.
   # alternative (read: portable) way is to change lemon.c to not include the base directory
   # when outputing the generating parser files, but instead strip it and use current working
   COMMAND cp
   ARGS ${CMAKE_CURRENT_SOURCE_DIR}/src/efind_parser.y ${CMAKE_CURRENT_BINARY_DIR}
   COMMAND cp
   ARGS ${CMAKE_CURRENT_SOURCE_DIR}/src/lempar.c ${CMAKE_CURRENT_BINARY_DIR}
   COMMAND ${CMAKE_CURRENT_BINARY_DIR}/lemon-pg
   ARGS ${CMAKE_CURRENT_BINARY_DIR}/efind_parser.y
   DEPENDS lemon-pg
   DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/efind_parser.y
   OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/efind_parser.c
   OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/efind_parser.h
   OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/efind_parser.y
   OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/efind_parser.out
   OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/lempar.c
)


# Since efind_parser.c does not exists yet when cmake is run, mark
# it as generated
SET_SOURCE_FILES_PROPERTIES(${CMAKE_CURRENT_BINARY_DIR}/efind_parser.c GENERATED)

#ADD_CUSTOM_COMMAND(
#   COMMAND cp ${CMAKE_CURRENT_BINARY_DIR}/efind_parser.c ${CMAKE_CURRENT_BINARY_DIR}/efind_parser_final.c
#   COMMAND cat ${CMAKE_CURRENT_SOURCE_DIR}/src/lemon_parser.c >> ${CMAKE_CURRENT_BINARY_DIR}/efind_parser_final.c
#   DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/efind_parser.c
#   OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/efind_parser_final.c
#)
#SET_SOURCE_FILES_PROPERTIES(${CMAKE_CURRENT_BINARY_DIR}/efind_parser_final.c GENERATED)

SET(SRC_LIST src/main.c
             ${CMAKE_CURRENT_BINARY_DIR}/efind_parser.c
             src/parse_expr.c
             src/scanner.c)

add_executable(efind ${SRC_LIST})
