
IF(DOWNLOAD)
  ExternalProject_Add(viennamath
    PREFIX viennamath
    GIT_REPOSITORY https://github.com/viennamath/viennamath-dev.git
    GIT_TAG master
    BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/viennamath"
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
  )
  ExternalProject_Get_Property(viennamath SOURCE_DIR)
ELSE(DOWNLOAD)
  ExternalProject_Add(viennamath
    PREFIX viennamath
    SOURCE_DIR $ENV{VIENNAMATHPATH}
    BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/viennamath"
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
  )
  ExternalProject_Get_Property(viennamath SOURCE_DIR)
ENDIF(DOWNLOAD)

SET(VIENNAMATH_INCLUDE_DIRS ${SOURCE_DIR})
