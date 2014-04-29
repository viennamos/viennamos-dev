CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

# ------------------------------------------------------------------------------
#
# NAME
#
# ------------------------------------------------------------------------------
PROJECT(ViennaMOS)

# ------------------------------------------------------------------------------
#
# PROJECT VERSION
#
# ------------------------------------------------------------------------------
SET(VERSION_MAJOR 1)
SET(VERSION_MINOR 0)
SET(VERSION_PATCH 0)


OPTION(DOWNLOAD     "Download external dependencies automatically"      OFF)

LIST(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

INCLUDE(ExternalProject)
INCLUDE(FindPackageHandleStandardArgs)

# ------------------------------------------------------------------------------
#
# *** SET DEFAULT BUILD TYPE ***
#
# ------------------------------------------------------------------------------
IF (NOT CMAKE_BUILD_TYPE)
  MESSAGE(STATUS "No build type selected - using Release")
  SET(CMAKE_BUILD_TYPE "Release")
ENDIF()

# ------------------------------------------------------------------------------
#
# SET GENERAL COMPILER/LINKER FLAGS
#
# ------------------------------------------------------------------------------

# This is due to some incompatibilities between Boost and Qt
ADD_DEFINITIONS(-DBOOST_TT_HAS_OPERATOR_HPP_INCLUDED)

# qt/vtk include files which seem to be deprecated O.o
# deactivating warning here, as we can't influence it anyway ..
IF ("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU" OR "${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
  set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-Wno-deprecated")
ENDIF ("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU" OR "${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")


# ------------------------------------------------------------------------------
#
# *** CHECK FOR C++11 SUPPORT ***
#
# ------------------------------------------------------------------------------
IF ("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU")
  execute_process(COMMAND ${CMAKE_CXX_COMPILER} -dumpversion OUTPUT_VARIABLE GCC_VERSION)
  IF    (GCC_VERSION VERSION_GREATER 4.7 OR GCC_VERSION VERSION_EQUAL 4.7)
    SET(CXX11_FLAG "-std=c++11")
  ELSEIF(GCC_VERSION VERSION_GREATER 4.3 OR GCC_VERSION VERSION_EQUAL 4.3 AND GCC_VERSION VERSION_LESS 4.7)
    SET(CXX11_FLAG "-std=c++0x")
  ELSE  (GCC_VERSION VERSION_GREATER 4.7 OR GCC_VERSION VERSION_EQUAL 4.7)
    MESSAGE(FATAL_ERROR "${PROJECT_NAME} requires g++ 4.3 or greater.")
  ENDIF ()
ELSEIF ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
  SET(CXX11_FLAG "-std=c++11")
ELSEIF ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Intel")
  SET(CXX11_FLAG "-std=c++11")
ELSE ()
  MESSAGE(WARNING "Your C++ compiler's C++11 support is not recognized!")
ENDIF ()

IF(DEFINED CXX11_FLAG)
  MESSAGE(STATUS "C++11 supported - using flag: "${CXX11_FLAG})
  SET(CXX11_AVAIL TRUE)
ELSE(DEFINED CXX11_FLAG)
  SET(CXX11_AVAIL FALSE)
ENDIF(DEFINED CXX11_FLAG)

# ------------------------------------------------------------------------------
#
# FIND QT4
#
# ------------------------------------------------------------------------------
FIND_PACKAGE(Qt4 REQUIRED QtCore QtGui QtXml)
INCLUDE(${QT_USE_FILE})
ADD_DEFINITIONS(${QT_DEFINITIONS})
SET(LIBRARIES ${QT_LIBRARIES})
MESSAGE(STATUS "Qt Include Path: "${QT_INCLUDE_DIR})
MESSAGE(STATUS "Qt Library Path: "${QT_LIBRARY_DIR})

# ------------------------------------------------------------------------------
#
# FIND VTK
#
# ------------------------------------------------------------------------------
FIND_PACKAGE(VTK REQUIRED NO_MODULE)
IF(VTK_FOUND)
  MESSAGE(STATUS "VTK found (Version "${VTK_MAJOR_VERSION}.${VTK_MINOR_VERSION}")")
  MESSAGE(STATUS "VTK Include Path: " ${VTK_INCLUDE_DIRS})
  MESSAGE(STATUS "VTK Library Path: " ${VTK_LIBRARY_DIRS})

  IF(${VTK_MAJOR_VERSION} GREATER 5)
    INCLUDE(${VTK_USE_FILE})
    SET(LIBRARIES ${LIBRARIES} ${VTK_LIBRARIES})
  ELSE(${VTK_MAJOR_VERSION} GREATER 5)

    SET(QVTKLIB "QVTK")
    # Look for QtVTK Kit, requires VTK to be compiled with Qt/GUI support
    SET(QTVTK_FOUND FALSE)
    FOREACH(KIT ${VTK_KITS}) # look for the "qvtk" string in the kits-list
      IF(${KIT} STREQUAL ${QVTKLIB})
        SET(QTVTK_FOUND TRUE)
      ENDIF(${KIT} STREQUAL ${QVTKLIB})
    ENDFOREACH(KIT)
    IF(QTVTK_FOUND)
      MESSAGE(STATUS "VTK Qt-Kit found")
      INCLUDE_DIRECTORIES(${VTK_INCLUDE_DIRS})
      LINK_DIRECTORIES(${VTK_LIBRARY_DIRS})
      #SET(LIBRARIES ${LIBRARIES} ${VTK_LIBRARIES})
      SET(LIBRARIES ${LIBRARIES} vtkCommon vtkFiltering vtkCharts)
      # Test if the actual libqvtk has been added by the VTK cmake script
      # if not, add it manually to the general library list ..
      SET(QTVTKLIB_FOUND FALSE)
      FOREACH(LIB ${VTK_LIBRARIES})
        SET(QTVTKLIB_FOUND TRUE)
      ENDFOREACH(LIB)
      IF(QTVTKLIB_FOUND)
        SET(LIBRARIES ${LIBRARIES} ${QVTKLIB})
      ENDIF(QTVTKLIB_FOUND)
    ELSE(QTVTK_FOUND) # give a hint what to do ..
      MESSAGE(FATAL_ERROR "VTK Qt-Kit not found - recompile VTK with GUI/Qt support!")
    ENDIF(QTVTK_FOUND)
  ENDIF(${VTK_MAJOR_VERSION} GREATER 5)
ENDIF(VTK_FOUND)

# ------------------------------------------------------------------------------
#
# FIND BOOST
#
# ------------------------------------------------------------------------------
SET(BOOST_MIN_VERSION 1.46.1)
#SET(BOOST_LIBS filesystem system chrono program_options)
#SET(Boost_USE_STATIC_LIBS       ON)
#SET(Boost_USE_MULTITHREADED     OFF)
#SET(Boost_USE_STATIC_RUNTIME    OFF)
#SET(Boost_NO_SYSTEM_PATHS       OFF)
FIND_PACKAGE(Boost ${BOOST_MIN_VERSION} COMPONENTS ${BOOST_LIBS} REQUIRED)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})
SET(LIBRARIES ${LIBRARIES} ${Boost_LIBRARIES})
MESSAGE(STATUS "Boost Include Path: "${Boost_INCLUDE_DIR})
MESSAGE(STATUS "Boost Library Path: "${Boost_LIBRARY_DIRS})

