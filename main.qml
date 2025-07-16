import QtQuick


Window {
    property string p_font_family: p_medium.status === FontLoader.Ready ? p_medium.font.family : p_medium.font.Serif

    width: Screen.desktopAvailableWidth
    height: Screen.desktopAvailableHeight
    visible: true
    title: qsTr("Space Aliens")
    color: "darkred"

    // Check when font is loaded
    State { name: 'loaded'; when: p_medium.status == FontLoader.Ready }
    FontLoader
    {
        id: p_medium
        source: "qrc:/fonts/fonts/Doto-ExtraBold.ttf"
    }

    Rectangle
    {
        id: game_background
        anchors.fill: parent
        visible: true
        gradient: Gradient {
            GradientStop { position: 0.0; color: "black" }
            GradientStop { position: 0.3; color: "darkgrey" }
            GradientStop { position: 0.7; color: "darkorange" }
            GradientStop { position: 1.0; color: "darkred" }
        }
        opacity: 0.85
    }

    Connections{
        target : controller
        function onGame_over(){ game_over_overlay.visible = true;}
        function onGame_restart(){ game_over_overlay.visible = false;}
    }

    Rectangle{
        id: game_over_overlay
        anchors.fill: parent
        visible: false
        color: "black"
        opacity: 0.8
        z: 1001
        Text{
            id: game_over_txt
            text: p_medium.status === FontLoader.Ready ? qsTr("Game over!") : qsTr("No font loaded!")
            color: "white"
            font.family: p_font_family
            font.pixelSize: 30
            bottomPadding: 150
            anchors.centerIn: parent
        }
        Column
        {
            spacing: 20
            anchors.centerIn: parent
            Row{
                spacing: 20
                Rectangle{
                    id: close_btn
                    width: 300
                    height: 50
                    color: "gray"
                    radius: 20
                    Text{
                        id: txt_quit
                        text: p_medium.status === FontLoader.Ready ? qsTr("Quit") : qsTr("No font loaded!")
                        font.family: p_medium.font.family
                        color: "white"
                        anchors.centerIn: parent
                        font.pixelSize: 30

                    }
                    MouseArea{
                        hoverEnabled: true
                        anchors.fill: parent
                        onEntered: close_btn.color = "blue"
                        onExited: close_btn.color = "gray"
                        onClicked: Qt.quit()
                    }
                }
                Rectangle{
                    id: restart_btn
                    width: 300
                    height: 50
                    radius: 20
                    color: "gray"
                    Text{
                        id: txt_restart
                        text: p_medium.status === FontLoader.Ready ? qsTr("Restart") : qsTr("No font loaded!")
                        font.family: p_medium.font.family
                        font.pixelSize: 30
                        color: "white"
                        anchors.centerIn: parent
                    }
                    MouseArea{
                        hoverEnabled: true
                        anchors.fill: parent
                        onEntered: restart_btn.color = "blue"
                        onExited: restart_btn.color = "gray"
                        onClicked: controller.restartGame()
                    }
                }
            }
        }
    }

    Rectangle {
        id: player
        width: 85
        height: 85
        x: controller ? controller.x : 100
        y: controller ? controller.y : 200
        visible: true
        focus: true
        color: "transparent"

        Image {
            id: player_img
            width: 55
            height: 55
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: parent.top
            source : "qrc:/player/rocket.png"
        }

        AnimatedImage {
            id: player_thruster
            width: 30
            height: 30
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: player_img.bottom
            source : "qrc:/player/thruster.gif"
            playing: true
        }

        Keys.onPressed: (event) => {
                            if(event.key === Qt.Key_Left) controller.moveLeft()
                            if(event.key === Qt.Key_Right) controller.move_right()
                            if(event.key === Qt.Key_Up) controller.applyThrust()
                            if(event.key === Qt.Key_Down) controller.moveDown()
                            if(event.key === Qt.Key_Space) controller.fireBullet()
                            player_thruster.playing = true
                        }
        Keys.onReleased: (event) => {
                            if(event.key === Qt.Key_Left || event.key === Qt.Key_Right){
                                controller.stop_movement();
                            }
                            player_thruster.playing = true
                        }


    }

    Text{
        id: score_board
        text: p_medium.status === FontLoader.Ready ? "Score: " + controller.show_score() : qsTr("No font loaded!")
        font.family: p_medium.font.family
        font.pixelSize: 40
        color: "white"
        x: 50
        y: 50
        Connections{
            target: controller
            function onScore_changed(){ score_board.text = "Score: " + controller.show_score() }
        }
    }

    Repeater{
        model: controller ? controller.Q_list_bullets : 0
        delegate: Bullet{}
    }
    Repeater{
        model: controller ? controller.Q_list_enemies : 0
        delegate: Enemy{}
    }
}
