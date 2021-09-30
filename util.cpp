#include "util.h"

#include <QDirIterator>
#include <QFileInfo>
#include <QFont>
#include <QFontMetrics>
#include <QSize>

namespace
{
  /*!
     Collects folder entries according to pre-configured directory iterator \a dirIt
     \param maximumLines number of maximum lines to collect
     \param collectedLines number of already collected lines
     \param dirIt directory iterator
     \param entries output container for the collected folder entries
   */
  void getDirEntries( int maximumLines, int &collectedLines, QDirIterator &dirIt, QStringList &entries )
  {
    if ( collectedLines == maximumLines )
      return;

    while( dirIt.hasNext() )
    {
      const auto dirEntry = dirIt.next();
      collectedLines++;
      if ( maximumLines != -1 && collectedLines == maximumLines )
      {
        entries << "...";
        break;
      }
      entries << dirEntry;
    }
  }
}

/*!
   Checks if a path given at \a path is valid
   \param path the path to check
   \param isDir true if the path points to a folder, otherwise to a file
   \return true if the path is valid
 */
bool fileInspector_n::util_n::isValid( const QString &path, bool isDir )
{
  if ( !path.isEmpty() )
  {
    QFileInfo pathFileInfo( path );
    return pathFileInfo.exists() && ( isDir ) ? pathFileInfo.isDir() : pathFileInfo.isFile();
  }
  return false;
}

/*!
   Collects content of a folder given at path \a path
   \param path the path to folder
   \param maximumLines number of maximum entries to collect
   \return collected folder entries
 */
QStringList fileInspector_n::util_n::getDirContent( const QString &path, int maximumLines )
{
  if ( !isValid( path, true ) )
    return {};

  int collectedLines = 0;
  QStringList retvalue;

  // first, collect all the sub-folders
  QDirIterator dirDirIt( path, QDir::Dirs | QDir::NoDotAndDotDot | QDir::AllDirs );
  getDirEntries( maximumLines, collectedLines, dirDirIt, retvalue );

  // collect files if there is enough place
  QDirIterator dirFileIt( path, QDir::Files );
  getDirEntries( maximumLines, collectedLines, dirFileIt, retvalue );

  return retvalue;
}

/*!
   Reads up to \a maximumSize bytes from a file with a path \a path
   \param path path to the file
   \param maximumSize number of maximum bytes to read. If -1 given, no limitation is applied
   \return the file content
 */
QString fileInspector_n::util_n::getTextFileContent( const QString &path, int maximumSize )
{
  if ( !isValid( path, false ) )
    return {};

  QFile file( path );
  if ( !file.open( QIODevice::ReadOnly ) )
    return {};

  QString retvalue;
  if ( maximumSize == -1 )
  {
    retvalue = file.readAll();
  }
  else
  {
    retvalue = file.read( maximumSize - 4 );
    retvalue.append( "\n..." );
  }

  file.close();

  return retvalue;
}

/*!
   Detects maximum size of content to be displayed on a widget without scrollbars with respect
   to widget size \a size and widget font \a font
   \param size the widget size
   \param font the widget font
   \return number of maximum size of the content to view on the widget without scrolling
 */
int fileInspector_n::util_n::getMaxContentSize( const QSize &size, const QFont &font )
{
  QFontMetrics fm( font );
  int averageCharWidthInPixels = fm.averageCharWidth();
  int charHeightInPixels = fm.height();

  return ( size.width() / averageCharWidthInPixels ) * ( size.height() / charHeightInPixels );
}


/*!
   Detects maximal number of lines to be displayed on a widget without scrollbars with respect
   to widget size \a size and widget font \a font
   \param size the widget size
   \param font the widget font
   \return maximal number of the lines to view on the widget without scrolling
 */
int fileInspector_n::util_n::getMaxContentLines( const QSize &size, const QFont &font )
{
  QFontMetrics fm( font );
  int charHeightInPixels = fm.height();

  return size.height() / charHeightInPixels;
}
