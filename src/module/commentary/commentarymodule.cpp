#include "commentarymodule.h"

CommentaryModule::CommentaryModule()
{
}
void CommentaryModule::setSettings(Settings *settings)
{
    m_settings = settings;
}

QSharedPointer<Versification> CommentaryModule::versification() const
{
    return m_versification;
}
