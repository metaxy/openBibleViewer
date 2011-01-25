#include "versification_cache.h"

Versification_Cache::Versification_Cache(const QMap<int, BookV11N> &data)
{
    m_books = data;
    m_filter = Versification::ReturnAll;
}
Versification_Cache::~Versification_Cache()
{
}
