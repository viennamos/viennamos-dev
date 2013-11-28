


INCLUDE(${CMAKE_CURRENT_SOURCE_DIR}/cmake/GetViennaMini.cmake)
MESSAGE(STATUS "ViennaMini Include Path: " ${VIENNAMINI_INCLUDE_DIRS})
MESSAGE(STATUS "ViennaMini Libraries: " ${VIENNAMINI_LIBRARIES})
INCLUDE_DIRECTORIES(${VIENNAMINI_INCLUDE_DIRS})

# Include current build directory, as qt generates files in there which have to
# be discoverable during the build process of the application
#
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

# Build the corresponding ViennaMOS module
#
SET(SOURCES
  viennamini.cpp
  viennaminimodule.cpp
  viennaminiform.cpp
  viennaminiworker.cpp
  ../../framework/src/stream_emitter.cpp
  )

SET(HEADERS
  viennamini.h
  viennaminimodule.h
  viennaminiform.h
  viennaminiworker.h
  )

SET(FORMS
  viennaminiform.ui
  )

SET(RESOURCES )

QT4_WRAP_CPP(HEADERS_MOC ${HEADERS} OPTIONS -I${CMAKE_SOURCE_DIR}/framework)
QT4_WRAP_UI(FORMS_HEADERS ${FORMS})
QT4_ADD_RESOURCES(RESOURCES_RCC ${RESOURCES})

SET(TARGET ${VMOS_MODULE_PREFIX}viennamini)
SET(TARGET_DEF VIENNAMINI_LIBRARY)
ADD_LIBRARY(${TARGET} SHARED ${SOURCES} ${HEADERS_MOC} ${FORMS_HEADERS} ${RESOURCES_RCC})
TARGET_LINK_LIBRARIES(${TARGET} ${LIBRARIES} ${VIENNAMINI_LIBRARIES})
SET_TARGET_PROPERTIES(${TARGET} PROPERTIES COMPILE_DEFINITIONS ${TARGET_DEF} PREFIX "")
INSTALL(TARGETS ${TARGET} DESTINATION ${INSTALL})
ADD_DEPENDENCIES(${TARGET} viennamini)
