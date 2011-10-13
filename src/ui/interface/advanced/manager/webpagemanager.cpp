#include "webpagemanager.h"

WebPageManager::WebPageManager()
{
}
void WebPageManager::setWindowManager(WindowManager *windowManager)
{
    m_windowManager = windowManager;
}
void WebPageManager::pharseUrl(const QString &url)
{
    WebForm *f = (WebForm*)(m_windowManager->getForm(m_windowManager->needWebWindow()));
    f->pharseUrl(url);
}
