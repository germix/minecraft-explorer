#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class WorldModel;
class QTreeView;
class QSplitter;
class TreeModel;
class TreeItem;

class MainWindow : public QMainWindow
{
    Q_OBJECT
    Ui::MainWindow* ui;
    QString currentSavesFolder;

    QSplitter* splitter;

    QWidget* viewWidget;

    TreeModel* treeModel;
    QTreeView* treeModelView;

    QAction* actionDirUp;
    QAction* actionDirEnter;
    QAction* actionOpenContainerFolder;
public:
    explicit MainWindow(QWidget* parent = 0);
    ~MainWindow();
private:
    void reloadWorlds();
    void updateActions();
    void addNbtTag(int type);
    void checkNbtTag(TreeItem* parent, QAction* action, int type);
private slots:
    void slotAction();

    void slotModelModified();

    void slotTreeView_customContextMenuRequested(const QPoint& pos);
    void slotTreeView_currentChanged(const QModelIndex& current, const QModelIndex& previous);
};

#endif // MAINWINDOW_H
