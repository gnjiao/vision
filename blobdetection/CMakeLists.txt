cmake_minimum_required(VERSION 2.8.3)
project(blobdetection)

## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages
find_package(catkin REQUIRED COMPONENTS roscpp sensor_msgs image_transport cv_bridge message_generation rosutil)
find_package(OpenCV REQUIRED)

add_message_files( FILES
  depth_point.msg
)

generate_messages(DEPENDENCIES std_msgs)

catkin_package(
#  INCLUDE_DIRS include
#  LIBRARIES blobdetection
  CATKIN_DEPENDS message_runtime roscpp sensor_msgs image_transport cv_bridge
   rosutil
#  DEPENDS system_lib
)

include_directories(include ${catkin_INCLUDE_DIRS})

#add_executable(blobDetector src/blobDetector.cpp)
#target_link_libraries(blobDetector ${catkin_LIBRARIES} ${OpenCV_LIBRARIES})
#add_dependencies(blobDetector ${catkin_EXPORTED_TARGETS})
add_executable(pixelDetector src/closestPixelDetector.cpp)
target_link_libraries(pixelDetector ${catkin_LIBRARIES} ${OpenCV_LIBRARIES} rosutil)
add_dependencies(pixelDetector ${catkin_EXPORTED_TARGETS})
