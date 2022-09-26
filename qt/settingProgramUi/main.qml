import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12

ApplicationWindow {

    visible : true
    width: 1200
    height: 900


    Material.background: "#323232"
    Material.accent: Material.Amber

    title: qsTr("TEST UI ver1.0")

    // default
    header: TabBar
    {
        id: tabBar
        currentIndex: swipeView.currentIndex

        width: parent.width


        Repeater
        {
            model: ["B1", "B2", "B3", "B4"]

            TabButton
            {
                text: modelData
            }
        }

    }


    // view index is currentIndex from Header
    SwipeView {
        id: swipeView
        currentIndex: tabBar.currentIndex
        anchors.fill: parent

        // if you want add pages, write the qml source path in model [] below
        // and "Add new File" as "qml" type. at that time, file name needed to be same below
        Repeater
        {
            model: ["/page1.qml","/page2.qml","/page3.qml","/page4.qml"]
            Loader
            {
                source: modelData
            }
        }

        width: parent.width
        height: parent.height
    }

    // pageIndicator is currentIndex from upper
    PageIndicator{
        id: indicator

        count: swipeView.count
        currentIndex: swipeView.currentIndex

        anchors.bottom: swipeView.bottom
        anchors.horizontalCenter: parent.horizontalCenter
    }



}

/*##^##
Designer {
    D{i:0;formeditorZoom:0.33}D{i:1}
}
##^##*/
