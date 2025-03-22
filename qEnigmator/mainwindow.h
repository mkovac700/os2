#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QScreen>

#include <QCryptographicHash>
#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>
#include <QDebug>

/*#include <CkCrypt2.h>
#include <CkPrng.h>

#include <CkRsa.h>
#include <CkPrivateKey.h>
#include <CkPublicKey.h>
*/

// #include <aes.h>
// #include <rsa.h>
// #include <osrng.h>
// #include <files.h>

#include <botan/auto_rng.h>
#include <botan/rsa.h>
#include <botan/pem.h>
#include <botan/symkey.h>
#include <botan/hex.h>
#include <botan/block_cipher.h>
#include <botan/cipher_mode.h>
#include <botan/data_src.h>
#include <botan/pubkey.h>
#include <botan/pk_ops.h>
#include <botan/hash.h>

#include <cryptopp/rsa.h>
#include <cryptopp/sha.h>
#include <cryptopp/filters.h>
#include <cryptopp/base64.h>
#include <cryptopp/osrng.h>
#include <cryptopp/files.h>
#include <cryptopp/hex.h>
#include <cryptopp/pem.h>

#include <QPointer>

class AboutDialog;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_lineEdit_3_CalculatedHash_textChanged(const QString &arg1);

    void on_lineEdit_3_LoadedHash_textChanged(const QString &arg1);

    void on_plainTextEdit_3_textChanged();

    void on_toolButton_3_LoadPlainText_clicked();

    void on_toolButton_3_SavePlainText_clicked();

    void on_toolButton_3_SaveHash_clicked();

    void on_toolButton_3_LoadHash_clicked();

    void on_toolButton_3_ClearPlainText_clicked();

    void on_toolButton_1_Generate_clicked();

    void on_toolButton_1_KeyPreview_clicked() ;

    void on_toolButton_1_ClearPlainText_clicked() ;

    void on_toolButton_2_LoadPlainText_clicked() ;

    void on_toolButton_2_SavePlainText_clicked() ;

    void on_toolButton_2_EncryptPlainText_clicked() ;

    void on_toolButton_2_DecryptPlainText_clicked() ;

    void on_toolButton_2_ClearPlainText_clicked() ;

    void on_toolButton_4_LoadPlainText_clicked() ;

    void on_toolButton_4_SavePlainText_clicked() ;

    void on_toolButton_4_SignPlainText_clicked() ;

    void on_plainTextEdit_4_Source_textChanged() ;

    void on_toolButton_4_ClearPlainText_clicked() ;

    void on_toolButton_4_LoadSignedText_clicked() ;

    void on_toolButton_4_SaveSignedText_clicked() ;

    void on_toolButton_4_VerifySignedText_clicked() ;

    void on_plainTextEdit_4_Sign_textChanged();

    void on_toolButton_4_ClearSignedText_clicked() ;

    void on_lineEdit_4_SourceHash_textChanged(const QString &arg1) ;

    void on_lineEdit_4_SignHash_textChanged(const QString &arg1) ;

    void on_actionAbout_triggered();

    void on_actionExit_triggered();

private:
    Ui::MainWindow *ui;

    //directories
    QDir keysDir;
    QDir docsDir;
    void CheckKeysDir();
    void CheckDocsDir();

    //enc-dec
    QString currentFile;
    QString currentFileContent;

    //about
    QPointer<AboutDialog> aboutDialog;

    //RNG
    Botan::AutoSeeded_RNG global_rng;

    std::vector<uint8_t> ct;

private:
    QString vectorToQString(const std::vector<uint8_t> &data);
    std::vector<uint8_t> qStringToVector(const QString &str);

    void LoadPEMKey(const std::string& filename, CryptoPP::RSA::PrivateKey& privateKey);
    void LoadPEMKey(const std::string& filename, CryptoPP::RSA::PublicKey& publicKey);
};
#endif // MAINWINDOW_H
