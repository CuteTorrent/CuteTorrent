#include "ExtratorrentSearchProvider.h"
#include <QDebug>
#include "defs.h"
QString ExtratorrentSearchProvider::Name()
{
	return "Extratorrent";
}

QString ExtratorrentSearchProvider::Url()
{
	return "http://extratorrent.cc";
}

int ExtratorrentSearchProvider::SupportedCategories()
{
	return ISerachProvider::All;
}

void ExtratorrentSearchProvider::PeformSearch(QString token, SearchCategories category, int page)
{
	QNetworkRequest request;
	request.setUrl(BuildUrl(token, category, page));
	m_pNetworkManager->get(request);
}

ExtratorrentSearchProvider::ExtratorrentSearchProvider()
{
	m_pNetworkManager = new QNetworkAccessManager(this);
	QObject::connect(m_pNetworkManager, SIGNAL(finished(QNetworkReply*)),
	                 this, SLOT(replyReady(QNetworkReply*)));
	m_categoryMap[Anime] = "1";
	m_categoryMap[Music] = "5";
	m_categoryMap[TV] = "8";
	m_categoryMap[Porn] = "533";
	m_categoryMap[Software] = "7";
	m_categoryMap[Games] = "3";
	m_categoryMap[Books] = "2";
	m_categoryMap[Movie] = "4";
}

void ExtratorrentSearchProvider::replyReady(QNetworkReply* pReply)
{
	if(pReply->error() == QNetworkReply::NoError)
	{
		QList<SearchResult> results;
		QString contentType = pReply->header(QNetworkRequest::ContentTypeHeader).toString();
		QString encoding = detectEncoding(contentType);
		QTextCodec* codec = QTextCodec::codecForName(encoding.toLocal8Bit());
		QString data = codec->makeDecoder()->toUnicode(pReply->readAll());
		QSgml sgml(data);
		QList<QSgmlTag*> tags;
		sgml.getElementsByAtribute("class", "tl", &tags);

		if(tags.count() > 0)
		{
			if(tags.count() > 1)
			{
				qCritical() << "More than one search result table found.";
			}

			QSgmlTag* resaltsTable = tags.at(0);
			QSgmlTag::QSgmlTaglist tableBody;
			int nChildCount = resaltsTable->Children.count();

			for(int i = 0; i < nChildCount; i++)
			{
				QSgmlTag* pChild = resaltsTable->Children.at(i);

				if(pChild->Name.compare("tr", Qt::CaseInsensitive) == 0)
				{
					tableBody.append(pChild);
				}
			}

			if(!tableBody.empty())
			{
				ParseTableBody(tableBody, &sgml);
			}
		}
	}
}

const QString ExtratorrentSearchProvider::BuildUrl(QString token, SearchCategories category, int page)
{
	if(category != All)
	{
		return QString("http://extratorrent.cc/advanced_search/?with=%1&s_cat=%2&page=%3").arg(token, m_categoryMap[category], QString::number(page));
	}
	else
	{
		return QString("http://extratorrent.cc/advanced_search/?with=%1&page=%2").arg(token, QString::number(page));
	}
}

