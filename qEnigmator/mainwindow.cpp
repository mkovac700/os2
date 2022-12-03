#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "encdecdialog.h"
#include "aboutdialog.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->move(QGuiApplication::screens().at(0)->geometry().center() - this->frameGeometry().center());

    ui->statusbar->setSizeGripEnabled(false);
    ui->tabWidgetMain->setCurrentIndex(0);

    ui->label_3_CompareResult->setTextFormat(Qt::RichText);

    CheckDocsDir();
    CheckKeysDir();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_lineEdit_3_CalculatedHash_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);

    if(ui->lineEdit_3_CalculatedHash->text().isEmpty() || ui->lineEdit_3_LoadedHash->text().isEmpty()){
        ui->label_3_CompareResult->setText("");
    }
    else if(ui->lineEdit_3_CalculatedHash->text() == ui->lineEdit_3_LoadedHash->text()){
        ui->label_3_CompareResult->setText("<img style='vertical-align:middle' src='://icons/icons8-ok-48.png' width='16' height='16'> Hashevi se podudaraju.");
    }
    else{
        ui->label_3_CompareResult->setText("<img style='vertical-align:middle' src='://icons/icons8-cancel-48.png' width='16' height='16'> Hashevi se ne podudaraju.");
    }
}


void MainWindow::on_lineEdit_3_LoadedHash_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);

    if(ui->lineEdit_3_CalculatedHash->text().isEmpty() || ui->lineEdit_3_LoadedHash->text().isEmpty()){
        ui->label_3_CompareResult->setText("");
    }
    else if(ui->lineEdit_3_CalculatedHash->text() == ui->lineEdit_3_LoadedHash->text()){
        ui->label_3_CompareResult->setText("<img style='vertical-align:middle' src='://icons/icons8-ok-48.png' width='16' height='16'> Hashevi se podudaraju.");
    }
    else{
        ui->label_3_CompareResult->setText("<img style='vertical-align:middle' src='://icons/icons8-cancel-48.png' width='16' height='16'> Hashevi se ne podudaraju.");
    }
}


void MainWindow::on_plainTextEdit_3_textChanged()
{
    if(!ui->plainTextEdit_3->toPlainText().isEmpty()){
        ui->lineEdit_3_CalculatedHash->setText(QString(QCryptographicHash::hash(ui->plainTextEdit_3->toPlainText().toUtf8(),QCryptographicHash::Sha256).toHex()));
    }
    else{
        ui->lineEdit_3_CalculatedHash->setText("");
    }
}


void MainWindow::on_toolButton_3_LoadPlainText_clicked()
{
    CheckDocsDir();

    QString fileContent;
    QString filename = QFileDialog::getOpenFileName(this,"Open File","./docs","Text (*.txt)");

    if(filename.isEmpty()) return;

    QFile file(filename);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) return;

    QTextStream in(&file);
    QString line = in.readLine();

    while(!line.isNull())
    {
        fileContent.append(line);
        line = in.readLine();

        if(!line.isNull()) fileContent.append("\n");
    }

    file.close();

    ui->plainTextEdit_3->clear();
    ui->plainTextEdit_3->setPlainText(fileContent);
}


