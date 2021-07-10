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
set(VERSION_MAJOR 1)
set(VERSION_MINOR 0)
set(VERSION_PATCH 0)


option(USE_STATIC "Consider a static link towards external libraries" OFF)


list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

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
  MESSAGE(STATUS "C++11 supported - using flag: " ${CXX11_FLAG})
  SET(CXX11_AVAIL TRUE)
ELSE(DEFINED CXX11_FLAG)
  SET(CXX11_AVAIL FALSE)
ENDIF(DEFINED CXX11_FLAG)

# ------------------------------------------------------------------------------
#
# FIND QT4
#
# ------------------------------------------------------------------------------
#SET(QT_MIN_VERSION 4.8.4)
#FIND_PACKAGE(Qt4 ${QT_MIN_VERSION} REQUIRED QtCore QtGui)
FIND_PACKAGE(Qt4 REQUIRED QtCore QtGui QtXml)
INCLUDE(${QT_USE_FILE})
ADD_DEFINITIONS(${QT_DEFINITIONS})
SET(LIBRARIES ${QT_LIBRARIES})
MESSAGE(STATUS "Qt Include Path: " ${QT_INCLUDE_DIR})
MESSAGE(STATUS "Qt Library Path: " ${QT_LIBRARY_DIR})

# ------------------------------------------------------------------------------
#
# FIND VTK
#
# ------------------------------------------------------------------------------
FIND_PACKAGE(VTK REQUIRED NO_MODULE)
IF(VTK_FOUND)
  MESSAGE(STATUS "VTK found (Version " ${VTK_MAJOR_VERSION}.${VTK_MINOR_VERSION} ")")
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

  
#  SET(QVTKLIB "")
#  IF(${VTK_MAJOR_VERSION} GREATER 5)
#    SET(QVTKLIB "vtkGUISupportQt")
#  ELSE(${VTK_MAJOR_VERSION} GREATER 5)
#    SET(QVTKLIB "QVTK")
#  ENDIF(${VTK_MAJOR_VERSION} GREATER 5)
#  
#  # Look for QtVTK Kit, requires VTK to be compiled with Qt/GUI support
#  SET(QTVTK_FOUND FALSE)
#  FOREACH(KIT ${VTK_KITS}) # look for the "qvtk" string in the kits-list
#    IF(${KIT} STREQUAL ${QVTKLIB})
#      SET(QTVTK_FOUND TRUE)
#    ENDIF(${KIT} STREQUAL ${QVTKLIB})
#  ENDFOREACH(KIT)
#  IF(QTVTK_FOUND)
#    MESSAGE(STATUS "VTK Qt-Kit found")
#    INCLUDE_DIRECTORIES(${VTK_INCLUDE_DIRS})
#    LINK_DIRECTORIES(${VTK_LIBRARY_DIRS})
#    #SET(LIBRARIES ${LIBRARIES} ${VTK_LIBRARIES})
#    SET(LIBRARIES ${LIBRARIES} vtkCommon vtkFiltering vtkCharts)
#    # Test if the actual libqvtk has been added by the VTK cmake script
#    # if not, add it manually to the general library list ..
#    SET(QTVTKLIB_FOUND FALSE)
#    FOREACH(LIB ${VTK_LIBRARIES})
#      SET(QTVTKLIB_FOUND TRUE)
#    ENDFOREACH(LIB)
#    IF(QTVTKLIB_FOUND)
#      SET(LIBRARIES ${LIBRARIES} ${QVTKLIB})
#    ENDIF(QTVTKLIB_FOUND)
#  ELSE(QTVTK_FOUND) # give a hint what to do ..
#    MESSAGE(FATAL_ERROR "VTK Qt-Kit not found - recompile VTK with GUI/Qt support!")
#  ENDIF(QTVTK_FOUND)
#ELSE(VTK_FOUND)
#  MESSAGE(STATUS "VTK not found")
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
MESSAGE(STATUS "Boost Include Path: " ${Boost_INCLUDE_DIR})
MESSAGE(STATUS "Boost Library Path: " ${Boost_LIBRARY_DIRS})

# ------------------------------------------------------------------------------
#
# Print the list of libraries to be linked against ViennaMOS
#
# ------------------------------------------------------------------------------
MESSAGE(STATUS "Libraries to be linked: " ${LIBRARIES})

# ------------------------------------------------------------------------------
#
# GENERAL INCLUDE DIRECTORIES
#
# ------------------------------------------------------------------------------
INCLUDE_DIRECTORIES(framework)
INCLUDE_DIRECTORIES(framework/include)
INCLUDE_DIRECTORIES(external/ViennaGrid)
INCLUDE_DIRECTORIES(external/ViennaData)
INCLUDE_DIRECTORIES(external/ViennaMaterials)
INCLUDE_DIRECTORIES(external/ViennaUtils)
INCLUDE_DIRECTORIES(external/ViennaMini)
INCLUDE_DIRECTORIES(external/ViennaMath)
INCLUDE_DIRECTORIES(external/ViennaFVM)
INCLUDE_DIRECTORIES(external/ViennaCL)

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
ADD_SUBDIRECTORY(modules/)
