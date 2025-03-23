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

        // CkRsa rsa;

        // rsa.GenerateKey(1024);

        // const char *publicKeyXml = rsa.exportPublicKey();
        // const char *privateKeyXml = rsa.exportPrivateKey();

        // Random number generator
        Botan::AutoSeeded_RNG rng;

        // Generate RSA private key
        Botan::RSA_PrivateKey rsa_priv_key(rng, 1024);

        // Extract the public key from the private key
        Botan::RSA_PublicKey rsa_pub_key(rsa_priv_key);

        // filelocation = keysDir.relativeFilePath("./keys");
        // filename = filelocation + "/javni_kljuc.pem";

        filelocation = keysDir.absolutePath();
        filename = filelocation + "/javni_kljuc.pem";

        i = 2;
        while(QFile::exists(filename)){
            filename = filelocation + "/javni_kljuc_" + QString::number(i++) + ".pem";
        }

        // CkPublicKey pubKey;
        // success = pubKey.LoadFromString(publicKeyXml);
        // success = pubKey.SavePemFile(false,filename.toLocal8Bit());

        // Save public key to file in PEM format
        QFile pub_key_file(filename);
        //QFile pub_key_file("javni_kljuc.pem");

        qDebug() << "Current working directory: " << QDir::currentPath().toStdString() ;
        qDebug() << "pub key file location" << filelocation;
        qDebug() << "pub key file:" << filename;

        if (pub_key_file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&pub_key_file);
            out << Botan::PEM_Code::encode(rsa_pub_key.public_key_bits(),"PUBLIC KEY").c_str();
            pub_key_file.close();
            qDebug() << "Public key saved to " << filename.toStdString();
            success = true;
        } else {
            qDebug() << "Failed to open public key file for writing.";
            success = false;
        }

        filelocation = keysDir.absolutePath();
        filename = filelocation + "/privatni_kljuc.pem";

        i = 2;
        while(QFile::exists(filename)){
            filename = filelocation + "/privatni_kljuc_" + QString::number(i++) + ".pem";
        }

        // CkPrivateKey privKey;
        // success = privKey.LoadXml(privateKeyXml);
        // success = privKey.SavePemFile(filename.toLocal8Bit());

        // Save private key to file in PEM format
        QFile priv_key_file(filename);

        if (priv_key_file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&priv_key_file);
            out << Botan::PEM_Code::encode(rsa_priv_key.private_key_bits(),"PRIVATE KEY").c_str();
            priv_key_file.close();
            qDebug() << "Private key saved to " << filename.toStdString();
            success = true;
        } else {
            qDebug() << "Failed to open private key file for writing.";
            success = false;
        }

        if(success){
            if(QMessageBox::Yes == QMessageBox::question(this,"Stvaranje ključeva","RSA par ključeva generiran. Želite li prikazati javni ključ?")){
                ui->plainTextEdit_1->setPlainText(Botan::PEM_Code::encode(rsa_pub_key.public_key_bits(),"PUBLIC KEY").c_str());
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

        QString filelocation;
        QString filename;

        Botan::AutoSeeded_RNG rng;  // Random number generator
        Botan::SymmetricKey key(rng, 32); // Generate 256-bit AES key (32 bytes x 8 = 256 bits)

        // CkPrng prng;
        // const char* secretKeyHex = prng.genRandom(32,"hex");

        QString hexKey = QString::fromStdString(Botan::hex_encode(key));

        // qDebug() << "randomly generated key: " << secretKeyHex << "\r\n";
        qDebug() << "randomly generated key: " << hexKey << "\r\n";

        filelocation = keysDir.absolutePath();
        filename = filelocation + "/tajni_kljuc.aes";

        int i = 2;
        while(QFile::exists(filename)){
            filename = filelocation + "/tajni_kljuc_" + QString::number(i++) + ".aes";
        }

        qDebug() << filename;

        QFile file(filename);

        if(file.open(QIODevice::WriteOnly | QIODevice::Text)){
            QTextStream out(&file);
            out << hexKey;

            file.close();

            qDebug() << "file saved successfully";

            if(QMessageBox::Yes == QMessageBox::question(this,"Stvaranje ključeva","AES-256 tajni ključ generiran. Želite li prikazati ključ?")){
                ui->plainTextEdit_1->setPlainText(hexKey);
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

void MainWindow::on_toolButton_1_ClearPlainText_clicked()
{
    ui->plainTextEdit_1->clear();
}



void MainWindow::on_toolButton_2_LoadPlainText_clicked()
{
    CheckDocsDir();

    currentFile = QFileDialog::getOpenFileName(this,"Open File",docsDir.absolutePath(),"Text (*.txt) ;; Encrypted (*.CRYPT)");
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

    QString filename = QFileDialog::getSaveFileName(this,"Save As",docsDir.absolutePath(),"Text (*.txt)");
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
            // CkCrypt2 crypt;

            // crypt.put_CryptAlgorithm("aes");
            // crypt.put_KeyLength(256);
            // crypt.put_EncodingMode("hex");

            // const char *keyHex = encKeyContent.toLocal8Bit();


            // Convert hex string to binary key
            std::vector<uint8_t> keyBytes = Botan::hex_decode(encKeyContent.toStdString());
            Botan::SymmetricKey secretKey = Botan::SymmetricKey(keyBytes);

            Botan::AutoSeeded_RNG rng;

            // Generate a random IV (Initialization Vector)
            // std::vector<uint8_t> iv(16);
            // rng.randomize(iv.data(), iv.size());

            // Create the AES cipher in CBC mode with padding
            // std::unique_ptr<Botan::Cipher_Mode> enc = Botan::Cipher_Mode::create("AES-256/CBC/PKCS7", Botan::Cipher_Dir::Encryption);
            std::unique_ptr<Botan::Cipher_Mode> enc = Botan::Cipher_Mode::create("AES-256/CBC/NoPadding", Botan::Cipher_Dir::Encryption);

            if (!enc) {
                qWarning() << "Failed to create AES cipher!";
            }

            enc->set_key(secretKey);
            // enc->start(iv);

            // Convert QString to QByteArray (UTF-8) and then to a byte vector
            //std::vector<uint8_t> plaintextBytes(currentFileContent.toUtf8().begin(), currentFileContent.toUtf8().end());

            // Convert QString to UTF-8 and make sure it's exactly 32 bytes
            // std::vector<uint8_t> plaintextBytes(32, 0);  // Initialize 32-byte array with zeros
            // QByteArray utf8Data = currentFileContent.toUtf8();
            // std::memcpy(plaintextBytes.data(), utf8Data.data(), qMin(32, utf8Data.size()));

            // Convert QString to bytes (UTF-8)
            QByteArray utf8Data = currentFileContent.toUtf8();
            std::vector<uint8_t> plaintextBytes(utf8Data.begin(), utf8Data.end());

            // Ensure the plaintext length is a multiple of block size (AES block size = 16)
            size_t block_size = 16;
            if (plaintextBytes.size() % block_size != 0) {
                size_t padding_size = block_size - (plaintextBytes.size() % block_size);
                plaintextBytes.insert(plaintextBytes.end(), padding_size, 0);  // Add padding
            }

            // Pad the input and encrypt
            // enc->finish(plaintextBytes);

            // Encrypt
            enc->start(); // ECB mode doesn't use IV
            enc->finish(plaintextBytes);

            // Combine IV and ciphertext (IV is needed for decryption)
            // std::vector<uint8_t> encryptedData = iv;
            // encryptedData.insert(encryptedData.end(), plaintextBytes.begin(), plaintextBytes.end());

            // QString encStr = QString::fromStdString(Botan::hex_encode(encryptedData));

            QString encStr = QString::fromStdString(Botan::hex_encode(plaintextBytes));

            // crypt.SetEncodedKey(keyHex,"hex");

            // QString encStr = crypt.encryptStringENC(currentFileContent.toLocal8Bit());

            // qDebug() << "Encrypted string: " << encStr;

            // ui->plainTextEdit_2->setPlainText(encStr);

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
            // CkRsa rsa;

            // const char *publicKeyPem = encKeyContent.toLocal8Bit();
            // CkPublicKey pubkey;

            // if(!pubkey.LoadFromString(publicKeyPem)){
            //     QMessageBox::critical(this,"Greška",pubkey.lastErrorText());
            //     qDebug() << "ERROR 1";
            //     return;
            // }

            // const char *publicKeyXml = pubkey.getXml();

            // if(!rsa.ImportPublicKey(publicKeyXml)){
            //     QMessageBox::critical(this,"Greška",rsa.lastErrorText());
            //     qDebug() << "ERROR 2";
            //     return;
            // }

            // Load the public key from PEM
            // Botan::DataSource_Memory pem(encKeyContent.toStdString());

            // std::string label = "PUBLIC KEY";

            // auto decodedPublicKey = Botan::PEM_Code::decode(encKeyContent.toStdString(), label);

            // Botan::RSA_PublicKey publicKey(Botan::AlgorithmIdentifier::Encoding_Option, decodedPublicKey);

            //Botan::AutoSeeded_RNG rng;

            // Decode PEM data
            std::string label = "PUBLIC KEY";
            Botan::DataSource_Memory data_src_pub(encKeyContent.toStdString());
            Botan::secure_vector<uint8_t> pub_key_bits = Botan::PEM_Code::decode(data_src_pub, label);

            // Create AlgorithmIdentifier for RSA
            Botan::AlgorithmIdentifier alg_id(Botan::OID::from_string("RSA"), Botan::AlgorithmIdentifier::USE_NULL_PARAM);

            // Construct RSA_PublicKey
            Botan::RSA_PublicKey rsa_pub_key(alg_id,pub_key_bits);

            QString plaintext = currentFileContent;

            // // Convert QString to bytes (UTF-8)
            // // std::string plaintext_utf8 = plaintext.toUtf8().toStdString();
            // QByteArray plaintext_utf8 = plaintext.toUtf8();
            // //std::vector<uint8_t> plaintext_data(plaintext_utf8.begin(), plaintext_utf8.end());
            // std::string pt = plaintext_utf8.toStdString();
            // const Botan::secure_vector<uint8_t> plaintext_data(pt.data(), pt.data() + pt.length());

            // // Encrypt a message using the public key
            // Botan::PK_Encryptor_EME encryptor(rsa_pub_key, global_rng, "OAEP(SHA-256)");
            // // std::vector<uint8_t> ciphertext = encryptor.encrypt(
            // //     reinterpret_cast<const uint8_t*>(plaintext_data.data()), plaintext_data.size(), global_rng);
            // const auto ciphertext = encryptor.encrypt(plaintext_data, global_rng);
            // ct = ciphertext;

            // std::string encrypted_text(ciphertext.begin(), ciphertext.end());

            // // const char *encryptedStr = rsa.encryptStringENC(currentFileContent.toLocal8Bit(),false);

            // qDebug() << "Encrypted string: " << encrypted_text;

            // ui->plainTextEdit_2->setPlainText(QString::fromStdString(encrypted_text));

            const auto plaintext_data = qStringToVector(plaintext);

            Botan::PK_Encryptor_EME encryptor(rsa_pub_key, global_rng, "OAEP(SHA-256)");
            const auto ciphertext_data = encryptor.encrypt(plaintext_data, global_rng);

            // const auto ciphertext = vectorToQString(ciphertext_data);

            // Convert binary ciphertext to Base64 for display
            QByteArray ciphertext_bytes(reinterpret_cast<const char*>(ciphertext_data.data()), ciphertext_data.size());
            QString ciphertext_base64 = QString::fromUtf8(ciphertext_bytes.toBase64());

            // ui->plainTextEdit_2->setPlainText(ciphertext);
            ui->plainTextEdit_2->setPlainText(ciphertext_base64);

            QFile file(currentFile);

            if( !file.open(QIODevice::WriteOnly | QIODevice::Text))
                return;

            // if(!file.open(QIODevice::WriteOnly))
            //     return;

            QTextStream out(&file);
            // out << ui->plainTextEdit_2->toPlainText() << "\n";
            // out << ui->plainTextEdit_2->toPlainText();
            // out << ciphertext;
            out << ciphertext_base64;

            file.close();

            QFileInfo fileInfo(currentFile);
            QString newFileName = fileInfo.dir().path() + "/" + currentFile.split("/").last().split(".").first() + ".CRYPT";

            qDebug() << "new file name: " << newFileName ;

            file.rename(newFileName);

            currentFile = newFileName;
            // currentFileContent = QString::fromStdString(encrypted_text);
            // currentFileContent = ciphertext;
            currentFileContent = ciphertext_base64;

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
    //const char *decKeyContent2;
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
        //decKeyContent2 = encdecDialog->keyFileContent2;
        isKeyOK = encdecDialog->ok;
        delete encdecDialog;
    }

    switch (decAlgorithm) {
    case 0: //aes
    {
        if(isKeyOK){
            // CkCrypt2 crypt;

            // crypt.put_CryptAlgorithm("aes");
            // crypt.put_KeyLength(256);
            // crypt.put_EncodingMode("hex");

            std::vector<uint8_t> keyBytes = Botan::hex_decode(decKeyContent.toStdString());
            Botan::SymmetricKey secretKey = Botan::SymmetricKey(keyBytes);

            //const char *keyHex = decKeyContent.toLocal8Bit();

            // crypt.SetEncodedKey(keyHex,"hex");

            //QString decStr = crypt.decryptStringENC(currentFileContent.toLocal8Bit());

            // Convert hex-encoded encrypted text to raw bytes
            std::vector<uint8_t> encryptedData = Botan::hex_decode(currentFileContent.toStdString());

            // if (encryptedData.size() < 16) {
            //     qWarning() << "Invalid encrypted data!";
            // }

            // Extract IV (first 16 bytes) and ciphertext (remaining bytes)
            // std::vector<uint8_t> iv(encryptedData.begin(), encryptedData.begin() + 16);
            // std::vector<uint8_t> ciphertext(encryptedData.begin() + 16, encryptedData.end());

            // Create AES-256 CBC decryption cipher
            // std::unique_ptr<Botan::Cipher_Mode> dec = Botan::Cipher_Mode::create("AES-256/CBC/PKCS7", Botan::Cipher_Dir::Decryption);
            std::unique_ptr<Botan::Cipher_Mode> dec = Botan::Cipher_Mode::create("AES-256/CBC/NoPadding", Botan::Cipher_Dir::Decryption);

            if (!dec) {
                qWarning() << "Failed to create AES cipher!";
            }

            dec->set_key(secretKey);
            // dec->start(iv);
            dec->start();
            // Decrypt the data
            dec->finish(encryptedData);

            // QString decStr = crypt.decryptStringENC(currentFileContent.toLocal8Bit());

            // Convert decrypted bytes back to QString
            // QString decStr = QString::fromUtf8(reinterpret_cast<const char*>(ciphertext.data()), ciphertext.size());
            QString decStr = QString::fromUtf8(reinterpret_cast<const char*>(encryptedData.data())).trimmed();

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
            // CkRsa rsa;

            // const char *privateKeyPem = decKeyContent2;

            // qDebug() << "private key pem: " << privateKeyPem;

            // CkPrivateKey privkey;

            // if(!privkey.LoadPem(privateKeyPem)){
            //     QMessageBox::critical(this,"Greška",privkey.lastErrorText());
            //     return;
            // }

            // const char *privateKeyXml = privkey.getXml();

            // if(!rsa.ImportPrivateKey(privateKeyXml)){
            //     QMessageBox::critical(this,"Greška",rsa.lastErrorText());
            //     return;
            // }

            try{

                //Botan::AutoSeeded_RNG rng;

                // Decode PEM data
                std::string label = "PRIVATE KEY";
                Botan::DataSource_Memory data_src_priv(decKeyContent.toStdString());
                Botan::secure_vector<uint8_t> priv_key_bits = Botan::PEM_Code::decode(data_src_priv, label);

                qDebug() << "private key content loaded: " << decKeyContent.toStdString();
                qDebug() << "private key content decoded: " << priv_key_bits;

                // Create AlgorithmIdentifier for RSA
                Botan::AlgorithmIdentifier alg_id(Botan::OID::from_string("RSA"), Botan::AlgorithmIdentifier::USE_NULL_PARAM);

                // Construct RSA_PrivateKey
                Botan::RSA_PrivateKey rsa_priv_key(alg_id, priv_key_bits);

                qDebug() << "Private key constructed!" << rsa_priv_key.private_key_info();

                // std::vector<uint8_t> ciphertext(currentFileContent.toStdString().begin(), currentFileContent.toStdString().end());

                qDebug() << "Ciphertext plain: " << currentFileContent.toStdString();
                //qDebug() << "Ciphertext vec: " << ciphertext;

                QString ciphertext = currentFileContent;

                QByteArray ciphertext_bytes = QByteArray::fromBase64(ciphertext.toUtf8());

                // const auto ciphertext_data = qStringToVector(ciphertext);
                std::vector<uint8_t> ciphertext_data(ciphertext_bytes.begin(), ciphertext_bytes.end());

                // Decrypt the message using the private key
                Botan::PK_Decryptor_EME decryptor(rsa_priv_key, global_rng, "OAEP(SHA-256)");
                // Botan::secure_vector<uint8_t> decrypted_data = decryptor.decrypt(
                //     ciphertext.data(), ciphertext.size());

                // const auto pt2 = decryptor.decrypt(ct);

                // qDebug() << "Data decrypted!";

                // // Convert decrypted data to QString
                // std::string pt2_1(pt2.begin(), pt2.end());
                // QString pt2_2 = QString::fromUtf8(pt2_1.c_str());

                // qDebug() << "Decrypted string org: " << pt2_2;


                // Botan::secure_vector<uint8_t> decrypted_data = decryptor.decrypt(ciphertext);

                // // Convert decrypted data to QString
                // std::string decrypted_text(decrypted_data.begin(), decrypted_data.end());
                // QString decrypted_qstring = QString::fromUtf8(decrypted_text.c_str());


                // // const char *decryptedStr = rsa.decryptStringENC(currentFileContent.toLocal8Bit(),true);

                // qDebug() << "Decrypted string: " << decrypted_qstring;

                const auto plaintext_data = decryptor.decrypt(ciphertext_data);
                std::string pt(plaintext_data.begin(), plaintext_data.end());
                QString plaintext = QString::fromUtf8(pt.c_str());
                // QString plaintext = vectorToQString(reinterpret_cast<const Botan::secure_vector<uint8_t>>(plaintext_data));

                // ui->plainTextEdit_2->setPlainText(decrypted_qstring);
                ui->plainTextEdit_2->setPlainText(plaintext);

                QFile file(currentFile);

                if( !file.open(QIODevice::WriteOnly | QIODevice::Text))
                    return;

                QTextStream out(&file);
                // out << ui->plainTextEdit_2->toPlainText() << "\n";
                out << plaintext;

                file.close();

                QFileInfo fileInfo(currentFile);
                QString newFileName = fileInfo.dir().path() + "/" + currentFile.split("/").last().split(".").first() + ".txt";

                qDebug() << "new file name: " << newFileName ;

                file.rename(newFileName);

                currentFile = newFileName;
                // currentFileContent = decrypted_qstring;
                currentFileContent = plaintext;

                ui->plainTextEdit_2->setReadOnly(false);


            } catch (const Botan::Exception& e) {
                qDebug() << "Botan Error: " << e.what();
                return ;
            } catch (const std::exception& e) {
                qDebug() << "Standard Error: " << e.what();
                return ;
            }
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
    CryptoPP::AutoSeededRandomPool rng2;

    // Generate private key
    CryptoPP::RSA::PrivateKey privateKey2;
    privateKey2.GenerateRandomWithKeySize(rng2, 2048);

    // Generate public key
    CryptoPP::RSA::PublicKey publicKey(privateKey2);

    try{
        // Save private key in PEM format
        CryptoPP::FileSink privateKeyFile("cryptopp_private_key.pem");
        PEM_Save(privateKeyFile, privateKey2);
        qDebug() << "Private key saved somewhere..." ;
        // Save public key in PEM format
        CryptoPP::FileSink publicKeyFile("cryptopp_public_key.pem");
        PEM_Save(publicKeyFile, publicKey);
    }catch(const CryptoPP::Exception& e){
        qDebug() << "Error saving keys! " << e.what();
        return;
    }


    QString currentFile = QFileDialog::getOpenFileName(this,"Open File","./keys","Key Files (*.pem)");
    // const char *decKeyContent = "";

    if(currentFile.isEmpty()) return;

    QFile file(currentFile);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) return;

    const char *decKeyContent = file.readAll();

    file.close();



    // std::string label = "PRIVATE KEY";
    // Botan::DataSource_Memory data_src_priv(decKeyContent);
    // Botan::secure_vector<uint8_t> priv_key_bits = Botan::PEM_Code::decode(data_src_priv, label);

    // qDebug() << "private key content loaded: " << decKeyContent;
    // qDebug() << "private key content decoded: " << priv_key_bits;

    // // Create AlgorithmIdentifier for RSA
    // Botan::AlgorithmIdentifier alg_id(Botan::OID::from_string("RSA"), Botan::AlgorithmIdentifier::USE_NULL_PARAM);

    // // Construct RSA_PrivateKey
    // Botan::RSA_PrivateKey rsa_priv_key(alg_id, priv_key_bits);

    // // const char *plaintextHash = ui->lineEdit_4_SourceHash->text().toLocal8Bit();

    // auto signer = rsa_priv_key.create_signature_op(global_rng, "EMSA_PKCS1(SHA-256)", "base");

    // auto hashh = qStringToVector(ui->lineEdit_4_SourceHash->text());

    // signer->update(hashh);

    // std::vector<uint8_t> signature = signer->sign(global_rng);
    // qDebug() << "Signature:" << QByteArray(reinterpret_cast<const char*>(signature.data()), signature.size()).toHex();

    // // Step 3: Convert the binary signature to Base64 for storage and display
    // QByteArray signature_bytes(reinterpret_cast<const char*>(signature.data()), signature.size());
    // QString signature_base64 = QString::fromUtf8(signature_bytes.toBase64());
    // qDebug() << "Base64-encoded signature:" << signature_base64;

    std::string hash = ui->lineEdit_4_SourceHash->text().toStdString();

    CryptoPP::RSA::PrivateKey privateKey;

    try{
        LoadPEMKey(currentFile.toStdString(), privateKey);
    }catch(const CryptoPP::Exception& e){
        qDebug() << "Error loading key! " << e.what();
        return;
    }

    // CryptoPP::AutoSeededRandomPool rng;
    // std::string signature;
    // CryptoPP::RSAES_OAEP_SHA_Encryptor encryptor(privateKey);
    // CryptoPP::StringSource(hash, true, new CryptoPP::PK_EncryptorFilter(rng, encryptor, new CryptoPP::StringSink(signature)));

    std::string signature = RSAEncryptWithPrivateKey(hash, privateKey);

    qDebug() << "signature: " << signature;

    std::string signature_base64;
    CryptoPP::StringSource(signature, true, new CryptoPP::Base64Encoder(new CryptoPP::StringSink(signature_base64), false));

    qDebug() << "signature: " << signature_base64;

    ui->plainTextEdit_4_Sign->setPlainText(QString::fromStdString(signature_base64));

    // QString signedStr = vectorToQString(signature);

    // ui->plainTextEdit_4_Sign->setPlainText(signedStr);

    // CkRsa rsa;

    // const char *privateKeyPem = currentFileContent;

    // CkPrivateKey privkey;

    // if(!privkey.LoadPem(privateKeyPem)){
    //     QMessageBox::critical(this,"Greška",privkey.lastErrorText());
    //     return;
    // }

    // const char *privateKeyXml = privkey.getXml();

    // if(!rsa.ImportPrivateKey(privateKeyXml)){
    //     QMessageBox::critical(this,"Greška",rsa.lastErrorText());
    //     return;
    // }

    // const char *plaintextHash = ui->lineEdit_4_SourceHash->text().toLocal8Bit();

    // const char *signedStr = rsa.encryptStringENC(plaintextHash,true);

    // ui->plainTextEdit_4_Sign->setPlainText(signedStr);



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
    // QString currentFile = QFileDialog::getOpenFileName(this,"Open File","./keys","Key Files (*.pem)");
    // QString currentFileContent = "";

    // if(currentFile.isEmpty()) return;

    // QFile file(currentFile);
    // if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) return;

    // QTextStream in(&file);
    // QString line = in.readLine();

    // while(!line.isNull())
    // {
    //     currentFileContent.append(line);
    //     line = in.readLine();

    //     if(!line.isNull()) currentFileContent.append("\n");
    // }

    // file.close();


    QString currentFile = QFileDialog::getOpenFileName(this,"Open File","./keys","Key Files (*.pem)");
    // const char *decKeyContent = "";

    if(currentFile.isEmpty()) return;

    QFile file(currentFile);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) return;

    const char *encKeyContent = file.readAll();

    file.close();

    CryptoPP::RSA::PublicKey publicKey;

    try{
        LoadPEMKey(currentFile.toStdString(), publicKey);
    }catch(const CryptoPP::Exception& e){
        qDebug() << "Error loading key! " << e.what();
        return;
    }

    try{
    CryptoPP::AutoSeededRandomPool rng;

    std::string signatureBase64 = ui->plainTextEdit_4_Sign->toPlainText().toStdString();

    std::string decryptedHash;
    //CryptoPP::RSAES_OAEP_SHA_Decryptor decryptor(publicKey);

    // Decode the Base64 signature back to binary
    std::string signatureBinary;
    CryptoPP::StringSource(signatureBase64, true, new CryptoPP::Base64Decoder(new CryptoPP::StringSink(signatureBinary)));

    // CryptoPP::StringSource(signatureBinary, true, new CryptoPP::PK_DecryptorFilter(rng, decryptor, new CryptoPP::StringSink(decryptedHash)));

    decryptedHash = RSADecryptWithPublicKey(signatureBinary, publicKey);

    ui->lineEdit_4_SignHash->setText(QString::fromStdString(decryptedHash));
    }catch(CryptoPP::Exception& e){
        qDebug() << "Error verifying! " << e.what();
    }

    // std::string label = "PUBLIC KEY";
    // Botan::DataSource_Memory data_src_pub(encKeyContent);
    // Botan::secure_vector<uint8_t> pub_key_bits = Botan::PEM_Code::decode(data_src_pub, label);

    // // Create AlgorithmIdentifier for RSA
    // Botan::AlgorithmIdentifier alg_id(Botan::OID::from_string("RSA"), Botan::AlgorithmIdentifier::USE_NULL_PARAM);

    // // Construct RSA_PublicKey
    // Botan::RSA_PublicKey rsa_pub_key(alg_id,pub_key_bits);

    // QString signedStr = ui->plainTextEdit_4_Sign->toPlainText();

    // // Step 6: Convert the Base64 string back to binary
    // QByteArray loaded_signature_bytes = QByteArray::fromBase64(signedStr.toUtf8());
    // std::vector<uint8_t> loaded_signature(loaded_signature_bytes.begin(), loaded_signature_bytes.end());

    // // Step 7: "Decrypt" the signature using the public key (raw RSA operation)
    // std::vector<uint8_t> decrypted_data = rsa_pub_key.raw_public_key_op(loaded_signature);
    // qDebug() << "Decrypted data:" << QByteArray(reinterpret_cast<const char*>(decrypted_data.data()), decrypted_data.size()).toHex();

    // // Step 8: Remove PKCS#1 v1.5 padding to retrieve the original hash
    // // PKCS#1 v1.5 padding starts with 0x00 0x01 ... 0x00
    // if (decrypted_data.size() >= hash.size() + 11) { // Minimum padding size is 11 bytes
    //     size_t padding_start = decrypted_data.size() - hash.size();
    //     if (decrypted_data[padding_start - 1] == 0x00) {
    //         std::vector<uint8_t> retrieved_hash(decrypted_data.begin() + padding_start, decrypted_data.end());
    //         qDebug() << "Retrieved hash:" << QByteArray(reinterpret_cast<const char*>(retrieved_hash.data()), retrieved_hash.size()).toHex();

    //         // Step 9: Compare the retrieved hash with the original hash
    //         if (retrieved_hash == hash) {
    //             qDebug() << "Signature is valid!";
    //         } else {
    //             qDebug() << "Signature is invalid!";
    //         }
    //     } else {
    //         qDebug() << "Invalid padding in decrypted data!";
    //     }
    // } else {
    //     qDebug() << "Decrypted data is too short to contain valid padding!";
    // }

    // CkRsa rsa;

    // const char *publicKeyPem = currentFileContent.toLocal8Bit();

    // CkPublicKey pubkey;

    // if(!pubkey.LoadFromString(publicKeyPem)){
    //     QMessageBox::critical(this,"Greška",pubkey.lastErrorText());
    //     return;
    // }

    // const char *publicKeyXml = pubkey.getXml();

    // if(!rsa.ImportPublicKey(publicKeyXml)){
    //     QMessageBox::critical(this,"Greška",rsa.lastErrorText());
    //     return;
    // }

    // const char *signedStr = ui->plainTextEdit_4_Sign->toPlainText().toLocal8Bit();

    // const char *verifiedStr = rsa.decryptStringENC(signedStr,false);

    // ui->lineEdit_4_SignHash->setText(verifiedStr);
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
    // docsDir = QDir(QCoreApplication::applicationDirPath()+"/docs/");

    // qDebug() << "exe dir" << QCoreApplication::applicationDirPath();
    // qDebug() << "docs dir" << docsDir.currentPath();

    QDir exeDir = QCoreApplication::applicationDirPath();
    docsDir = exeDir.absoluteFilePath("docs");

    qDebug() << "exe dir:" << exeDir.absolutePath();
    qDebug() << "docs dir:" << docsDir.absolutePath();

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

QString MainWindow::vectorToQString(const std::vector<uint8_t> &data)
{
    // Assuming the data is UTF-8 encoded
    return QString::fromUtf8(reinterpret_cast<const char*>(data.data()), data.size());
}

std::vector<uint8_t> MainWindow::qStringToVector(const QString &str)
{
    QByteArray utf8Data = str.toUtf8(); // Convert QString to UTF-8 encoded QByteArray
    return std::vector<uint8_t>(utf8Data.begin(), utf8Data.end());
}

void MainWindow::LoadPEMKey(const std::string &filename, CryptoPP::RSA::PrivateKey &privateKey)
{
    CryptoPP::FileSource file(filename.c_str(), true);
    PEM_Load(file, privateKey);
}

void MainWindow::LoadPEMKey(const std::string &filename, CryptoPP::RSA::PublicKey &publicKey)
{
    CryptoPP::FileSource file(filename.c_str(), true);
    PEM_Load(file, publicKey);
}

std::string MainWindow::RSAEncryptWithPrivateKey(const std::string &input, const CryptoPP::RSA::PrivateKey &privateKey)
{
    CryptoPP::AutoSeededRandomPool rng;

    // Convert the input string to an Integer
    CryptoPP::Integer inputInteger(reinterpret_cast<const CryptoPP::byte*>(input.data()), input.size());

    // Apply the private key operation (c = m^d mod n)
    CryptoPP::Integer ciphertextInteger = privateKey.CalculateInverse(rng, inputInteger);

    // Convert the result back to a string
    size_t ciphertextSize = ciphertextInteger.MinEncodedSize();
    std::string ciphertext(ciphertextSize, 0); // Resize the string to hold the encoded data
    ciphertextInteger.Encode(reinterpret_cast<CryptoPP::byte*>(&ciphertext[0]), ciphertext.size());

    return ciphertext;
}

std::string MainWindow::RSADecryptWithPublicKey(const std::string &input, const CryptoPP::RSA::PublicKey &publicKey)
{
    // Convert the input string to an Integer
    CryptoPP::Integer inputInteger(reinterpret_cast<const CryptoPP::byte*>(input.data()), input.size());

    // Apply the public key operation (m = c^e mod n)
    CryptoPP::Integer decryptedInteger = publicKey.ApplyFunction(inputInteger);

    // Convert the result back to a string
    size_t decryptedSize = decryptedInteger.MinEncodedSize();
    std::string decrypted(decryptedSize, 0); // Resize the string to hold the encoded data
    decryptedInteger.Encode(reinterpret_cast<CryptoPP::byte*>(&decrypted[0]), decrypted.size());

    return decrypted;
}

void MainWindow::CheckKeysDir()
{
    QDir exeDir = QCoreApplication::applicationDirPath();
    keysDir = exeDir.absoluteFilePath("keys");

    // keysDir = QDir(QCoreApplication::applicationDirPath()+"/keys/");


    // qDebug() << "exe dir" << QCoreApplication::applicationDirPath();
    // qDebug() << "keys dir" << keysDir.currentPath();

    qDebug() << "exe dir:" << exeDir.absolutePath();
    qDebug() << "keys dir:" << keysDir.absolutePath();

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
