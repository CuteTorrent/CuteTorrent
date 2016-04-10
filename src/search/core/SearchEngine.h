#pragma once
#include <QtCore>
#include "ISerachProvider.h"
#include "SearchItemsStorrage.h"
#include "SearchCommon.h"
#include <QtScript/QScriptEngine>
#include "QSgmlTag.h"
#include <searchproviders/CustomScriptSearchProvider.h>

class QScriptEngineDebugger;
class QMainWindow;

class SearchEngine : public QObject
{
	Q_OBJECT
public:
	static SearchEnginePtr getInstance();
	~SearchEngine();
	void DoSerach(QString& token, ISerachProvider::SearchCategories category, int page);
	SearchItemsStorragePtr GetResults();
	QList<CustomScriptSearchProvider*> GetSearchProviders();
	void enableScriptDebugging();
	void disableScriptDebugging();
	bool isEnabledScriptDebugging();
	signals:
	void GotResults();
private:
	void setBusy(bool val);

	struct SearchInfo
	{
		QString ScriptName;
		QString GlobalName;
		QString IconName;
	};

	QScriptEngineDebugger* m_scriptDebugger;
	QMainWindow* m_debuggerWindow;
	bool m_scriptDebugingEnabled;
	QList<SearchInfo> m_scriptSearchInfos;
	QScriptEngine* m_scriptEngine;
	void initScriptEngine();
	SearchEngine();
	void loadSearchProvider(const QString path);
	void init();
	SearchItemsStorragePtr m_result;
	QList<CustomScriptSearchProvider*> m_pSearchProviders;
	static boost::weak_ptr<SearchEngine> m_pInstance;
	static void listSearchResFromScriptVal(const QScriptValue& value, QList<SearchResult*>& list);
	static QScriptValue listSearchResToScriptVal(QScriptEngine* engine, const QList<SearchResult*>& list);
	static QScriptValue listSgmTagsToScriptVal(QScriptEngine* engine, const QList<QSgmlTag*>& list);
	static void listSgmlTagsFromScriptVal(const QScriptValue& value, QList<QSgmlTag*>& list);

	static void sgmlTagFromScriptVal(const QScriptValue& value, QSgmlTag*& list);
	static QScriptValue sgmTagToScriptVal(QScriptEngine* engine, QSgmlTag* const& list);

	static QScriptValue searchResultCtor(QScriptContext* context, QScriptEngine* engine);
	static QScriptValue searchProviderCtor(QScriptContext* context, QScriptEngine* engine);
private slots:
	void OnSettngsChnaged(QString, QString);
	void OnSearchReady(QList<SearchResult*> result);
};

