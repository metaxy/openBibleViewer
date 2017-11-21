set(CMAKE_INCLUDE_CURRENT_DIR ON)
#set(CMAKE_AUTOMOC ON)


SET(
    OBVCore_SRCS
    swordmanager.cpp
    actions.cpp

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
    module/dictionary/esworddictionary.cpp
    module/dictionary/mysworddictionary.cpp
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
    module/book/pdfbook.cpp
    module/commentary/webcommentary.cpp
    module/commentary/commentary.cpp
    module/commentary/commentarymodule.cpp
    module/commentary/thewordcommentary.cpp
    module/commentary/eswordcommentary.cpp
    module/commentary/myswordcommentary.cpp
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
    settings/settingsloader.cpp
    versereplacer.cpp
    version.cpp
    xbelreader.cpp
    xbelwriter.cpp
    downloadinfile.cpp

    iconcache.cpp

    rtftools.cpp
    rmac.cpp

    link/strongurl.cpp
    link/verseurl.cpp
    link/verseurlrange.cpp
    link/biblelink.cpp
    link/url.cpp
    link/urlconverter.cpp
    link/urlconverter2.cpp
    zefania.cpp

    books/booktree.cpp

    raw/blockidgen.cpp
    raw/bookblock.cpp
    raw/chapterblock.cpp
    raw/elementaryblock.cpp
    raw/rblock.cpp
    raw/rmetadata.cpp
    raw/styleblock.cpp
    raw/textfragment.cpp
    raw/verseblock.cpp
    raw/captionblock.cpp
    raw/prologblock.cpp
    raw/xreffragment.cpp
    raw/remarksblock.cpp
    raw/mediablock.cpp
    raw/brfragment.cpp
    raw/gramblock.cpp
    raw/supblock.cpp
    raw/divblock.cpp
    raw/noteblock.cpp

    raw/parser/rawtohtmlparser.cpp
    raw/parser/rawtotextrangeparser.cpp

    raw/blocktools.cpp
)

set ( OBVCore_MOCS
    actions.h
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
QT5_WRAP_CPP( CORE_MOCS ${OBVCore_MOCS})

ADD_LIBRARY( OBVCore STATIC ${OBVCore_SRCS} ${CORE_MOCS}) 
