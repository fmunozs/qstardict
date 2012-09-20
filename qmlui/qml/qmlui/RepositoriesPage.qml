// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1
import com.nokia.meego 1.0
import com.nokia.extras 1.0

Page {
    tools:  ToolBarLayout {

        ToolIcon {
            id: backIcon
            platformIconId: "toolbar-back"
            onClicked: {
                appWindow.pageStack.pop();
            }
        }

    }
    anchors.margins: UiConstants.DefaultMargin
    property string urlDict: ""
    property string nameDict: ""


    QueryDialog {
        id: queryError
        titleText: qsTr("Error")
        message: qsTr("Please wait for the current download to finish.")
        acceptButtonText: qsTr("Ok")
    }

    QueryDialog {
        id: query
        titleText: qsTr("Confirmation")
        message: qsTr("Do you want to install this dictionary?")
        acceptButtonText: qsTr("Yes")
        rejectButtonText: qsTr("No")
        onAccepted: {

            if (!myObject.download(nameDict, urlDict)) {
                queryError.open()
            }

        }
    }

    Label {
        id: labelText
        text: qsTr("Download dictionaries")
        anchors {left:parent.left; right: parent.right}
        anchors.margins: UiConstants.DefaultMargin

        height: 50
    }

    XmlListModel {
        id: xmlModel
        source: "http://apps.beford.net/qstardict/stardict.xml"
        query: "/rss/channel/item"

        XmlRole { name: "title"; query: "title/string()" }
        XmlRole { name: "subtitle"; query: "description/string()" }
        XmlRole { name: "link"; query: "link/string()" }
    }

    ListView {
        id: listViewDict
        anchors {top: labelText.bottom; left: parent.left; right: parent.right; bottom: parent.bottom}
        model: xmlModel
        delegate: ListDelegate {
            onClicked: {
                var var1 = xmlModel.get(index)
                nameDict = var1.title
                urlDict = var1.link
                query.open()
            }
        }
        anchors.margins: 5
        clip: true
    }


    ScrollDecorator {
        flickableItem: listViewDict
    }

}