# ------------------------------------------------------------------------------
#
# Print the list of libraries to be linked against ViennaMOS
#
# ------------------------------------------------------------------------------
#MESSAGE(STATUS "Libraries to be linked: "${LIBRARIES})

# ------------------------------------------------------------------------------
#
# GENERAL INCLUDE DIRECTORIES
#
# ------------------------------------------------------------------------------
INCLUDE_DIRECTORIES(framework)
INCLUDE_DIRECTORIES(framework/include)

# ------------------------------------------------------------------------------
#
# INCLUDE EXTERNAL PROJECTS
#
# ------------------------------------------------------------------------------
IF(DOWNLOAD)
  MESSAGE(STATUS "Downloading ViennaStar during the build step ..")
ELSE(DOWNLOAD)
  MESSAGE(STATUS "Using local ViennaStar via VIENNA*PATH environment variables ..")
ENDIF(DOWNLOAD)

SET(BUILD_VIENNASTAR_SHARED OFF)

INCLUDE(cmake/GetViennaFVM.cmake)
MESSAGE(STATUS "ViennaFVM Include Path: " ${VIENNAFVM_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${VIENNAFVM_INCLUDE_DIRS})

INCLUDE(cmake/GetViennaGrid.cmake)
MESSAGE(STATUS "ViennaGrid Include Path: " ${VIENNAGRID_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${VIENNAGRID_INCLUDE_DIRS})

INCLUDE(cmake/GetViennaData.cmake)
MESSAGE(STATUS "ViennaData Include Path: " ${VIENNADATA_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${VIENNADATA_INCLUDE_DIRS})

INCLUDE(cmake/GetViennaMath.cmake)
MESSAGE(STATUS "ViennaMath Include Path: " ${VIENNAMATH_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${VIENNAMATH_INCLUDE_DIRS})

INCLUDE(cmake/GetViennaCL.cmake)
MESSAGE(STATUS "ViennaCL Include Path: " ${VIENNACL_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${VIENNACL_INCLUDE_DIRS})

INCLUDE(cmake/GetViennaMaterials.cmake)
MESSAGE(STATUS "ViennaMaterials Include Path: " ${VIENNAMATERIALS_INCLUDE_DIRS})
MESSAGE(STATUS "ViennaMaterials Libraries: " ${VIENNAMATERIALS_LIBRARIES})
INCLUDE_DIRECTORIES(${VIENNAMATERIALS_INCLUDE_DIRS})
SET(LIBRARIES ${LIBRARIES} ${VIENNAMATERIALS_LIBRARIES})

INCLUDE(cmake/GetViennaMesh.cmake)
MESSAGE(STATUS "ViennaMesh Include Path: " ${VIENNAMESH_INCLUDE_DIRS})
MESSAGE(STATUS "ViennaMesh Libraries: " ${VIENNAMESH_LIBRARIES})
INCLUDE_DIRECTORIES(${VIENNAMESH_INCLUDE_DIRS})
SET(LIBRARIES ${LIBRARIES} ${VIENNAMESH_LIBRARIES})

INCLUDE(${CMAKE_CURRENT_SOURCE_DIR}/cmake/GetViennaMini.cmake)
MESSAGE(STATUS "ViennaMini Include Path: " ${VIENNAMINI_INCLUDE_DIRS})
MESSAGE(STATUS "ViennaMini Libraries: " ${VIENNAMINI_LIBRARIES})
INCLUDE_DIRECTORIES(${VIENNAMINI_INCLUDE_DIRS})

# ------------------------------------------------------------------------------
#
# SET INSTALL PATH
#
# ------------------------------------------------------------------------------
SET(INSTALL ${CMAKE_BINARY_DIR}/install)

# ------------------------------------------------------------------------------
#
# HANDLE RPATH
#
# ------------------------------------------------------------------------------
# add the automatically determined parts of the RPATH
# which point to directories outside the build tree to the install RPATH
SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

# ------------------------------------------------------------------------------
#
# Compatibility with Qt-Creator
#
# ------------------------------------------------------------------------------
cmake_policy(SET CMP0009 NEW)
file( GLOB_RECURSE QtCreatorCompatibility_SRC framework/*.hpp framework/*.h modules/*.hpp modules/*.h)
add_custom_target( QtCreatorCompatibility SOURCES ${QtCreatorCompatibility_SRC} )

# ------------------------------------------------------------------------------
#
# CONFIGURE FRAMEWORK
#
# ------------------------------------------------------------------------------
ADD_SUBDIRECTORY(framework/)

# ------------------------------------------------------------------------------
#
# CONFIGURE MODULES
#
# ------------------------------------------------------------------------------
SET(VMOS_MODULE_PREFIX "vmos_module_")
#ADD_SUBDIRECTORY(modules/)
