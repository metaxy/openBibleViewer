#include "iconcache.h"

IconCache::IconCache()
{
    QStyle *style = QApplication::style();

    folderIcon.addPixmap(style->standardPixmap(QStyle::SP_DirClosedIcon), QIcon::Normal, QIcon::Off);
    folderIcon.addPixmap(style->standardPixmap(QStyle::SP_DirOpenIcon), QIcon::Normal, QIcon::On);

    bibleIcon = QIcon::fromTheme("page-2sides", QIcon(":/icons/16x16/page-2sides.png"));
    dictionayIcon = QIcon::fromTheme("text-x-texinfo", QIcon(":/icons/16x16/text-x-texinfo.png"));
    bookIcon = QIcon::fromTheme("text-x-generic", QIcon(":/icons/16x16/text-x-generic.png"));
}
