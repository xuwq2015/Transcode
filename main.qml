import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.2
import QtQuick.Window 2.2
//import UIComponents 1.0

ApplicationWindow {
    visible: true
    width: 640
    height: 430
    title: qsTr("文件编码转换工具")

    property int fileSize: 0
    property int hundredPercent: 0
    property string statusInfo

    Connections {
        target: transcoding
        onErrorInfo: messageDialog.show(errorInfoString)
        onAppendFile: libraryModel.append({"title":filePath, "status":"未完成"})
        onChangeStatus: {
            changeFileStatus(index, fileStatus)
        }
        onSetHundredPercent: setHundredPercent(num)
        onChangeStatusBarInfo: statusInfo = stringInfo
    }

    MessageDialog {
        id: messageDialog
        title: qsTr("May I have your attention, please?")

        function show(caption) {
            messageDialog.text = caption;
            messageDialog.open();
        }
    }

    FileDialog {
        id: fileDialog
        title: "Please choose a file"
        nameFilters: [ "文本文件(*.txt)", "All files (*)" ] //文件类型过滤器
        selectMultiple: true        //可选择多个文件
        onAccepted: {
            getFilePath(fileUrls)
        }
    }

    FileDialog {
        id: dirDialog
        title: "Please choose a file"
        selectFolder: true
        onAccepted: {
            getDirPath(fileUrl)
        }
    }
    FileDialog {
        id: saveDirFileDialog
        title: "Please choose a file"
        selectFolder: true          //可选文件夹
        onAccepted: {
            getSaveDirStr(fileUrl)
        }
    }
    Item {
        id: manWindow
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: statusBar.top
        ColumnLayout {
            anchors.fill: parent
            RowLayout {
                spacing: 8
                TableView {
                    id: tableView
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    //headerVisible: false
                    TableViewColumn {
                        role: "title"
                        width: tableView.width - 102
                        title: "文件名"
                    }
                    TableViewColumn {
                        role: "status"
                        width: 100
                        resizable: false    //不可调整大小
                        title: "当前状态"
                    }
                    model: libraryModel
                }
                ColumnLayout {
                    Layout.alignment: Qt.AlignCenter
                    Layout.rightMargin:8
                    spacing: 10
                    Button {
                        id: button3
                        text: "添加文件"
                        checkable: false
                        onClicked: fileDialog.open()
                    }
                    Button {
                        id: button4
                        text: "添加文件夹"
                        checkable: false
                        onClicked: dirDialog.open()
                    }
                    Button {
                        id: button6
                        text: "删除文件"
                        checkable: false
                        onClicked: {
                            if(tableView.currentRow === -1)
                                messageDialog.show("请选择要删除的文件")
                            else{
                                libraryModel.remove(tableView.currentRow)
                                transcoding.removeFileList(tableView.currentRow)
                            }
                        }
                    }
                    Button {
                        id: button5
                        text: "全部删除"
                        checkable: false
                        onClicked: {
                            libraryModel.clear()
                            transcoding.removeFileList()
                        }
                    }
                    Button {
                        id: button1
                        text: "开始转换"
                        onClicked: {
                            transcoding.startTarnscode(saveDirTextField.text, sourceCodeComboBox.currentText,
                                                              aimsCodeComboBox.currentText)
                        }
                    }
                    Button {
                        id: button2
                        text: "退出"
                        onClicked: Qt.quit()
                    }
                }
            }
            RowLayout {
                Label {
                    Layout.leftMargin: 6
                    text: "源文件编码:"
                }
                ComboBox {
                    id: sourceCodeComboBox
                    Layout.rightMargin: 8
                    model: [ "UTF-8", "UTF-16", "UTF-16BE", "UTF-16LE", "UTF-32", "GB18030", "Big5" ]
                    editable: true
                }
                Label {
                    text: "目标文件编码:"
                }
                ComboBox {
                    id: aimsCodeComboBox
                    model: [ "UTF-8", "UTF-16", "UTF-16BE", "UTF-16LE", "UTF-32", "GB18030", "Big5" ]
                    editable: true
                }
            }
            RowLayout {
                Layout.bottomMargin: 6
                Label {
                    id: progressBarLabel
                    Layout.leftMargin: 6
                    text: "转换进度:"
                }
                ProgressBar {
                    id: progressBar
                    Layout.fillWidth: true
                }
                Label {
                    id: hundredPercentLabel
                    text: hundredPercent + '%'
                    Layout.rightMargin: 8
                }
            }
            RowLayout {
                Layout.bottomMargin: 6
                Label {
                    id: saveDirLabel
                    Layout.leftMargin: 6
                    text: "保存目录:"
                }
                TextField {
                    id: saveDirTextField
                    Layout.fillWidth: true
                }
                Button {
                    id: saveDirButton
                    text: "选择"
                    Layout.rightMargin: 8
                    onClicked: saveDirFileDialog.open()
                }
            }
        }
    }
    StatusBar {
        id: statusBar
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        Label {
            id: statusBarLabel
            anchors.fill: parent
            text: "转换工具:" + statusInfo
        }
    }

    ListModel {
        id: libraryModel
    }
    function getFilePath(fileUrls) {
        var filepath = new String(fileUrls);
        transcoding.resolveFilePathStr(filepath)
    }
    function getDirPath(fileUrl) {
        var filepath = new String(fileUrl);
        transcoding.getDirPath(filepath);
    }
    function getSaveDirStr(fileUrl) {
        var filepath = new String(fileUrl)
        saveDirTextField.text=filepath.slice(8)
    }
    function changeFileStatus(index, fileStatus) {
        if(fileStatus == 0) {
            libraryModel.setProperty(index, "status", "已完成")
        }else if(fileStatus == -1) {
            libraryModel.setProperty(index, "status", "转换失败")
        }
    }
    function setHundredPercent(num) {
        hundredPercent = num
        progressBar.value = num / 100
    }
}
