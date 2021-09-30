#pragma once

#include <QStringList>

class QFont;
class QSize;

namespace fileInspector_n::util_n
{
  bool isValid( const QString &, bool );
  QStringList getDirContent( const QString &, int = -1 );
  QString getTextFileContent( const QString &, int = -1 );
  int getMaxContentSize( const QSize &, const QFont & );
  int getMaxContentLines( const QSize &, const QFont & );
}
