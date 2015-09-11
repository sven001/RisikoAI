# Source files (*.cpp files)

set (SOURCE_FILES
  "${AI1_DIR}/RiskAI_NeuroTitan.cpp"
  "${AI1_DIR}/RiskMapEvaluator.cpp"
  "${AI1_DIR}/NeuralNet.cpp"
  "${AI1_DIR}/SMatrix.cpp"
  "${AI1_DIR}/SVector.cpp"
  "${AI1_DIR}/dllmain.cpp"
)

# Include files (*.h files)
set (HEADER_FILES
  "${AI1_DIR}/RiskAI_NeuroTitan.h"
  "${AI1_DIR}/RiskMapEvaluator.h"
  "${AI1_DIR}/NeuralNet.h"
  "${AI1_DIR}/SMatrix.h"
  "${AI1_DIR}/SVector.h"
)

# Include directories
set (INCLUDE_DIRS
  "${AI1_DIR}"
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
add_library (${RISK_AI1} SHARED
  ${SOURCE_FILES}
  ${HEADER_FILES}
  ${QT_UI_FILES}
  ${QT_RESOURCES_FILES}
)

# Linking with provided libraries
target_link_libraries (${RISK_AI1}
  ${RISK_SDK}
  ${QT_LIBRARIES}
)
