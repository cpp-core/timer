cmake_minimum_required (VERSION 3.22 FATAL_ERROR)
project (timer VERSION 0.1 LANGUAGES C CXX)

# Make including this file idempotent
#
if(TARGET timer_docs)
  return()
endif()

set(TARGET_BASE timer_docs)
set(PROJECT_BASE timer-docs)

find_package(Doxygen REQUIRED)

# If we are running locally
#
set(CMAKE_DOCS_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/docs)
if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
  message("-- ${PROJECT_BASE}: CMAKE_CURRENT_SOURCE_DIR: ${CMAKE_CURRENT_SOURCE_DIR}")
  message("-- ${PROJECT_BASE}: CMAKE_SOURCE_DIR: ${CMAKE_SOURCE_DIR}")
  message("-- ${PROJECT_BASE}: CMAKE_DOCS_OUTPUT_DIR: ${CMAKE_DOCS_OUTPUT_DIR}")
endif()

# Find the source files
#
set(REPO_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../)
file(GLOB_RECURSE FILES_INCLUDE ${REPO_DIR}/include/*.h)
file(GLOB_RECURSE FILES_SRC ${REPO_DIR}/src/*.cpp)
file(GLOB_RECURSE FILES_TEST ${REPO_DIR}/test/*.cpp)

# Doxygen consumes header files and produces xml files.
#
set(DOXYGEN_INPUT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../include)
set(DOXYGEN_OUTPUT_DIR ${CMAKE_DOCS_OUTPUT_DIRECTORY}/doxygen)
set(DOXYGEN_CONFIG_IN ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in)
set(DOXYGEN_CONFIG_FILE ${CMAKE_DOCS_OUTPUT_DIRECTORY}/Doxyfile)
set(DOXYGEN_OUTPUT_FILE ${DOXYGEN_OUTPUT_DIR}/xml/index.xml)

message("-- ${PROJECT_BASE}: DOXYGEN_INPUT_DIR: ${DOXYGEN_INPUT_DIR}")
message("-- ${PROJECT_BASE}: DOXYGEN_OUTPUT_DIR: ${DOXYGEN_OUTPUT_DIR}")
message("-- ${PROJECT_BASE}: DOXYGEN_CONFIG_IN: ${DOXYGEN_CONFIG_IN}")
message("-- ${PROJECT_BASE}: DOXYGEN_CONFIG_FILE: ${DOXYGEN_CONFIG_FILE}")
message("-- ${PROJECT_BASE}: DOXYGEN_OUTPUT_FILE: ${DOXYGEN_OUTPUT_FILE}")

configure_file(${DOXYGEN_CONFIG_IN} ${DOXYGEN_CONFIG_FILE} @ONLY)

file(MAKE_DIRECTORY ${DOXYGEN_OUTPUT_DIR})

add_custom_command(OUTPUT ${DOXYGEN_OUTPUT_FILE}
  DEPENDS ${FILES_INCLUDE}
  COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_CONFIG_FILE}
  MAIN_DEPENDENCY ${DOXYFILE_OUT} ${DOXYFILE_IN}
  COMMENT "Doxygen generating xml"
  VERBATIM)

add_custom_target(${TARGET_BASE}_doxygen DEPENDS ${DOXYGEN_OUTPUT_FILE})

# Rst files generated by script for the file reference
#
set(GENERATED_OUTPUT_DIR ${CMAKE_DOCS_OUTPUT_DIRECTORY}/sphinx/generated)
set(GENERATED_OUTPUT_FILE ${GENERATED_OUTPUT_DIR}/include/core/dir.rst)

add_custom_command(OUTPUT ${GENERATED_OUTPUT_FILE}
  DEPENDS ${FILES_INCLUDE} ${FILES_SRC} ${FILES_TEST}
  COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/src_to_rst.py -r ${REPO_DIR} -s include/core include/coro src/core src/coro src/tools test/src/core test/src/coro -t ${GENERATED_OUTPUT_DIR}
  COMMENT "src_to_rst.py generating to ${GENERATED_OUTPUT_DIR}" 
  VERBATIM)

add_custom_target(${TARGET_BASE}_file_reference DEPENDS ${GENERATED_OUTPUT_FILE})

# Sphinx configuration
#
set(SPHINX_INPUT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/sphinx)
set(SPHINX_BUILD_DIR ${CMAKE_DOCS_OUTPUT_DIRECTORY}/sphinx)
set(SPHINX_OUTPUT_DIR ${CMAKE_DOCS_OUTPUT_DIRECTORY}/html)
set(SPHINX_OUTPUT_FILE ${SPHINX_OUTPUT_DIR}/index.html)
set(SPHINX_CONFIG_IN ${SPHINX_INPUT_DIR}/conf.py.in)
set(SPHINX_CONFIG_FILE ${SPHINX_BUILD_DIR}/conf.py)
configure_file(${SPHINX_CONFIG_IN} ${SPHINX_CONFIG_FILE} @ONLY)

# Target to copy Sphinx source files to build dir
#
set(SPHINX_BUILD_DIR ${CMAKE_DOCS_OUTPUT_DIRECTORY}/sphinx)
file(GLOB_RECURSE SPHINX_FILES RELATIVE ${SPHINX_INPUT_DIR} *.css *.html *.md *.png *.py *.rst)

add_custom_target(${TARGET_BASE}_cp_sphinx)
foreach(FILE ${SPHINX_FILES})
  set(SRC_FILE ${SPHINX_INPUT_DIR}/${FILE})
  set(DST_FILE ${SPHINX_BUILD_DIR}/${FILE})
  add_custom_command(OUTPUT ${DST_FILE}
    COMMAND ${CMAKE_COMMAND} -E copy ${SRC_FILE} ${DST_FILE}
    DEPENDS ${SRC_FILE})
  set_property(TARGET ${TARGET_BASE}_cp_sphinx APPEND PROPERTY SOURCES ${DST_FILE})
endforeach()

# Target to run sphinx to generate html
#
get_property(SPHINX_DEPEND
  TARGET ${TARGET_BASE}_cp_sphinx
  PROPERTY SOURCES)
add_custom_command(OUTPUT ${SPHINX_OUTPUT_FILE}
  DEPENDS ${SPHINX_DEPEND} ${GENERATED_OUTPUT_FILE} ${DOXYGEN_OUTPUT_FILE}
  COMMAND sphinx-build -a -b html ${SPHINX_BUILD_DIR} ${SPHINX_OUTPUT_DIR}
  COMMENT "Sphinx generating html"
  VERBATIM)

add_custom_target(${TARGET_BASE} DEPENDS ${SPHINX_OUTPUT_FILE})

message("-- ${PROJECT_BASE}: Documentation: ${CMAKE_DOCS_OUTPUT_DIRECTORY}")

