
add_executable(openBibleViewer WIN32 ${openBibleViewer_SRCS} ${UIS} ${RSCS} ${MOCS})
set_source_files_properties(src/main.cpp PROPERTIES COMPILE_DEFINITIONS main=main)

target_link_libraries( 
  openBibleViewer
    OBVCore
    RtfReader
    ${ZLIB_LIBRARY}
    Qt5::Core
    Qt5::Gui
    Qt5::Xml
    Qt5::Network
    Qt5::Sql
    Qt5::PrintSupport
    Qt5::WebKitWidgets
    Qt5::Script
    ${CL_LIBS}
)

message(STATUS "MCLUCENE_DLL_SHARED : ${MCLUCENE_DLL_SHARED}")
message(STATUS "MCLUCENE_DLL_CORE : ${MCLUCENE_DLL_CORE}")
message(STATUS "UNZIP_EXE : ${UNZIP_EXE}")
message(STATUS "ZLIB_DLL : ${ZLIB_DLL}")
message(STATUS "MQT_BIN_DIR : ${MQT_BIN_DIR}")
install (
  FILES
    "${MQT_BIN_DIR}/Qt5Core.dll"
    "${MQT_BIN_DIR}/Qt5Xml.dll"
    "${MQT_BIN_DIR}/Qt5Gui.dll"
    "${MQT_BIN_DIR}/Qt5Network.dll"
    "${MQT_BIN_DIR}/Qt5Script.dll"
    "${MQT_BIN_DIR}/Qt5Sql.dll"
    "${MQT_BIN_DIR}/Qt5WebKit.dll"
    "${MQT_BIN_DIR}/Qt5WebKitWidgets.dll"
    "${MQT_BIN_DIR}/Qt5PrintSupport.dll"
    "${MQT_BIN_DIR}/Qt5Widgets.dll"
    "${MQT_BIN_DIR}/libGLESv2.dll"
    "${MQT_BIN_DIR}/libEGL.dll"
    "${MCLUCENE_DLL_SHARED}"
    "${MCLUCENE_DLL_CORE}"
    "${UNZIP_EXE}"
    "${ZLIB_DLL}"
  DESTINATION 
    bin
)
    
# CPack

# There is a bug in NSI that does not handle full unix paths properly. Make
# sure there is at least one set of four (4) backlasshes.
set(CPACK_NSIS_INSTALLED_ICON_NAME "bin\\\\openBibleViewer.exe")
set(CPACK_PACKAGE_ICON "${WIN_DEV_DIR2}openBibleViewer\\\\src\\\\icons\\\\124x124\\\\openBibleViewer.png")
set(CPACK_NSIS_DISPLAY_NAME "openBibleViewer")
set(CPACK_NSIS_HELP_LINK "http://metaxy.github.com/openBibleViewer")
set(CPACK_NSIS_URL_INFO_ABOUT "http://metaxy.github.com/openBibleViewer")
set(CPACK_NSIS_MODIFY_PATH OFF)
set(CPACK_PACKAGE_INSTALL_DIRECTORY "openBibleViewer")
set(CPACK_GENERATOR "TBZ2")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "openBibleViewer Project")
set(CPACK_PACKAGE_VENDOR "Paul Walger")
set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/README.md")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set(CPACK_PACKAGE_VERSION_MAJOR "0")
set(CPACK_PACKAGE_VERSION_MINOR "9")
set(CPACK_PACKAGE_VERSION_PATCH "0")

set(EXECUTABLE "${PROJECT_NAME}${CMAKE_EXECUTABLE_SUFFIX}")

include(CPack)
include(InstallRequiredSystemLibraries)
include(DeployQt5)
install_qt5_executable("openBibleViewer")
