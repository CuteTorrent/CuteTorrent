#include "EmailNotifier.h"
#include "smtp/smtpclient.h"
#include "QApplicationSettings.h"
#include "smtp/mimehtml.h"
#include "smtp/mimeinlinefile.h"
#include <QIcon>
#include <QBuffer>

EmailNotifier::EmailNotifier(QObject* parent)
{
}

void EmailNotifier::SendEmail(QString to, QString subject, QString body)
{
	QApplicationSettingsPtr settings = QApplicationSettings::getInstance();
	boost::scoped_ptr<SmtpClient> smtpClient(new SmtpClient(
	            settings->valueString("rss", "smtp_host"),
	            settings->valueInt("rss", "smtp_port"),
	            static_cast<SmtpClient::ConnectionType>(settings->valueInt("rss", "smtp_conn_type"))
	        ));
	MimeMessage message;
	message.setSender(EmailAddress("notification@cutetorrent.info", "CuteTorrent"));
	message.addRecipient(EmailAddress(to));
	message.setSubject(subject);
	message.setHeaderEncoding(MimePart::_8Bit);
	MimeHtml html;
	html.setHtml(
	    "<table style="
	    "\"border-collapse: collapse;border-spacing: 0;width: 650px;min-width: 650px\">"
	    "	<tbody>"
	    "		<tr>"
	    "			<td style="
	    "			\"padding: 0;vertical-align: top;font-size: 1px;line-height: 1px\">"
	    "			&nbsp;</td>"
	    "		</tr>"
	    "	</tbody>"
	    "</table>"
	    "<table style="
	    "\"border-collapse: collapse;border-spacing: 0;Margin-left: auto;Margin-right: auto;width: 602px\">"
	    "<tbody>"
	    "		<tr>"
	    "			<td style="
	    "			\"padding: 0;vertical-align: top;font-size: 1px;line-height: 1px;background-color: #e9e9e9;width: 1px\">"
	    "			&nbsp;</td>"
	    "		</tr>"
	    "		<tr>"
	    "			<td style="
	    "			\"padding: 32px 0;vertical-align: top;mso-line-height-rule: at-least\">"
	    "			<div id=\"emb-email-header\" style="
	    "			\"color: #41637e; font-family: sans-serif; font-size: 26px; font-weight: 700; letter-spacing: -0.02em; line-height: 32px; text-align: center\">"
	    "				<img alt=\"\" height=\"64\" src="
	    "				\"cid:icon\""
	    "				style="
	    "				\"border: 0;-ms-interpolation-mode: bicubic;display: block;Margin-left: auto;Margin-right: auto;max-width: 64px\""
	    "				width=\"64\"></div>"
	    "			</td>"
	    "		</tr>"
	    "	</tbody>"
	    "</table>"
	    "<table style="
	    "\"border-collapse: collapse;border-spacing: 0;font-size: 1px;line-height: 1px;background-color: #e9e9e9;Margin-left: auto;Margin-right: auto\""
	    "width=\"602\">"
	    "	<tbody>"
	    "		<tr>"
	    "			<td style=\"padding: 0;vertical-align: top\"></td>"
	    "		</tr>"
	    "	</tbody>"
	    "</table>"
	    "<table style="
	    "\"border-collapse: collapse;border-spacing: 0;Margin-left: auto;Margin-right: auto\">"
	    "<tbody>"
	    "		<tr>"
	    "			<td style="
	    "			\"padding: 0;vertical-align: top;font-size: 1px;line-height: 1px;background-color: #e9e9e9;width: 1px\">"
	    "			</td>"
	    "			<td style=\"padding: 0;vertical-align: top\">"
	    "				<table style="
	    "				\"border-collapse: collapse;border-spacing: 0;Margin-left: auto;Margin-right: auto;width: 600px;background-color: #f7f7f7;font-size: 14px;table-layout: fixed\">"
	    "				<tbody>"
	    "						<tr>"
	    "							<td style="
	    "							\"padding: 0;vertical-align: top;text-align: left\">"
	    "								<div>"
	    "									<div style="
	    "									\"font-size: 32px;line-height: 32px\">"
	    "										&nbsp;"
	    "									</div>"
	    "								</div>"
	    "								<table style="
	    "								\"border-collapse: collapse;border-spacing: 0;table-layout: fixed;width: 100%\">"
	    "								<tbody>"
	    "										<tr>"
	    "											<td style="
	    "											\"padding: 0;vertical-align: top;padding-left: 32px;padding-right: 32px;word-break: break-word;word-wrap: break-word\">"
	    "											<p style="
	    "											\"Margin-top: 0;color: #565656;font-family: Georgia,serif;font-size: 16px;line-height: 25px;Margin-bottom: 25px;font-style: italic;font-size: 14px;\">"
	    + body + "</p>"
	    "											</td>"
	    "										</tr>"
	    "									</tbody>"
	    "								</table>"
	    "								<div style=\"font-size: 8px;line-height: 8px\">"
	    "									&nbsp;"
	    "								</div>"
	    "							</td>"
	    "						</tr>"
	    "					</tbody>"
	    "				</table>"
	    "			</td>"
	    "			<td style="
	    "			\"padding: 0;vertical-align: top;font-size: 1px;line-height: 1px;background-color: #e9e9e9;width: 1px\">"
	    "			</td>"
	    "		</tr>"
	    "	</tbody>"
	    "</table>"
	    "<table style="
	    "\"border-collapse: collapse;border-spacing: 0;font-size: 1px;line-height: 1px;background-color: #e9e9e9;Margin-left: auto;Margin-right: auto\""
	    "width=\"602\">"
	    "	<tbody>"
	    "		<tr>"
	    "			<td style=\"padding: 0;vertical-align: top\"></td>"
	    "		</tr>"
	    "	</tbody>"
	    "</table>"
	    "<div style=\"font-size: 1px;line-height: 32px;width: 100%\">"
	    "	&nbsp;"
	    "</div>"
	    "<table style="
	    "\"border-collapse: collapse;border-spacing: 0;Margin-left: auto;Margin-right: auto;width: 602px\">"
	    "<tbody>"
	    "		<tr>"
	    "			<td style="
	    "			\"padding: 0;vertical-align: top;font-size: 1px;line-height: 1px;background-color: #e9e9e9;width: 1px\">"
	    "			&nbsp;</td>"
	    "		</tr>"
	    "		<tr>"
	    "			<td style=\"padding: 0;vertical-align: top\">"
	    "				<table style=\"border-collapse: collapse;border-spacing: 0\">"
	    "					<tbody>"
	    "						<tr>"
	    "							<td style="
	    "							\"padding: 0;vertical-align: top;width: 250px;padding-top: 32px;padding-bottom: 64px\">"
	    "							<table style="
	    "							\"border-collapse: collapse;border-spacing: 0;table-layout: fixed;width: 100%\">"
	    "								<tbody>"
	    "										<tr>"
	    "											<td style="
	    "											\"padding: 0;vertical-align: top;padding-left: 0;padding-right: 10px;word-break: break-word;word-wrap: break-word;text-align: left;font-size: 12px;line-height: 20px;color: #999;font-family: Georgia,serif\">"
	    "											<div>"
	    "													<a href=\"http://cutetorrent.info\">CuteTorrent</a>"
	    "												</div>"
	    "											</td>"
	    "										</tr>"
	    "									</tbody>"
	    "								</table>"
	    "							</td>"
	    "							<td style="
	    "							\"padding: 0;vertical-align: top;width: 350px;padding-top: 32px;padding-bottom: 64px\">"
	    "							<table style="
	    "							\"border-collapse: collapse;border-spacing: 0;table-layout: fixed;width: 100%\">"
	    "								<tbody>"
	    "										<tr>"
	    "											<td style="
	    "											\"padding: 0;vertical-align: top;padding-left: 10px;padding-right: 0;word-break: break-word;word-wrap: break-word;font-size: 12px;line-height: 20px;color: #999;font-family: Georgia,serif;text-align: right\">"
	    "											<div>"
	    "													This email was sent to you"
	    "													beacuse you enabled"
	    "													notifications of automated"
	    "													downloads"
	    "												</div>"
	    "											</td>"
	    "										</tr>"
	    "									</tbody>"
	    "								</table>"
	    "							</td>"
	    "						</tr>"
	    "					</tbody>"
	    "				</table>"
	    "			</td>"
	    "		</tr>"
	    "	</tbody>"
	    "</table>"
	);
	QImage iconImage = QIcon(":/icons/app.ico").pixmap(64, 64).toImage();
	QByteArray byteArray;
	QBuffer buffer(&byteArray);
	iconImage.save(&buffer, "PNG");
	MimeInlineFile inlineIcon(byteArray, "appicon.png");
	inlineIcon.setContentId("icon");
	inlineIcon.setContentType("image/png");
	message.addPart(&html);
	message.addPart(&inlineIcon);
	smtpClient->connectToHost();

	if (smtpClient->waitForReadyConnected(5000))
	{
		smtpClient->login(
		    settings->valueString("rss", "smtp_user"),
		    settings->securedValueString("rss", "smtp_password")
		);

		if (smtpClient->waitForAuthenticated(5000))
		{
			smtpClient->sendMail(message);

			if (smtpClient->waitForMailSent(5000))
			{
				smtpClient->quit();
			}
		}
	}
}