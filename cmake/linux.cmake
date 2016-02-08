add_definitions(-std=c++0x)
if(NOT CMAKE_BUILD_TYPE STREQUAL "Release")
  add_definitions( -Wall -Wextra -Wformat -g3)
endif() 
set(CL_LIBS clucene-core)

add_executable(openBibleViewer ${openBibleViewer_SRCS} ${UIS} ${RSCS} ${MOCS} ${OBVCore_SRCS})


target_link_libraries( 
  openBibleViewer
    OBVCore
    RtfReader
    ${qt_libraries}
    ${qt_qtcore_library} 
    ${qt_qtgui_library} 
    ${qt_qtxml_library} 
    ${qt_qtnetwork_library} 
    ${qt_qtwebkit_library} 
    ${qt_qtscript_library}
    ${qt_qtsql_library}
    ${cl_libs}
    ${sw_libs}
    ${zlib_libraries}
    ${quazip_libraries}
)
install( 
  TARGETS openBibleViewer
  RUNTIME DESTINATION bin )
  
install( 
  FILES "${openBibleViewer_SOURCE_DIR}/src/icons/124x124/openBibleViewer.png"
  DESTINATION "share/pixmaps/" )

install( 
  FILES "${openBibleViewer_SOURCE_DIR}/openBibleViewer.desktop"
  DESTINATION "share/applications/" )