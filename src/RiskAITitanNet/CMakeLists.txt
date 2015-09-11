# Source files (*.cpp files)

set (SOURCE_FILES
  "${AI3_DIR}/RiskAI_TitanNet.cpp"
  "${AI3_DIR}/RiskMapEvaluator.cpp"
  "${AI3_DIR}/dllmain.cpp"
)

# Include files (*.h files)
set (HEADER_FILES
  "${AI3_DIR}/RiskAI_TitanNet.h"
  "${AI3_DIR}/RiskMapEvaluator.h"
)

# Include directories
set (INCLUDE_DIRS
  "${AI3_DIR}"
  "${SDK_DIR}"
)

# Qt UI files (*.ui files)
set (QT_UI_FILES_
)

# Qt Ressources files (*.qrc files)
set (QT_RESOURCES_FILES_
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
add_library (${RISK_AI3} SHARED
  ${SOURCE_FILES}
  ${HEADER_FILES}
  ${QT_UI_FILES}
  ${QT_RESOURCES_FILES}
)

# Linking with provided libraries
target_link_libraries (${RISK_AI3}
  ${RISK_SDK}
  ${QT_LIBRARIES}
)
