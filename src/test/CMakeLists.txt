
SET( TEST_LIBRARIES OBVCore ${QT_QTTEST_LIBRARY} ${QT_LIBRARIES} 
                        ${QT_QTCORE_LIBRARY} 
                        ${QT_QTGUI_LIBRARY} 
                        ${QT_QTXML_LIBRARY} 
                        ${QT_QTNETWORK_LIBRARY} 
                        ${QT_QTWEBKIT_LIBRARY} 
                        ${QT_QTSCRIPT_LIBRARY}
                        ${QT_QTSQL_LIBRARY})

MACRO(BUILD_TEST name)

    SET( ${name}_SRCS ${name}test.cpp )
    SET( ${name}_MOCS ${name}test.h )
    QT4_WRAP_CPP( ${name}__MOCS ${${name}_MOCS})
    ADD_EXECUTABLE(${name}Test ${${name}_SRCS} ${${name}__MOCS})
    TARGET_LINK_LIBRARIES( ${name}Test ${TEST_LIBRARIES})

ENDMACRO(BUILD_TEST)


BUILD_TEST("versereplacer")
BUILD_TEST("biblelink")