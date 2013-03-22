#include "blockrules.h"

BlockRules::BlockRules()
{
}
bool BlockRules::isEmpty()
{
    return m_filterBySelector.isEmpty() && m_blackListUrl.isEmpty();
}
