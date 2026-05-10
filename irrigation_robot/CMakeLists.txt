cmake_minimum_required(VERSION 3.8)
project(irrigation_robot)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)

add_executable(lesson1_minimal_node src/lesson1_minimal_node.cpp)
ament_target_dependencies(lesson1_minimal_node rclcpp)

add_executable(lesson2_publisher src/lesson2_publisher.cpp)
ament_target_dependencies(lesson2_publisher rclcpp std_msgs)

install(TARGETS
  lesson1_minimal_node
  lesson2_publisher
  DESTINATION lib/${PROJECT_NAME}
)

ament_package()
