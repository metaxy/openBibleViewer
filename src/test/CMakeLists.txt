add_definitions(-DSRC_PATH="${CMAKE_SOURCE_DIR}")

SET( TEST_LIBRARIES OBVCore ${QT_QTTEST_LIBRARY} ${QT_LIBRARIES} 
                        ${QT_QTCORE_LIBRARY} 
                        ${QT_QTGUI_LIBRARY} 
                        ${QT_QTXML_LIBRARY} 
                        ${QT_QTNETWORK_LIBRARY} 
                        ${QT_QTWEBKIT_LIBRARY} 
                        ${QT_QTSCRIPT_LIBRARY}
                        ${QT_QTSQL_LIBRARY})

FUNCTION(BUILD_TEST name exName)

    SET( ${name}_SRCS ${name}test.cpp testtools.cpp)
    SET( ${name}_MOCS ${name}test.h testtools.h)
    QT4_WRAP_CPP( ${name}__MOCS ${${name}_MOCS})
    ADD_EXECUTABLE(${exName}Test ${${name}_SRCS} ${${name}__MOCS})
    TARGET_LINK_LIBRARIES( ${exName}Test ${TEST_LIBRARIES}
                        RtfReader
                        ${QT_LIBRARIES}
                        ${QT_QTCORE_LIBRARY}
                        ${QT_QTGUI_LIBRARY}
                        ${QT_QTXML_LIBRARY}
                        ${QT_QTNETWORK_LIBRARY}
                        ${QT_QTWEBKIT_LIBRARY}
                        ${QT_QTSCRIPT_LIBRARY}
                        ${QT_QTSQL_LIBRARY}
                        clucene-core
                        ${ZLIB_LIBRARY}
)

ENDFUNCTION(BUILD_TEST)


BUILD_TEST("versereplacer" "VerseReplacer")
BUILD_TEST("biblelink" "BibleLink")
BUILD_TEST("v11n" "V11N")
BUILD_TEST("moduletools" "ModuleTools")
BUILD_TEST("zefaniabible" "ZefaniaBible")
BUILD_TEST("rtftools" "RtfTools")
