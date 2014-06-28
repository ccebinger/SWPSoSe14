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
    QPushButton const* okButton = ui->ui_buttonBox->button(QDialogButtonBox::Ok);
    QPushButton const* restoreButton = ui->ui_buttonBox->button(QDialogButtonBox::RestoreDefaults);
    connect(okButton, SIGNAL(clicked()), this, SLOT(okClick()));
    connect(restoreButton, SIGNAL(clicked()), this, SLOT(restoreDefaultsClick()));

    connect(ui->ui_unconnectedRailsToolButton, SIGNAL(clicked()), this, SLOT(colorToolButtonClicked()));
    connect(ui->ui_connectedRailsToolButton, SIGNAL(clicked()), this, SLOT(colorToolButtonClicked()));
    connect(ui->ui_functionNamesToolButton, SIGNAL(clicked()), this, SLOT(colorToolButtonClicked()));
    connect(ui->ui_functionCallsToolButton, SIGNAL(clicked()), this, SLOT(colorToolButtonClicked()));
    connect(ui->ui_stringsToolButton, SIGNAL(clicked()), this, SLOT(colorToolButtonClicked()));
    connect(ui->ui_variablesToolButton, SIGNAL(clicked()), this, SLOT(colorToolButtonClicked()));

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

    QPixmap pixmap(20, 20);
    pixmap.fill(ApplicationPreferences::unconnectedRailsColor);
    ui->ui_unconnectedRailsToolButton->setIcon(pixmap);

    pixmap.fill(ApplicationPreferences::connectedRailsColor);
    ui->ui_connectedRailsToolButton->setIcon(pixmap);

    pixmap.fill(ApplicationPreferences::functionNamesColor);
    ui->ui_functionNamesToolButton->setIcon(pixmap);

    pixmap.fill(ApplicationPreferences::functionCallsColor);
    ui->ui_functionCallsToolButton->setIcon(pixmap);

    pixmap.fill(ApplicationPreferences::stringsColor);
    ui->ui_stringsToolButton->setIcon(pixmap);

    pixmap.fill(ApplicationPreferences::variablesColor);
    ui->ui_variablesToolButton->setIcon(pixmap);
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
    }
}

void ApplicationPreferencesDialog::okClick()
{
    ApplicationPreferences::interpreterLocation = ui->ui_interpreterPathLineEdit->text();
    ApplicationPreferences::compilerLocation = ui->ui_compilerPathLineEdit->text();
    ApplicationPreferences::createASGFiles = ui->ui_generateASGCheckBox->isChecked();
    ApplicationPreferences::createGraphVizFiles = ui->ui_generateGraphVizCheckBox->isChecked();
    ApplicationPreferences::quietCompilerOutput = ui->ui_quietCompilerOutputCheckBox->isChecked();
    ApplicationPreferences::unconnectedRailsColor = m_unconnectedRailsColor;
    ApplicationPreferences::connectedRailsColor = m_connectedRailsColor;
    ApplicationPreferences::functionNamesColor = m_functionNamesColor;
    ApplicationPreferences::functionCallsColor = m_functionCallsColor;
    ApplicationPreferences::stringsColor = m_stringsColor;
    ApplicationPreferences::variablesColor = m_variablesColor;
    this->accept();
}

void ApplicationPreferencesDialog::restoreDefaultsClick()
{
    ApplicationPreferences::interpreterLocation = ApplicationDefaultValues::interpreterLocation;
    ApplicationPreferences::compilerLocation = ApplicationDefaultValues::compilerLocation;
    ApplicationPreferences::createASGFiles = ApplicationDefaultValues::createASGFiles;
    ApplicationPreferences::createGraphVizFiles = ApplicationDefaultValues::createGraphVizFiles;
    ApplicationPreferences::quietCompilerOutput = ApplicationDefaultValues::quietCompilerOutput;
    ApplicationPreferences::unconnectedRailsColor = ApplicationDefaultValues::unconnectedRailsColor;
    ApplicationPreferences::connectedRailsColor = ApplicationDefaultValues::connectedRailsColor;
    ApplicationPreferences::functionNamesColor = ApplicationDefaultValues::functionNamesColor;
    ApplicationPreferences::functionCallsColor = ApplicationDefaultValues::functionCallsColor;
    ApplicationPreferences::stringsColor = ApplicationDefaultValues::stringsColor;
    ApplicationPreferences::variablesColor = ApplicationDefaultValues::variablesColor;

    init();
}
