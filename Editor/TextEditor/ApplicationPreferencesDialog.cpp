#include "ApplicationPreferencesDialog.h"
#include "ui_ApplicationPreferencesDialog.h"

#include "ApplicationPreferences.h"

#include <QPushButton>
#include <QDialogButtonBox>
#include <QColorDialog>

#include <assert.h>

ApplicationPreferencesDialog::ApplicationPreferencesDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ApplicationPreferencesDialog)
{
    ui->setupUi(this);
    ui->ui_functionNamesToolButton->hide();
    ui->ui_functionNamesLabel->hide();
    QPushButton const* okButton = ui->ui_buttonBox->button(QDialogButtonBox::Ok);
    QPushButton const* restoreButton = ui->ui_buttonBox->button(QDialogButtonBox::RestoreDefaults);
    connect(okButton, SIGNAL(clicked()), this, SLOT(okClick()));
    connect(restoreButton, SIGNAL(clicked()), this, SLOT(restoreDefaultsClick()));

    connect(ui->ui_unconnectedRailsToolButton, SIGNAL(clicked()), this, SLOT(colorToolButtonClicked()));
    connect(ui->ui_connectedRailsToolButton, SIGNAL(clicked()), this, SLOT(colorToolButtonClicked()));
    connect(ui->ui_functionNamesToolButton, SIGNAL(clicked()), this, SLOT(colorToolButtonClicked()));
    connect(ui->ui_functionCallsToolButton, SIGNAL(clicked()), this, SLOT(colorToolButtonClicked()));
    connect(ui->ui_systemFunctionToolButton, SIGNAL(clicked()), this, SLOT(colorToolButtonClicked()));
    connect(ui->ui_stringsToolButton, SIGNAL(clicked()), this, SLOT(colorToolButtonClicked()));
    connect(ui->ui_variablesToolButton, SIGNAL(clicked()), this, SLOT(colorToolButtonClicked()));
    connect(ui->ui_grabbedTextToolButton, SIGNAL(clicked()), this, SLOT(colorToolButtonClicked()));

    init();
}

ApplicationPreferencesDialog::~ApplicationPreferencesDialog()
{
    delete ui;
}

void ApplicationPreferencesDialog::init()
{
    // read from settings
    ui->ui_interpreterPathLineEdit->setFileInfo(QFileInfo(ApplicationPreferences::interpreterLocation));
    ui->ui_compilerPathLineEdit->setFileInfo(QFileInfo(ApplicationPreferences::compilerLocation));

    ui->ui_generateASGCheckBox->setChecked(ApplicationPreferences::createASGFiles);
    ui->ui_generateGraphVizCheckBox->setChecked(ApplicationPreferences::createGraphVizFiles);
    ui->ui_quietCompilerOutputCheckBox->setChecked(ApplicationPreferences::quietCompilerOutput);

    ui->ui_showLineNumbersCheckBox->setChecked(ApplicationPreferences::showLineNumbers);
    ui->ui_showWhiteSpacesCheckBox->setChecked(ApplicationPreferences::showWhiteSpaces);
    ui->ui_showEditorLinesCheckBox->setChecked(ApplicationPreferences::showEditorLines);

    QPixmap pixmap(20, 20);
    pixmap.fill(ApplicationPreferences::unconnectedRailsColor);
    ui->ui_unconnectedRailsToolButton->setIcon(pixmap);
    m_unconnectedRailsColor = ApplicationPreferences::unconnectedRailsColor;

    pixmap.fill(ApplicationPreferences::connectedRailsColor);
    ui->ui_connectedRailsToolButton->setIcon(pixmap);
    m_connectedRailsColor = ApplicationPreferences::connectedRailsColor;

    pixmap.fill(ApplicationPreferences::systemFunctionColor);
    ui->ui_systemFunctionToolButton->setIcon(pixmap);
    m_systemFunctionColor = ApplicationPreferences::systemFunctionColor;

    pixmap.fill(ApplicationPreferences::functionNamesColor);
    ui->ui_functionNamesToolButton->setIcon(pixmap);
    m_functionNamesColor = ApplicationPreferences::functionNamesColor;

    pixmap.fill(ApplicationPreferences::functionCallsColor);
    ui->ui_functionCallsToolButton->setIcon(pixmap);
    m_functionCallsColor = ApplicationPreferences::functionCallsColor;

    pixmap.fill(ApplicationPreferences::stringsColor);
    ui->ui_stringsToolButton->setIcon(pixmap);
    m_stringsColor = ApplicationPreferences::stringsColor;

    pixmap.fill(ApplicationPreferences::variablesColor);
    ui->ui_variablesToolButton->setIcon(pixmap);
    m_variablesColor = ApplicationPreferences::variablesColor;

    pixmap.fill(ApplicationPreferences::grabColor);
    ui->ui_grabbedTextToolButton->setIcon(pixmap);
    m_grabbedTextColor = ApplicationPreferences::grabColor;
}

