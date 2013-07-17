
option(BUILD_TESTING "Build the tests " ON)

include(CTest)
include(CMakeDependentOption)

# Installation directories
##########################

set(INSTALL_INCLUDE_DIR include CACHE PATH
   "Installation directory for headers")
if(WIN32 AND NOT CYGWIN)
   set(DEF_INSTALL_CMAKE_DIR CMake)
else()
   set(DEF_INSTALL_CMAKE_DIR lib/cmake/viennadata)
endif()
set(INSTALL_CMAKE_DIR ${DEF_INSTALL_CMAKE_DIR} CACHE PATH
   "Installation directory for CMake files")

if(NOT IS_ABSOLUTE "${INSTALL_CMAKE_DIR}")
   set(INSTALL_CMAKE_DIR "${CMAKE_INSTALL_PREFIX}/${INSTALL_CMAKE_DIR}")
endif()
file(RELATIVE_PATH CONF_REL_INSTALL_PREFIX "${INSTALL_CMAKE_DIR}"
   "${CMAKE_INSTALL_PREFIX}")
if(NOT IS_ABSOLUTE "${INSTALL_INCLUDE_DIR}")
   set(INSTALL_INCLUDE_DIR "${CMAKE_INSTALL_PREFIX}/${INSTALL_INCLUDE_DIR}")
endif()
file(RELATIVE_PATH CONF_REL_INCLUDE_DIR "${INSTALL_CMAKE_DIR}"
   "${INSTALL_INCLUDE_DIR}")

# User options
##############

option(BUILD_EXAMPLES "Build example programs" ON)

option(ENABLE_PEDANTIC_FLAGS "Enable pedantic compiler flags" OFF)

mark_as_advanced(ENABLE_PEDANTIC_FLAGS)

# Set high warning level on GCC
if(ENABLE_PEDANTIC_FLAGS)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic")
endif()

# Disable Warning 4996 (std::copy is unsafe ...) on Visual Studio
#if (MSVC)
#  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4996")
#endif()


# Export
########

configure_file(cmake/ViennaDataConfig.cmake.in
   ${PROJECT_BINARY_DIR}/ViennaDataConfig.cmake @ONLY)

configure_file(cmake/ViennaDataConfigVersion.cmake.in
   ${PROJECT_BINARY_DIR}/ViennaDataConfigVersion.cmake @ONLY)

export(PACKAGE ViennaData)

# Install
#########

install(FILES
   ${PROJECT_BINARY_DIR}/ViennaDataConfig.cmake
   ${PROJECT_BINARY_DIR}/ViennaDataConfigVersion.cmake
   DESTINATION ${INSTALL_CMAKE_DIR} COMPONENT dev)

