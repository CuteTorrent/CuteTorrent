#pragma once

#include <QtGui>

#include "CustomWindow.h"
#include "ui_messagebox.h"

class CustomMessageBox : public BaseWindow<QDialog>
{
	Q_OBJECT
	Q_DISABLE_COPY(CustomMessageBox)

public:
	struct CustomButonInfo
	{
		QString text;
		QDialogButtonBox::ButtonRole role;
	};

	typedef QList<CustomButonInfo> CustomButtonInfoList;

	enum Button
	{
		// keep this in sync with QDialogButtonBox::StandardButton
		NoButton = 0x00000000,
		Ok = 0x00000400,
		Save = 0x00000800,
		SaveAll = 0x00001000,
		Open = 0x00002000,
		Yes = 0x00004000,
		YesToAll = 0x00008000,
		No = 0x00010000,
		NoToAll = 0x00020000,
		Abort = 0x00040000,
		Retry = 0x00080000,
		Ignore = 0x00100000,
		Close = 0x00200000,
		Cancel = 0x00400000,
		Discard = 0x00800000,
		Help = 0x01000000,
		Apply = 0x02000000,
		Reset = 0x04000000,
		RestoreDefaults = 0x08000000,
		CustomButton1 = 0x10000000,
		CustomButton2 = 0x20000000,
		CustomButton3 = 0x40000000,


		FirstButton = Ok, // internal
		LastButton = CustomButton3, // internal

		YesAll = YesToAll, // obsolete
		NoAll = NoToAll, // obsolete
		YesNo = Yes | No,

		Default = 0x00000100, // obsolete
		Escape = 0x00000200, // obsolete
		FlagMask = 0x00000300, // obsolete
		ButtonMask = ~FlagMask // obsolete
	};

	Q_DECLARE_FLAGS(Buttons, Button)

	explicit CustomMessageBox(QWidget* parent = 0);
	QPushButton* CreateButton(uint sb, CustomButtonInfoList customButtonsText) const;
	CustomMessageBox(QMessageBox::Icon icon, const QString& title, const QString& text,
	                 Buttons buttons = NoButton, QWidget* parent = 0,
	                 CustomButtonInfoList customButtonsText = CustomButtonInfoList());
	~CustomMessageBox();

	static Button critical(QWidget* parent, const QString& title,
	                       const QString& text, Buttons buttons = Ok, CustomButtonInfoList customButtonsText = CustomButtonInfoList());
	static Button information(QWidget* parent, const QString& title,
	                          const QString& text, Buttons buttons = Ok, CustomButtonInfoList customButtonsText = CustomButtonInfoList());
	static Button question(QWidget* parent, const QString& title,
	                       const QString& text, Buttons buttons = Buttons(Yes | No), CustomButtonInfoList customButtonsText = CustomButtonInfoList());
	static Button warning(QWidget* parent, const QString& title, const QString& text,
	                      Buttons buttons = Ok, CustomButtonInfoList customButtonsText = CustomButtonInfoList());
	static void about(QWidget* parent, const QString& title, const QString& text);
	static Button critical(const QString& title,
		const QString& text, Buttons buttons = Ok, CustomButtonInfoList customButtonsText = CustomButtonInfoList());
	static Button information(const QString& title,
		const QString& text, Buttons buttons = Ok, CustomButtonInfoList customButtonsText = CustomButtonInfoList());
	static Button question(const QString& title,
		const QString& text, Buttons buttons = Buttons(Yes | No), CustomButtonInfoList customButtonsText = CustomButtonInfoList());
	static Button warning(const QString& title, const QString& text,
		Buttons buttons = Ok, CustomButtonInfoList customButtonsText = CustomButtonInfoList());
	static void about(const QString& title, const QString& text);
	static Button showNewMessageBox(QWidget* parent,
	                                QMessageBox::Icon icon, const QString& title, const QString& text,
	                                Buttons buttons, CustomButtonInfoList customButtonsText);
private:
	CustomButtonInfoList customButtonsInfo;
	QPixmap standardIcon(QMessageBox::Icon icon) const;
	QAbstractButton* clickedButton;
	Button _clickedButton() const;
	bool isClosed;
	Button cancelButton;
protected:
	void showEvent(QShowEvent* event) override;

private:
	QMessageBox::Icon m_icon;
	Ui::CustomMessageBox* ui;
	QPushButton* getCloseBtn() override;
	QWidget* getTitleBar() override;
	QLabel* getTitleLabel() override;
	QLabel* getTitleIcon() override;
	QWidget* centralWidget() override;
private slots:
	void buttonClicked(QAbstractButton*);
	void accept() override;
	void reject() override;
};

