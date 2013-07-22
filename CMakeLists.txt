PROJECT(ViennaFVM)

cmake_minimum_required(VERSION 2.6)

#if you don't want the full compiler output, remove the following line
SET(CMAKE_VERBOSE_MAKEFILE ON)

#specify custom boost path if needed:
#SET(BOOSTPATH "/opt/boost")

# alternate boost path for macs
IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
   INCLUDE_DIRECTORIES("/opt/local/include")
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")


#specify include and source directory
INCLUDE_DIRECTORIES(".")
INCLUDE_DIRECTORIES("external")
INCLUDE_DIRECTORIES($ENV{VIENNAMATH})
INCLUDE_DIRECTORIES($ENV{VIENNAGRID})
INCLUDE_DIRECTORIES($ENV{VIENNACL})
INCLUDE_DIRECTORIES($ENV{VIENNADATA})
INCLUDE_DIRECTORIES(${BOOSTPATH})

#set default build type 
IF (NOT CMAKE_BUILD_TYPE)
  SET(CMAKE_BUILD_TYPE "Release")
ENDIF()

#set compiler flags
SET(CMAKE_CXX_FLAGS "-Wall -pedantic")
SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O3 -g")
SET(CMAKE_CXX_FLAGS_RELEASE "-O3")
SET(CMAKE_CXX_FLAGS_DEBUG  "-O0 -g")

#list all source files here
ADD_EXECUTABLE(poisson_1d     examples/tutorial/poisson_1d.cpp)
ADD_EXECUTABLE(poisson_2d     examples/tutorial/poisson_2d.cpp)
ADD_EXECUTABLE(poisson_3d     examples/tutorial/poisson_3d.cpp)
ADD_EXECUTABLE(nin_1d         examples/tutorial/nin_1d.cpp)
ADD_EXECUTABLE(nin_2d_laplace examples/tutorial/nin_2d_laplace.cpp)
ADD_EXECUTABLE(mosfet         examples/tutorial/mosfet.cpp)
ADD_EXECUTABLE(mosfet_3d      examples/tutorial/mosfet_3d.cpp)


##Compatibility with Qt-Creator
file( GLOB_RECURSE QtCreatorCompatibility_SRC
    viennafvm/*.hpp viennafvm/*.h)
add_custom_target( QtCreatorCompatibility SOURCES ${QtCreatorCompatibility_SRC} )
