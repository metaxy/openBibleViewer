/***************************************************************************
openBibleViewer - Bible Study Tool
Copyright (C) 2009-2011 Paul Walger
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
#ifndef MODULEDISPLAYSETTINGS_H
#define MODULEDISPLAYSETTINGS_H

class ModuleDisplaySettings
{
public:
    ModuleDisplaySettings();
    void setLoadNotes(bool loadNotes);
    /**
     * Load notes when loading the module
     */
    bool loadNotes();

    void setShowMarks(bool showMarks);
    /**
     * Show the marks from notes.
     * If this is true, loadNotes is also set true.
     */
    bool showMarks();

    void setShowNotes(bool showNotes);
    /**
     * Show a link to a note connected to the verse.
     * If this is true, loadNotes is also set true.
     */
    bool showNotes();

    void setShowBottomToolBar(bool showToolBar);
    bool showBottomToolBar();

    void setShowStrong(bool showStrong);
    bool showStrong();

    void setShowStudyNotes(bool showStudyNotes);
    bool showStudyNotes();

    void setShowRefLinks(bool showRefLinks);
    bool showRefLinks();

    void setShowRMac(bool b);
    bool showRMac();

    void setShowCaptions(bool b);
    bool showCaptions();

    void setShowProlog(bool b);
    bool showProlog();

    void setShowMedia(bool b);
    bool showMedia();

    void setShowGram(bool b);
    bool showGrams();

private:
    bool m_loadNotes;
    bool m_showMarks;
    bool m_showNotes;
    bool m_showStudyNotes;
    bool m_showBottomToolBar;
    bool m_showStrong;
    bool m_showRefLinks;

    bool m_showRMac;
    bool m_showCaptions;
    bool m_showProlog;
    bool m_showMedia;
    bool m_showGram;


};

#endif // MODULEDISPLAYSETTINGS_H
