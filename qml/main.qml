import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Layouts 1.1
import QtQuick.Dialogs 1.2

ApplicationWindow {
    id: scene1
    title: qsTr("Chess")
    visible: true
    width: 800
    height: 600
    
    property int squareSize: 70

    property var images: [
      {'imgPath' : "/images/white_pawn.svg"},
      {'imgPath' : "/images/black_pawn.svg"},
      {'imgPath' : "/images/white_rook.svg"},
      {'imgPath' : "/images/black_rook.svg"},
      {'imgPath' : "/images/white_knight.svg"},
      {'imgPath' : "/images/black_knight.svg"},
      {'imgPath' : "/images/white_bishop.svg"},
      {'imgPath' : "/images/black_bishop.svg"},
      {'imgPath' : "/images/white_king.svg"},
      {'imgPath' : "/images/black_king.svg"},
      {'imgPath' : "/images/white_queen.svg"},
      {'imgPath' : "/images/black_queen.svg"},
    ]

    Item {
      id: gameBoard
      x: 0
      y: 0
      width : logic.boardSize * squareSize
      height: logic.boardSize * squareSize
      
      Image {
        source: "/images/chess_board.jpg"
        height: gameBoard.height
        width: gameBoard.width
      }
    }
    Item {
          id: playScene
          visible: false
          width: 800
          height: 600

          Repeater {
            model: logic
            Image {
              height: squareSize
              width : squareSize

              visible: (isActive)? true:false
              x: squareSize * positionX
              y: squareSize * positionY

              source: images[type].imgPath

              MouseArea {
                anchors.fill: parent
                drag.target: parent


                property int startX: 0
                property int startY: 0

                visible: (logic.isMyTurn(type))?true:false

                onPressed: {
                  startX = parent.x;
                  startY = parent.y;
                }

                onReleased: {
                  var fromX = startX / squareSize;
                  var fromY = startY / squareSize;
                  var toX   = (parent.x + mouseX) / squareSize;
                  var toY   = (parent.y + mouseY) / squareSize;
                  logic.move(fromX, fromY, toX, toY);

                  if (!logic.isWKing()) {
                       messageDialogText.text ="Black player win";
                       messageDialog.visible = true;
                  }
                  else if (!logic.isBKing()){
                      messageDialogText.text ="White player win";
                      messageDialog.visible = true;
                  }

                }
              }
            }
          }
    }
    Item {
          id: loadScene
          visible: false
          width: 800
          height: 600

          Repeater {
            model: logic
            Image {
              height: squareSize
              width : squareSize

              visible: (isActive)? true:false
              x: squareSize * positionX
              y: squareSize * positionY
              source: images[type].imgPath

            }
          }
          Button {
            id: nextButton
            y:150
            x:570
            width: 220
            height: 50
           // anchors.right: parent.right
//            anchors.leftMargin: 10
//            anchors.rightMargin: 10
            text: "next"
            onClicked: {
               logic.makeNextStep();
            }
          }
          Button {
            id: prewButton
           // y: 150
//            anchors.left: gameBoard.right
//            anchors.right: nextButton.left
//            anchors.leftMargin: 10
//            anchors.rightMargin: 10
            y:225
            x:570
            width: 220
            height: 50
            text: "prev"
            onClicked: {
               logic.makePrevStep();
            }
          }
    }
    Button {
      id: startButton
      height: 50
      anchors.left: gameBoard.right
      anchors.right: parent.right
      anchors.leftMargin: 10
      anchors.rightMargin: 10
      text: "New game"
      onClicked: {
          playScene.visible = true;
          loadScene.visible = false;
          saveButton.visible = true;
          loadButton.enabled = false;
          mainMenubutton.visible = true;
          this.visible = false;
          loadButton.visible = false;
          prewButton.visible = false;
          nextButton.visible = false;
            logic.makeNewField();
            logic.clear();
      }
    }

    Button {
      id: loadButton
      y: 75
      height: 50
      enabled: false;
      anchors.left: gameBoard.right
      anchors.right: parent.right
      anchors.leftMargin: 10
      anchors.rightMargin: 10

      text: "load"

      onClicked: {
          saveButton.enabled = true;
          saveButton.visible = false;
          mainMenubutton.visible = false;
          loadScene.visible = true;
          prewButton.visible = true;
          nextButton.visible = true;
           logic.makeNewField();

      }
    }
    Button {
      id: mainMenubutton
      visible: false;
      height: 50
       anchors.left: gameBoard.right;
      anchors.right: parent.right
      anchors.leftMargin: 10
      anchors.rightMargin: 10
      text: "main"
      onClicked: {
           playScene.visible = false;
          startButton.visible = true;
          loadButton.visible = true;
          this.visible = false;
          saveButton.visible = false;
            logic.makeNewField();
      }
    }
    Button {
      id: saveButton
      y: 75
      height: 50
      anchors.left: gameBoard.right
      anchors.right: parent.right
      anchors.leftMargin: 10
      anchors.rightMargin: 10
      visible: false;

      text: "save"

      onClicked: {
            saveButton.enabled = false;
             loadButton.enabled = true;
      }
    }
    Button {
        id: exitButton
        y:500
        height: 50
        anchors.left: gameBoard.right
        anchors.right: parent.right
        anchors.leftMargin: 10
        anchors.rightMargin: 10
        text:"Exit"
        onClicked: {
           Qt.quit()
        }
    }
    Dialog {
        id: messageDialog
        width: 200
        height: 100
        title: "Game Over"
        standardButtons: StandardButton.Ok
        Text {
            anchors.centerIn: parent
            id: messageDialogText
        }
    }


}
