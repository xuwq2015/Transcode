#ifndef TRANSCODING_H
#define TRANSCODING_H

#include <QObject>
#include <QFile>
#include <QDir>
#include <QFileInfo>
#include <QTextCodec>
#include <QByteArray>

class Transcoding : public QObject
{
    Q_OBJECT
public:
    explicit Transcoding(QObject *parent = 0);
    Q_INVOKABLE void startTarnscode(QString, QString, QString);
    Q_INVOKABLE void addFileList(QString);
    Q_INVOKABLE void resolveFilePathStr(QString);
    Q_INVOKABLE void getDirPath(QString);
private:
    int readWriteFile(QFile *, QFile *);            //对文件进行读写转码操作
    QString extractFileName(QString, QString);
    QString saveDirString;          //目标文件保存目录
    QString sourceFileCode;         //源文件编码
    QString aimsFileCode;           //目标文件编码
    QStringList filePathList;   //需要转码的文件列表
signals:
    void errorInfo(QString errorInfoString);
    void appendFile(QString filePath);
public slots:
};

#endif // TRANSCODING_H
