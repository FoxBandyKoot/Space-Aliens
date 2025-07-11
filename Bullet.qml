import QtQuick

Item {
    id: bullet
    width: 4
    height: 12
    x: model.x
    y: model.y
    //anchors.horizontalCenter: parent.horizontalCenter

    Image{
        id: bullet_img
        anchors.fill: parent
        source: "qrc:/bullets/bullet.png"
    }


}
