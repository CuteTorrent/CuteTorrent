#include "SearchEngine.h"
#include <QMessageBox>
#include <boost/weak_ptr.hpp>
#include <QtScriptTools/QScriptEngineDebugger>
#include <QApplication>
#include "CustomScriptSearchProvider.h"
#include <helpers/StaticHelpers.h>
#include "SearchResult.h"
#include <QMainWindow>
#include <QAction>
#include <QApplicationSettings.h>
boost::weak_ptr<SearchEngine> SearchEngine::m_pInstance;


SearchEngine::SearchEngine() : m_scriptDebugger(NULL), m_debuggerWindow(NULL), m_scriptDebugingEnabled(false)
{
	m_scriptEngine = new QScriptEngine(this);
	initScriptEngine();
	init();
	m_result = SearchItemsStorrage::getInstance();
	QApplicationSettingsPtr pSettings = QApplicationSettings::getInstance();
	connect(pSettings.get(), SIGNAL(PropertyChanged(QString, QString)), SLOT(OnSettngsChnaged(QString, QString)));
}

QScriptValue SearchEngine::searchProviderCtor(QScriptContext* context, QScriptEngine* engine)
{
	CustomScriptSearchProvider* object = new CustomScriptSearchProvider();
	QScriptValue scriptValue = engine->newQObject(object);
	object->setScriptObject(scriptValue);
	return scriptValue;
}

void SearchEngine::OnSettngsChnaged(QString group, QString key)
{
	if (group == "Search" && key == "script_debuging_enabled")
	{
		QApplicationSettingsPtr pSettings = QApplicationSettings::getInstance();

		if (pSettings->valueBool("Search", "script_debuging_enabled"))
		{
			if (!isEnabledScriptDebugging())
			{
				enableScriptDebugging();
			}
		}
		else
		{
			if (isEnabledScriptDebugging())
			{
				disableScriptDebugging();
			}
		}
	}
}

QScriptValue SearchEngine::searchResultCtor(QScriptContext* context, QScriptEngine* engine)
{
	SearchResult* object = new SearchResult();
	return engine->newQObject(object);
}


QScriptValue SearchEngine::listSearchResToScriptVal(QScriptEngine* engine, const QList<SearchResult*>& list)
{
	int len = list.length();
	QScriptValue val = engine->newArray(len);

	for (int i = 0; i < len; i++)
	{
		val.setProperty(QString::number(i), engine->newQObject(list[i]));
	}

	return val;
}


void SearchEngine::listSearchResFromScriptVal(const QScriptValue& value, QList<SearchResult*>& list)
{
	int len = value.property("length").toInt32();
	list.reserve(len);

	for (int i = 0; i < len; i++)
	{
		list.append(qobject_cast<SearchResult*>(value.property(QString::number(i)).toQObject()));
	}
}

QScriptValue SearchEngine::listSgmTagsToScriptVal(QScriptEngine* engine, const QList<QSgmlTag*>& list)
{
	int len = list.length();
	QScriptValue val = engine->newArray(len);

	for (int i = 0; i < len; i++)
	{
		val.setProperty(QString::number(i), engine->newQObject(list[i]));
	}

	return val;
}

void SearchEngine::listSgmlTagsFromScriptVal(const QScriptValue& value, QList<QSgmlTag*>& list)
{
	int len = value.property("length").toInt32();
	list.reserve(len);

	for (int i = 0; i < len; i++)
	{
		list.append(qobject_cast<QSgmlTag*>(value.property(QString::number(i)).toQObject()));
	}
}

QScriptValue SearchEngine::sgmTagToScriptVal(QScriptEngine* engine, QSgmlTag* const& sgmlTag)
{
	return engine->newQObject(sgmlTag);
}

void SearchEngine::sgmlTagFromScriptVal(const QScriptValue& value, QSgmlTag*& sgmlTag)
{
	sgmlTag = qobject_cast<QSgmlTag*>(value.toQObject());
}


void SearchEngine::initScriptEngine()
{
	qScriptRegisterMetaType<QList<SearchResult*>>(m_scriptEngine, listSearchResToScriptVal, listSearchResFromScriptVal);
	qScriptRegisterMetaType<QList<QSgmlTag*>>(m_scriptEngine, listSgmTagsToScriptVal, listSgmlTagsFromScriptVal);
	qScriptRegisterMetaType<QSgmlTag*>(m_scriptEngine, sgmTagToScriptVal, sgmlTagFromScriptVal);
	QScriptValue searchProvCtor = m_scriptEngine->newFunction(searchProviderCtor);
	QScriptValue searchResCtor = m_scriptEngine->newFunction(searchResultCtor);
	QScriptValue searchResultMetaObject = m_scriptEngine->newQMetaObject(&SearchResult::staticMetaObject, searchResCtor);
	QScriptValue searchProviderMetaObject = m_scriptEngine->newQMetaObject(&CustomScriptSearchProvider::staticMetaObject, searchProvCtor);
	m_scriptEngine->globalObject().setProperty("SearchResult", searchResultMetaObject);
	m_scriptEngine->globalObject().setProperty("CustomScriptSearchProvider", searchProviderMetaObject);
	m_scriptEngine->globalObject().setProperty("Enums", searchProviderMetaObject);
}

