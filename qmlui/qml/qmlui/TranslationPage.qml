// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1
import com.nokia.meego 1.0


Page {

    function setTranslation(msg) {
        translationView.text = msg;
        flickabe_item.contentY = 0;
        //console.log(msg);
    }

    function setFirst(w) {
        // nWords = "%%"+w;
        currentWord = w;
        backWord.visible = false;
    }
    property string nWords: "";
    property string currentWord: ""

    tools:     ToolBarLayout {

        ToolIcon {
            id: backIcon
            platformIconId: "toolbar-back"
            onClicked: {
                nWords = "";
                appWindow.pageStack.pop();
            }
        }


        ToolIcon {
            id: backWord
            visible: false
            platformIconId: "toolbar-previous"
            anchors.right: (parent === undefined) ? undefined : parent.right
            onClicked: {
                if (nWords.length > 0) {
                    var word = nWords.substring(nWords.lastIndexOf("%%")+2);
                    currentWord = word;
                    translationPage.setTranslation(myObject.translate(word))
                    nWords = nWords.substring(0, nWords.lastIndexOf("%%"));
                    //console.log(nWords + "  - "+ word)
                }
            }

        }


    }
    anchors.margins: UiConstants.DefaultMargin
    PinchArea {
        property real initHeight: 0

        anchors.fill: parent
        onPinchStarted: {
            initHeight = translationView.font.pixelSize
        }
        onPinchUpdated: {
            translationView.font.pixelSize = initHeight * pinch.scale
        }
        Flickable {
            id: flickabe_item
            anchors.fill: parent
            height: parent.height
            contentWidth: parent.width
            contentHeight: translationView.height


            Label {
                id: translationView
                anchors.left: parent.left
                anchors.right: parent.right
                text: ""
                wrapMode: Text.WordWrap
                font.pointSize: 15
                onLinkActivated: {
                    //console.log(link);

                    if (link.indexOf("http://") >= 0 || link.indexOf("ftp://") >= 0 || link.indexOf("https://") >= 0 ) {

                        //console.log("Visitar "+link)
                        Qt.openUrlExternally(link);
                        return;
                    }
                    var word = "";
                    if (link.indexOf("quick?") === 0) {
                        word = link.substring(6)
                    } else {
                        word = link;

                    }
                    console.log("Buscando "+word)

                    var translation = myObject.translate(word);
                    //if (currentWord != word) {
                    nWords +=  "%%" + currentWord;
                    currentWord = word;
                    //}
                    backWord.visible = true;

                    translationPage.setTranslation(translation)
                }
            }

        }
    }
    ScrollDecorator {
        flickableItem: flickabe_item
        anchors.right: parent.right
    }
}
