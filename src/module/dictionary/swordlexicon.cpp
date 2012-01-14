#include "swordlexicon.h"

SwordLexicon::SwordLexicon()
{
}
SwordLexicon::~SwordLexicon()
{
}
QString SwordLexicon::getEntry(const QString &entry)
{

}

QStringList SwordLexicon::getAllKeys()
{
    /*namespace DU = util::directory;

    if (!module()) {
        return 0;
    }

    if (m_entryList) return m_entryList;

    m_entryList = new QStringList();

    //Check for buggy modules! They will not be loaded any more.
    if ( name() == QString("ZhEnglish")) {
        qWarning() << "Module ZhEnglish is buggy and will not be loaded.";
        return m_entryList;
    }
    if ( name() == QString("EReo_en")) {
        qWarning() << "Module EReo_en is buggy and will not be loaded.";
        return m_entryList;
    }

    QString dir(DU::getUserCacheDir().absolutePath());
    QFile f1( QString(dir).append("/").append(name()));

    if ( f1.open( QIODevice::ReadOnly ) ) {
        QDataStream s( &f1 );
        QString ModuleVersion, CacheVersion, QDataStreamVersion;
        s >> ModuleVersion;
        s >> CacheVersion;
        s >> QDataStreamVersion;

        qDebug() << "Lexicon cache metadata"
        << "Name" << name()
        << "ModuleVersion" << ModuleVersion
        << "CacheVersion" << CacheVersion
        << "QDataStreamVersion" << QDataStreamVersion;

        // Check if cache is valid
        if (ModuleVersion == config(CSwordModuleInfo::ModuleVersion)
                && CacheVersion == CACHE_FORMAT
                && QDataStreamVersion == QString::number(s.version())) {
            s >> *m_entryList;

            f1.close();
            qDebug() << "Read" << m_entryList->count() << "entries from lexicon cache for module" << name();
            return m_entryList;
        }

        f1.close();
    }


    // Ok, no cache or invalid.

    qDebug() << "Read all entries of lexicon" << name();

    sword::SWModule* my_module = module();
    my_module->setSkipConsecutiveLinks(true);
    (*my_module) = sword::TOP;
    snap(); //snap to top entry

    do {
        if ( isUnicode() ) {
            m_entryList->append(QString::fromUtf8(my_module->KeyText()));
        }
        else {
            //for latin1 modules use fromLatin1 because of speed
            QTextCodec* codec = QTextCodec::codecForName("Windows-1252");
            m_entryList->append(codec->toUnicode(my_module->KeyText()));
        }

        (*my_module)++;
    }
    while ( !my_module->Error() );

    (*my_module) = sword::TOP; //back to the first entry

    my_module->setSkipConsecutiveLinks(false);

    if (m_entryList->count()) {
        m_entryList->first().simplified();

        if (m_entryList->first().trimmed().isEmpty()) {
            m_entryList->erase( m_entryList->begin() );
        }
    }

    qDebug() << "Writing cache file for lexicon module" << name();

    if (m_entryList->count()) {
        //create cache
        QString dir(DU::getUserCacheDir().absolutePath());
        QFile f2( QString(dir).append("/").append(name()) );

        if (f2.open( QIODevice::WriteOnly )) {
            QDataStream s( &f2 );
            s << config(CSwordModuleInfo::ModuleVersion) //store module version
            << QString(CACHE_FORMAT) //store BT version -- format may change
            << QString::number(s.version()) //store QDataStream version -- format may change
            << *m_entryList;
            f2.close();
        }
    }

    return m_entryList;*/
}

void SwordLexicon::search(SearchQuery query, SearchResult *result)
{

}

MetaInfo SwordLexicon::readInfo(const QString &name)
{

}