void SearchEngine::loadSearchProvider(const QString path)
{
	if (QDir(path).exists())
	{
		qDebug() << "Loading Search Provider from:" << path;
		QString configPath = StaticHelpers::CombinePathes(path, "search.ini");
		QSettings searchSettings(configPath, QSettings::IniFormat);
		SearchInfo info;
		searchSettings.beginGroup("SearchEngine");
		info.GlobalName = searchSettings.value("GlobalName").toString();
		info.IconName = searchSettings.value("Icon").toString();
		info.ScriptName = searchSettings.value("ScriptName").toString();
		m_scriptSearchInfos.append(info);
		searchSettings.endGroup();
		QString scriptFileName = StaticHelpers::CombinePathes(path, info.ScriptName);
		QFile scriptFile(scriptFileName);

		if (scriptFile.open(QIODevice::ReadOnly))
		{
			m_scriptEngine->evaluate(scriptFile.readAll(), scriptFileName);
			QScriptValue jsSearchProvider = m_scriptEngine->globalObject().property(info.GlobalName);

			if (jsSearchProvider.isObject() && jsSearchProvider.isValid())
			{
				CustomScriptSearchProvider* provider = qobject_cast<CustomScriptSearchProvider*>(jsSearchProvider.toQObject());

				if (provider != NULL)
				{
					m_pSearchProviders.append(provider);
					provider->setIcon(QIcon(StaticHelpers::CombinePathes(path, info.IconName)));
					connect(provider, SIGNAL(SearchReady(QList<SearchResult*>)), SLOT(OnSearchReady(QList<SearchResult*>)), Qt::QueuedConnection);
				}
				else
				{
					qCritical() << "SearchProvider ctor has error";
				}
			}

			scriptFile.close();
		}
		else
		{
			qCritical() << "Failed to open script file " << scriptFileName << scriptFile.errorString();
		}
	}
}

void SearchEngine::init()
{
#ifdef Q_WS_WIN
	QString rootPath = StaticHelpers::CombinePathes(QApplication::applicationDirPath(), "searchEngines");
#endif
#ifdef Q_OS_UNIX
	QString rootPath = "/usr/share/cutetorrent/searchEngines/";
#endif
	QDir rootDir = QDir(rootPath);

	if (rootDir.exists())
	{
		QStringList engineDirs = rootDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);

		foreach(QString styleDir, engineDirs)
			{
				loadSearchProvider(StaticHelpers::CombinePathes(rootPath, styleDir));
			}
	}
	else
	{
		qCritical() << "No searchEngine dir found!!!";
	}
}

SearchEnginePtr SearchEngine::getInstance()
{
	SearchEnginePtr instance = m_pInstance.lock();

	if (!instance)
	{
		instance.reset(new SearchEngine());
		m_pInstance = instance;
	}

	return instance;
}

SearchEngine::~SearchEngine()
{
	qDeleteAll(m_pSearchProviders);
}

void SearchEngine::DoSerach(QString& token, ISerachProvider::SearchCategories category, int page)
{
	m_result->clear();

	for (int i = 0; i < m_pSearchProviders.size(); i++)
	{
		ISerachProvider* searchProvider = m_pSearchProviders[i];
		searchProvider->PeformSearch(token, category, page);
	}
}

void SearchEngine::OnSearchReady(QList<SearchResult*> result)
{
	m_result->append(result);
	emit GotResults();
}

SearchItemsStorragePtr SearchEngine::GetResults()
{
	return m_result;
}

QList<CustomScriptSearchProvider*> SearchEngine::GetSearchProviders()
{
	return m_pSearchProviders;
}


void SearchEngine::enableScriptDebugging()
{
	m_scriptDebugingEnabled = true;
	m_scriptDebugger = new QScriptEngineDebugger(this);
	m_scriptDebugger->attachTo(m_scriptEngine);
	m_debuggerWindow = m_scriptDebugger->standardWindow();
	m_debuggerWindow->resize(1024, 640);
	m_debuggerWindow->show();
}

void SearchEngine::disableScriptDebugging()
{
	m_scriptDebugingEnabled = false;
	m_debuggerWindow->close();
	m_debuggerWindow = NULL;
	m_scriptDebugger->detach();
	delete m_scriptDebugger;
	m_scriptDebugger = NULL;
}

bool SearchEngine::isEnabledScriptDebugging()
{
	return m_scriptDebugingEnabled;
}

