#ifndef APPLICATIONPREFERENCESDIALOG_H
#define APPLICATIONPREFERENCESDIALOG_H

#include <QDialog>

namespace Ui {
class ApplicationPreferencesDialog;
}

class ApplicationPreferencesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ApplicationPreferencesDialog(QWidget *parent = 0);
    ~ApplicationPreferencesDialog();

private:
    Ui::ApplicationPreferencesDialog *ui;
};

#endif // APPLICATIONPREFERENCESDIALOG_H
