/***************************************************************************
openBibleViewer - Bible Study Tool
Copyright (C) 2009-2011 Paul Walger <metaxy@walger.name>
This program is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the Free
Software Foundation; either version 3 of the License, or (at your option)
any later version.
This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
You should have received a copy of the GNU General Public License along with
this program; if not, see <http://www.gnu.org/licenses/>.
*****************************************************************************/
#include "zefaniabibletest.h"
#include <QtTest/QtTest>

#include <QDebug>
#include "src/core/moduletools.h"
#include "src/core/raw/parser/rawtohtmlparser.h"
void ZefaniaBibleTests::initTestCase()
{
    m_zef = new ZefaniaBible();
    m_settings = new Settings();
    ModuleSettings *root = m_settings->createRoot();
    m_mSet = m_settings->newModuleSettings(-1);
    m_mSet->moduleName = "Test Module";
    m_mSet->modulePath = "/home/paul/bible/zefania/de/elberfelder1905-strongs.xml";
    m_mSet->setDisplaySettings(root->displaySettings());
    m_zef->setSettings(m_settings);
}
void ZefaniaBibleTests::testLoad()
{
    QBENCHMARK_ONCE(m_zef->loadBibleData(m_mSet->moduleID, m_mSet->modulePath));
}
void ZefaniaBibleTests::testReadRange()
{
    if(!m_zef->loaded()) {
        m_zef->loadBibleData(m_mSet->moduleID, m_mSet->modulePath);
    }
    QBENCHMARK_ONCE(m_zef->rawTextRange(58,0,0,10));
}
void ZefaniaBibleTests::testRawReadBook()
{
    if(!m_zef->loaded()) {
        m_zef->loadBibleData(m_mSet->moduleID, m_mSet->modulePath);
    }
    ZefaniaXmlReader reader(m_mSet->modulePath, m_zef->versification());
   //m_zef->readBookBlock(0);
    QBENCHMARK_ONCE(reader.readBookBlock(45));

}
void ZefaniaBibleTests::testRawReadChapter()
{
    if(!m_zef->loaded()) {
        m_zef->loadBibleData(m_mSet->moduleID, m_mSet->modulePath);
    }
    ZefaniaXmlReader reader(m_mSet->modulePath, m_zef->versification());
    RawToHtmlParser parser;
    ChapterBlock block = reader.readChapterBlock(0,0);
    parser.parseChapter(&block);
}
void ZefaniaBibleTests::cleanupTestCase()
{
    delete m_zef;
    delete m_settings->getModuleSettings(-1);
    delete m_settings;
    delete m_mSet;
}

QTEST_MAIN( ZefaniaBibleTests )
