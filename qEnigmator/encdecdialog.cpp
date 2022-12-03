#include "encdecdialog.h"
#include "ui_encdecdialog.h"

EncDecDialog::EncDecDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EncDecDialog)
{
    ui->setupUi(this);
}

EncDecDialog::~EncDecDialog()
{
    delete ui;
}

void EncDecDialog::on_comboBox_Algorithm_currentIndexChanged(int index)
{
    algorithm = index;
}


void EncDecDialog::on_pushButton_KeyFile_clicked()
{
    QString currentFile = QFileDialog::getOpenFileName(this,"Open File","./keys","Key Files (*.aes *.pem)");

    if(currentFile.isEmpty()) return;

    keyFileName = currentFile.split("/").last();

    QFile file(currentFile);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) return;

    QTextStream in(&file);
    QString line = in.readLine();

    while(!line.isNull())
    {
        keyFileContent.append(line);
        line = in.readLine();

        if(!line.isNull()) keyFileContent.append("\n");
    }

    file.close();

    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) return;

    keyFileContent2 = file.readAll();

    file.close();

    switch (algorithm) {
    case 0: //aes
    {
        if(!keyFileName.endsWith(".aes") || (keyFileContent2[0] == '\0' && keyFileContent == "")){
            QMessageBox::critical(this,"Greška","Neispravan AES-256 ključ!");
            ok = false;
        }
        else{
            ui->labelSelectedFile->setText(keyFileName);
            ok = true;
        }

        break;
    }
    case 1:
    {
        if(!keyFileName.endsWith(".pem") || (keyFileContent2[0] == '\0' && keyFileContent == "")){
            QMessageBox::critical(this,"Greška","Neispravan RSA-1024 ključ!");
            ok = false;
        }
        else{
            ui->labelSelectedFile->setText(keyFileName);
            ok = true;
        }

        break;
    }
    default:
        break;
    }

    qDebug() << "Key file content in encdec: " << keyFileContent;
    qDebug() << "Key file content 2 in encdec: " << keyFileContent2;
}