void ExtratorrentSearchProvider::ParseTableBody(QSgmlTag::QSgmlTaglist tableBody, QSgml* sgml)
{
	QQueue<QSgmlTag*> traverseQueue;
	traverseQueue.append(tableBody);
	int tdCounter = 0;
	QList<SearchResult*> searchResults;
	SearchResult* pCurrentSearchResult = NULL;

	while(!traverseQueue.empty())
	{
		QSgmlTag* pCurrent = traverseQueue.dequeue();
		traverseQueue.append(pCurrent->Children);

		if(pCurrent->Name.compare("td", Qt::CaseInsensitive) == 0)
		{
			QString sizeHtmlStr;
			QString seedersCountStr;
			QString peersCountStr;

			switch(tdCounter)
			{
				case 0:
				{
					//ToDo: extract torrent url
					int nChildCount = pCurrent->Children.count();

					for(int i = 0; i < nChildCount; i++)
					{
						QSgmlTag* pChild = pCurrent->Children.at(i);

						if(pChild->Name.compare("a", Qt::CaseInsensitive) == 0)
						{
							pCurrentSearchResult = new SearchResult();
							pCurrentSearchResult->TorrentFileUrl = Url() + pChild->getAttributeValue("href");
							break;
						}
					}

					break;
				}

				case 1:
				{
					// No action on block containing category
					break;
				}

				case 2:
				{
					if(pCurrent->checkAttribute("class", "tli"))
					{
						int nChildCount = pCurrent->Children.count();

						for(int i = 0; i < nChildCount; i++)
						{
							QSgmlTag* pChild = pCurrent->Children.at(i);

							if(pChild->Name.compare("a", Qt::CaseInsensitive) == 0 && pCurrentSearchResult != NULL)
							{
								pCurrentSearchResult->Name = sgml->getInnerText(pChild);
								pCurrentSearchResult->TorrentDescUrl = Url() + pChild->getAttributeValue("href");
								break;
							}
						}
					}

					break;
				}

				case 3:
				{
					//Size
					sizeHtmlStr = sgml->getInnerText(pCurrent);
					sizeHtmlStr = sizeHtmlStr.replace("&nbsp;", " ");

					if(sizeHtmlStr.contains(" tb", Qt::CaseInsensitive) || sizeHtmlStr.contains(" gb", Qt::CaseInsensitive) ||
					        sizeHtmlStr.contains(" kb", Qt::CaseInsensitive) || sizeHtmlStr.contains(" mb", Qt::CaseInsensitive) ||
					        sizeHtmlStr.contains(" b", Qt::CaseInsensitive))
					{
						QStringList parts1 = sizeHtmlStr.split(' ');
						bool ok;
						double size = parts1[0].toDouble(&ok);

						if(ok && pCurrentSearchResult != NULL)
						{
							switch(parts1[1][0].toLower().toLatin1())
							{
								case 'k':
									size *= KbFloat;
									break;

								case 'm':
									size *= KbInt * KbFloat;
									break;

								case 'g':
									size *= KbInt * KbInt * KbFloat;
									break;

								case 't':
									size *= KbInt * KbInt * KbInt * KbFloat;
									break;

								case 'b':
									break;
							}

							pCurrentSearchResult->size = size;
						}
						else if(pCurrentSearchResult != NULL)
						{
							pCurrentSearchResult->size = -1;
						}
					}

					break;
				}

				case 4:
				{
					//seeders count
					seedersCountStr = sgml->getInnerText(pCurrent);
					bool ok;
					long nSeedsCount = seedersCountStr.toLong(&ok);

					if(ok && pCurrentSearchResult != NULL)
					{
						pCurrentSearchResult->seeders = nSeedsCount;
					}
					else if(pCurrentSearchResult != NULL)
					{
						pCurrentSearchResult->seeders = -1;
					}

					break;
				}

				case 5:
				{
					//peers count
					peersCountStr = sgml->getInnerText(pCurrent);
					bool ok;
					long nPeersCount = peersCountStr.toLong(&ok);

					if(ok && pCurrentSearchResult != NULL)
					{
						pCurrentSearchResult->leechers = nPeersCount;
					}
					else if(pCurrentSearchResult != NULL)
					{
						pCurrentSearchResult->leechers = -1;
					}

					break;
				}

				case 6:
				{
					// end of item
					pCurrentSearchResult->Engine = Name();
					searchResults.append(pCurrentSearchResult);
					pCurrentSearchResult = NULL;
				}

				default:
					break;
			}

			tdCounter++;

			if(tdCounter >= 7)
			{
				tdCounter = 0;
			}
		}
	}

	emit SearchReady(searchResults);
}

QString ExtratorrentSearchProvider::detectEncoding(QString contentType)
{
	QString encoding = "UTF-8";
	QStringList parts = contentType.split(';');

	for each (QString part in parts)
	{
		if (part.contains("charset"))
		{
			QStringList charsetParts = part.split('=');
			encoding = charsetParts.last();
		}
	}

	return encoding;
}

