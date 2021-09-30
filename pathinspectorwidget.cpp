#include "pathinspectorwidget.h"

#include <QDir>
#include <QFileSystemModel>
#include <QItemSelection>
#include <QMenu>
#include <QPushButton>
#include <QTreeView>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include "detailwidget.h"

/*!
   C-tor
   \param parent parent widget
 */
pathInspectorWidget_c::pathInspectorWidget_c( QWidget *parent ) :
  QWidget( parent ),
  _detailWidget{ nullptr },
  _navigateUpButton{ nullptr },
  _navigateHomeButton{ nullptr },
  _fileSystemModel{ nullptr },
  _fileTreeView{ nullptr },
  _fileTreeContextMenu{ nullptr }
{
  _navigateUpButton = new QPushButton( tr( "Up" ), this );
  _navigateHomeButton = new QPushButton( tr( "Home" ), this );

  QHBoxLayout *buttonHboxLayout = new QHBoxLayout;
  buttonHboxLayout->addWidget( _navigateUpButton );
  buttonHboxLayout->addWidget( _navigateHomeButton );

  _fileSystemModel = new QFileSystemModel( this );
  setupModel();

  _fileTreeView = new QTreeView( this );
  setupTree();

  _detailWidget = new detailWidget_c;

  QVBoxLayout *navigationLayout = new QVBoxLayout;
  navigationLayout->addLayout( buttonHboxLayout );
  navigationLayout->addWidget( _fileTreeView );

  QHBoxLayout *mainLayout = new QHBoxLayout;
  mainLayout->addLayout( navigationLayout );
  mainLayout->addWidget( _detailWidget );

  setLayout( mainLayout );

  connect( _navigateUpButton, &QPushButton::clicked, this, &pathInspectorWidget_c::handleNavigateUp );
  connect( _navigateHomeButton, &QPushButton::clicked, this, &pathInspectorWidget_c::handleNavigateHome );

  connect( _fileTreeView->selectionModel(), &QItemSelectionModel::selectionChanged,
           this, &pathInspectorWidget_c::fileTreeSelectionChanged );
  connect( _detailWidget, &detailWidget_c::listPath, this, &pathInspectorWidget_c::folderSelected );
  connect( this, &pathInspectorWidget_c::selectionChanged, _detailWidget, &detailWidget_c::handleSelectionPath );
}

/*!
   Setup of the file system model
 */
void pathInspectorWidget_c::setupModel()
{
  _fileSystemModel->setOption( QFileSystemModel::DontWatchForChanges );
  _fileSystemModel->setFilter( QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot | QDir::AllDirs );
}

/*!
   Setup of the tree view widget
 */
void pathInspectorWidget_c::setupTree()
{
  const auto columnCount = _fileSystemModel->columnCount();
  const auto index = _fileSystemModel->setRootPath( QDir::currentPath() );

  _fileTreeView->setModel( _fileSystemModel );
  _fileTreeView->setRootIndex( index );
  // hiding Date Modified from the view
  _fileTreeView->hideColumn( columnCount - 1 );

  _fileTreeView->setContextMenuPolicy( Qt::CustomContextMenu );
  _fileTreeContextMenu = new QMenu( _fileTreeView );
  QAction *listAction = new QAction( tr( "List" ), this );
  _fileTreeContextMenu->addAction( listAction );
  connect( _fileTreeView, &QTreeView::customContextMenuRequested, this,
           &pathInspectorWidget_c::handleCustomMenuActivation );
  connect( listAction, &QAction::triggered, this, &pathInspectorWidget_c::handleContextMenuListAction );
}

/*!
   Slot to receive a folder to list
   \param folderPath absolute path to the folder
 */
void pathInspectorWidget_c::folderSelected( const QString &folderPath )
{
  if ( !folderPath.isEmpty() )
  {
    QModelIndex index = _fileSystemModel->setRootPath( folderPath );
    _fileTreeView->setRootIndex( index );
    _fileTreeView->scrollTo( index );
    emit selectionChanged( folderPath );
  }
}

/*!
   Slot to handle the tree view menu activation
   \param menuActivationPoint a point where the menu was requested
   The context menu is available for folders only
 */
void pathInspectorWidget_c::handleCustomMenuActivation( const QPoint &menuActivationPoint )
{
  QModelIndex index = _fileTreeView->indexAt( menuActivationPoint );
  if ( index.isValid() && _fileSystemModel->isDir( index ) )
    _fileTreeContextMenu->exec( _fileTreeView->viewport()->mapToGlobal( menuActivationPoint ) );
}

/*!
   Slot to handle activation of List context menu item
 */
void pathInspectorWidget_c::handleContextMenuListAction()
{
  QModelIndex index = _fileTreeView->currentIndex();
  folderSelected( _fileSystemModel->filePath( index ) );
}

/*!
   Slot to respond on tree view selection changes
   \param selected selected tree view item
   Emits \em selectionChanges signal with the selection path
 */
void pathInspectorWidget_c::fileTreeSelectionChanged( const QItemSelection &selected, const QItemSelection & )
{
  // QModelIndexList
  const auto selectedIndexes = selected.indexes();
  if ( !selectedIndexes.isEmpty() )
  {
    // QModelIndex
    const auto selectedIndex = selectedIndexes.at( 0 );
    const QString selectionPath = _fileSystemModel->filePath( selectedIndex );
    emit selectionChanged( selectionPath );
  }
}

/*!
   Slot to handle activation of Up push button
 */
void pathInspectorWidget_c::handleNavigateUp()
{
  auto fileSystemModelRootDir = _fileSystemModel->rootDirectory();
  if ( fileSystemModelRootDir.cdUp() )
    folderSelected( fileSystemModelRootDir.absolutePath() );
}

/*!
   Slot to handle activation of Home push button
 */
void pathInspectorWidget_c::handleNavigateHome()
{
  folderSelected( QDir::homePath() );
}
