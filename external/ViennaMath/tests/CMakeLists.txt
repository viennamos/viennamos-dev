
PROJECT(viennamath)

cmake_minimum_required(VERSION 2.6)

#if you don't want the full compiler output, remove the following line
SET(CMAKE_VERBOSE_MAKEFILE ON)

#specify include directory
#INCLUDE_DIRECTORIES("${PROJECT_SOURCE_DIR}/viennama")
INCLUDE_DIRECTORIES("..")

#add definitions, compiler switches, etc.
#ADD_DEFINITIONS(-Wall -pedantic -O3)
ADD_DEFINITIONS(-Wall -pedantic -O0 -g)


#-----------------------------------------------------------------------------
# Configure Dart testing support.  This should be done before any
# MESSAGE(FATAL_ERROR ...) commands are invoked.
INCLUDE(CTest)
ENABLE_TESTING()
IF(BUILD_TESTING)
  SET(BUILDNAME "${BUILDNAME}" CACHE STRING "Name of build on the dashboard")
  MARK_AS_ADVANCED(BUILDNAME)
ENDIF(BUILD_TESTING)

#-----------------------------------------------------------------------------
# add the source files which should be tested without the trailing *.cpp
SET(PROGS runtime_test compiletime_test compiletime_evaluation compiletime_manipulation diff_test equation_test)
#-----------------------------------------------------------------------------

#-----------------------------------------------------------------------------
FOREACH(PROG ${PROGS})
   ADD_EXECUTABLE(${PROG} ../src/${PROG}.cpp)
   ADD_test(${PROG} ${PROG})
ENDFOREACH(PROG)
#-----------------------------------------------------------------------------

ADD_EXECUTABLE(external_linkage  ../src/external_1.cpp
                                 ../src/external_2.cpp)
ADD_test(external_linkage external_linkage)
