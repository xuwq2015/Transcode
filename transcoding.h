#ifndef TRANSCODING_H
#define TRANSCODING_H

#include <QObject>
#include <QFile>
#include <QDir>
#include <QFileInfo>
#include <QTextCodec>
#include <QByteArray>
#include <QDebug>

class Transcoding : public QObject
{
    Q_OBJECT
public:
    explicit Transcoding(QObject *parent = 0);
    Q_INVOKABLE void startTarnscode(QString, QString, QString);

    Q_INVOKABLE void resolveFilePathStr(QString);
    Q_INVOKABLE void getDirPath(QString);

    Q_INVOKABLE void removeFileList(int);
    Q_INVOKABLE void removeFileList();
private:
    int readWriteFile(QFile *, QFile *);            //对文件进行读写转码操作
    QString extractFileName(QString, QString);
    void addFileListModel(QString);

    QString saveDirString;          //目标文件保存目录
    QString sourceFileCode;         //源文件编码
    QString aimsFileCode;           //目标文件编码
    QStringList filePathList;   //需要转码的文件列表
    double completedSize;
    double currentFileNum;
signals:
    void errorInfo(QString errorInfoString);
    void appendFile(QString filePath);
    void changeStatus(int index, int fileStatus);

    void setFileSize(int num);
    void setHundredPercent(int num);

    void changeStatusBarInfo(QString stringInfo);
public slots:
};

#endif // TRANSCODING_H