void MainWindow::on_toolButton_3_SavePlainText_clicked()
{
    CheckDocsDir();

    QString filename = QFileDialog::getSaveFileName(this,"Save As","./docs","Text (*.txt)");
    if( filename.isEmpty())
        return;

    if(!filename.endsWith(".txt")) filename += ".txt";

    QFile file(filename);

    if( !file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream out(&file);
    out << ui->plainTextEdit_3->toPlainText() << "\n";

    file.close();
}


void MainWindow::on_toolButton_3_SaveHash_clicked()
{
    CheckDocsDir();

    QString filename = QFileDialog::getSaveFileName(this,"Save As","./docs","Hash (*.hash)");
    if( filename.isEmpty())
        return;

    if(!filename.endsWith(".hash")) filename += ".hash";

    QFile file(filename);

    if( !file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream out(&file);
    out << ui->lineEdit_3_CalculatedHash->text();

    file.close();
}


void MainWindow::on_toolButton_3_LoadHash_clicked()
{
    CheckDocsDir();

    QString fileContent;
    QString filename = QFileDialog::getOpenFileName(this,"Open File","./docs","Hash (*.hash)");

    if(filename.isEmpty()) return;

    QFile file(filename);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) return;

    QTextStream in(&file);
    QString line = in.readLine();

    fileContent.append(line);

    file.close();

    ui->lineEdit_3_LoadedHash->clear();
    ui->lineEdit_3_LoadedHash->setText(fileContent);
}


void MainWindow::on_toolButton_3_ClearPlainText_clicked()
{
    ui->plainTextEdit_3->clear();
}


void MainWindow::on_toolButton_1_Generate_clicked()
{
    CheckKeysDir();

    switch (ui->comboBox_1_KeyType->currentIndex()) {
    case 1: //RSA
    {
        ui->plainTextEdit_1->clear();

        QString filelocation;
        QString filename;
        int i;
        bool success;

        CkRsa rsa;

        rsa.GenerateKey(1024);

        const char *publicKeyXml = rsa.exportPublicKey();
        const char *privateKeyXml = rsa.exportPrivateKey();

        filelocation = keysDir.relativeFilePath("./keys");
        filename = filelocation + "/javni_kljuc.pem";

        i = 2;
        while(QFile::exists(filename)){
            filename = filelocation + "/javni_kljuc_" + QString::number(i++) + ".pem";
        }

        CkPublicKey pubKey;
        success = pubKey.LoadFromString(publicKeyXml);
        success = pubKey.SavePemFile(false,filename.toLocal8Bit());

        filelocation = keysDir.relativeFilePath("./keys");
        filename = filelocation + "/privatni_kljuc.pem";

        i = 2;
        while(QFile::exists(filename)){
            filename = filelocation + "/privatni_kljuc_" + QString::number(i++) + ".pem";
        }

        CkPrivateKey privKey;
        success = privKey.LoadXml(privateKeyXml);
        success = privKey.SavePemFile(filename.toLocal8Bit());

        if(success){
            if(QMessageBox::Yes == QMessageBox::question(this,"Stvaranje ključeva","RSA par ključeva generiran. Želite li prikazati javni ključ?")){
                ui->plainTextEdit_1->setPlainText(pubKey.getPem(false));
            }
        }
        else{
            QMessageBox::critical(this,"Stvaranje ključeva","Neuspješno stvaranje RSA para ključeva!");
        }

        break;
    }
    case 0: //AES
    {
        ui->plainTextEdit_1->clear();

        CkPrng prng;
        const char* secretKeyHex = prng.genRandom(32,"hex");

        qDebug() << "randomly generated key: " << secretKeyHex << "\r\n";

        QString filelocation = keysDir.relativeFilePath("./keys");
        QString filename = filelocation + "/tajni_kljuc.aes";
        int i = 2;
        while(QFile::exists(filename)){
            filename = filelocation + "/tajni_kljuc_" + QString::number(i++) + ".aes";
        }

        qDebug() << filename;

        QFile file(filename);

        if(file.open(QIODevice::WriteOnly | QIODevice::Text)){
            QTextStream out(&file);
            out << secretKeyHex;

            file.close();

            qDebug() << "file saved successfully";

            if(QMessageBox::Yes == QMessageBox::question(this,"Stvaranje ključeva","AES-256 tajni ključ generiran. Želite li prikazati ključ?")){
                ui->plainTextEdit_1->setPlainText(secretKeyHex);
            }

        }
        else{
            qDebug() << "error saving file";

            QMessageBox::critical(this,"Stvaranje ključeva","Neuspješno stvaranje AES-256 tajnog ključa!");
        }

        break;
    }
    default:
        break;
    }
}


void MainWindow::on_toolButton_1_KeyPreview_clicked()
{
    QString fileContent;
    QString filename = QFileDialog::getOpenFileName(this,"Open File","./keys","Key Files (*.aes *.pem)");

    if(filename.isEmpty()) return;

    QFile file(filename);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) return;

    QTextStream in(&file);
    QString line = in.readLine();

    while(!line.isNull())
    {
        fileContent.append(line);
        line = in.readLine();

        if(!line.isNull()) fileContent.append("\n");
    }

    file.close();

    ui->plainTextEdit_1->clear();
    ui->plainTextEdit_1->setPlainText(fileContent);
}

