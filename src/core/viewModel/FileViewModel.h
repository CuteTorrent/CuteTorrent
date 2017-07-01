#pragma once

#include "FileViewTreeItem.h"
#include "FileViewSortProxyModel.h"
#include <QAction>
#include <QPixmapCache>
#include <libtorrent/torrent_handle.hpp>


class QTreeView;
using namespace libtorrent;
struct file_entry;
class FileViewModel : public QAbstractItemModel
{

	Q_OBJECT
public:
	
	FileViewModel(QTreeView* proxyModel, QObject* parrent = NULL);
	~FileViewModel(void);
	int rowCount(const QModelIndex& parent = QModelIndex()) const override;
	int columnCount(const QModelIndex& parent = QModelIndex()) const override;
	QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
	bool setDataSource(const torrent_handle& storage);
	QVariant headerData(int section, Qt::Orientation orientation,
	                    int role = Qt::DisplayRole) const override;
	bool setHeaderData(int section, Qt::Orientation orientation, const QVariant& value,
	                   int role = Qt::EditRole) override;
	QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const override;
	QModelIndex parent(const QModelIndex& child) const override;
	void Update();
	void retranslateUI();

	virtual Qt::ItemFlags flags(const QModelIndex& index) const override;
	void BuildTree();
	int GetRow(FileViewTreeItem* parentItem) const;
	void AddPath(std::string path, file_entry fe);
public slots:
	void FileTabContextMenu(const QPoint& point);
	void OpenDirSelected();
	void OpenFileSelected();
	void SetLowForCurrentFile();
	void SetMediumForCurrentFile();
	void SetHighForCurrentFile();
	void SetNotDownloadForCurrentFile();
	void OnFileRenameCompleted();
	void RenameSelectedFile();
private:
	long long CalculateFolderSize(FileViewTreeItem* item) const;
	float CalculateFolderReady(FileViewTreeItem* item) const;
	void setupFileTabelContextMenu();
	const static int COLUMN_COUNT = 4;
	QList<QString> headerStringsData;
	torrent_handle dataSource;
	QAction* renameFile;
	void setFilePriority(int priorityToSet);
	void SetItemPriority(FileViewTreeItem* item, int priority, const QModelIndex& sourceIndex);
	QMenu* fileTabMenu;
	QAction* openFile;
	QAction* openDir;
	QMenu* priorityMenu;
	QAction* lowPriority;
	QAction* mediumPriority;
	QAction* highPriority;
	QAction* dontDownload;
	QTreeView* m_pView;
	FileViewSortProxyModel* m_pProxyModel;
	std::vector<int64_t> m_Progresses;
	static QHash<QString, QPixmapCache::Key> extToKeys;
	static QPixmapCache iconCache;
	FileViewTreeItem* m_pRoot;
	QString GetFullPath(FileViewTreeItem* pItem);
};

