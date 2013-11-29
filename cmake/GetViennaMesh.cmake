IF(DOWNLOAD)
  ExternalProject_Add(viennamesh
    PREFIX viennamesh
    GIT_REPOSITORY https://github.com/viennamesh/viennamesh-dev.git
    GIT_TAG master
    BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/viennamesh"
    CMAKE_ARGS -DBUILD_SHARED_LIBS=${BUILD_VIENNASTAR_SHARED} -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE} 
      -DBUILD_EXAMPLES=OFF -DBUILD_EXTERNAL_LIB_TRIANGLE=ON -DBUILD_EXTERNAL_LIB_TETGEN=ON
      -DBUILD_DOXYGEN_DOCS=OFF -DBUILD_MANUAL=OFF -DENABLE_DIST=OFF
    INSTALL_COMMAND ""
  )
  ExternalProject_Get_Property(viennamesh SOURCE_DIR)
  ExternalProject_Get_Property(viennamesh BINARY_DIR)
ELSE(DOWNLOAD)
  ExternalProject_Add(viennamesh
    PREFIX viennamesh
    SOURCE_DIR $ENV{VIENNAMESHPATH}
    BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/viennamesh"
    CMAKE_ARGS -DBUILD_SHARED_LIBS=${BUILD_VIENNASTAR_SHARED} -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE} 
      -DBUILD_EXAMPLES=OFF -DBUILD_EXTERNAL_LIB_TRIANGLE=ON -DBUILD_EXTERNAL_LIB_TETGEN=ON
      -DBUILD_DOXYGEN_DOCS=OFF -DBUILD_MANUAL=OFF -DENABLE_DIST=OFF
    INSTALL_COMMAND ""
  )
  ExternalProject_Get_Property(viennamesh SOURCE_DIR)
  ExternalProject_Get_Property(viennamesh BINARY_DIR)
ENDIF(DOWNLOAD)


SET(VIENNAMESH_INCLUDE_DIRS ${SOURCE_DIR})
SET(VIENNAMESH_INCLUDE_DIRS ${VIENNAMESH_INCLUDE_DIRS} ${SOURCE_DIR}/external)

IF(BUILD_SHARED)
  set(LIBSUFFIX ".so")
ELSE(BUILD_SHARED)
  set(LIBSUFFIX ".a")
ENDIF(BUILD_SHARED)

SET(VIENNAMESH_LIBRARIES ${BINARY_DIR}/external/triangle/${CMAKE_FIND_LIBRARY_PREFIXES}triangle${LIBSUFFIX})
SET(VIENNAMESH_LIBRARIES ${VIENNAMESH_LIBRARIES} ${BINARY_DIR}/external/tetgen/${CMAKE_FIND_LIBRARY_PREFIXES}tetgen${LIBSUFFIX})







