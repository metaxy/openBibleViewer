#ifndef SWORDMANAGER_H
#define SWORDMANAGER_H
#include "src/core/dbghelper.h"
template <typename C>
 class Singleton
 {
 public:
    static C* instance ()
    {
        myDebug() << "get instance";
       if (!_instance)
          _instance = new C ();
       return _instance;
    }
    virtual
    ~Singleton ()
    {
       _instance = 0;
    }
 private:
    static C* _instance;
 protected:
    Singleton () { }
 };
 template <typename C> C* Singleton <C>::_instance = 0;

#ifdef BUILD_WITH_SWORD
#include <swmgr.h>

class SwordManager : public Singleton<SwordManager>
{
    friend class Singleton<SwordManager>;
public:
   ~SwordManager();
    sword::SWMgr * getManager() const;
protected:
    SwordManager();
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
