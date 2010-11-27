/***************************************************************************
openBibleViewer - Bible Study Tool
Copyright (C) 2009-2010 Paul Walger
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
#ifndef BIBLEDISPLAYSETTINGS_H
#define BIBLEDISPLAYSETTINGS_H

class BibleDisplaySettings
{
public:
    BibleDisplaySettings();
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

private:
    bool m_loadNotes;
    bool m_showMarks;
    bool m_showNotes;

};

#endif // BIBLEDISPLAYSETTINGS_H
