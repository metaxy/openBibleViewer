set(CMAKE_INSTALL_PREFIX "/Applications") 
set(CL_LIBS clucene-core)
# see https://gitlab.kitware.com/cmake/cmake/tree/master/Source/QtDialog

#clang xcode 4.5.2 tested

add_executable(openBibleViewer ${openBibleViewer_SRCS} ${UIS} ${RSCS} ${MOCS} ${OBVCore_SRCS})

message("ZLib: ${ZLIB_LIBRARIES}")

target_link_libraries( 
  openBibleViewer)
    OBVCore
    RtfReader
    ${ZLIB_LIBRARIES}
    Qt5::Core
    Qt5::Gui
    Qt5::Xml
    Qt5::Network
    Qt5::Sql
    Qt5::PrintSupport
    Qt5::WebKitWidgets
    Qt5::Script
    ${CL_LIBS}
    ${sw_libs}
    ${quazip_libraries}
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
set(CPACK_PACKAGE_VERSION_MINOR "9")
set(CPACK_PACKAGE_VERSION_PATCH "0")

include(CPack)
set(CMAKE_INSTALL_SYSTEM_RUNTIME_DESTINATION "${BIN_INSTALL_DIR}")

include(InstallRequiredSystemLibraries)

set(EXECUTABLE "${PROJECT_NAME}.app")
set(CPACK_OUTPUT_FILE_PREFIX "${CMAKE_CURRENT_SOURCE_DIR}/build_results/")

#include(DeployQt5)
#install_qt5_executable("openBibleViewer")
