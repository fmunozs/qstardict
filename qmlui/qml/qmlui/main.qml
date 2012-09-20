import QtQuick 1.1
import com.nokia.meego 1.0

PageStackWindow {
    id: appWindow
    initialPage: mainPage


    Component.onCompleted: startupFunction();

    function startupFunction() {
        if (myObject.firstRun()) {
            queryWelcome.open();
        }

        if (myObject.getThemeStatus()) {
            //console.log("invertir tema???")
            theme.inverted = true;
        }

        return true
    }



    QueryDialog {
        id: query
        icon: "file:///usr/share/icons/hicolor/80x80/apps/qmlui80.png"
        titleText: qsTr("QStarDict")
        message: qsTr("© 2012\nLicensed under GPLv2")
        acceptButtonText: qsTr("Website")
        rejectButtonText: qsTr("Close")
        onAccepted: Qt.openUrlExternally("http://apps.beford.net/qstardict")
    }

    QueryDialog {
        id: queryWelcome
        objectName: "queryWelcome"
        titleText: qsTr("Welcome")
        message: qsTr("Please remember to rate the application. If you have any problem please contact us to get it sorted.")
        acceptButtonText: qsTr("Go Rate")
        rejectButtonText: qsTr("Later")
        onAccepted: Qt.openUrlExternally("http://store.ovi.com/content/246404")
    }

    MainPage {
        id: mainPage
    }

    SettingsPage {
        id: settingsPage
    }

    TranslationPage {
        id: translationPage
    }

    RepositoriesPage {
        id: repositoriesPage
    }

    ToolBarLayout {
        id: commonTools


        ToolIcon {
            platformIconId: "toolbar-view-menu"
            anchors.right: (parent === undefined) ? undefined : parent.right
            onClicked: (myMenu.status === DialogStatus.Closed) ? myMenu.open() : myMenu.close()
        }

    }

    Menu {
        id: myMenu
        visualParent: pageStack
        MenuLayout {
            MenuItem {
                text: qsTr("Settings")
                onClicked: {
                    appWindow.pageStack.push(settingsPage)
                }
            }
            MenuItem {
                text: qsTr("More dictionaries")
                onClicked: {
                    appWindow.pageStack.push(repositoriesPage)
                }
            }
            MenuItem {
                text: qsTr("About")
                onClicked: {
                    query.open();
                }
            }
        }
    }
}
