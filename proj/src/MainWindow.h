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
public:
    explicit MainWindow(QWidget* parent = 0);
    ~MainWindow();
private:
    void reloadWorlds();
private slots:
    void slotAction();

    void slotTreeView_customContextMenuRequested(const QPoint& pos);
};

#endif // MAINWINDOW_H
