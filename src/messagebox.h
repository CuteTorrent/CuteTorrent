#pragma once

#include <QtGui>

#include "CustomWindow.h"
#include "ui_messagebox.h"

class MyMessageBox : public BaseWindow<QDialog>
{
	Q_OBJECT
private:

	QPixmap standardIcon(QMessageBox::Icon icon);
	QAbstractButton* clickedButton;
	QMessageBox::StandardButton _clickedButton();
	bool isClosed;
	QMessageBox::StandardButton cancelButton;
protected:


public:
	explicit MyMessageBox(QWidget* parent = 0);
	MyMessageBox(QMessageBox::Icon icon, const QString& title, const QString& text,
	             QMessageBox::StandardButtons buttons = QMessageBox::NoButton, QWidget* parent = 0,
	             Qt::WindowFlags flags = Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);
	~MyMessageBox();

	static QMessageBox::StandardButton critical(QWidget* parent, const QString& title,
	        const QString& text, QMessageBox::StandardButtons buttons = QMessageBox::Ok);
	static QMessageBox::StandardButton information(QWidget* parent, const QString& title,
	        const QString& text, QMessageBox::StandardButtons buttons = QMessageBox::Ok);
	static QMessageBox::StandardButton question(QWidget* parent, const QString& title,
	        const QString& text, QMessageBox::StandardButtons buttons = QMessageBox::Ok);
	static QMessageBox::StandardButton warning(QWidget* parent, const QString& title, const QString& text,
	        QMessageBox::StandardButtons buttons = QMessageBox::Ok);
	static void about(QWidget* parent, const QString& title, const QString& text);
	static QMessageBox::StandardButton showNewMessageBox(QWidget* parent,
	        QMessageBox::Icon icon, const QString& title, const QString& text,
	        QMessageBox::StandardButtons buttons);
private:
	void showDialog();
	Ui::MessageBox* ui;
	QPushButton* getCloseBtn() override;
	QWidget* getTitleBar() override;
	QLabel* getTitleLabel() override;
	QLabel* getTitleIcon() override;
	QWidget* centralWidget() override;
private slots:
	void buttonClicked(QAbstractButton*);
	void accept();
	void reject();



};