void MainWindow::CheckKeysDir()
{
    keysDir = QDir(QCoreApplication::applicationDirPath()+"/keys/");

    qDebug() << QCoreApplication::applicationDirPath();
    qDebug() << keysDir.currentPath();

    if(!keysDir.exists()){
        qDebug() << "keys directory doesnt exist, creating new directory...";
        if(keysDir.mkpath(".")){
            qDebug() << "keys directory created successfully";
        }
        else{
            qDebug() << "error creating keys directory";
        }
    }
    else{
        qDebug() << "keys directory exist already";
    }
}


void MainWindow::on_toolButton_1_ClearPlainText_clicked()
{
    ui->plainTextEdit_1->clear();
}



void MainWindow::on_toolButton_2_LoadPlainText_clicked()
{
    CheckDocsDir();

    currentFile = QFileDialog::getOpenFileName(this,"Open File","./docs","Text (*.txt) ;; Encrypted (*.CRYPT)");
    currentFileContent = "";

    if(currentFile.isEmpty()) return;

    QFile file(currentFile);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) return;

    QTextStream in(&file);
    QString line = in.readLine();

    while(!line.isNull())
    {
        currentFileContent.append(line);
        line = in.readLine();

        if(!line.isNull()) currentFileContent.append("\n");
    }

    file.close();

    ui->plainTextEdit_2->clear();
    ui->plainTextEdit_2->setPlainText(currentFileContent);

    if(currentFile.endsWith(".CRYPT")){
        ui->plainTextEdit_2->setReadOnly(true);
    }
    else{
        ui->plainTextEdit_2->setReadOnly(false);
    }

}


