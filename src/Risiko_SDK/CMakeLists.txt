# Source files (*.cpp files)
set (SOURCE_FILES
  "${SDK_DIR}/Player.cpp"
  "${SDK_DIR}/RiskAI.cpp"
  "${SDK_DIR}/RiskCard.cpp"
  "${SDK_DIR}/RiskCardDeck.cpp"
  "${SDK_DIR}/RiskGameLogic.cpp"
  "${SDK_DIR}/RiskMap.cpp"
  "${SDK_DIR}/RiskMapConnection.cpp"
  "${SDK_DIR}/RiskMapContinent.cpp"
  "${SDK_DIR}/RiskMapLand.cpp"
)

# Include files (*.h files)
set (HEADER_FILES
  "${SDK_DIR}/Player.h"
  "${SDK_DIR}/RiskAI.h"
  "${SDK_DIR}/RiskCard.h"
  "${SDK_DIR}/RiskCardDeck.h"
  "${SDK_DIR}/RiskGameLogic.h"
  "${SDK_DIR}/RiskMap.h"
  "${SDK_DIR}/RiskMapConnection.h"
  "${SDK_DIR}/RiskMapContinent.h"
  "${SDK_DIR}/RiskMapLand.h"
  "${SDK_DIR}/RiskDefines.h"
)

# Include directories
set (INCLUDE_DIRS
  "${SDK_DIR}"
)

# Qt UI files (*.ui files)
set (QT_UI_FILES_
)

# Qt Ressources files (*.qrc files)
set (QT_RESOURCES_FILES_
)

# External libraries
set (EXT_LIBRARIES
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

# Binary file definition
add_library (${RISK_SDK} STATIC
  ${SOURCE_FILES}
  ${HEADER_FILES}
  ${QT_UI_FILES}
  ${QT_RESOURCES_FILES}
)

# Linking with provided libraries
target_link_libraries (${RISK_SDK}
  ${EXT_LIBRARIES}
  ${QT_LIBRARIES}
)
