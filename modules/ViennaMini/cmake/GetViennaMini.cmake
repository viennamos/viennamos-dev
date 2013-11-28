IF(DOWNLOAD)
  ExternalProject_Add(viennamini
    PREFIX viennamini
    GIT_REPOSITORY https://github.com/viennamini/viennamini-dev.git
    GIT_TAG master
    BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/viennamini"
    CMAKE_ARGS -DBUILD_SHARED_LIBS=${BUILD_VIENNASTAR_SHARED} -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE} -DBUILD_EXAMPLES=OFF -DBUILD_TESTS=OFF
    INSTALL_COMMAND ""
  )
  ExternalProject_Get_Property(viennamini SOURCE_DIR)
  ExternalProject_Get_Property(viennamini BINARY_DIR)
ELSE(DOWNLOAD)
  ExternalProject_Add(viennamini
    PREFIX viennamini
    SOURCE_DIR $ENV{VIENNAMINIPATH}
    BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/viennamini"
    CMAKE_ARGS -DBUILD_SHARED_LIBS=${BUILD_VIENNASTAR_SHARED} -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE} -DBUILD_EXAMPLES=OFF -DBUILD_TESTS=OFF
    INSTALL_COMMAND ""
  )
  ExternalProject_Get_Property(viennamini SOURCE_DIR)
  ExternalProject_Get_Property(viennamini BINARY_DIR)
ENDIF(DOWNLOAD)


SET(VIENNAMINI_INCLUDE_DIRS ${SOURCE_DIR})

IF(BUILD_SHARED)
  set(LIBSUFFIX ".so")
ELSE(BUILD_SHARED)
  set(LIBSUFFIX ".a")
ENDIF(BUILD_SHARED)

SET(VIENNAMINI_LIBRARIES ${BINARY_DIR}/${CMAKE_FIND_LIBRARY_PREFIXES}viennamini${LIBSUFFIX})