void MainWindow::on_toolButton_2_SavePlainText_clicked()
{
    CheckDocsDir();

    QString filename = QFileDialog::getSaveFileName(this,"Save As","./docs","Text (*.txt)");
    if( filename.isEmpty())
        return;

    if(!filename.endsWith(".txt")) filename += ".txt";

    QFile file(filename);

    if( !file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream out(&file);
    out << ui->plainTextEdit_2->toPlainText() << "\n";

    file.close();

    currentFile = filename;
    currentFileContent = ui->plainTextEdit_2->toPlainText();
}


void MainWindow::on_toolButton_2_EncryptPlainText_clicked()
{
    if(currentFile.isEmpty()){
        QMessageBox::critical(this,"Greška","Datoteka nije učitana!\n\nNapomena: Prije enkripcije sadržaj uređivača teksta mora biti pohranjen u datoteku!");
        return;
    }

    if(!currentFile.endsWith(".txt")){
        QMessageBox::critical(this,"Greška","Učitana datoteka nije tekstualna!");
        return;
    }

    int encAlgorithm;
    QString encKeyContent;
    //const char *encKeyContent;
    bool isKeyOK = false;

    EncDecDialog *encdecDialog = new EncDecDialog;
    encdecDialog->setWindowTitle("Postavke enkripcije");
    auto result = encdecDialog->exec();

    if(result == QDialog::Rejected){
        qDebug() << "rejected";
        delete encdecDialog;
        return;
    }
    else{
        qDebug() << "accepted";
        encAlgorithm = encdecDialog->algorithm;
        encKeyContent = encdecDialog->keyFileContent;
        isKeyOK = encdecDialog->ok;
        delete encdecDialog;
    }

    switch (encAlgorithm) {
    case 0: //AES
    {
        if(isKeyOK){
            CkCrypt2 crypt;

            crypt.put_CryptAlgorithm("aes");
            crypt.put_KeyLength(256);
            crypt.put_EncodingMode("hex");

            const char *keyHex = encKeyContent.toLocal8Bit();

            crypt.SetEncodedKey(keyHex,"hex");

            QString encStr = crypt.encryptStringENC(currentFileContent.toLocal8Bit());

            qDebug() << "Encrypted string: " << encStr;

            ui->plainTextEdit_2->setPlainText(encStr);

            QFile file(currentFile);

            if( !file.open(QIODevice::WriteOnly | QIODevice::Text))
                return;

            QTextStream out(&file);
            out << ui->plainTextEdit_2->toPlainText() << "\n";

            file.close();

            QFileInfo fileInfo(currentFile);
            QString newFileName = fileInfo.dir().path() + "/" + currentFile.split("/").last().split(".").first() + ".CRYPT";

            qDebug() << "new file name: " << newFileName ;

            file.rename(newFileName);

            currentFile = newFileName;
            currentFileContent = encStr;

            ui->plainTextEdit_2->setReadOnly(true);

        }
        else{
            QMessageBox::critical(this,"Greška","Ključ nije odabran ili nije ispravan.");
        }

        break;
    }
    case 1: //RSA
    {
        if(isKeyOK){
            CkRsa rsa;

            const char *publicKeyPem = encKeyContent.toLocal8Bit();
            CkPublicKey pubkey;

            if(!pubkey.LoadFromString(publicKeyPem)){
                QMessageBox::critical(this,"Greška",pubkey.lastErrorText());
                qDebug() << "ERROR 1";
                return;
            }

            const char *publicKeyXml = pubkey.getXml();

            if(!rsa.ImportPublicKey(publicKeyXml)){
                QMessageBox::critical(this,"Greška",rsa.lastErrorText());
                qDebug() << "ERROR 2";
                return;
            }

            const char *encryptedStr = rsa.encryptStringENC(currentFileContent.toLocal8Bit(),false);

            qDebug() << "Encrypted string: " << encryptedStr;

            ui->plainTextEdit_2->setPlainText(encryptedStr);

            QFile file(currentFile);

            if( !file.open(QIODevice::WriteOnly | QIODevice::Text))
                return;

            QTextStream out(&file);
            out << ui->plainTextEdit_2->toPlainText() << "\n";

            file.close();

            QFileInfo fileInfo(currentFile);
            QString newFileName = fileInfo.dir().path() + "/" + currentFile.split("/").last().split(".").first() + ".CRYPT";

            qDebug() << "new file name: " << newFileName ;

            file.rename(newFileName);

            currentFile = newFileName;
            currentFileContent = encryptedStr;

            ui->plainTextEdit_2->setReadOnly(true);
        }
        else{
            QMessageBox::critical(this,"Greška","Ključ nije odabran ili nije ispravan.");
        }

        break;
    }

    default:
        break;
    }
}

void MainWindow::on_toolButton_2_DecryptPlainText_clicked()
{
    if(currentFile.isEmpty()){
        QMessageBox::critical(this,"Greška","Datoteka nije učitana!\n\nNapomena: Prije dekripcije potrebno je učitati kriptiranu datoteku!");
        return;
    }

    if(!currentFile.endsWith(".CRYPT")){
        QMessageBox::critical(this,"Greška","Učitana datoteka nije kriptirana!");
        return;
    }

    int decAlgorithm;
    QString decKeyContent;
    const char *decKeyContent2;
    bool isKeyOK = false;

    EncDecDialog *encdecDialog = new EncDecDialog;
    encdecDialog->setWindowTitle("Postavke dekripcije");
    auto result = encdecDialog->exec();

    if(result == QDialog::Rejected){
        qDebug() << "rejected";
        delete encdecDialog;
        return;
    }
    else{
        qDebug() << "accepted";
        decAlgorithm = encdecDialog->algorithm;
        decKeyContent = encdecDialog->keyFileContent;
        decKeyContent2 = encdecDialog->keyFileContent2;
        isKeyOK = encdecDialog->ok;
        delete encdecDialog;
    }

    switch (decAlgorithm) {
    case 0: //aes
    {
        if(isKeyOK){
            CkCrypt2 crypt;

            crypt.put_CryptAlgorithm("aes");
            crypt.put_KeyLength(256);
            crypt.put_EncodingMode("hex");

            const char *keyHex = decKeyContent.toLocal8Bit();

            crypt.SetEncodedKey(keyHex,"hex");

            QString decStr = crypt.decryptStringENC(currentFileContent.toLocal8Bit());

            qDebug() << "Decrypted string: " << decStr;

            ui->plainTextEdit_2->setPlainText(decStr);

            QFile file(currentFile);

            if( !file.open(QIODevice::WriteOnly | QIODevice::Text))
                return;

            QTextStream out(&file);
            out << ui->plainTextEdit_2->toPlainText() << "\n";

            file.close();

            QFileInfo fileInfo(currentFile);
            QString newFileName = fileInfo.dir().path() + "/" + currentFile.split("/").last().split(".").first() + ".txt";

            qDebug() << "new file name: " << newFileName ;

            file.rename(newFileName);

            currentFile = newFileName;
            currentFileContent = decStr;

            ui->plainTextEdit_2->setReadOnly(false);
        }
        else{
            QMessageBox::critical(this,"Greška","Ključ nije odabran ili nije ispravan.");
        }

        break;
    }
    case 1: //rsa
    {
        if(isKeyOK){
            CkRsa rsa;

            const char *privateKeyPem = decKeyContent2;

            qDebug() << "private key pem: " << privateKeyPem;

            CkPrivateKey privkey;

            if(!privkey.LoadPem(privateKeyPem)){
                QMessageBox::critical(this,"Greška",privkey.lastErrorText());
                return;
            }

            const char *privateKeyXml = privkey.getXml();

            if(!rsa.ImportPrivateKey(privateKeyXml)){
                QMessageBox::critical(this,"Greška",rsa.lastErrorText());
                return;
            }

            const char *decryptedStr = rsa.decryptStringENC(currentFileContent.toLocal8Bit(),true);

            qDebug() << "Encrypted string: " << decryptedStr;

            ui->plainTextEdit_2->setPlainText(decryptedStr);

            QFile file(currentFile);

            if( !file.open(QIODevice::WriteOnly | QIODevice::Text))
                return;

            QTextStream out(&file);
            out << ui->plainTextEdit_2->toPlainText() << "\n";

            file.close();

            QFileInfo fileInfo(currentFile);
            QString newFileName = fileInfo.dir().path() + "/" + currentFile.split("/").last().split(".").first() + ".txt";

            qDebug() << "new file name: " << newFileName ;

            file.rename(newFileName);

            currentFile = newFileName;
            currentFileContent = decryptedStr;

            ui->plainTextEdit_2->setReadOnly(false);
        }
        else{
            QMessageBox::critical(this,"Greška","Ključ nije odabran ili nije ispravan.");
        }

        break;
    }
    default:
        break;
    }
}

void MainWindow::on_toolButton_2_ClearPlainText_clicked()
{
    ui->plainTextEdit_2->clear();
    ui->plainTextEdit_2->setReadOnly(false);
    currentFile = "";
    currentFileContent = "";
}


void MainWindow::on_toolButton_4_LoadPlainText_clicked()
{
    CheckDocsDir();

    QString currentFile = QFileDialog::getOpenFileName(this,"Open File","./docs","Text (*.txt)");
    QString currentFileContent = "";

    if(currentFile.isEmpty()) return;

    QFile file(currentFile);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) return;

    QTextStream in(&file);
    QString line = in.readLine();

    while(!line.isNull())
    {
        currentFileContent.append(line);
        line = in.readLine();

        if(!line.isNull()) currentFileContent.append("\n");
    }

    file.close();

    ui->plainTextEdit_4_Source->clear();
    ui->plainTextEdit_4_Source->setPlainText(currentFileContent);
}


