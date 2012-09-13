SET(
    OBVCore_SRCS
    swordmanager.cpp
    context.cpp
    
    moduletools.cpp
    
    verse/chapter.cpp
    verse/versebook.cpp

    verse/range.cpp
    verse/ranges.cpp
    verse/rangeenum.cpp
    verse/textrange.cpp
    verse/textranges.cpp
    verse/verse.cpp
    verse/versification.cpp
    verse/versificationextendeddata.cpp
    verse/versification/versification_kjv.cpp
    verse/versification/versification_zefania.cpp
    verse/versification/versification_biblequote.cpp
    verse/versification/versification_cache.cpp
    verse/versification/bookv11n.cpp
    verse/reftext.cpp


    notes/notes.cpp
    notes/xmlnotes.cpp
    notes/textnotes.cpp
    notes/simplenotes.cpp
    notes/notesitemview.cpp
    dbghelper.cpp
    verseselection.cpp

    basicclass.cpp
    moduledownloader.cpp
    history.cpp
    search/search.cpp
    search/searchprovider.cpp
    search/searchproviders.cpp
    search/searchresult.cpp
    search/searchquery.cpp
    search/searchhit.cpp
    search/searchtools.cpp
    settings/settings.cpp
    settings/modulesettings.cpp
    settings/session.cpp
    settings/moduledisplaysettings.cpp
    versereplacer.cpp
    version.cpp
    xbelreader.cpp
    xbelwriter.cpp
    downloadinfile.cpp

    iconcache.cpp
    
    qzip.cpp
    rtftools.cpp


    link/strongurl.cpp
    link/verseurl.cpp
    link/verseurlrange.cpp
    link/biblelink.cpp
    link/url.cpp
    link/urlconverter.cpp
    link/urlconverter2.cpp

    books/booktree.cpp
)
QT4_AUTOMOC( ${OBVCore_SRCS} )

set ( OBVCore_MOCS
    context.h
    notes/notes.h
    notes/xmlnotes.h
    notes/textnotes.h
    notes/simplenotes.h
    notes/notesitemview.h
    downloadinfile.h

    moduledownloader.h
    verse/versification/versification_kjv.h
    verse/versification/versification_zefania.h
    verse/versification/versification_cache.h
    verse/versification.h )
QT4_WRAP_CPP( CORE_MOCS ${OBVCore_MOCS})
ADD_LIBRARY( OBVCore STATIC ${OBVCore_SRCS} ${CORE_MOCS}) 
