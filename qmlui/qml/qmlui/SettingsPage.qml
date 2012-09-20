// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1
import com.nokia.meego 1.0
import com.nokia.extras 1.0

Page {

    id: settingsPage

    tools: ToolBarLayout {
        visible: true

        ToolIcon {
            platformIconId: "toolbar-back"
            onClicked: appWindow.pageStack.pop()
        }
    }


    Flickable {
        id: settingsFlickable
        anchors.fill: parent

        interactive: true
        contentWidth: parent.width
        //contentHeight: settingsColumn.height + 30
        clip: true

        Column {
            id: settingsColumn
            spacing: 10
            width: parent.width - 40
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: parent.top
            anchors.topMargin: 20

            GroupSeparator {
                title: "Appearance"
            }
            Label {
                text: "Inverted theme"
                width: parent.width
                height: invertedThemeSwitch.height
                verticalAlignment: Text.AlignVCenter

                Switch {
                    id: invertedThemeSwitch
                    anchors.right: parent.right
                    checked: myObject.getThemeStatus()
                    onCheckedChanged: {
                        theme.inverted = !theme.inverted;
                        myObject.themeStatus(theme.inverted)
                        //console.debug("guardando setting")
                    }


                }
            }
            /*

            Label {
                text: "Use monospace font"
                width: parent.width
                verticalAlignment: Text.AlignVCenter

                Switch {
                    id: monospaceCheckbox
                    anchors.right: parent.right
                    checked: false


                }
            }



            Label {
                text: "Font size"
                width: parent.width
                height: fontSlider.height
                verticalAlignment: Text.AlignVCenter

                Slider {
                    id: fontSlider
                    minimumValue: 12
                    maximumValue: 40
                    stepSize: 1
                    width: 300
                    valueIndicatorVisible: true
                    //value: appSettings.fontSize
                    anchors.right: fontSliderLabel.left


                }
                Label {
                    id: fontSliderLabel
                    text: fontSlider.value
                    width: 50
                    anchors.right: parent.right
                    anchors.verticalCenter: parent.verticalCenter
                }
            }
            */
            GroupSeparator {
                title: "Installed dictionaries"
            }
            ListView {
                id: listViewDict
                width: parent.width;
                height: 510
                anchors { left: parent.left; right: parent.right}
                model: dictModel
                delegate: ListDelegate { }
                anchors.margins: 5
                clip: true
            }
            ScrollDecorator {
                flickableItem: listViewDict
            }

        }
    }

    ScrollDecorator {
        flickableItem: settingsFlickable
    }

}
/*


Page {
    tools: commonTools
    anchors.margins: UiConstants.DefaultMargin


    Button {
        id: botonInvertir
        width: parent.width;
        anchors {left: parent.left; right: parent.right;}
        text: "Invert theme";
        onClicked: {
            invertTheme()
            myObject.themeStatus(theme.inverted)
        }
    }

    Label {
        id: labelText
        text: qsTr("Installed dictionaries")
        anchors {top: botonInvertir.bottom; left:parent.left; right: parent.right}
        anchors.margins: 10
        height: 20
    }

    ListView {
        id: listViewDict
        width: parent.width;
        anchors {top: labelText.bottom; left: parent.left; right: parent.right; bottom: parent.bottom}
        model: dictModel
        delegate: ListDelegate {


            }
        anchors.margins: 5
        clip: true
    }



}
*/
