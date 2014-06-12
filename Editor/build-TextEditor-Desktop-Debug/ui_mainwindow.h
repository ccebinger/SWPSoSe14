/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.0.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <EditTableWidget.h>
#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QRadioButton>
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
    QAction *actionUndo;
    QAction *actionRedo;
    QWidget *ui_centralwidget;
    QHBoxLayout *horizontalLayout;
    QGroupBox *ui_sourceGroupBox;
    QVBoxLayout *verticalLayout;
    EditTableWidget *ui_sourceEditTableWidget;
    QGridLayout *gridLayout;
    QLabel *ui_rowValueLabel;
    QLabel *ui_rowLabel;
    QLabel *ui_columnValueLabel;
    QGroupBox *ui_insertModeGroupBox;
    QHBoxLayout *horizontalLayout_3;
    QRadioButton *ui_overRadioButton;
    QRadioButton *ui_insertRadioButton;
    QRadioButton *ui_smartInserRadioButton;
    QSpacerItem *ui_sourceSpacer;
    QLabel *ui_columnLabel;
    QGroupBox *ui_writeDirectionGroupBox;
    QHBoxLayout *horizontalLayout_4;
    QRadioButton *ui_horizontalDirectionRadioButton;
    QRadioButton *ui_verticalDirectionRadioButton;
    QRadioButton *ui_smartDirectionRadioButton;
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
        actionUndo = new QAction(MainWindow);
        actionUndo->setObjectName(QStringLiteral("actionUndo"));
        actionRedo = new QAction(MainWindow);
        actionRedo->setObjectName(QStringLiteral("actionRedo"));
        ui_centralwidget = new QWidget(MainWindow);
        ui_centralwidget->setObjectName(QStringLiteral("ui_centralwidget"));
        horizontalLayout = new QHBoxLayout(ui_centralwidget);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        ui_sourceGroupBox = new QGroupBox(ui_centralwidget);
        ui_sourceGroupBox->setObjectName(QStringLiteral("ui_sourceGroupBox"));
        ui_sourceGroupBox->setFlat(false);
        verticalLayout = new QVBoxLayout(ui_sourceGroupBox);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        ui_sourceEditTableWidget = new EditTableWidget(ui_sourceGroupBox);
        ui_sourceEditTableWidget->setObjectName(QStringLiteral("ui_sourceEditTableWidget"));
        ui_sourceEditTableWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        ui_sourceEditTableWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        ui_sourceEditTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
        ui_sourceEditTableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
        ui_sourceEditTableWidget->setTextElideMode(Qt::ElideMiddle);
        ui_sourceEditTableWidget->setShowGrid(true);
        ui_sourceEditTableWidget->setGridStyle(Qt::DotLine);
        ui_sourceEditTableWidget->setWordWrap(false);
        ui_sourceEditTableWidget->setCornerButtonEnabled(false);
        ui_sourceEditTableWidget->horizontalHeader()->setVisible(false);
        ui_sourceEditTableWidget->verticalHeader()->setVisible(false);

        verticalLayout->addWidget(ui_sourceEditTableWidget);

        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        ui_rowValueLabel = new QLabel(ui_sourceGroupBox);
        ui_rowValueLabel->setObjectName(QStringLiteral("ui_rowValueLabel"));
        ui_rowValueLabel->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);

        gridLayout->addWidget(ui_rowValueLabel, 0, 3, 1, 1);

        ui_rowLabel = new QLabel(ui_sourceGroupBox);
        ui_rowLabel->setObjectName(QStringLiteral("ui_rowLabel"));
        ui_rowLabel->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);

        gridLayout->addWidget(ui_rowLabel, 0, 2, 1, 1);

        ui_columnValueLabel = new QLabel(ui_sourceGroupBox);
        ui_columnValueLabel->setObjectName(QStringLiteral("ui_columnValueLabel"));
        ui_columnValueLabel->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);

        gridLayout->addWidget(ui_columnValueLabel, 0, 5, 1, 1);

        ui_insertModeGroupBox = new QGroupBox(ui_sourceGroupBox);
        ui_insertModeGroupBox->setObjectName(QStringLiteral("ui_insertModeGroupBox"));
        horizontalLayout_3 = new QHBoxLayout(ui_insertModeGroupBox);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(0, 0, 0, 0);
        ui_overRadioButton = new QRadioButton(ui_insertModeGroupBox);
        ui_overRadioButton->setObjectName(QStringLiteral("ui_overRadioButton"));
        ui_overRadioButton->setChecked(true);

        horizontalLayout_3->addWidget(ui_overRadioButton);

        ui_insertRadioButton = new QRadioButton(ui_insertModeGroupBox);
        ui_insertRadioButton->setObjectName(QStringLiteral("ui_insertRadioButton"));
        ui_insertRadioButton->setEnabled(true);

        horizontalLayout_3->addWidget(ui_insertRadioButton);

        ui_smartInserRadioButton = new QRadioButton(ui_insertModeGroupBox);
        ui_smartInserRadioButton->setObjectName(QStringLiteral("ui_smartInserRadioButton"));
        ui_smartInserRadioButton->setEnabled(false);
        ui_smartInserRadioButton->setCheckable(true);

        horizontalLayout_3->addWidget(ui_smartInserRadioButton);


        gridLayout->addWidget(ui_insertModeGroupBox, 0, 0, 1, 1);

        ui_sourceSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(ui_sourceSpacer, 0, 1, 1, 1);

        ui_columnLabel = new QLabel(ui_sourceGroupBox);
        ui_columnLabel->setObjectName(QStringLiteral("ui_columnLabel"));
        ui_columnLabel->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);

        gridLayout->addWidget(ui_columnLabel, 0, 4, 1, 1);

        ui_writeDirectionGroupBox = new QGroupBox(ui_sourceGroupBox);
        ui_writeDirectionGroupBox->setObjectName(QStringLiteral("ui_writeDirectionGroupBox"));
        horizontalLayout_4 = new QHBoxLayout(ui_writeDirectionGroupBox);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        horizontalLayout_4->setContentsMargins(0, 0, 0, 0);
        ui_horizontalDirectionRadioButton = new QRadioButton(ui_writeDirectionGroupBox);
        ui_horizontalDirectionRadioButton->setObjectName(QStringLiteral("ui_horizontalDirectionRadioButton"));
        ui_horizontalDirectionRadioButton->setChecked(true);

        horizontalLayout_4->addWidget(ui_horizontalDirectionRadioButton);

        ui_verticalDirectionRadioButton = new QRadioButton(ui_writeDirectionGroupBox);
        ui_verticalDirectionRadioButton->setObjectName(QStringLiteral("ui_verticalDirectionRadioButton"));
        ui_verticalDirectionRadioButton->setEnabled(false);
        ui_verticalDirectionRadioButton->setCheckable(true);

        horizontalLayout_4->addWidget(ui_verticalDirectionRadioButton);

        ui_smartDirectionRadioButton = new QRadioButton(ui_writeDirectionGroupBox);
        ui_smartDirectionRadioButton->setObjectName(QStringLiteral("ui_smartDirectionRadioButton"));
        ui_smartDirectionRadioButton->setEnabled(false);
        ui_smartDirectionRadioButton->setCheckable(true);

        horizontalLayout_4->addWidget(ui_smartDirectionRadioButton);


        gridLayout->addWidget(ui_writeDirectionGroupBox, 1, 0, 1, 1);


        verticalLayout->addLayout(gridLayout);


        horizontalLayout->addWidget(ui_sourceGroupBox);

        ui_ioGroupBox = new QGroupBox(ui_centralwidget);
        ui_ioGroupBox->setObjectName(QStringLiteral("ui_ioGroupBox"));
        ui_ioGroupBox->setFlat(false);
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
        ui_editMenu->addAction(actionUndo);
        ui_editMenu->addAction(actionRedo);
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
        actionUndo->setText(QApplication::translate("MainWindow", "Undo", 0));
        actionRedo->setText(QApplication::translate("MainWindow", "Redo", 0));
        ui_sourceGroupBox->setTitle(QApplication::translate("MainWindow", "Source", 0));
        ui_rowValueLabel->setText(QApplication::translate("MainWindow", "1", 0));
        ui_rowLabel->setText(QApplication::translate("MainWindow", "Row", 0));
        ui_columnValueLabel->setText(QApplication::translate("MainWindow", "1", 0));
        ui_insertModeGroupBox->setTitle(QApplication::translate("MainWindow", "Insert Mode", 0));
        ui_overRadioButton->setText(QApplication::translate("MainWindow", "OVER", 0));
        ui_insertRadioButton->setText(QApplication::translate("MainWindow", "INSERT", 0));
        ui_smartInserRadioButton->setText(QApplication::translate("MainWindow", "SMART", 0));
        ui_columnLabel->setText(QApplication::translate("MainWindow", "Column", 0));
        ui_writeDirectionGroupBox->setTitle(QApplication::translate("MainWindow", "Write Direction", 0));
        ui_horizontalDirectionRadioButton->setText(QApplication::translate("MainWindow", "Horizontal", 0));
        ui_verticalDirectionRadioButton->setText(QApplication::translate("MainWindow", "Vertical", 0));
        ui_smartDirectionRadioButton->setText(QApplication::translate("MainWindow", "Smart", 0));
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
