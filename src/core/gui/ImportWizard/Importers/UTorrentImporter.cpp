#include "UTorrentImporter.h"
#include <QFile>
#include <QDir>
#include "StaticHelpers.h"
#include <libtorrent/bencode.hpp>
#include <map>
#include <QDebug>
#include <TorrentManager.h>
#include <gui/Controls/messagebox.h>

UTorrentImporter::UTorrentImporter(QObject* parent) : m_pWorkerThread(new QThread(this))
{
	moveToThread(m_pWorkerThread);

	connect(m_pWorkerThread, SIGNAL(started()), SLOT(runImport()));
	connect(m_pWorkerThread, SIGNAL(finished()), SLOT(onFinished()));
}

UTorrentImporter::~UTorrentImporter()
{
}

QString UTorrentImporter::BTClientName()
{
	return "uTorrent";
}

bool UTorrentImporter::TryDetectResumePath(QString& path)
{
	QString defaultResumePath = StaticHelpers::CombinePathes(QDir::homePath(), "AppData", "Roaming", "uTorrent", "resume.dat");
	if (QFile::exists(defaultResumePath))
	{
		path = defaultResumePath;
		return true;
	}

	return false;
}

bool UTorrentImporter::StartImport(QString path)
{
	if (QFile::exists(path))
	{
		m_resumeFilePath = path;
		m_bIsRunning = true;
		m_pWorkerThread->start();
		return true;
	}
	return false;
}

void UTorrentImporter::CancelImport()
{
	m_bIsRunning = false;
}

bool UTorrentImporter::isRunning()
{
	return m_bIsRunning;
}

void UTorrentImporter::onFinished()
{
}

void UTorrentImporter::runImport()
{
	QFile file(m_resumeFilePath);
	QFileInfo resumeFileInfo(m_resumeFilePath);
	TorrentManagerPtr pTorrentManager = TorrentManager::getInstance();
	if (file.open(QIODevice::ReadOnly))
	{
		QByteArray utResumeData = file.readAll();
		file.close();
		entry allResumeFile;
		try
		{
			emit progress(tr("Reading resume file:") + m_resumeFilePath, 1);
			allResumeFile = bdecode(utResumeData.begin(), utResumeData.end());
		}
		catch (...)
		{
			CustomMessageBox::critical("Error", tr("FAILED_DECODE_UTORRENT_RESUME_FILE"));
			return;
		}

		std::map<std::string, entry> rootEnties = allResumeFile.dict();
		size_t size = rootEnties.size();
		if (rootEnties.count(".fileguard") > 0)
		{
			size--;
		}
		if (rootEnties.count("rec") > 0)
		{
			size--;
		}
		entry::dictionary_type::const_iterator i = rootEnties.begin(), begin = rootEnties.begin();
		for (; i != rootEnties.end(); ++i)
		{
			QString torrentFileName = QString::fromStdString(i->first);
			qDebug() << torrentFileName;
			if (torrentFileName.startsWith(".fileguard") || torrentFileName.startsWith("rec"))
			{
				//size--;
				continue;
			}
			torrentFileName = StaticHelpers::CombinePathes(resumeFileInfo.dir().absolutePath(), torrentFileName);
			entry torrentResume = i->second;
			QString savePath;
			QString captionStr;
			try
			{
				if (entry* p = torrentResume.find_key("path"))
				{
					std::string save_path = p->string();
					savePath = QDir::toNativeSeparators(QString::fromUtf8(save_path.c_str()));
					int lastPathSeparator = savePath.lastIndexOf(QDir::separator());
					if (lastPathSeparator > -1)
					{
						captionStr = savePath.mid(lastPathSeparator);
						savePath = savePath.remove(lastPathSeparator, savePath.length() - lastPathSeparator);
					}
				}
			}
			catch (...)
			{
			}
			if (m_bIsRunning == false)
			{
				break;
			}
			qDebug() << savePath;
			error_code ec;
			QString infoMessage = tr("Adding torrent: ") + (captionStr.isEmpty() ? torrentFileName : captionStr);
			int index = distance(begin, i);
			qDebug() << "progress" << index << size;
			emit progress(infoMessage, index * 100 / size);
			pTorrentManager->AddTorrent(torrentFileName, savePath, ec);
			if (ec)
			{
				emit error((captionStr.isEmpty() ? torrentFileName : captionStr), StaticHelpers::translateLibTorrentError(ec));
				//				CustomMessageBox::critical(NULL, "Error", StaticHelpers::translateLibTorrentError(ec));
			}
		}
	}
}

