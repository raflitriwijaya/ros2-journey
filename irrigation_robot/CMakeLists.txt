cmake_minimum_required(VERSION 3.8)
project(irrigation_robot)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(std_srvs REQUIRED)

add_executable(lesson1_minimal_node src/lesson1_minimal_node.cpp)
ament_target_dependencies(lesson1_minimal_node rclcpp)

add_executable(lesson2_publisher src/lesson2_publisher.cpp)
ament_target_dependencies(lesson2_publisher rclcpp std_msgs)

add_executable(lesson3_subscriber src/lesson3_subscriber.cpp)
ament_target_dependencies(lesson3_subscriber rclcpp std_msgs)

add_executable(lesson4_service_server src/lesson4_service_server.cpp)
ament_target_dependencies(lesson4_service_server rclcpp std_srvs)

add_executable(lesson4_service_client src/lesson4_service_client.cpp)
ament_target_dependencies(lesson4_service_client rclcpp std_srvs)

add_executable(lesson5_param_node src/lesson5_param_node.cpp)
ament_target_dependencies(lesson5_param_node rclcpp)

install(TARGETS
  lesson1_minimal_node
  lesson2_publisher
  lesson3_subscriber
  lesson4_service_server
  lesson4_service_client
  lesson5_param_node
  DESTINATION lib/${PROJECT_NAME}
)

ament_package()
