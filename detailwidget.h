#pragma once

#include <QPalette>
#include <QWidget>

class QLineEdit;
class QPushButton;
class QTextEdit;

/*!
   Widget class to define file system selection details
   If the selection is a folder, brief listing of the folder content is displayed in the preview pane
   If the selection is a text file, small portion of its contect is displayed in the preview pane
   For other files, name, size and type file attributes are displayed in the preview pane
   The class makes it possible for a user to see the selection absolute path and type in another path for a listing.
 */
class detailWidget_c : public QWidget
{
  Q_OBJECT

  private:
    // Selection absolute path
    QLineEdit *_pathLine;
    // List button
    QPushButton *_pathListButton;
    // Size of the preview pane
    QSize _previewSize;
    // Preview pane
    QTextEdit *_preview;
    // Palette for a path, that is valid for the listing, i.e. a folder
    QPalette _pathValidPalette;
    // Palette for a path, that is not valid for the listing, i.e. a file
    QPalette _pathNotValidPalette;

  public:
    detailWidget_c( QWidget * = nullptr );
    virtual ~detailWidget_c() = default;

  protected:
    virtual void setupPathValidPalette();
    virtual void setupPathNotValidPalette();
    virtual void setupPreview();

  private:
    bool isPathValidForListing( const QString & ) const;
    void handleSelectionDetails( const QString & );

  public slots:
    void handleSelectionPath( const QString & );

  private slots:
    void pathLineTextChanged( const QString & );
    void listManuallyEditedPath();

  signals:
    void listPath( const QString & );
};

