#include "simplemodule.h"

SimpleModule::SimpleModule()
{
}
void SimpleModule::setSettings(Settings *settings)
{
    m_settings = settings;
}
bool SimpleModule::loaded() const
{
    return m_loaded;
}
