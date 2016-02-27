#include "transcoding.h"

Transcoding::Transcoding(QObject *parent) : QObject(parent){
}

void Transcoding::startTarnscode(QString dirString, QString sourceFileCodeString, QString aimsFileCodeString) {
    if(filePathList.isEmpty()) {
       emit errorInfo("请添加需要转换编码的文件!!!");
       return;
    }
    saveDirString = dirString;
    sourceFileCode = sourceFileCodeString;
    aimsFileCode = aimsFileCodeString;
    QString aimsFileName;
    completedSize = 0;

    QFile sourceFile;   //源文件指针
    QFile aimsFile;     //目标文件指针

    for(int num = 0; num < filePathList.size(); num++) {
        aimsFileName = extractFileName(filePathList.at(num), saveDirString);

        sourceFile.setFileName(filePathList.at(num));
        aimsFile.setFileName(aimsFileName);
        if(aimsFile.exists()) {
            emit errorInfo("error:文件"+aimsFileName+"已存在");
            continue;
        }

        if (!sourceFile.open(QIODevice::ReadOnly)) {
            emit errorInfo("文件:"+filePathList.at(num)+"打开失败");
            continue;
        }
        if (!aimsFile.open(QIODevice::WriteOnly)) {
            aimsFile.close();
            if(!aimsFile.open(QIODevice::WriteOnly)) {
                emit errorInfo("文件:"+aimsFileName+"打开失败");
                sourceFile.close();
                continue;
            }
        }

        if(readWriteFile(&sourceFile, &aimsFile) == -1) {
            emit errorInfo("文件:"+filePathList.at(num)+"转换失败!!!");
            sourceFile.close();
            aimsFile.close();
            aimsFile.remove();
            continue;
        } else {
            emit changeStatus(num, 0);
        }

        sourceFile.close();
        aimsFile.close();
    }
    emit setHundredPercent(100);
    emit errorInfo("转换完成!!!");
}

/*从源文件中读取文件内容，并写入目标文件*/
int Transcoding::readWriteFile(QFile *sourceFile, QFile *aimsFile) {
    QTextCodec *sourceTextCodec = QTextCodec::codecForName(sourceFileCode.toLatin1());
    QTextCodec *aimsTextCode = QTextCodec::codecForName(aimsFileCode.toLatin1());
    int currentWriteSize = 0;
    currentFileNum += 1;
    while(!sourceFile->atEnd()) {
        QByteArray line = sourceFile->readLine();
        if(line.isNull()) {
            return -1;
        }
        QString string = sourceTextCodec->toUnicode(line);
        line = aimsTextCode->fromUnicode(string);
        if((currentWriteSize = aimsFile->write(line)) == -1)
            return -1;
        else
            completedSize += currentWriteSize;
    }
    emit setHundredPercent(currentFileNum/filePathList.size()*100);
    emit changeStatusBarInfo("正在转换的文件:"+sourceFile->fileName());
    return 0;
}

/*从源文件名中提取目标文件名，包括目标文件保存目录*/
QString Transcoding::extractFileName(QString extractSourceFileName, QString extractSaveDir) {
    int extractIndex;
    QString extractAimsFileName;

    extractIndex = extractSourceFileName.lastIndexOf('/');
    extractAimsFileName = extractSourceFileName.mid(extractIndex);
    extractAimsFileName = extractSaveDir + extractAimsFileName;

    return extractAimsFileName;
}

void Transcoding::resolveFilePathStr(QString filePathStr) {
    int strIndex = 0;
    if(filePathStr.isEmpty())
        return;

    while((strIndex = filePathStr.indexOf(',')) != -1) {
        QString str = filePathStr.left(strIndex);       //从左向右截取interceptLen个字符
        str = str.mid(8);
        addFileListModel(str);
        filePathStr = filePathStr.mid(strIndex + 1);
    }
    addFileListModel(filePathStr.mid(8));
}

void Transcoding::getDirPath(QString getDir) {
    getDir = getDir.mid(8);     //去掉文件路径前的"file:///"

    QDir qDir(getDir);
    if (!qDir.exists()) {   //判断目录是否存在
        emit errorInfo("此目录不存在");
        return;
    }

    /*获取getDir目录内的所有文件，不包括目录*/
    qDir.setFilter(QDir::Files);
    QFileInfoList fileInfoList= qDir.entryInfoList();
    if(fileInfoList.isEmpty()) {
        emit errorInfo("此文件夹中没有任何文件");
        return;
    }
    for(int index =0; index < fileInfoList.size(); index++) {
        addFileListModel(fileInfoList.at(index).absoluteFilePath());
    }
}

void Transcoding::removeFileList(int index) {
    filePathList.removeAt(index);
    emit setHundredPercent(0);
}

void Transcoding::removeFileList() {
    filePathList.clear();
    emit setHundredPercent(0);

}

void Transcoding::addFileListModel(QString filePathString) {
    emit appendFile(filePathString);
    filePathList.append(filePathString);
    emit setHundredPercent(0);
}
