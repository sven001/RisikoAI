# Source files (*.cpp files)
set (SOURCE_FILES
  "${GAME_DIR}/main.cpp"
  "${GAME_DIR}/DialogAttack.cpp"
  "${GAME_DIR}/DialogMove.cpp"
  "${GAME_DIR}/DialogStartAIContest.cpp"
  "${GAME_DIR}/DialogStartGame.cpp"
  "${GAME_DIR}/GraphItemConnection.cpp"
  "${GAME_DIR}/GraphItemLand.cpp"
  "${GAME_DIR}/HumanPlayerGUI.cpp"
  "${GAME_DIR}/PlayerAI.cpp"
  "${GAME_DIR}/PlayerHuman.cpp"
  "${GAME_DIR}/PlayerNeutral.cpp"
  "${GAME_DIR}/QGameFlowViewer.cpp"
  "${GAME_DIR}/risiko.cpp"
  "${GAME_DIR}/RiskFlowControl.cpp"
  "${GAME_DIR}/RiskMapGraphicsView.cpp"
  "${GAME_DIR}/RiskMapScene.cpp"
)

# Include files (*.h files)
set (HEADER_FILES
  "${GAME_DIR}/DialogAttack.h"
  "${GAME_DIR}/DialogMove.h"
  "${GAME_DIR}/DialogStartAIContest.h"
  "${GAME_DIR}/DialogStartGame.h"
  "${GAME_DIR}/GraphItemConnection.h"
  "${GAME_DIR}/GraphItemLand.h"
  "${GAME_DIR}/HumanPlayerGUI.h"
  "${GAME_DIR}/PlayerAI.h"
  "${GAME_DIR}/PlayerHuman.h"
  "${GAME_DIR}/PlayerNeutral.h"
  "${GAME_DIR}/QGameFlowViewer.h"
  "${GAME_DIR}/risiko.h"
  "${GAME_DIR}/RiskFlowControl.h"
  "${GAME_DIR}/RiskMapGraphicsView.h"
  "${GAME_DIR}/RiskMapScene.h"
  "${SDK_DIR}/RiskDefines.h"
)

# Qt UI files (*.ui files)
set (QT_UI_FILES
   "${GAME_DIR}/DialogAttack.ui"
   "${GAME_DIR}/DialogMove.ui"
   "${GAME_DIR}/DialogStartAIContest.ui"
   "${GAME_DIR}/DialogStartGame.ui"
   "${GAME_DIR}/HumanPlayerGUI.ui"
   "${GAME_DIR}/QGameFlowViewer.ui"
   "${GAME_DIR}/risiko.ui"
   "${GAME_DIR}/RiskMapScene.ui"
)

# Qt Ressources files (*.qrc files)
set (QT_RESOURCES_FILES
    "${GAME_DIR}/risiko.qrc"
)

# Include directories
set (INCLUDE_DIRS
  "${GAME_DIR}"
  "${SDK_DIR}"
)

# Find the QtWidgets library
find_package(Qt5Core)
find_package(Qt5Gui)
find_package(Qt5Widgets)

set (QT_LIBRARIES
	Qt5::Core
	Qt5::Gui
	Qt5::Widgets
)

# Setting include directories
include_directories (${INCLUDE_DIRS})

add_executable (${RISK_GAME}
  ${SOURCE_FILES}
  ${HEADER_FILES}
  ${QT_UI_FILES}
  ${QT_RESOURCES_FILES}
)

# Linking with provided libraries
target_link_libraries (${RISK_GAME}
  ${RISK_SDK}
  ${QT_LIBRARIES}
)
