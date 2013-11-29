

# Include current build directory, as qt generates files in there which have to
# be discoverable during the build process of the application
#
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

# Build the corresponding ViennaMOS module
#
SET(SOURCES
  device_generator.cpp
  device_generator_module.cpp
  device_generator_form.cpp
  )

SET(HEADERS
  device_generator.h
  device_generator_module.h
  device_generator_form.h
  )

SET(FORMS
  device_generator_form.ui
  )

SET(RESOURCES )

QT4_WRAP_CPP(HEADERS_MOC ${HEADERS} OPTIONS -I${CMAKE_SOURCE_DIR}/framework)
QT4_WRAP_UI(FORMS_HEADERS ${FORMS})
QT4_ADD_RESOURCES(RESOURCES_RCC ${RESOURCES})

SET(TARGET ${VMOS_MODULE_PREFIX}device_generator)
SET(TARGET_DEF DEVICEGENERATORSHARED_LIBRARY)
ADD_LIBRARY(${TARGET} SHARED ${SOURCES} ${HEADERS_MOC} ${FORMS_HEADERS} ${RESOURCES_RCC})
TARGET_LINK_LIBRARIES(${TARGET} ${LIBRARIES} ${VIENNAMINI_LIBRARIES})
SET_TARGET_PROPERTIES(${TARGET} PROPERTIES COMPILE_DEFINITIONS ${TARGET_DEF} PREFIX "")
INSTALL(TARGETS ${TARGET} DESTINATION ${INSTALL})
ADD_DEPENDENCIES(${TARGET} viennamesh)
ADD_DEPENDENCIES(${TARGET} viennamini)
