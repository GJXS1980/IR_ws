cmake_minimum_required(VERSION 3.0.2)
project(xmate_robot_2)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

find_package(catkin REQUIRED COMPONENTS
  ar_track_alvar_msgs
  cv_bridge
  geometry_msgs
  image_transport
  roscpp
  rospy
  std_msgs
  visualization_msgs
)

catkin_package(
#  INCLUDE_DIRS include
#  LIBRARIES castlex_voice_system
#  CATKIN_DEPENDS roscpp rospy std_msgs
#  DEPENDS system_lib
)

include_directories(
  include
  /usr/include/eigen3
  ${PROJECT_SOURCE_DIR}/inc
  ${PROJECT_SOURCE_DIR}/inc/include
  ${OpenCV_INCLUDE_DIRS}
  ${catkin_INCLUDE_DIRS}
)

link_directories(
  lib
  ${catkin_LIB_DIRS}
)

# place_bjj_demo.cpp
add_executable(place_bjj_demo src/place_bjj_demo.cpp)
target_link_libraries(place_bjj_demo ${catkin_LIBRARIES} -lrt -ldl -lorocos-kdl -lglog -lpthread  -lRCI_Client -lxMateModel)

# new_grasp_demo.cpp
add_executable(new_grasp_demo src/new_grasp_demo.cpp)
target_link_libraries(new_grasp_demo ${catkin_LIBRARIES} -lrt -ldl -lorocos-kdl -lglog -lpthread  -lRCI_Client -lxMateModel)

# new_place_demo.cpp
add_executable(new_place_demo src/new_place_demo.cpp)
target_link_libraries(new_place_demo ${catkin_LIBRARIES} -lrt -ldl -lorocos-kdl -lglog -lpthread  -lRCI_Client -lxMateModel)

# get_robot_moment.cpp
add_executable(get_robot_moment src/get_robot_moment.cpp)
target_link_libraries(get_robot_moment ${catkin_LIBRARIES} -lrt -ldl -lorocos-kdl -lglog -lpthread  -lRCI_Client -lxMateModel)

# new_product_place_demo.cpp
add_executable(new_product_place_demo src/new_product_place_demo.cpp)
target_link_libraries(new_product_place_demo ${catkin_LIBRARIES} -lrt -ldl -lorocos-kdl -lglog -lpthread  -lRCI_Client -lxMateModel)

# new_product_place_demo.cpp
add_executable(new_product_grasp_demo src/new_product_grasp_demo.cpp)
target_link_libraries(new_product_grasp_demo ${catkin_LIBRARIES} -lrt -ldl -lorocos-kdl -lglog -lpthread  -lRCI_Client -lxMateModel)

# bjj_grasp_demo.cpp
add_executable(bjj_grasp_demo src/bjj_grasp_demo.cpp)
target_link_libraries(bjj_grasp_demo ${catkin_LIBRARIES} -lrt -ldl -lorocos-kdl -lglog -lpthread  -lRCI_Client -lxMateModel)

# test_demo.cpp
add_executable(test_demo src/test_demo.cpp)
target_link_libraries(test_demo ${catkin_LIBRARIES} -lrt -ldl -lorocos-kdl -lglog -lpthread  -lRCI_Client -lxMateModel)

add_executable(place_grasp_demo src/place_grasp_demo.cpp)
target_link_libraries(place_grasp_demo ${catkin_LIBRARIES} -lrt -ldl -lorocos-kdl -lglog -lpthread  -lRCI_Client -lxMateModel)

add_executable(product_ar_test src/product_ar_test.cpp)
target_link_libraries(product_ar_test ${catkin_LIBRARIES} -lrt -ldl -lorocos-kdl -lglog -lpthread  -lRCI_Client -lxMateModel)

add_executable(place_grasp_demo_new src/place_grasp_demo_new.cpp)
target_link_libraries(place_grasp_demo_new ${catkin_LIBRARIES} -lrt -ldl -lorocos-kdl -lglog -lpthread  -lRCI_Client -lxMateModel)

add_executable(voice_grasp_place src/voice_grasp_place.cpp)
target_link_libraries(voice_grasp_place ${catkin_LIBRARIES} -lrt -ldl -lorocos-kdl -lglog -lpthread  -lRCI_Client -lxMateModel)

