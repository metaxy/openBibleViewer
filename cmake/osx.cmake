set(CMAKE_INSTALL_PREFIX "/Applications") 
set(CL_LIBS clucene-core)

#clang xcode 4.5.2 tested
add_definitions(-std=c++11)

add_executable(openBibleViewer ${openBibleViewer_SRCS} ${UIS} ${RSCS} ${MOCS} ${OBVCore_SRCS})

target_link_libraries( 
  openBibleViewer
    OBVCore
    RtfReader
    ${QT_LIBRARIES}
    ${QT_QTCORE_LIBRARY} 
    ${QT_QTGUI_LIBRARY} 
    ${QT_QTXML_LIBRARY} 
    ${QT_QTNETWORK_LIBRARY} 
    ${QT_QTWEBKIT_LIBRARY} 
    ${QT_QTSCRIPT_LIBRARY}
    ${QT_QTSQL_LIBRARY}
    ${CL_LIBS}
    ${ZLIB_LIBRARIES}
)

install( 
  TARGETS openBibleViewer
  RUNTIME DESTINATION . )
         
set(CMAKE_INSTALL_PREFIX "/Applications")
set(CPACK_GENERATOR "DragNDrop")
set(CPACK_DMG_FORMAT "UDBZ")
set(CPACK_DMG_VOLUME_NAME "${PROJECT_NAME}")
set(CPACK_SYSTEM_NAME "OSX")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "openBibleViewer")
set(CPACK_GENERATOR "TBZ2")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "openBibleViewer Project")
set(CPACK_PACKAGE_VENDOR "Paul Walger")
set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/README.md")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set(CPACK_PACKAGE_VERSION_MAJOR "0")
set(CPACK_PACKAGE_VERSION_MINOR "8")
set(CPACK_PACKAGE_VERSION_PATCH "0")

include(CPack)
set(CMAKE_INSTALL_SYSTEM_RUNTIME_DESTINATION "${BIN_INSTALL_DIR}")

include(InstallRequiredSystemLibraries)

set(EXECUTABLE "${PROJECT_NAME}.app")
set(CPACK_OUTPUT_FILE_PREFIX "${CMAKE_CURRENT_SOURCE_DIR}/build_results/")

include(DeployQt4)
install_qt4_executable("openBibleViewer")