#ifndef SWORDMANAGER_H
#define SWORDMANAGER_H
#include <swmgr.h>

class SwordManager
{
public:
    SwordManager();
    ~SwordManager();
    void init();
private:

    sword::SWMgr *m_library;
};

#endif // SWORDMANAGER_H
