#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QModelIndex>
#include <QTranslator>

namespace Ui {
class MainWindow;
}

class WorldModel;
class QTreeView;
class QSplitter;
class TreeModel;
class TreeItem;
class RecentFilesMenu;

class MainWindow : public QMainWindow
{
    Q_OBJECT
    Ui::MainWindow* ui;
    QString currentFolder;

    QSplitter* splitter;

    QWidget* viewWidget;

    TreeModel* treeModel;
    QTreeView* treeModelView;

    TreeItem* lastFindItem;
    QModelIndex lastFindIndex;
    int lastFindPosition;
    QString lastFindName;
    QString lastFindValue;

    RecentFilesMenu* recentFiles;

    QString languagesPath;
    QString currentLanguage;
    QTranslator currentTranslator;
public:
    explicit MainWindow(QWidget* parent = 0);
    ~MainWindow();
public:
    void changeEvent(QEvent* e);
private:
    void updateActions();
    void addNbtTag(int type);
    void checkNbtTag(TreeItem* parent, QAction* action, int type);
    void findNextItem();
    void openFolder(const QString& folder);
    void initRecentFilesMenu(const QByteArray& state);
    void loadLanguage(QString language);
    void initLanguages(QString initialLocale);
private slots:
    void slotAction();

    void slotModelModified();

    void slotLanguageChanged(QAction* action);

    void slotClipboard_dataChanged();

    void slotRecentFiles_fileTriggered(const QString& fileName);

    void slotTreeView_customContextMenuRequested(const QPoint& pos);
    void slotTreeView_doubleClicked(const QModelIndex& index);
    void slotTreeView_currentChanged(const QModelIndex& current, const QModelIndex& previous);
};

#endif // MAINWINDOW_H
