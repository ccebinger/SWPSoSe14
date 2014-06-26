#include "ApplicationPreferencesDialog.h"
#include "ui_ApplicationPreferencesDialog.h"

ApplicationPreferencesDialog::ApplicationPreferencesDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ApplicationPreferencesDialog)
{
    ui->setupUi(this);
}

ApplicationPreferencesDialog::~ApplicationPreferencesDialog()
{
    delete ui;
}
