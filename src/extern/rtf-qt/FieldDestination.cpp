/*
    Copyright (C)  2010  Brad Hards <bradh@frogmouth.net>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "FieldDestination.h"

#include "rtfreader.h"
#include "controlword.h"
namespace RtfReader
{
FieldDestination::FieldDestination(Reader *reader, AbstractRtfOutput *output, const QString &name) :
    Destination(reader, output, name), m_fldinst(false), m_fldrslt(false)
{
}

FieldDestination::~FieldDestination()
{}

void FieldDestination::handleControlWord(const QString &controlWord, bool hasValue, const int value)
{
    if(controlWord == "fldrslt") {
        m_fldrslt = true;
    } else if(controlWord == "fldinst") {
        m_fldinst = true;
    } else if(controlWord == "*") {
        // handled elsewhere
    } else {
        m_fldrslt = false;
        m_fldrslt = false;
        if(ControlWord::isDestination(controlWord)) {
            qDebug() << "unhandled *Destination* control word in FieldDestinationn:" << controlWord;
        } else {
            qDebug() << "unhandled control word in FieldDestination:" << controlWord;
        }
    }
}

void FieldDestination::handlePlainText(const QString &plainText)
{
    qDebug() << plainText << m_fldinst << m_fldrslt;
    if(m_fldinst && !m_fldrslt) {
       m_url = plainText;
       const QString hyperlink = "HYPERLINK ";
       if(m_url.startsWith(hyperlink)) {
           m_url.remove(0, hyperlink.size());
       }

       return;
    } else if(m_fldinst && m_fldrslt) {
        m_text = plainText;
         m_output->appendLink(m_url, m_text);
        return;
    }

}
void FieldDestination::aboutToEndDestination()
{

}
}
