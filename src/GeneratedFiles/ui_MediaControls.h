/********************************************************************************
** Form generated from reading UI file 'MediaControls.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MEDIACONTROLS_H
#define UI_MEDIACONTROLS_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QWidget>
#include <phonon/seekslider.h>
#include <phonon/volumeslider.h>

QT_BEGIN_NAMESPACE

class Ui_MediaControls
{
public:
    QGridLayout *gridLayout;
    QLabel *currentTimeLabel;
    QPushButton *openFileButton;
    QPushButton *openUrlButton;
    QPushButton *pauseButton;
    QPushButton *reverseButton;
    QPushButton *forwardButton;
    Phonon::VolumeSlider *volumeSlider;
    QPushButton *playButton;
    Phonon::SeekSlider *seekSlider;
    QPushButton *fullScreenButton;
    QLabel *totalTimeLabel;

    void setupUi(QWidget *MediaControls)
    {
        if (MediaControls->objectName().isEmpty())
            MediaControls->setObjectName(QString::fromUtf8("MediaControls"));
        MediaControls->resize(416, 70);
        MediaControls->setFocusPolicy(Qt::ClickFocus);
        MediaControls->setContextMenuPolicy(Qt::NoContextMenu);
        MediaControls->setStyleSheet(QString::fromUtf8("#MediaControls{\n"
"border-radius: 10px;\n"
"border:solid 1px #2D2D2D;\n"
"background-color: qlineargradient(spread:pad, x1:1, y1:1, x2:1, y2:0, stop:0 rgba(93, 93, 93, 255), stop:1 rgba(45, 45, 45, 255));\n"
"}\n"
""));
        gridLayout = new QGridLayout(MediaControls);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setSizeConstraint(QLayout::SetMinimumSize);
        currentTimeLabel = new QLabel(MediaControls);
        currentTimeLabel->setObjectName(QString::fromUtf8("currentTimeLabel"));
        QPalette palette;
        QBrush brush(QColor(227, 227, 227, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::WindowText, brush);
        QBrush brush1(QColor(0, 0, 0, 0));
        brush1.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Button, brush1);
        palette.setBrush(QPalette::Active, QPalette::Base, brush1);
        palette.setBrush(QPalette::Active, QPalette::Window, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Button, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Base, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Window, brush1);
        QBrush brush2(QColor(120, 120, 120, 255));
        brush2.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Disabled, QPalette::WindowText, brush2);
        palette.setBrush(QPalette::Disabled, QPalette::Button, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Base, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Window, brush1);
        currentTimeLabel->setPalette(palette);

        gridLayout->addWidget(currentTimeLabel, 1, 0, 1, 1);

        openFileButton = new QPushButton(MediaControls);
        openFileButton->setObjectName(QString::fromUtf8("openFileButton"));
        openFileButton->setStyleSheet(QString::fromUtf8("background-color:\"transparent\";\n"
"border-color:\"transparent\""));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/PlayerControlImages/FileButton.png"), QSize(), QIcon::Normal, QIcon::Off);
        openFileButton->setIcon(icon);
        openFileButton->setFlat(true);

        gridLayout->addWidget(openFileButton, 0, 7, 1, 1);

        openUrlButton = new QPushButton(MediaControls);
        openUrlButton->setObjectName(QString::fromUtf8("openUrlButton"));
        openUrlButton->setStyleSheet(QString::fromUtf8("background-color:\"transparent\";\n"
"border-color:\"transparent\""));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/PlayerControlImages/UrlButton.png"), QSize(), QIcon::Normal, QIcon::Off);
        openUrlButton->setIcon(icon1);
        openUrlButton->setFlat(true);

        gridLayout->addWidget(openUrlButton, 0, 8, 1, 1);

        pauseButton = new QPushButton(MediaControls);
        pauseButton->setObjectName(QString::fromUtf8("pauseButton"));
        pauseButton->setStyleSheet(QString::fromUtf8("background-color:\"transparent\";\n"
"border-color:\"transparent\""));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/PlayerControlImages/PauseButton.png"), QSize(), QIcon::Normal, QIcon::Off);
        pauseButton->setIcon(icon2);
        pauseButton->setFlat(true);

        gridLayout->addWidget(pauseButton, 0, 4, 1, 1);

        reverseButton = new QPushButton(MediaControls);
        reverseButton->setObjectName(QString::fromUtf8("reverseButton"));
        reverseButton->setStyleSheet(QString::fromUtf8("background-color:\"transparent\";\n"
"border-color:\"transparent\""));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/PlayerControlImages/RateButtonReverse.png"), QSize(), QIcon::Normal, QIcon::Off);
        reverseButton->setIcon(icon3);
        reverseButton->setFlat(true);

        gridLayout->addWidget(reverseButton, 0, 5, 1, 1);

        forwardButton = new QPushButton(MediaControls);
        forwardButton->setObjectName(QString::fromUtf8("forwardButton"));
        forwardButton->setStyleSheet(QString::fromUtf8("background-color:\"transparent\";\n"
"border-color:\"transparent\""));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/PlayerControlImages/RateButtonForward.png"), QSize(), QIcon::Normal, QIcon::Off);
        forwardButton->setIcon(icon4);
        forwardButton->setFlat(true);

        gridLayout->addWidget(forwardButton, 0, 6, 1, 1);

        volumeSlider = new Phonon::VolumeSlider(MediaControls);
        volumeSlider->setObjectName(QString::fromUtf8("volumeSlider"));

        gridLayout->addWidget(volumeSlider, 0, 0, 1, 3);

        playButton = new QPushButton(MediaControls);
        playButton->setObjectName(QString::fromUtf8("playButton"));
        playButton->setStyleSheet(QString::fromUtf8("background-color:\"transparent\";\n"
"border-color:\"transparent\""));
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/PlayerControlImages/PlayButton.png"), QSize(), QIcon::Normal, QIcon::Off);
        playButton->setIcon(icon5);
        playButton->setFlat(true);

        gridLayout->addWidget(playButton, 0, 3, 1, 1);

        seekSlider = new Phonon::SeekSlider(MediaControls);
        seekSlider->setObjectName(QString::fromUtf8("seekSlider"));
        seekSlider->setMinimumSize(QSize(0, 0));

        gridLayout->addWidget(seekSlider, 1, 1, 1, 8);

        fullScreenButton = new QPushButton(MediaControls);
        fullScreenButton->setObjectName(QString::fromUtf8("fullScreenButton"));
        fullScreenButton->setStyleSheet(QString::fromUtf8("background-color:\"transparent\";\n"
"border-color:\"transparent\""));
        QIcon icon6;
        icon6.addFile(QString::fromUtf8(":/PlayerControlImages/FullscreenButton.png"), QSize(), QIcon::Normal, QIcon::Off);
        fullScreenButton->setIcon(icon6);
        fullScreenButton->setFlat(true);

        gridLayout->addWidget(fullScreenButton, 0, 9, 1, 2);

        totalTimeLabel = new QLabel(MediaControls);
        totalTimeLabel->setObjectName(QString::fromUtf8("totalTimeLabel"));
        QPalette palette1;
        palette1.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette1.setBrush(QPalette::Active, QPalette::Button, brush1);
        palette1.setBrush(QPalette::Active, QPalette::Base, brush1);
        palette1.setBrush(QPalette::Active, QPalette::Window, brush1);
        palette1.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette1.setBrush(QPalette::Inactive, QPalette::Button, brush1);
        palette1.setBrush(QPalette::Inactive, QPalette::Base, brush1);
        palette1.setBrush(QPalette::Inactive, QPalette::Window, brush1);
        palette1.setBrush(QPalette::Disabled, QPalette::WindowText, brush2);
        palette1.setBrush(QPalette::Disabled, QPalette::Button, brush1);
        palette1.setBrush(QPalette::Disabled, QPalette::Base, brush1);
        palette1.setBrush(QPalette::Disabled, QPalette::Window, brush1);
        totalTimeLabel->setPalette(palette1);

        gridLayout->addWidget(totalTimeLabel, 1, 9, 1, 2);

        gridLayout->setColumnMinimumWidth(0, 45);
        gridLayout->setColumnMinimumWidth(1, 50);
        gridLayout->setColumnMinimumWidth(2, 30);
        gridLayout->setColumnMinimumWidth(3, 30);
        gridLayout->setColumnMinimumWidth(4, 30);
        gridLayout->setColumnMinimumWidth(5, 30);
        gridLayout->setColumnMinimumWidth(6, 30);
        gridLayout->setColumnMinimumWidth(7, 30);
        gridLayout->setColumnMinimumWidth(8, 30);
        gridLayout->setColumnMinimumWidth(9, 20);
        gridLayout->setColumnMinimumWidth(10, 25);

        retranslateUi(MediaControls);

        QMetaObject::connectSlotsByName(MediaControls);
    } // setupUi

    void retranslateUi(QWidget *MediaControls)
    {
        currentTimeLabel->setText(QApplication::translate("MediaControls", "00:00:00", 0, QApplication::UnicodeUTF8));
        openFileButton->setText(QString());
        openUrlButton->setText(QString());
        pauseButton->setText(QString());
        reverseButton->setText(QString());
        forwardButton->setText(QString());
        playButton->setText(QString());
        fullScreenButton->setText(QString());
        totalTimeLabel->setText(QApplication::translate("MediaControls", "00:00:00", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(MediaControls);
    } // retranslateUi

};

namespace Ui {
    class MediaControls: public Ui_MediaControls {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MEDIACONTROLS_H
