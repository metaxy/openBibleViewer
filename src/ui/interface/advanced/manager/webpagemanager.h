#ifndef WEBPAGEMANAGER_H
#define WEBPAGEMANAGER_H
#include "src/core/basicclass.h"
#include "src/ui/interface/advanced/manager/windowmanager.h"

class WebPageManager : public BasicClass
{
public:
    WebPageManager();
    void setWindowManager(WindowManager *windowManager);

    void pharseUrl(const QString &url);

private:
    WindowManager *m_windowManager;
};

#endif // WEBPAGEMANAGER_H