void MainWindow::on_toolButton_4_SavePlainText_clicked()
{
    CheckDocsDir();

    QString filename = QFileDialog::getSaveFileName(this,"Save As","./docs","Text (*.txt)");
    if( filename.isEmpty())
        return;

    if(!filename.endsWith(".txt")) filename += ".txt";

    QFile file(filename);

    if( !file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream out(&file);
    out << ui->plainTextEdit_4_Source->toPlainText() << "\n";

    file.close();
}


void MainWindow::on_toolButton_4_SignPlainText_clicked()
{
    QString currentFile = QFileDialog::getOpenFileName(this,"Open File","./keys","Key Files (*.pem)");
    const char *currentFileContent = "";

    if(currentFile.isEmpty()) return;

    QFile file(currentFile);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) return;

    currentFileContent = file.readAll();

    file.close();

    CkRsa rsa;

    const char *privateKeyPem = currentFileContent;

    CkPrivateKey privkey;

    if(!privkey.LoadPem(privateKeyPem)){
        QMessageBox::critical(this,"Greška",privkey.lastErrorText());
        return;
    }

    const char *privateKeyXml = privkey.getXml();

    if(!rsa.ImportPrivateKey(privateKeyXml)){
        QMessageBox::critical(this,"Greška",rsa.lastErrorText());
        return;
    }

    const char *plaintextHash = ui->lineEdit_4_SourceHash->text().toLocal8Bit();

    const char *signedStr = rsa.encryptStringENC(plaintextHash,true);

    ui->plainTextEdit_4_Sign->setPlainText(signedStr);


}

