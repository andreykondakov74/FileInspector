#include "pathinspectormain.h"

#include <QAction>
#include <QDir>
#include <QFileDialog>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>

#include "pathinspectorwidget.h"

/*!
   C-tor
   \param parent parent widget
 */
pathInspectorMain_c::pathInspectorMain_c( QWidget *parent )
  : QMainWindow( parent ),
    _pathInspectorWidget{ nullptr }
{
  setObjectName( "PathInspector" );
  setWindowTitle( tr( "Path Inspector" ) );

  // Actions
  QAction *openAction = new QAction( tr( "Open" ), this );
  openAction->setShortcut( Qt::CTRL+Qt::Key_O );
  connect( openAction, &QAction::triggered, this, &pathInspectorMain_c::slotOpenDialog );

  QAction *exitAction = new QAction( tr( "Exit" ), this );
  exitAction->setShortcut( Qt::ALT+Qt::Key_F4 );
  connect( exitAction, &QAction::triggered, this, &pathInspectorMain_c::close );

  QMenuBar *mb = menuBar();

  QMenu *menuFile = new QMenu( tr( "File" ), this );
  menuFile->addAction( openAction );
  menuFile->addAction( exitAction );

  mb->addMenu( menuFile );

  _pathInspectorWidget = new pathInspectorWidget_c( this );
  setCentralWidget( _pathInspectorWidget );
  connect( this, &pathInspectorMain_c::folderSelected, _pathInspectorWidget, &pathInspectorWidget_c::folderSelected );

  resize( 850, 600 );
}

/*!
   Slot to open a file seleciton dialog
   Only folders can be selected using the dialog.
   Emits signal \em folderSelected once the selection is done by user.
 */
void pathInspectorMain_c::slotOpenDialog()
{
  QFileDialog fileDialog( this, tr( "Choose a folder..." ) );
  fileDialog.setFileMode( QFileDialog::Directory );
  fileDialog.setOption( QFileDialog::ShowDirsOnly, true );
  fileDialog.setDirectory( QDir::rootPath() );
  if ( fileDialog.exec() )
  {
    QString selectedFolder = fileDialog.selectedFiles().at( 0 );
    emit folderSelected( selectedFolder );
  }
}

