#ifndef ENCDECDIALOG_H
#define ENCDECDIALOG_H

#include <QDialog>
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>
#include <QDebug>

namespace Ui {
class EncDecDialog;
}

class EncDecDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EncDecDialog(QWidget *parent = nullptr);
    ~EncDecDialog();

    int algorithm = 0;
    QString keyFileContent;
    const char *keyFileContent2;

    bool ok = false;

private slots:
    void on_comboBox_Algorithm_currentIndexChanged(int index);

    void on_pushButton_KeyFile_clicked();

private:
    Ui::EncDecDialog *ui;

    QString keyFileName;
};

#endif // ENCDECDIALOG_H
