
IF(DOWNLOAD)
  ExternalProject_Add(viennamaterials
    PREFIX viennamaterials
    GIT_REPOSITORY https://github.com/viennamaterials/viennamaterials-dev.git
    GIT_TAG master
    BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/viennamaterials"
    CMAKE_ARGS -DBUILD_SHARED_LIBS=${BUILD_VIENNASTAR_SHARED} -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE} -DBUILD_EXAMPLES=OFF
    INSTALL_COMMAND ""
  )
  ExternalProject_Get_Property(viennamaterials SOURCE_DIR)
  ExternalProject_Get_Property(viennamaterials BINARY_DIR)
ELSE(DOWNLOAD)
  ExternalProject_Add(viennamaterials
    PREFIX viennamaterials
    SOURCE_DIR $ENV{VIENNAMATERIALSPATH}
    BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/viennamaterials"
    CMAKE_ARGS -DBUILD_SHARED_LIBS=${BUILD_VIENNASTAR_SHARED} -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE} -DBUILD_EXAMPLES=OFF
    INSTALL_COMMAND ""
  )
  ExternalProject_Get_Property(viennamaterials SOURCE_DIR)
  ExternalProject_Get_Property(viennamaterials BINARY_DIR)
ENDIF(DOWNLOAD)


SET(VIENNAMATERIALS_INCLUDE_DIRS ${SOURCE_DIR})

IF(BUILD_SHARED)
  set(LIBSUFFIX ".so")
ELSE(BUILD_SHARED)
  set(LIBSUFFIX ".a")
ENDIF(BUILD_SHARED)

SET(VIENNAMATERIALS_LIBRARIES ${BINARY_DIR}/${CMAKE_FIND_LIBRARY_PREFIXES}viennamaterials${LIBSUFFIX})