void ApplicationPreferencesDialog::colorToolButtonClicked()
{
    QToolButton *toolButton = dynamic_cast< QToolButton * >(sender());
    assert(toolButton);
    QColorDialog colorDlg;
    int result = colorDlg.exec();
    QColor selectedColor;
    if(result == QDialog::Accepted)
    {
        selectedColor = colorDlg.selectedColor();
        QPixmap pixmap(20, 20);
        pixmap.fill(selectedColor);
        toolButton->setIcon(pixmap);
        if(toolButton == ui->ui_unconnectedRailsToolButton)
        {
            m_unconnectedRailsColor = selectedColor;
        }
        else if(toolButton == ui->ui_connectedRailsToolButton)
        {
            m_connectedRailsColor = selectedColor;
        }
        else if(toolButton == ui->ui_systemFunctionToolButton)
        {
            m_systemFunctionColor = selectedColor;
        }
        else if(toolButton == ui->ui_functionNamesToolButton)
        {
            m_functionNamesColor = selectedColor;
        }
        else if(toolButton == ui->ui_functionCallsToolButton)
        {
            m_functionCallsColor = selectedColor;
        }
        else if(toolButton == ui->ui_stringsToolButton)
        {
            m_stringsColor = selectedColor;
        }
        else if(toolButton == ui->ui_variablesToolButton)
        {
            m_variablesColor = selectedColor;
        }
        else if(toolButton == ui->ui_grabbedTextToolButton)
        {
            m_grabbedTextColor = selectedColor;
        }
    }
}

void ApplicationPreferencesDialog::okClick()
{
    ApplicationPreferences::interpreterLocation = ui->ui_interpreterPathLineEdit->text();
    ApplicationPreferences::compilerLocation = ui->ui_compilerPathLineEdit->text();
    ApplicationPreferences::createASGFiles = ui->ui_generateASGCheckBox->isChecked();
    ApplicationPreferences::createGraphVizFiles = ui->ui_generateGraphVizCheckBox->isChecked();
    ApplicationPreferences::quietCompilerOutput = ui->ui_quietCompilerOutputCheckBox->isChecked();

    ApplicationPreferences::showLineNumbers = ui->ui_showLineNumbersCheckBox->isChecked();
    ApplicationPreferences::showWhiteSpaces = ui->ui_showWhiteSpacesCheckBox->isChecked();
    ApplicationPreferences::showEditorLines = ui->ui_showEditorLinesCheckBox->isChecked();
    ApplicationPreferences::unconnectedRailsColor = m_unconnectedRailsColor;
    ApplicationPreferences::connectedRailsColor = m_connectedRailsColor;
    ApplicationPreferences::systemFunctionColor = m_systemFunctionColor;
    ApplicationPreferences::functionNamesColor = m_functionNamesColor;
    ApplicationPreferences::functionCallsColor = m_functionCallsColor;
    ApplicationPreferences::stringsColor = m_stringsColor;
    ApplicationPreferences::variablesColor = m_variablesColor;
    ApplicationPreferences::grabColor = m_grabbedTextColor;
    this->accept();
}

void ApplicationPreferencesDialog::restoreDefaultsClick()
{
    int currentIndex = ui->ui_tabWidget->currentIndex();
    if(currentIndex == ui->ui_tabWidget->indexOf(ui->ui_pathsTab))
    {
        ApplicationPreferences::interpreterLocation = ApplicationDefaultValues::interpreterLocation;
        ApplicationPreferences::compilerLocation = ApplicationDefaultValues::compilerLocation;
        ApplicationPreferences::createASGFiles = ApplicationDefaultValues::createASGFiles;
        ApplicationPreferences::createGraphVizFiles = ApplicationDefaultValues::createGraphVizFiles;
        ApplicationPreferences::quietCompilerOutput = ApplicationDefaultValues::quietCompilerOutput;
    }
    else if(currentIndex == ui->ui_tabWidget->indexOf(ui->ui_editorTab))
    {
        ApplicationPreferences::showLineNumbers = ApplicationDefaultValues::showLineNumbers;
        ApplicationPreferences::showWhiteSpaces = ApplicationDefaultValues::showWhiteSpaces;
        ApplicationPreferences::showEditorLines = ApplicationDefaultValues::showEditorLines;
    }
    else if(currentIndex == ui->ui_tabWidget->indexOf(ui->ui_colorsTab))
    {
        ApplicationPreferences::unconnectedRailsColor = ApplicationDefaultValues::unconnectedRailsColor;
        ApplicationPreferences::connectedRailsColor = ApplicationDefaultValues::connectedRailsColor;
        ApplicationPreferences::systemFunctionColor = ApplicationDefaultValues::systemFunctionColor;
        ApplicationPreferences::functionNamesColor = ApplicationDefaultValues::functionNamesColor;
        ApplicationPreferences::functionCallsColor = ApplicationDefaultValues::functionCallsColor;
        ApplicationPreferences::stringsColor = ApplicationDefaultValues::stringsColor;
        ApplicationPreferences::variablesColor = ApplicationDefaultValues::variablesColor;
        ApplicationPreferences::grabColor = ApplicationDefaultValues::grabColor;
    }

    init();
}
