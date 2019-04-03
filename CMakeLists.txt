# Specify the minimum version for CMake
cmake_minimum_required(VERSION 3.1.0)
project(plotgui)

add_definitions("-std=c++11")

set(BUILD_WITH_CATKIN ON)

set(CMAKE_AUTOMOC ON)
SET(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
SET(qt_gui_cpp_USE_QT_MAJOR_VERSION 5)


find_package(catkin
  REQUIRED
  COMPONENTS
    ros_node_utils
)

find_package(Eigen3 REQUIRED)
find_package(Boost REQUIRED  COMPONENTS system)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Core REQUIRED)
find_package(Qt5PrintSupport REQUIRED)


# For cmake problems refer https://stackoverflow.com/questions/47471629/fail-to-use-q-object-macro-in-cmake-project
# http://doc.qt.io/qt-5/cmake-manual.html
set(plotgui_SRCS  PlotGui.cpp)
qt5_wrap_ui(plotgui_UIS_H plotgui.ui)
qt5_wrap_cpp(plotgui_MOCS include/plotgui/PlotGui.h include/plotgui/ScopeGui.h include/plotgui/qcustomplot.h)

catkin_package(
  INCLUDE_DIRS
    include
  LIBRARIES
    ${PROJECT_NAME} # exports the  lib to catkin_LIBRARIES
  CATKIN_DEPENDS
    ros_node_utils
  DEPENDS
)

get_target_property(QtCore_location Qt5::Core LOCATION)
get_target_property(QtWidgets_location Qt5::Widgets LOCATION)


#message(WARNING ${CMAKE_MODULE_PATH})
#message(WARNING ${QtWidgets_location})
#message(WARNING ${Qt5Widgets_VERSION})
#message(WARNING ${Qt5Widgets_LIBRARIES})
#message(WARNING ${Qt5Widgets_INCLUDE_DIRS})
#message(WARNING ${CMAKE_PREFIX_PATH})
#message(WARNING ${EIGEN3_INCLUDE_DIR})
#message(WARNING ${EIGEN3_LIBRARIES})

# The following folder will be included
include_directories(
	include
	${EIGEN3_INCLUDE_DIR}
	${Boost_INCLUDE_DIRS}
	${CMAKE_CURRENT_BINARY_DIR}
  ${catkin_INCLUDE_DIRS}
  #${Qt5Widgets_INCLUDE_DIRS}
  #${Qt5Core_INCLUDE_DIRS}
  #${Qt5Gui_INCLUDE_DIRS}
)

add_library(${PROJECT_NAME}
	SHARED
	src/PlotGui.cpp
	src/ScopeGui.cpp
	src/qcustomplot.cpp
	${plotgui_MOCS}
)

#set_property(TARGET ${PROJECT_NAME} PROPERTY POSITION_INDEPENDENT_CODE ON)

qt5_use_modules(${PROJECT_NAME} Core Gui Widgets PrintSupport)
target_link_libraries(
  ${PROJECT_NAME}
	${catkin_LIBRARIES}
  ${Boost_LIBRARIES}
  ${Qt5Widgets_LIBRARIES}
  ${Qt5Gui_LIBRARIES}
  ${Qt5Core_LIBRARIES}
  ${Qt5PrintSupport_LIBRARIES}
)

add_executable(ros_scope
	src/ros_scope.cpp
)
target_link_libraries(ros_scope
  ${PROJECT_NAME}
	${catkin_LIBRARIES}
)



add_executable(plottest
	test/plotTest.cpp
)
target_link_libraries(plottest
  ${PROJECT_NAME}
	${catkin_LIBRARIES}
)

add_executable(subplottest
	test/subPlotTest.cpp
)
target_link_libraries(subplottest
  ${PROJECT_NAME}
	${catkin_LIBRARIES}
)

add_executable(scopeguitest
	test/scopeTest.cpp
)
target_link_libraries(scopeguitest
  ${PROJECT_NAME}
	${catkin_LIBRARIES}
)

install(DIRECTORY include/${PROJECT_NAME}/
  DESTINATION ${CATKIN_PACKAGE_INCLUDE_DESTINATION}
  FILES_MATCHING PATTERN "*.h"
  PATTERN ".svn" EXCLUDE
)

if(${BUILD_WITH_CATKIN})

else()
  install(TARGETS  ${PROJECT_NAME} DESTINATION /usr/local/plotgui/lib)
  install(DIRECTORY include DESTINATION /usr/local/plotgui )
  install(FILES FindPlotgui.cmake DESTINATION /usr/share/cmake-3.5/Modules/)
endif()
