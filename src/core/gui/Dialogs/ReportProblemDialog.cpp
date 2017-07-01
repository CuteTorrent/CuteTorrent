#include "ReportProblemDialog.h"
#include "EmailValidator.h"
#include "messagebox.h"
#include "json.h"
#include <QNetworkRequest>
#include <QNetworkReply>
#include <helpers/StaticHelpers.h>

QPushButton* ReportProblemDialog::getCloseBtn()
{
	return pbClose;
}

QWidget* ReportProblemDialog::getTitleBar()
{
	return titleBar;
}

QWidget* ReportProblemDialog::centralWidget()
{
	return m_centralWidget;
}

QLabel* ReportProblemDialog::getTitleLabel()
{
	return LTitle;
}

QLabel* ReportProblemDialog::getTitleIcon()
{
	return tbMenu;
}

ReportProblemDialog::ReportProblemDialog(QWidget* parent)
	: BaseWindow<QDialog>(OnlyCloseButton, NoResize, parent)
	  , m_pNetManager(new QNetworkAccessManager(this))
{
	setupUi(this);
	setupCustomWindow();
	setupWindowIcons();
	emailEdit->setValidator(new EmailValidator(this));
	connect(m_pNetManager, SIGNAL(finished(QNetworkReply*)), SLOT(OnFinished(QNetworkReply*)));
}

void ReportProblemDialog::SendReport()
{
	if (checkFields())
	{
		QApplication::setOverrideCursor(Qt::WaitCursor);
		QtJson::JsonObject postData;
		postData["name"] = nameEdit->text();
		postData["email"] = emailEdit->text();

		if (sendOSInfoCheckBox->isChecked())
		{
			QProcess dxDiagProcess;
			QStringList arguments;
			QString reportFileName = QString("dxdiagReport.%1.txt").arg(QApplication::applicationPid());
			QString dxDiagReportFilePath = StaticHelpers::CombinePathes(QStandardPaths::writableLocation(QStandardPaths::TempLocation), reportFileName);
			arguments << "/whql:on" << QString("/t%1").arg(dxDiagReportFilePath);
			dxDiagProcess.start("dxdiag.exe", arguments);
			dxDiagProcess.waitForFinished();
			QFile reportFile(dxDiagReportFilePath);
			if (reportFile.open(QIODevice::ReadOnly))
			{
				QString reportData = QString::fromLocal8Bit(reportFile.readAll());
				postData["problem"] = problemDescriptionEdit->toPlainText().append("\n").append(reportData);
				reportFile.close();
				QFile::remove(dxDiagReportFilePath);
			}
			else
			{
				QApplication::restoreOverrideCursor();
				CustomMessageBox::critical("Windows Info creation failed", "Failed to create DxDaig Report.\nError: " + reportFile.errorString());
				return;
			}
		}
		else
		{
			postData["problem"] = problemDescriptionEdit->toPlainText();
		}


		QByteArray data = QtJson::serialize(postData);
		qDebug() << "Sending JSON:" << QString(data);
		QNetworkRequest request(QUrl("http://integration.cutetorrent.info/report.php"));
		request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
		m_pNetManager->post(request, data);
	}
}

void ReportProblemDialog::OnFinished(QNetworkReply* pReply)
{
	QApplication::restoreOverrideCursor();
	if (pReply->error() == QNetworkReply::NoError)
	{
		qDebug() << "Server output:" << pReply->readAll();
		CustomMessageBox::information(tr("SEND_SUCCES"), tr("REPORT_THNX_MSG"));
		accept();
	}
	else
	{
		CustomMessageBox::critical(tr("Error"), pReply->errorString() + pReply->readAll());
		reject();
	}
}

bool ReportProblemDialog::checkFields()
{
	if (nameEdit->text().isEmpty())
	{
		CustomMessageBox::warning(tr("INFVALID_FIELD"), tr("NAME_EMPTY_ERROR"));
		return false;
	}

	QString email = emailEdit->text();
	int pos = 0;
	if (email.isEmpty() && emailEdit->validator()->validate(email, pos) == QValidator::Invalid)
	{
		CustomMessageBox::warning(tr("INFVALID_FIELD"), tr("EMAIL_EMPTY_OR_INVALID_ERROR"));
		return false;
	}


	if (problemDescriptionEdit->toHtml().isEmpty())
	{
		CustomMessageBox::warning(tr("INFVALID_FIELD"), tr("DESCRIPTION_EMPTY_ERROR"));
		return false;
	}

	return true;
}

