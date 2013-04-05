#ifndef BLOCKRULES_H
#define BLOCKRULES_H
#include <QStringList>
class BlockRules
{
public:
    BlockRules();

    QStringList m_filterBySelector;

    QStringList m_blackListUrl;


    bool isEmpty();

};

#endif // BLOCKRULES_H
