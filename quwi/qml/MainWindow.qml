import QtQuick.Window 2.2
import QtQuick.Controls 2.12
import QtQuick 2.15
import QtQml.StateMachine 1.15 as SM
import QtQuick.Layouts 1.15
import QtGraphicalEffects 1.0

import com.quwi.api 1.0
import com.quwi.components 1.0

ApplicationWindow {
    id: window

    x: width / 2
    y: height / 2

    width: Screen.desktopAvailableWidth / 2
    height: Screen.desktopAvailableHeight / 2

    visible: true

    StackView {
        id: stackView

        initialItem: projectsViewComponent
        anchors.fill: parent
    }

    Item {
        id: projectItem

        property var id
        property var status
        property var users
        property string name
    }

    Component {
        id: projectsViewComponent

        ListView {
            id: projectsView

            Component.onCompleted: {
                if(projectsState.active) {
                    apiManager.enlistUserProjects()
                }
            }

            spacing: 5

            //anchors.fill: parent

            Connections {
                target: apiManager

                function onProjectsReceivedV(projects) {
                    projectModel.insertProjectsV(projects)
                }

                function onProjectsUpdated() {
                    apiManager.enlistUserProjects()
                }
            }

            model: ProjectModel {
                id: projectModel
            }

            delegate: Component{
                Item {
                    id: delegateItem

                    width: ListView.view.width
                    height: 40

                    states: [
                        State {
                            name: "HIGHLIGHT"
                            PropertyChanges { target: itemRectangle; color: "#0A000000" }
                        },
                        State {
                            name: "CLEAN"
                            PropertyChanges { target: itemRectangle; color: "#00000000"}
                        }
                    ]

                    Rectangle {
                        id: itemRectangle

                        anchors.fill: parent

                        Row {
                            anchors.fill: parent

                            spacing: 5

                            /*Image {
                                width: delegateItem.height
                                height: delegateItem.height
                                fillMode: Image.PreserveAspectFit
                                source: logourl
                            }*/

                            Item {
                                width: delegateItem.width / 4
                                height: delegateItem.height

                                Image {
                                    width: delegateItem.height
                                    height: delegateItem.height
                                    fillMode: Image.PreserveAspectFit
                                    source: logourl

                                    layer.enabled: true
                                    layer.effect: OpacityMask {
                                        maskSource: mask
                                    }

                                    Rectangle {
                                        id: mask

                                        width: delegateItem.height
                                        height: delegateItem.height
                                        radius: delegateItem.height / 2
                                        visible: false
                                    }
                                }

                            }

                            Text {
                                width: delegateItem.width / 4
                                height: delegateItem.height

                                text: name
                            }

                            Text {
                                width: delegateItem.width / 4
                                height: delegateItem.height

                                text: status
                            }

                            Column {
                                width: delegateItem.width / 4
                                height: delegateItem.height

                                Text {
                                    text: timespentweek
                                }

                                Text {
                                    text: timespentmonth
                                }

                                Text {
                                    text: timespenttotal
                                }
                            }
                        }
                    }

                    MouseArea {
                        id: delegateItemArea

                        anchors.fill: parent

                        hoverEnabled: true

                        onClicked: {
                            stackView.replace(projectPropertiesComponent)

                            console.log("clicked id: " + id)

                            projectItem.id = id
                            projectItem.name = name
                            projectItem.status = status
                        }

                        onEntered: {
                            parent.state = "HIGHLIGHT"
                        }

                        onExited: {
                            parent.state = "CLEAN"
                        }
                    }
                }
            }
        }
    }

    Component {
        id: projectPropertiesComponent

        Item {
            id: projectPropertiesItem

            ColumnLayout {
                anchors.fill: parent

                RowLayout {
                    Text {
                        id: statusText

                        text: projectItem.status
                    }

                    Switch {
                        id: statusSwitcher

                        enabled: false

                        position: {
                            return projectItem.status === "Active" ? 1 : 0;
                        }
                    }
                }

                RowLayout {
                    Text {
                        id: nameText

                        text: "Name:"
                    }

                    TextField {
                        id: projectName

                        placeholderText: "Project Name"
                        text: projectItem.name
                    }
                }

                Flow {
                    spacing: 5

                    Text { text: "Users:";}
                }


                Button {
                    text: "Cancel"

                    onClicked: {
                        console.log("Cancel")

                        stackView.replace(projectsViewComponent)
                    }
                }

                Button {
                    text: "Save"

                    onClicked: {
                        console.log("Save")

                        apiManager.updateProjectName(projectItem.id, projectName.text)
                        stackView.replace(projectsViewComponent)
                    }
                }

                Item {
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    Rectangle {
                        anchors.fill: parent
                        color: "#00000000"
                    }
                }
            }
        }
    }

    APIManager {
        id: apiManager

        onErrorOccured: {
            console.log(errorString);
            errorText.text = errorString
        }
    }

    SM.StateMachine {
        id: stateMachine

        initialState: loginState
        running: true

        SM.State {
            id: loginState

            onEntered: {
                if(!authorizationDialog.visible) {
                    authorizationDialog.open()
                }

                errorText.visible = false
                stackView.visible = false
            }

            SM.SignalTransition {
                targetState: errorState
                signal: apiManager.errorOccured
            }

            SM.SignalTransition {
                targetState: projectsState
                signal: apiManager.loginOccured
            }
        }

        SM.State {
            id: errorState

            onEntered: {
                errorText.visible = true
            }

            SM.SignalTransition {
                targetState: loginState
                signal: loginInput.textChanged
            }

            SM.SignalTransition {
                targetState: loginState
                signal: passwordInput.textChanged
            }

        }

        SM.FinalState {
            id: projectsState

            onEntered: {
                if(authorizationDialog.opened) {
                    authorizationDialog.close()
                }

                apiManager.enlistUserProjects()

                stackView.visible = true

                stateMachine.running = false
            }
        }
    }

    Popup {
        id: authorizationDialog

        anchors.centerIn: parent

        width: parent.width / 2
        height: parent.height / 2

        modal: true
        focus: true
        closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutsideParent

        ColumnLayout {
            anchors.fill: parent

            spacing: 2

            Text {
                text: "LOGIN"
            }

            TextField {
                id: loginInput

                Layout.fillWidth: parent

                placeholderText: "johndoe@company.com"
            }

            TextField {
                id: passwordInput

                Layout.fillWidth: parent

                placeholderText: "password"
            }

            Button {
                id: checkCredentialsButton

                text: "LOGIN"

                onClicked: {
                    apiManager.authorizeUser(loginInput.text, passwordInput.text)
                }
            }

            Text {
                id: errorText

                visible: false

                color: "#FF0000"
            }
        }
    }
}
