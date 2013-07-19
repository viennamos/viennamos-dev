
PROJECT(viennafem)

cmake_minimum_required(VERSION 2.6)

#if you don't want the full compiler output, remove the following line
SET(CMAKE_VERBOSE_MAKEFILE ON)

#add definitions, compiler switches, etc.
ADD_DEFINITIONS(-Wall -pedantic -O3)
#ADD_DEFINITIONS(-Wall -pedantic -O0 -g)

#specify include directory
INCLUDE_DIRECTORIES("..")
INCLUDE_DIRECTORIES($ENV{VIENNAMATH})
INCLUDE_DIRECTORIES($ENV{VIENNAGRID})
INCLUDE_DIRECTORIES($ENV{VIENNACL})
INCLUDE_DIRECTORIES($ENV{VIENNADATA})




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
SET(PROGS poisson_2d poisson_3d)
#SET(PROGS poisson_2d)
#-----------------------------------------------------------------------------

#-----------------------------------------------------------------------------
FOREACH(PROG ${PROGS})
   ADD_EXECUTABLE(${PROG} ../src/${PROG}.cpp)
   ADD_test(${PROG} ${PROG})
ENDFOREACH(PROG)
#-----------------------------------------------------------------------------
