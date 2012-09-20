import QtQuick 1.1
import com.nokia.meego 1.0
import com.nokia.extras 1.0


Page {
    tools: commonTools
    anchors.margins: UiConstants.DefaultMargin
    Component.onCompleted: {
        textField.forceActiveFocus();
//        textField.openSoftwareInputPanel();
    }

    SipAttributes {
        id: sipAttributes
        actionKeyLabel: "Search"
        actionKeyHighlighted: true
        actionKeyEnabled: false
    }

    TextField {
        id: textField
        visible: true
        platformSipAttributes: sipAttributes
        placeholderText: qsTr("Search")
        platformStyle: TextFieldStyle {}
        anchors {left: parent.left; right: parent.right;}
        inputMethodHints: Qt.ImhNoPredictiveText | Qt.ImhPreferLowercase | Qt.ImhNoAutoUppercase
        onTextChanged: {
            if (text.length == 0) {
                sipAttributes.actionKeyEnabled = false
            } else {
                if (text.length > 2) {
                    myObject.searchWord(text)
                } else {
                    sipAttributes.actionKeyEnabled = true
                }
            }
        }

        onFocusChanged: textField.selectAll()


        Keys.onReturnPressed:
        {
            translationPage.setFirst(text);
            translationPage.setTranslation(myObject.translate(text));
            appWindow.pageStack.push(translationPage)
        }

        Image {
            anchors.right: parent.right
            smooth: true
            fillMode: Image.PreserveAspectFit
            anchors.verticalCenter: parent.verticalCenter
            source: textField.text ? "image://theme/icon-m-input-clear" + (theme.inverted ? "-inverse" : "") : "image://theme/icon-m-common-search" + (theme.inverted ? "-inverse" : "")
            MouseArea {
                anchors.fill: parent
                anchors.margins: -10
                onClicked: {
                    if (textField.text != "") {
                        textField.text = ""
                        myObject.clearWords()
                    }
                }
            }
        }
    }



    ListView {
        id: listView
        clip: true
        anchors { top: textField.bottom; left: parent.left; right: parent.right; bottom: parent.bottom }
        anchors.topMargin: UiConstants.DefaultMargin
        model: myModel
        visible: true
        delegate: simpleDelegate

        Component {
            id: simpleDelegate
            Rectangle {
                height: UiConstants.ListItemHeightDefault
                radius: 10
                anchors.margins: UiConstants.DefaultMargin
                width: listView.width
                color: theme.inverted ? ((index % 2 == 0)?"#222":"#111") : ((index % 2 == 0)?"#eee":"#ddd")
                Label {
                    text: title
                    anchors.verticalCenter: parent.verticalCenter
                    font.bold: true
                    anchors.margins: UiConstants.DefaultMargin
                    anchors.left: parent.left
                }
                Image {
                    source: "image://theme/icon-m-common-drilldown-arrow" + (theme.inverted ? "-inverse" : "")
                    anchors.right: parent.right
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.margins: UiConstants.DefaultMargin
                }
                MouseArea {
                    anchors.fill: parent

                    onClicked: {
                        translationPage.setFirst(title);
                        translationPage.setTranslation(myObject.translate(title))
                        appWindow.pageStack.push(translationPage)
                    }
                }
            }
        }



    }
    ScrollDecorator {
        flickableItem: listView
    }

}
