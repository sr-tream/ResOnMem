#ifndef USERINTERFACE_H
#define USERINTERFACE_H

#include "ui_userinterface.h"
#include <QSettings>
#include <QFileDialog>
#include "codegenerator.h"

class UserInterface : public QWidget, private Ui::UserInterface
{
    Q_OBJECT

public:
    explicit UserInterface(QWidget *parent = 0);

protected:
    void closeEvent(QCloseEvent *e) override;

private slots:
    void on_inc_gNS_toggled(bool checked);

    void on_selectFile_clicked();

    void on_resName_textChanged(const QString &arg1);

    void on_g_resName_textChanged(const QString &arg1);

    void on_generate_clicked();

private:
    QSettings *settings;
    CodeGenerator *cgen = nullptr;

    void removePunct(QString &str);
};

#endif // USERINTERFACE_H
