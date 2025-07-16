import QtQuick

Item {
    property var ePng: [
        "qrc:/enemies/enemy1.png",
        "qrc:/enemies/enemy2.png",
        "qrc:/enemies/enemy3.png",
        "qrc:/enemies/enemy4.png",
        "qrc:/enemies/enemy5.png"
    ]

    property int eIndex: Math.floor(Math.random() * ePng.length)

    id: enemy
    width: 45
    height: 45
    x: model.x
    y: model.y

    Image{
        id: image
        anchors.fill: parent
        source: ePng[model.m_index]
        visible: !model.m_is_destroyed
        Connections{
            target: controller
            function onEnemy_changed(){
                image.visible = false;
            }
        }
    }
}
