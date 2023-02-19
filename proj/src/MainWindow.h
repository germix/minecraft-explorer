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

class FindData
{
public:
    TreeItem* lastFindItem;
    QModelIndex lastFindIndex;
    int lastFindPosition;
    QString lastFindName;
    QString lastFindValue;
public:
    FindData()
    {
        lastFindItem = nullptr;
        lastFindPosition = -1;
    }
public:
    void clear()
    {
        lastFindItem = nullptr;
        lastFindIndex = QModelIndex();
        lastFindName = QString();
        lastFindValue = QString();
        lastFindPosition = -1;
    }
};

class MainWindow : public QMainWindow
{
    Q_OBJECT
    Ui::MainWindow* ui;

    QString                         lastFolder;
    QHash<QString, QTreeView*>      mapFolderToTree;
    QHash<TreeModel*, FindData*>    mapModelToFindData;

    int                             lastTabActive;
    QStringList                     lastOpenFolders;

    RecentFilesMenu*                recentFiles;

    QString                         languagesPath;
    QString                         currentLanguage;
    QTranslator                     currentTranslator;

public:
    explicit MainWindow(QWidget* parent = 0);
    ~MainWindow();
public:
    void closeEvent(QCloseEvent* e);
    void changeEvent(QEvent* e);
private:
    void loadSettings();
    void saveSettings();
    void updateActions();
    void updateTabLabel();
    void addNbtTag(int type);
    void checkNbtTag(TreeItem* parent, QAction* action, int type);
    void findNextItem();
    void openFolder(const QString& folder);
    void enterFolder(QTreeView* treeView, TreeModel* treeModel, const QString& folder);
    void initRecentFilesMenu(const QByteArray& state);
    void loadLanguage(QString language);
    void initLanguages(QString initialLocale);
    bool closeAll();
    bool closeEditor(QTreeView* editor);
    bool checkForUnsavedChanges(TreeModel* treeModel);
    TreeModel* getTreeModel(QTreeView* treeView);
    QTreeView* currentTreeView();
    FindData* getFindData(TreeModel* treeModel);
private slots:
    void slotAction();

    void slotModelModified();

    void slotLanguageChanged(QAction* action);

    void slotClipboard_dataChanged();

    void slotRecentFiles_fileTriggered(const QString& fileName);

    void slotTabWidget_currentChanged(int index);
    void slotTabWidget_tabCloseRequested(int index);
    void slotTabWidget_customContextMenuRequested(const QPoint& pos);

    void slotTreeView_customContextMenuRequested(const QPoint& pos);
    void slotTreeView_doubleClicked(const QModelIndex& index);
    void slotTreeView_currentChanged(const QModelIndex& current, const QModelIndex& previous);
};

#endif // MAINWINDOW_H
