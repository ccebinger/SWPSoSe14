/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.2.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *ui_newFileAction;
    QAction *ui_openFileAction;
    QAction *ui_saveFileAction;
    QAction *ui_quitAction;
    QAction *ui_saveAsAction;
    QAction *ui_setInterpreterAction;
    QAction *ui_runInterpreterAction;
    QAction *ui_setInputFileAction;
    QAction *ui_setOutputFileAction;
    QWidget *ui_centralwidget;
    QHBoxLayout *horizontalLayout;
    QGroupBox *ui_sourceGroupBox;
    QVBoxLayout *verticalLayout;
    QPlainTextEdit *ui_sourcePlainTextEdit;
    QHBoxLayout *ui_ioHorizontalLayout;
    QSpacerItem *ui_sourceSpacer;
    QLabel *ui_rowLabel;
    QLabel *ui_rowValueLabel;
    QLabel *ui_columnLabel;
    QLabel *ui_columnValueLabel;
    QGroupBox *ui_ioGroupBox;
    QVBoxLayout *verticalLayout_2;
    QLabel *ui_inputLabel;
    QPlainTextEdit *ui_inputPlainTextEdit;
    QLabel *ui_outpuLabel;
    QPlainTextEdit *ui_outputPlainTextEdit;
    QMenuBar *ui_menubar;
    QMenu *ui_menuMenu;
    QMenu *ui_editMenu;
    QStatusBar *ui_statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(832, 545);
        ui_newFileAction = new QAction(MainWindow);
        ui_newFileAction->setObjectName(QStringLiteral("ui_newFileAction"));
        ui_newFileAction->setShortcutContext(Qt::ApplicationShortcut);
        ui_openFileAction = new QAction(MainWindow);
        ui_openFileAction->setObjectName(QStringLiteral("ui_openFileAction"));
        ui_openFileAction->setShortcutContext(Qt::ApplicationShortcut);
        ui_saveFileAction = new QAction(MainWindow);
        ui_saveFileAction->setObjectName(QStringLiteral("ui_saveFileAction"));
        ui_saveFileAction->setShortcutContext(Qt::ApplicationShortcut);
        ui_quitAction = new QAction(MainWindow);
        ui_quitAction->setObjectName(QStringLiteral("ui_quitAction"));
        ui_quitAction->setShortcutContext(Qt::ApplicationShortcut);
        ui_saveAsAction = new QAction(MainWindow);
        ui_saveAsAction->setObjectName(QStringLiteral("ui_saveAsAction"));
        ui_saveAsAction->setShortcutContext(Qt::ApplicationShortcut);
        ui_setInterpreterAction = new QAction(MainWindow);
        ui_setInterpreterAction->setObjectName(QStringLiteral("ui_setInterpreterAction"));
        ui_runInterpreterAction = new QAction(MainWindow);
        ui_runInterpreterAction->setObjectName(QStringLiteral("ui_runInterpreterAction"));
        ui_setInputFileAction = new QAction(MainWindow);
        ui_setInputFileAction->setObjectName(QStringLiteral("ui_setInputFileAction"));
        ui_setOutputFileAction = new QAction(MainWindow);
        ui_setOutputFileAction->setObjectName(QStringLiteral("ui_setOutputFileAction"));
        ui_centralwidget = new QWidget(MainWindow);
        ui_centralwidget->setObjectName(QStringLiteral("ui_centralwidget"));
        horizontalLayout = new QHBoxLayout(ui_centralwidget);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        ui_sourceGroupBox = new QGroupBox(ui_centralwidget);
        ui_sourceGroupBox->setObjectName(QStringLiteral("ui_sourceGroupBox"));
        verticalLayout = new QVBoxLayout(ui_sourceGroupBox);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        ui_sourcePlainTextEdit = new QPlainTextEdit(ui_sourceGroupBox);
        ui_sourcePlainTextEdit->setObjectName(QStringLiteral("ui_sourcePlainTextEdit"));
        ui_sourcePlainTextEdit->setLineWrapMode(QPlainTextEdit::NoWrap);

        verticalLayout->addWidget(ui_sourcePlainTextEdit);

        ui_ioHorizontalLayout = new QHBoxLayout();
        ui_ioHorizontalLayout->setObjectName(QStringLiteral("ui_ioHorizontalLayout"));
        ui_sourceSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        ui_ioHorizontalLayout->addItem(ui_sourceSpacer);

        ui_rowLabel = new QLabel(ui_sourceGroupBox);
        ui_rowLabel->setObjectName(QStringLiteral("ui_rowLabel"));

        ui_ioHorizontalLayout->addWidget(ui_rowLabel);

        ui_rowValueLabel = new QLabel(ui_sourceGroupBox);
        ui_rowValueLabel->setObjectName(QStringLiteral("ui_rowValueLabel"));

        ui_ioHorizontalLayout->addWidget(ui_rowValueLabel);

        ui_columnLabel = new QLabel(ui_sourceGroupBox);
        ui_columnLabel->setObjectName(QStringLiteral("ui_columnLabel"));

        ui_ioHorizontalLayout->addWidget(ui_columnLabel);

        ui_columnValueLabel = new QLabel(ui_sourceGroupBox);
        ui_columnValueLabel->setObjectName(QStringLiteral("ui_columnValueLabel"));

        ui_ioHorizontalLayout->addWidget(ui_columnValueLabel);


        verticalLayout->addLayout(ui_ioHorizontalLayout);


        horizontalLayout->addWidget(ui_sourceGroupBox);

        ui_ioGroupBox = new QGroupBox(ui_centralwidget);
        ui_ioGroupBox->setObjectName(QStringLiteral("ui_ioGroupBox"));
        verticalLayout_2 = new QVBoxLayout(ui_ioGroupBox);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        ui_inputLabel = new QLabel(ui_ioGroupBox);
        ui_inputLabel->setObjectName(QStringLiteral("ui_inputLabel"));

        verticalLayout_2->addWidget(ui_inputLabel);

        ui_inputPlainTextEdit = new QPlainTextEdit(ui_ioGroupBox);
        ui_inputPlainTextEdit->setObjectName(QStringLiteral("ui_inputPlainTextEdit"));
        ui_inputPlainTextEdit->setLineWrapMode(QPlainTextEdit::NoWrap);

        verticalLayout_2->addWidget(ui_inputPlainTextEdit);

        ui_outpuLabel = new QLabel(ui_ioGroupBox);
        ui_outpuLabel->setObjectName(QStringLiteral("ui_outpuLabel"));

        verticalLayout_2->addWidget(ui_outpuLabel);

        ui_outputPlainTextEdit = new QPlainTextEdit(ui_ioGroupBox);
        ui_outputPlainTextEdit->setObjectName(QStringLiteral("ui_outputPlainTextEdit"));
        ui_outputPlainTextEdit->setLineWrapMode(QPlainTextEdit::NoWrap);
        ui_outputPlainTextEdit->setReadOnly(true);

        verticalLayout_2->addWidget(ui_outputPlainTextEdit);


        horizontalLayout->addWidget(ui_ioGroupBox);

        horizontalLayout->setStretch(0, 2);
        horizontalLayout->setStretch(1, 1);
        MainWindow->setCentralWidget(ui_centralwidget);
        ui_menubar = new QMenuBar(MainWindow);
        ui_menubar->setObjectName(QStringLiteral("ui_menubar"));
        ui_menubar->setGeometry(QRect(0, 0, 832, 25));
        ui_menuMenu = new QMenu(ui_menubar);
        ui_menuMenu->setObjectName(QStringLiteral("ui_menuMenu"));
        ui_editMenu = new QMenu(ui_menubar);
        ui_editMenu->setObjectName(QStringLiteral("ui_editMenu"));
        MainWindow->setMenuBar(ui_menubar);
        ui_statusbar = new QStatusBar(MainWindow);
        ui_statusbar->setObjectName(QStringLiteral("ui_statusbar"));
        MainWindow->setStatusBar(ui_statusbar);

        ui_menubar->addAction(ui_menuMenu->menuAction());
        ui_menubar->addAction(ui_editMenu->menuAction());
        ui_menuMenu->addAction(ui_newFileAction);
        ui_menuMenu->addAction(ui_openFileAction);
        ui_menuMenu->addAction(ui_saveFileAction);
        ui_menuMenu->addAction(ui_saveAsAction);
        ui_menuMenu->addSeparator();
        ui_menuMenu->addAction(ui_quitAction);
        ui_editMenu->addAction(ui_setInterpreterAction);
        ui_editMenu->addAction(ui_runInterpreterAction);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Rail Editor", 0));
        ui_newFileAction->setText(QApplication::translate("MainWindow", "&New File", 0));
        ui_newFileAction->setShortcut(QApplication::translate("MainWindow", "Ctrl+N", 0));
        ui_openFileAction->setText(QApplication::translate("MainWindow", "&Open File", 0));
        ui_openFileAction->setShortcut(QApplication::translate("MainWindow", "Ctrl+O", 0));
        ui_saveFileAction->setText(QApplication::translate("MainWindow", "&Save File", 0));
        ui_saveFileAction->setShortcut(QApplication::translate("MainWindow", "Ctrl+S", 0));
        ui_quitAction->setText(QApplication::translate("MainWindow", "&Quit", 0));
        ui_quitAction->setShortcut(QApplication::translate("MainWindow", "Ctrl+Q, Alt+X", 0));
        ui_saveAsAction->setText(QApplication::translate("MainWindow", "Save &As", 0));
        ui_saveAsAction->setShortcut(QApplication::translate("MainWindow", "Ctrl+Shift+S", 0));
        ui_setInterpreterAction->setText(QApplication::translate("MainWindow", "Set I&nterpter Location", 0));
        ui_runInterpreterAction->setText(QApplication::translate("MainWindow", "&Run Interpreter", 0));
        ui_setInputFileAction->setText(QApplication::translate("MainWindow", "Set &Input File", 0));
        ui_setOutputFileAction->setText(QApplication::translate("MainWindow", "Set &Output File", 0));
        ui_sourceGroupBox->setTitle(QApplication::translate("MainWindow", "Source", 0));
        ui_rowLabel->setText(QApplication::translate("MainWindow", "Row", 0));
        ui_rowValueLabel->setText(QApplication::translate("MainWindow", "1", 0));
        ui_columnLabel->setText(QApplication::translate("MainWindow", "Column", 0));
        ui_columnValueLabel->setText(QApplication::translate("MainWindow", "1", 0));
        ui_ioGroupBox->setTitle(QApplication::translate("MainWindow", "I/O", 0));
        ui_inputLabel->setText(QApplication::translate("MainWindow", "Input", 0));
        ui_outpuLabel->setText(QApplication::translate("MainWindow", "Output", 0));
        ui_menuMenu->setTitle(QApplication::translate("MainWindow", "&Menu", 0));
        ui_editMenu->setTitle(QApplication::translate("MainWindow", "&Edit", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
