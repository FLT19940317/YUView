/*  YUView - YUV player with advanced analytics toolset
*   Copyright (C) 2015  Institut für Nachrichtentechnik
*                       RWTH Aachen University, GERMANY
*
*   YUView is free software; you can redistribute it and/or modify
*   it under the terms of the GNU General Public License as published by
*   the Free Software Foundation; either version 2 of the License, or
*   (at your option) any later version.
*
*   YUView is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
*   GNU General Public License for more details.
*
*   You should have received a copy of the GNU General Public License
*   along with YUView.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef PLAYLISTITEM_H
#define PLAYLISTITEM_H

#include <QTreeWidgetItem>
#include <QStackedWidget>
#include "typedef.h"
#include <assert.h>

class playlistItem :
  public QObject,
  public QTreeWidgetItem 
{
  Q_OBJECT

public:
  
  /* The default constructor requires the user to set a name that will be displayed in the treeWidget and
   * provide a pointer to the widget stack for the properties panels. The constructor will then call 
   * addPropertiesWidget to add the custom properties panel.
  */
  playlistItem(QString itemNameOrFileName, QStackedWidget *stack);
  virtual ~playlistItem();

  QString getName() { return text(0); }

  /* Is this item indexed by a frame number or by a duration
   * 
   * A playlist item may be indexed by a frame number, or it may be a static object that is shown 
   * for a set amount of time.
   * TODO: Add more info here or in the class description
  */
  virtual bool isIndexedByFrame() = 0;
 
  // Return the info title and info list to be shown in the fileInfo groupBox.
  // The default implementations will return empty strings/list.
  virtual QString getInfoTitel() { return ""; }
  virtual QList<infoItem> getInfoList() { return QList<infoItem>(); }

  // Get the title of the properties panel
  virtual QString getPropertiesTitle() = 0;
  // If the playlist item is selected, call this function to show the associated
  // properties widget in the properties widget stack.
  void showPropertiesWidget();

  // Does the playlist item currently accept drops of the given item?
  virtual bool acceptDrops(playlistItem *draggingItem) { return false; }

  // ----- Video ----

  // Does the playlist item prvode video? If yes the following functions can be used
  // to access it.
  virtual bool  providesVideo() { return false; }

  virtual int   getFrameRate() { return 0; }
  virtual QSize getVideoSize() { return QSize(); }
  virtual int   getNumberFrames() { return -1; }

  virtual void drawFrame(int frameIdx) {}

  virtual int  sampling() { return 1; }

  // ------ Statistics ----

  // Does the playlistItem provide statistics? If yes, the following functions can be
  // used to access it
  virtual bool provideStatistics() { return false; }

signals:
  // Emitted if somthing in the playlist item changed and a redraw is necessary
  void signalRedrawItem();

protected:
  // Initialize the properties widget. IMPORTANT: Do not forget to call this from the derived constructor.
  void initPropertiesWidget() { assert(propertiesStack); if (propertiesStack != NULL) propertiesWidgetIdx = createPropertiesWidget( propertiesStack ); }

private:
  int propertiesWidgetIdx;
  QStackedWidget *propertiesStack;

  // Create the properties widget and add it to stack. Return the stack index.
  // Overload this function in a child class to create a custom widget. The default
  // implementation here will add an empty widget.
  virtual int createPropertiesWidget( QStackedWidget *stack );
};

#endif // PLAYLISTITEM_H
