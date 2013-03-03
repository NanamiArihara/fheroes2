/***************************************************************************
 *   Copyright (C) 2013 by Andrey Afletdinov <fheroes2@gmail.com>          *
 *                                                                         *
 *   Part of the Free Heroes2 Engine:                                      *
 *   http://sourceforge.net/projects/fheroes2                              *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include <QtGui>

#include "mapwindow.h"

MapWindow::MapWindow()
{
    setAttribute(Qt::WA_DeleteOnClose);
    isUntitled = true;
    isModified = false;
}

void MapWindow::newFile(int sequenceNumber)
{
    isUntitled = true;

    QTextStream ss(& curFile);

    ss << "map_";
    ss.setFieldWidth(4);
    ss.setPadChar('0');
    ss << sequenceNumber;
    ss.setFieldWidth(0);
    ss << ".xml";

/*
    connect(&mapData, SIGNAL(selectionChanged(void)),
		    this, SLOT(mapWasSelectionChanged(void)));

    connect(&mapData, SIGNAL(dataModified(void)),
        	    this, SLOT(mapWasModified(void)));
*/

    mapWasModified();
}

bool MapWindow::loadFile(const QString & fileName)
{
    QApplication::setOverrideCursor(Qt::WaitCursor);

    if(! mapData.loadMap(fileName))
    {
	QApplication::restoreOverrideCursor();
        QMessageBox::warning(this, tr("Map Editor"),
                             tr("Cannot read file %1.")
                             .arg(fileName));
        return false;
    }

    setScene(& mapData);

    QApplication::restoreOverrideCursor();
    setCurrentFile(fileName);

    connect(&mapData, SIGNAL(selectionChanged(void)),
		    this, SLOT(mapWasSelectionChanged(void)));

    connect(&mapData, SIGNAL(dataModified(void)),
        	    this, SLOT(mapWasModified(void)));

    return true;
}

bool MapWindow::save(void)
{
    return isUntitled ? saveAs() : saveFile(curFile);
}

bool MapWindow::saveAs(void)
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save As"),
                                                    curFile);
    return fileName.isEmpty() ? false : saveFile(fileName);
}

bool MapWindow::saveFile(const QString & fileName)
{
/*
    QFile file(fileName);

    if(!file.open(QFile::WriteOnly | QFile::Text))
    {
        QMessageBox::warning(this, tr("Map Editor"),
                             tr("Cannot write file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return false;
    }

    QTextStream out(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    out << toPlainText();

    QApplication::restoreOverrideCursor();
    setCurrentFile(fileName);

*/
    return true;
}

QString MapWindow::userFriendlyCurrentFile(void)
{
    return strippedName(curFile);
}

QString MapWindow::currentFile(void)
{
    return curFile;
}

void MapWindow::closeEvent(QCloseEvent* event)
{
    if(maybeSave())
        event->accept();
    else
        event->ignore();
}

void MapWindow::mapWasSelectionChanged(void)
{
    emit copyAvailable(mapData.selectedItems().size());
}

void MapWindow::mapWasModified(void)
{
    isModified = true;
    setWindowTitle(curFile + "[*]");
    setWindowModified(true);
}

bool MapWindow::maybeSave(void)
{
/*
    if(isModified)
    {
	QMessageBox::StandardButton ret;

        ret = QMessageBox::warning(this, tr("Map Editor"),
                     tr("'%1' has been modified.\n"
		     "Do you want to save your changes?") .arg(userFriendlyCurrentFile()),
                     QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);

        if(ret == QMessageBox::Save)
            return save();
        else
	if(ret == QMessageBox::Cancel)
    	    return false;
    }
*/
    return true;
}

void MapWindow::setCurrentFile(const QString & fileName)
{
    curFile = QFileInfo(fileName).canonicalFilePath();
    isUntitled = false;
    isModified = false;
    setWindowModified(false);
    setWindowTitle(userFriendlyCurrentFile() + "[*]");
}

QString MapWindow::strippedName(const QString & fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}