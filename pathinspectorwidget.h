#pragma once

#include <QWidget>

class detailWidget_c;
class QFileSystemModel;
class QItemSelection;
class QMenu;
class QPoint;
class QPushButton;
class QTreeView;

/*!
   Defines a central widget of the application.
   Contains a tree view and some buttons for navigation along with preview logic \em detailWidget_c
   in order to display selection details.
 */
class pathInspectorWidget_c : public QWidget
{
  Q_OBJECT

  private:
    // The selection preview logic
    detailWidget_c *_detailWidget;
    // Navigation Up button
    QPushButton *_navigateUpButton;
    // Navigation Home button
    QPushButton *_navigateHomeButton;
    // File system data model
    QFileSystemModel *_fileSystemModel;
    // File system tree view
    QTreeView *_fileTreeView;
    // Context menu for the file system tree view
    QMenu *_fileTreeContextMenu;

  public:
    pathInspectorWidget_c( QWidget * = nullptr );

  protected:
    virtual ~pathInspectorWidget_c() = default;
    virtual void setupModel();
    virtual void setupTree();

  public slots:
    void folderSelected( const QString & );
    void handleCustomMenuActivation( const QPoint & );
    void handleContextMenuListAction();

  private slots:
    void fileTreeSelectionChanged( const QItemSelection &, const QItemSelection & );
    void handleNavigateUp();
    void handleNavigateHome();

  signals:
    void selectionChanged( const QString & );
};


