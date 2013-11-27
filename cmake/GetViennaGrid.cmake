
IF(DOWNLOAD)
  ExternalProject_Add(viennagrid
    PREFIX viennagrid
    GIT_REPOSITORY https://github.com/viennagrid/viennagrid-dev.git
    GIT_TAG master
    BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/viennagrid"
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
  )
  ExternalProject_Get_Property(viennagrid SOURCE_DIR)
ELSE(DOWNLOAD)
  ExternalProject_Add(viennagrid
    PREFIX viennagrid
    SOURCE_DIR $ENV{VIENNAGRIDPATH}
    BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/viennagrid"
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
  )
  ExternalProject_Get_Property(viennagrid SOURCE_DIR)
ENDIF(DOWNLOAD)


SET(VIENNAGRID_INCLUDE_DIRS ${SOURCE_DIR})
