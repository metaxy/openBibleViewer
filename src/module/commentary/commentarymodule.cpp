#include "commentarymodule.h"

CommentaryModule::CommentaryModule()
{
}


QSharedPointer<Versification> CommentaryModule::versification() const
{
    return m_versification;
}
