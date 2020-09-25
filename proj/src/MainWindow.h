#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class WorldModel;
class QTreeView;
class QSplitter;

class MainWindow : public QMainWindow
{
    Q_OBJECT
    Ui::MainWindow* ui;
    QString currentSavesFolder;

    QWidget* viewWidget;
    WorldModel* worldModel;
    QTreeView* worldTreeView;
    QSplitter* splitter;
public:
    explicit MainWindow(QWidget* parent = 0);
    ~MainWindow();
private slots:
    void slotAction();
private:
    void reloadWorlds();
};

#endif // MAINWINDOW_H