void MainWindow::on_plainTextEdit_4_Source_textChanged()
{
    if(!ui->plainTextEdit_4_Source->toPlainText().isEmpty()){
        ui->lineEdit_4_SourceHash->setText(QCryptographicHash::hash(ui->plainTextEdit_4_Source->toPlainText().toUtf8(),QCryptographicHash::Sha256).toHex());
        ui->lineEdit_4_SourceHash->setCursorPosition(0);
    }
    else{
        ui->lineEdit_4_SourceHash->setText("");
    }
}


void MainWindow::on_toolButton_4_ClearPlainText_clicked()
{
    ui->plainTextEdit_4_Source->clear();
}


void MainWindow::on_toolButton_4_LoadSignedText_clicked()
{
    CheckDocsDir();

    QString currentFile = QFileDialog::getOpenFileName(this,"Open File","./docs","Signature (*.SIGNATURE)");
    QString currentFileContent = "";

    if(currentFile.isEmpty()) return;

    QFile file(currentFile);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) return;

    QTextStream in(&file);
    QString line = in.readLine();

    while(!line.isNull())
    {
        currentFileContent.append(line);
        line = in.readLine();

        if(!line.isNull()) currentFileContent.append("\n");
    }

    file.close();

    ui->plainTextEdit_4_Sign->clear();
    ui->plainTextEdit_4_Sign->setPlainText(currentFileContent);
}


