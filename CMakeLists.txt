# Specify the minimum version for CMake
cmake_minimum_required(VERSION 3.1.0)
project(plotgui)

add_definitions("-std=c++11")

set(CMAKE_AUTOMOC ON)
SET(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
SET(qt_gui_cpp_USE_QT_MAJOR_VERSION 5)


find_package(catkin REQUIRED COMPONENTS)

find_package(Eigen3 REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Core REQUIRED)
find_package(Qt5PrintSupport REQUIRED)


# For cmake problems refer https://stackoverflow.com/questions/47471629/fail-to-use-q-object-macro-in-cmake-project
# http://doc.qt.io/qt-5/cmake-manual.html
set(plotgui_SRCS  PlotGui.cpp)
qt5_wrap_ui(plotgui_UIS_H plotgui.ui)
qt5_wrap_cpp(plotgui_MOCS include/PlotGui.h include/qcustomplot.h)

catkin_package(
  INCLUDE_DIRS
    include
  LIBRARIES
    ${PROJECT_NAME} # exports the ode_solver lib to catkin_LIBRARIES
  CATKIN_DEPENDS
  DEPENDS
)

get_target_property(QtCore_location Qt5::Core LOCATION)
get_target_property(QtWidgets_location Qt5::Widgets LOCATION)
message(WARNING ${Qt5Widgets_VERSION})
message(WARNING ${Qt5Widgets_LIBRARIES})
message(WARNING ${Qt5Widgets_INCLUDE_DIRS})
message(WARNING ${Qt5Widgets_FOUND})

# The following folder will be included
include_directories(
	include
	${EIGEN3_INCLUDE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
  #${Qt5Widgets_INCLUDE_DIRS}
  #${Qt5Core_INCLUDE_DIRS}
  #${Qt5Gui_INCLUDE_DIRS}
)

add_library(${PROJECT_NAME}
	SHARED
	src/PlotGui.cpp
	src/qcustomplot.cpp
	${plotgui_MOCS}
)
#set_property(TARGET ${PROJECT_NAME} PROPERTY POSITION_INDEPENDENT_CODE ON)

qt5_use_modules(${PROJECT_NAME} Core Gui Widgets PrintSupport)
target_link_libraries(
  ${PROJECT_NAME}
	${catkin_LIBRARIES}
  ${Qt5Widgets_LIBRARIES}
  ${Qt5Gui_LIBRARIES}
  ${Qt5Core_LIBRARIES}
  ${Qt5PrintSupport_LIBRARIES}
)


add_executable(guitest
	test/test.cpp
)
target_link_libraries(guitest
  ${PROJECT_NAME}
	${catkin_LIBRARIES}
)

install(DIRECTORY include
  DESTINATION ${CATKIN_PACKAGE_INCLUDE_DESTINATION}
  FILES_MATCHING PATTERN "*.h"
  PATTERN ".svn" EXCLUDE
)
