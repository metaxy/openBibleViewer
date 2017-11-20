if(NOT CMAKE_BUILD_TYPE STREQUAL "Release")
  add_definitions( -Wall -Wextra -Wformat -g3)
endif() 
set(CL_LIBS clucene-core)

add_executable(openBibleViewer ${openBibleViewer_SRCS} ${UIS} ${RSCS} ${MOCS} ${OBVCore_SRCS})


target_link_libraries( 
  openBibleViewer
    OBVCore
    RtfReader
    ${ZLIB_LIBRARIES}
    Qt5::Widgets
    Qt5::Core
    Qt5::Gui
    Qt5::Xml
    Qt5::Network
    Qt5::WebEngine
    Qt5::Sql
    ${cl_libs}
    ${sw_libs}

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
