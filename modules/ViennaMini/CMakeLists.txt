
# include current build directory, as qt generates files in there which have to
# be discoverable during the build process of the application
#
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

# build the ViennaMini library
SET(VIENNAMINI ${CMAKE_SOURCE_DIR}/external/ViennaMini)
AUX_SOURCE_DIRECTORY(${VIENNAMINI}/src VIENNAMINI_LIB_SOURCES) 
ADD_LIBRARY(viennamini_core STATIC ${VIENNAMINI_LIB_SOURCES})
SET_TARGET_PROPERTIES(viennamini_core PROPERTIES COMPILE_FLAGS "-fPIC")
SET(LIBRARIES ${LIBRARIES} viennamini_core)

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

SET(TARGET ViennaMini)
SET(TARGET_DEF VIENNAMINI_LIBRARY)
ADD_LIBRARY(${TARGET} SHARED ${SOURCES} ${HEADERS_MOC} ${FORMS_HEADERS} ${RESOURCES_RCC})
TARGET_LINK_LIBRARIES(${TARGET} ${LIBRARIES})
SET_TARGET_PROPERTIES(${TARGET} PROPERTIES COMPILE_DEFINITIONS ${TARGET_DEF})
INSTALL(TARGETS ${TARGET} DESTINATION ${INSTALL})
