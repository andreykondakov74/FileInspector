#include "detailwidget.h"

#include <QFileInfo>
#include <QLineEdit>
#include <QMimeDatabase>
#include <QPushButton>
#include <QTextEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include "util.h"

/*!
   C-tor
   \param parent parent widget
 */
detailWidget_c::detailWidget_c( QWidget *parent ) :
  QWidget( parent ),
  _pathLine{ nullptr },
  _pathListButton{ nullptr },
  _previewSize{ 0, 0 },
  _preview{ nullptr }
{
  _pathLine = new QLineEdit( this );

  _pathListButton = new QPushButton( tr( "List" ), this );
  _pathListButton->setDisabled( true );

  QHBoxLayout *listBoxLayout = new QHBoxLayout;
  listBoxLayout->addWidget( _pathLine );
  listBoxLayout->addWidget( _pathListButton );

  _preview = new QTextEdit;

  QVBoxLayout *detailsLayout = new QVBoxLayout;
  detailsLayout->addWidget( _preview );

  QVBoxLayout *mainLayout = new QVBoxLayout;
  QMargins mainLayoutMargins = mainLayout->contentsMargins();

  mainLayoutMargins.setTop( 0 );
  mainLayoutMargins.setBottom( 0 );
  mainLayout->setContentsMargins( mainLayoutMargins );

  mainLayout->addLayout( listBoxLayout );
  mainLayout->addLayout( detailsLayout );

  setLayout( mainLayout );

  setupPreview();
  setupPathValidPalette();
  setupPathNotValidPalette();

  connect( _pathLine, &QLineEdit::textChanged, this, &detailWidget_c::pathLineTextChanged );
  connect( _pathListButton, &QPushButton::clicked, this, &detailWidget_c::listManuallyEditedPath );
}

/*!
   Configures palette for a path, that is valid for the listing, i.e. a folder
 */
void detailWidget_c::setupPathValidPalette()
{
  _pathValidPalette.setColor( QPalette::Base, Qt::white );
}

/*!
   Configures palette for a path, that is not valid for the listing, i.e. a file
 */
void detailWidget_c::setupPathNotValidPalette()
{
  _pathNotValidPalette.setColor( QPalette::Base, Qt::red );
}

/*!
   Configures preview pane
 */
void detailWidget_c::setupPreview()
{
  _preview->setReadOnly( true );
  _preview->setVerticalScrollBarPolicy( Qt::ScrollBarAsNeeded );
  _preview->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
  _previewSize = _preview->size();
}

/*!
   Checks if the path given at \a path if valid for listing, i.e. folder
   \param path to check
   \return true if the path is valid for listing
 */
bool detailWidget_c::isPathValidForListing( const QString &path ) const
{
  return fileInspector_n::util_n::isValid( path, true );
}

/*!
   Handles details of the file system entry given at \a selectionPath
   \param selectionPath absolute path to the file system entry
   For a folder, a short listing is given in the preview pane
   For a text file, small portion is read and displayed in the preview pane
   For other files, file attributes name, size and type are displayed.
 */
void detailWidget_c::handleSelectionDetails( const QString &selectionPath )
{
  _preview->clear();

  if ( !QFileInfo::exists( selectionPath ) )
    return;

  if ( fileInspector_n::util_n::isValid( selectionPath, false ) )
  {
    QMimeDatabase mdb;
    QMimeType mime = mdb.mimeTypeForFile( selectionPath );

    if ( mime.inherits( "text/plain" ) )
    {
      const int maxPreviewContentSize = fileInspector_n::util_n::getMaxContentSize( _previewSize, _preview->currentFont() );

      QString fileContent = fileInspector_n::util_n::getTextFileContent( selectionPath, maxPreviewContentSize );
      if ( !fileContent.isEmpty() )
        _preview->setText( fileContent );
    }
    else
    {
      QFileInfo selectionFileInfo( selectionPath );
      _preview->setText( QString( "Name: %1\nType: %2\nSize: %3" ).arg( selectionPath ).arg( mime.name() ).
                         arg( selectionFileInfo.size() ) );
    }
  }
  else if ( fileInspector_n::util_n::isValid( selectionPath, true ) )
  {
    const int maxPreviewLines = fileInspector_n::util_n::getMaxContentLines( _previewSize, _preview->currentFont() );

    QStringList folderContent = fileInspector_n::util_n::getDirContent( selectionPath, maxPreviewLines );
    if ( !folderContent.isEmpty() )
      _preview->setText( folderContent.join( "\n" ) );
  }
}

/*!
   Slot to handle file system entry
   \param selectionPath absolute path for selected file system entry
 */
void detailWidget_c::handleSelectionPath( const QString &selectionPath )
{
  if ( !selectionPath.isEmpty() )
  {
    _pathLine->setText( selectionPath );
    handleSelectionDetails( selectionPath );
  }
}

/*!
   Slot to handle changes in the path display and edit line \em _pathLine
   \param newPath content of the path display line
 */
void detailWidget_c::pathLineTextChanged( const QString &newPath )
{
  if ( isPathValidForListing( newPath ) )
  {
    _pathListButton->setEnabled( true );
    _pathLine->setPalette( _pathValidPalette );
  }
  else
  {
    _pathListButton->setEnabled( false );
    _pathLine->setPalette( _pathNotValidPalette );
  }
}

/*!
   Slot to respond to user clicks on List button \em _pathListButton
 */
void detailWidget_c::listManuallyEditedPath()
{
  emit listPath( _pathLine->text() );
}