void MainWindow::on_toolButton_4_SaveSignedText_clicked()
{
    CheckDocsDir();

    QString filename = QFileDialog::getSaveFileName(this,"Save As","./docs","Signature (*.SIGNATURE)");
    if( filename.isEmpty())
        return;

    if(!filename.endsWith(".SIGNATURE")) filename += ".SIGNATURE";

    QFile file(filename);

    if( !file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream out(&file);
    out << ui->plainTextEdit_4_Sign->toPlainText() << "\n";

    file.close();
}


void MainWindow::on_toolButton_4_VerifySignedText_clicked()
{
    QString currentFile = QFileDialog::getOpenFileName(this,"Open File","./keys","Key Files (*.pem)");
    QString currentFileContent = "";

    if(currentFile.isEmpty()) return;

    QFile file(currentFile);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) return;

    QTextStream in(&file);
    QString line = in.readLine();

    while(!line.isNull())
    {
        currentFileContent.append(line);
        line = in.readLine();

        if(!line.isNull()) currentFileContent.append("\n");
    }

    file.close();

    CkRsa rsa;

    const char *publicKeyPem = currentFileContent.toLocal8Bit();

    CkPublicKey pubkey;

    if(!pubkey.LoadFromString(publicKeyPem)){
        QMessageBox::critical(this,"Greška",pubkey.lastErrorText());
        return;
    }

    const char *publicKeyXml = pubkey.getXml();

    if(!rsa.ImportPublicKey(publicKeyXml)){
        QMessageBox::critical(this,"Greška",rsa.lastErrorText());
        return;
    }

    const char *signedStr = ui->plainTextEdit_4_Sign->toPlainText().toLocal8Bit();

    const char *verifiedStr = rsa.decryptStringENC(signedStr,false);

    ui->lineEdit_4_SignHash->setText(verifiedStr);
    ui->lineEdit_4_SignHash->setCursorPosition(0);
}


void MainWindow::on_plainTextEdit_4_Sign_textChanged()
{
    ui->lineEdit_4_SignHash->clear();
}


void MainWindow::on_toolButton_4_ClearSignedText_clicked()
{
    ui->plainTextEdit_4_Sign->clear();
}


void MainWindow::on_lineEdit_4_SourceHash_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);

    if(ui->lineEdit_4_SourceHash->text().isEmpty() || ui->lineEdit_4_SignHash->text().isEmpty()){
        ui->label_4_VerificationResult->setText("");
    }
    else if(ui->lineEdit_4_SourceHash->text() == ui->lineEdit_4_SignHash->text()){
        ui->label_4_VerificationResult->setText("<img style='vertical-align:middle' src='://icons/icons8-ok-48.png' width='16' height='16'> Digitalni potpis je valjan.");
    }
    else{
        ui->label_4_VerificationResult->setText("<img style='vertical-align:middle' src='://icons/icons8-cancel-48.png' width='16' height='16'> Digitalni potpis nije valjan.");
    }
}


void MainWindow::on_lineEdit_4_SignHash_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);

    if(ui->lineEdit_4_SourceHash->text().isEmpty() || ui->lineEdit_4_SignHash->text().isEmpty()){
        ui->label_4_VerificationResult->setText("");
    }
    else if(ui->lineEdit_4_SourceHash->text() == ui->lineEdit_4_SignHash->text()){
        ui->label_4_VerificationResult->setText("<img style='vertical-align:middle' src='://icons/icons8-ok-48.png' width='16' height='16'> Digitalni potpis je valjan.");
    }
    else{
        ui->label_4_VerificationResult->setText("<img style='vertical-align:middle' src='://icons/icons8-cancel-48.png' width='16' height='16'> Digitalni potpis nije valjan.");
    }
}

void MainWindow::CheckDocsDir()
{
    docsDir = QDir(QCoreApplication::applicationDirPath()+"/docs/");

    qDebug() << QCoreApplication::applicationDirPath();
    qDebug() << docsDir.currentPath();

    if(!docsDir.exists()){
        qDebug() << "docs directory doesnt exist, creating new directory...";
        if(docsDir.mkpath(".")){
            qDebug() << "docs directory created successfully";
        }
        else{
            qDebug() << "error creating docs directory";
        }
    }
    else{
        qDebug() << "docs directory exist already";
    }
}

void MainWindow::on_actionAbout_triggered()
{
    if(aboutDialog){
        aboutDialog->activateWindow();
    }
    else{
        aboutDialog = new AboutDialog(this);
        aboutDialog->setAttribute(Qt::WA_DeleteOnClose);
        aboutDialog->setWindowFlags(aboutDialog->windowFlags() & ~Qt::WindowContextHelpButtonHint);
        aboutDialog->show();
    }
}


void MainWindow::on_actionExit_triggered()
{
    close();
}
