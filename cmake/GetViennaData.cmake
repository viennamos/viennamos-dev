
IF(DOWNLOAD)
  ExternalProject_Add(viennadata
    PREFIX viennadata
    GIT_REPOSITORY https://github.com/viennadata/viennadata-dev.git
    GIT_TAG master
    BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/viennadata"
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
  )
  ExternalProject_Get_Property(viennadata SOURCE_DIR)
ELSE(DOWNLOAD)
  ExternalProject_Add(viennadata
    PREFIX viennadata
    SOURCE_DIR $ENV{VIENNADATAPATH}
    BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/viennadata"
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
  )
  ExternalProject_Get_Property(viennadata SOURCE_DIR)
ENDIF(DOWNLOAD)

SET(VIENNADATA_INCLUDE_DIRS ${SOURCE_DIR})
