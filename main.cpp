#include "pathinspectormain.h"

#include <QApplication>

int main( int argc, char *argv[] )
{
  QApplication a( argc, argv );
  pathInspectorMain_c w;
  w.show();
  return a.exec();
}
