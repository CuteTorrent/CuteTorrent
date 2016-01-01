#include "RaitingDialog.h"
#include "RaitingWidget.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <json.h>
#include <QNetworkInterface>
#include <messagebox.h>

RaitingDialog::RaitingDialog(QWidget* parent)
	: BaseWindow<QDialog>(FullTitle, NoResize, parent)
	, m_pNetworkAccessManager(new QNetworkAccessManager(this))
{
	setupUi(this);
	setupCustomWindow();
	setupWindowIcons();
	setupRaitingControls();
	connect(buttonBox->button(QDialogButtonBox::Ok), SIGNAL(clicked()),SLOT(sendRaiting()));
	connect(buttonBox->button(QDialogButtonBox::Cancel), SIGNAL(clicked()), SLOT(reject()));
	connect(m_pNetworkAccessManager, SIGNAL(finished(QNetworkReply*)), SLOT(OnRaitingSent(QNetworkReply*)));
}

void RaitingDialog::sendRaiting()
{
	QString mac = getMacAddress();

	QString macHash = QCryptographicHash::hash(mac.toUtf8(), QCryptographicHash::Md5).toHex();
	qDebug() << "Mac:" << mac << "UserID" << macHash;
	QtJson::JsonObject raitingObject;

	raitingObject["uid"] = macHash;
	raitingObject["custom_msg"] = plainTextEdit->toPlainText();
	raitingObject["speed"] = QString::number(m_raitingWidgets[SPEED]->rating(),103,2);
	raitingObject["design"] = QString::number(m_raitingWidgets[DESIGN]->rating(), 103, 2);
	raitingObject["usability"] = QString::number(m_raitingWidgets[USABILITY]->rating(), 103, 2);
	raitingObject["possibilities"] = QString::number(m_raitingWidgets[POSSIBILITIES]->rating(), 103, 2);
	QtJson::JsonObject postObject;
	postObject["rating"] = raitingObject;
	QByteArray data = QtJson::serialize(postObject);
	qDebug() << "Sending Rating JSON:" << QString(data);
	QNetworkRequest request(QUrl("http://integration.cutetorrent.info/rating"));
	request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
	m_pNetworkAccessManager->post(request, data);

}
QString RaitingDialog::getMacAddress()
{
	foreach(QNetworkInterface netInterface, QNetworkInterface::allInterfaces())
	{
		// Return only the first non-loopback MAC Address
		if (!(netInterface.flags() & QNetworkInterface::IsLoopBack))
			return netInterface.hardwareAddress();
	}
	return QString();
}
void RaitingDialog::OnRaitingSent(QNetworkReply* pReply)
{
	QApplication::restoreOverrideCursor();
	if (pReply->error() == QNetworkReply::NoError)
	{
		qDebug() << "Server output:" << pReply->readAll();
		CustomMessageBox::information(this, tr("SEND_SUCCES"), tr("RATING_THNX_MSG"));
		accept();

	}
	else
	{
		CustomMessageBox::critical(this, tr("Error"), pReply->errorString() + pReply->readAll());
		reject();
	}
}

QPushButton* RaitingDialog::getCloseBtn()
{
	return pbClose;
}

QWidget* RaitingDialog::getTitleBar()
{
	return titleBar;
}

QWidget* RaitingDialog::centralWidget()
{
	return m_centralWidget;
}

QLabel* RaitingDialog::getTitleLabel()
{
	return LTitle;
}

QLabel* RaitingDialog::getTitleIcon()
{
	return tbMenu;
}

void RaitingDialog::setupRaitingControls()
{
	for (int i = 0; i < RATING_COUNT; i++)
	{
		RatingWidget* ratingWidget = new RatingWidget(this);
		mainLayout->addWidget(ratingWidget, i, 1);
		m_raitingWidgets.append(ratingWidget);
	}
}
