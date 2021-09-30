#pragma once

#include <QMainWindow>

class pathInspectorWidget_c;

/*!
   Main window of the application
   Initializes a central widget and provides an application menu.
 */
class pathInspectorMain_c : public QMainWindow
{
  Q_OBJECT

  private:
    // The central widget
    pathInspectorWidget_c *_pathInspectorWidget;
  public:
    pathInspectorMain_c( QWidget * = nullptr );
    ~pathInspectorMain_c() = default;
  private slots:
    void slotOpenDialog();
  signals:
    void folderSelected( const QString & );
};

