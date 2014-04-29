## [begin] detail
##
#MACRO(SUBDIRLIST result curdir)
#  FILE(GLOB children RELATIVE ${curdir} ${curdir}/*)
#  SET(dirlist "")
#  FOREACH(child ${children})
#    IF(IS_DIRECTORY ${curdir}/${child})
#        SET(dirlist ${dirlist} ${child})
#    ENDIF()
#  ENDFOREACH()
#  SET(${result} ${dirlist})
#ENDMACRO()

#SUBDIRLIST(SUBDIRS ${CMAKE_CURRENT_SOURCE_DIR})
##
## [end] detail


#MESSAGE(STATUS "Detected modules:")
#FOREACH(subdir ${SUBDIRS})
#  MESSAGE(STATUS "  " ${subdir})
#ENDFOREACH()

#MESSAGE(STATUS "Configuring modules now ..")
#FOREACH(subdir ${SUBDIRS})
#  ADD_SUBDIRECTORY(${subdir})
#ENDFOREACH()

ADD_SUBDIRECTORY(DeviceGenerator)
#ADD_SUBDIRECTORY(ViennaMini)
