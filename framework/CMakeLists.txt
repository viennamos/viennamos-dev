
# include current build directory, as qt generates files in there which have to
# be discoverable during the build process of the application
#
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

SET(SOURCES
  src/main.cpp
  src/mainwindow.cpp
  src/module_manager.cpp
  src/rendereditor.cpp
  src/charteditor.cpp
  src/chartentry.cpp
  src/modulecontrol.cpp
  src/multiview.cpp
  src/render3d.cpp
  src/chart2d.cpp
  src/newview.cpp
  src/messenger.cpp
  src/qdebugstream.cpp
  src/license.cpp
  src/materialmanager.cpp
  src/xmlstreamio.cpp
  src/module_interface.cpp
  src/table_entry.cpp
  src/charttableparameters.cpp
  src/screenshot.cpp
  src/quantity.cpp
  src/stream_emitter.cpp
  src/device.cpp
  )

SET(SOURCES ${SOURCES}
  external/Paraview/vtkPVScalarBarActor.cxx
  external/Paraview/vtkPVAxesWidget.cxx
  external/Paraview/vtkPVAxesActor.cxx
)

SET(HEADERS 
  mainwindow.h 
  module_manager.h 
  module_interface.h 
  rendereditor.h 
  charteditor.h 
  modulecontrol.h 
  multiview.h 
  render3d.h 
  chart2d.h 
  newview.h 
  messenger.h
  license.h
  materialmanager.h 
  )

SET(HEADERS
  include/mainwindow.h
  include/module_manager.h
  include/module_interface.h
  include/rendereditor.h
  include/charteditor.h
  include/modulecontrol.h
  include/multiview.h
  include/render3d.h
  include/chart2d.h
  include/newview.h
  include/messenger.h
  include/license.h
  include/materialmanager.h
  include/screenshot.h
  include/stream_emitter.h
)

FILE(GLOB FORMS ui/*.ui)

SET(RESOURCES thesource.qrc)

QT4_WRAP_CPP(HEADERS_MOC ${HEADERS})
QT4_WRAP_UI(FORMS_HEADERS ${FORMS})
QT4_ADD_RESOURCES(RESOURCES_RCC ${RESOURCES})

SET(TARGET viennamos)
ADD_EXECUTABLE(${TARGET} ${SOURCES} ${HEADERS_MOC} ${FORMS_HEADERS} ${RESOURCES_RCC})
TARGET_LINK_LIBRARIES(${TARGET} ${LIBRARIES})
INSTALL(TARGETS ${TARGET} DESTINATION ${INSTALL})
