SET(
    OBVCore_SRCS
    swordmanager.cpp
    context.cpp
    
    moduletools.cpp
    
    module/bible/bible.cpp
    module/bible/biblequote.cpp
    module/bible/zefania-bible.cpp

    module/modulemanager.cpp
    module/module.cpp
    module/versemodule.cpp
    module/textrangesversemodule.cpp
    module/simplemoduleclass.cpp
    module/versetable.cpp
    module/modulemap.cpp
    module/dictionary/zefania-lex.cpp
    module/dictionary/swordlexicon.cpp
    module/dictionary/dictionary.cpp
    module/dictionary/webdictionary.cpp
    module/dictionary/dictionarymodule.cpp
    module/dictionary/biblequote-dict.cpp
    module/dictionary/theworddictionary.cpp
    module/bible/biblemodule.cpp
    module/bible/thewordbible.cpp
    module/bible/swordbible.cpp
    module/metainfo.cpp
    module/searchablemodule.cpp
    module/webpage.cpp
    module/response/response.cpp
    module/response/urlresponse.cpp
    module/response/stringresponse.cpp
    module/response/textrangesresponse.cpp
    module/response/htmlresponse.cpp

    module/book/rtfbook.cpp
    module/book/book.cpp
    module/book/bookmodule.cpp
    module/book/txtbook.cpp
    module/book/treebook.cpp
    module/book/treebookmodule.cpp
    module/book/thewordtopic.cpp
    module/commentary/webcommentary.cpp
    module/commentary/commentary.cpp
    module/commentary/commentarymodule.cpp
    module/commentary/thewordcommentary.cpp
    module/book/eswordtopic.cpp
    module/simplemodule.cpp

    
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

    raw/blockidgen.cpp
    raw/bookblock.cpp
    raw/chapterblock.cpp
    raw/elementaryblock.cpp
    raw/rblock.cpp
    raw/rmetadata.cpp
    raw/stylefragment.cpp
    raw/textfragment.cpp
    raw/verseblock.cpp
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
