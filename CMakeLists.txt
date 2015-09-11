cmake_minimum_required(VERSION 2.8.11)

project(Risiko_AI)

# Initialize CMake
include ("./cmake/init.cmake")

# sev: from http://doc.qt.io/qt-5/cmake-manual.html
# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)
SET(CMAKE_AUTOUIC ON) 
SET(CMAKE_AUTORCC ON) 

# sev: define names
set (RISK_GAME "Risiko")
set (RISK_SDK "Risiko_SDK")
set (RISK_AI1 "RiskAINeuroTitan")
set (RISK_AI2 "RiskAISimple")
set (RISK_AI3 "RiskAITitanNet")

# Call the binary's CMakeLists.txt file
set (SDK_DIR "${PROJECT_SOURCE_DIR}/src/Risiko_SDK")
set (GAME_DIR "${PROJECT_SOURCE_DIR}/src/Risiko")
set (AI1_DIR "${PROJECT_SOURCE_DIR}/src/${RISK_AI1}")
set (AI2_DIR "${PROJECT_SOURCE_DIR}/src/${RISK_AI2}")
set (AI3_DIR "${PROJECT_SOURCE_DIR}/src/${RISK_AI3}")

add_subdirectory (${SDK_DIR})
add_subdirectory (${GAME_DIR})
add_subdirectory (${AI1_DIR})
add_subdirectory (${AI2_DIR})
add_subdirectory (${AI3_DIR})
