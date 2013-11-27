
IF(DOWNLOAD)
  ExternalProject_Add(viennafvm
    PREFIX viennafvm
    GIT_REPOSITORY https://github.com/viennafvm/viennafvm-dev.git
    GIT_TAG master
    BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/viennafvm"
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
  )
  ExternalProject_Get_Property(viennafvm SOURCE_DIR)
ELSE(DOWNLOAD)
  ExternalProject_Add(viennafvm
    PREFIX viennafvm
    SOURCE_DIR $ENV{VIENNAFVMPATH}
    BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/viennafvm"
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
  )
  ExternalProject_Get_Property(viennafvm SOURCE_DIR)
ENDIF(DOWNLOAD)


SET(VIENNAFVM_INCLUDE_DIRS ${SOURCE_DIR})
