
SET( TEST_LIBRARIES OBVCore ${QT_QTTEST_LIBRARY} ${QT_LIBRARIES} 
                        ${QT_QTCORE_LIBRARY} 
                        ${QT_QTGUI_LIBRARY} 
                        ${QT_QTXML_LIBRARY} 
                        ${QT_QTNETWORK_LIBRARY} 
                        ${QT_QTWEBKIT_LIBRARY} 
                        ${QT_QTSCRIPT_LIBRARY}
                        ${QT_QTSQL_LIBRARY})


SET( VerseReplacer_SRCS versereplacertest.cpp )
#QT4_AUTOMOC( ${VerseReplacer_SRCS} )
set ( VerseReplacer_MOCS
    versereplacertest.h
)
QT4_WRAP_CPP( A_MOCS ${VerseReplacer_MOCS})
ADD_EXECUTABLE(VerseReplacerTest ${VerseReplacer_SRCS} ${A_MOCS})
TARGET_LINK_LIBRARIES( VerseReplacerTest ${TEST_LIBRARIES})

#ADD_TEST( NAME VerseReplacerTests COMMAND VerseReplacerTests )