#ifndef SWORDMANAGER_H
#define SWORDMANAGER_H

#ifdef BUILD_WITH_SWORD

#include <swmgr.h>

class SwordManager
{
public:
    SwordManager();
    ~SwordManager();
    void init();

    sword::SWMgr * getManager() const;
private:

    sword::SWMgr *m_library;
};
#else
class SwordManager
{
public:
    SwordManager();
};

#endif //BUILD_WITH_SWORD

#endif // SWORDMANAGER_H
