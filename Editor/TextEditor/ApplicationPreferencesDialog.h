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

    bool colorChanged() const;

private:
    void init();

    Ui::ApplicationPreferencesDialog *ui;

    QColor m_unconnectedRailsColor;
    QColor m_connectedRailsColor;
    QColor m_functionNamesColor;
    QColor m_functionCallsColor;
    QColor m_stringsColor;
    QColor m_variablesColor;

    bool m_colorChanged;

private slots:
    void colorToolButtonClicked();
    void restoreDefaultsClick();
    void okClick();
};

#endif // APPLICATIONPREFERENCESDIALOG_H
