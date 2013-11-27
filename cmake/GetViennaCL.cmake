
IF(DOWNLOAD)
  ExternalProject_Add(viennacl
    PREFIX viennacl
    GIT_REPOSITORY https://github.com/viennacl/viennacl-dev.git
    GIT_TAG master
    BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/viennacl"
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
  )
  ExternalProject_Get_Property(viennacl SOURCE_DIR)
ELSE(DOWNLOAD)
  ExternalProject_Add(viennacl
    PREFIX viennacl
    SOURCE_DIR $ENV{VIENNACLPATH}
    BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/viennacl"
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
  )
  ExternalProject_Get_Property(viennacl SOURCE_DIR)
ENDIF(DOWNLOAD)

SET(VIENNACL_INCLUDE_DIRS ${SOURCE_DIR})
