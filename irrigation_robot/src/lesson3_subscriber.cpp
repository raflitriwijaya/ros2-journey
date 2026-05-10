#include <memory>
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

class SoilListener : public rclcpp::Node
{
public:
  SoilListener() : Node("soil_listener")
  {
    subscription_ = create_subscription<std_msgs::msg::String>(
      "/soil_moisture",
      10,
      std::bind(&SoilListener::listener_callback, this, std::placeholders::_1)
    );
    RCLCPP_INFO(get_logger(), "Listening for soil moisture data...");
  }

private:
  void listener_callback(const std_msgs::msg::String & msg)
  {
    RCLCPP_INFO(get_logger(), "Received: '%s'", msg.data.c_str());
  }

  rclcpp::Subscription<std_msgs::msg::String>::SharedPtr subscription_;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<SoilListener>());
  rclcpp::shutdown();
  return 0;
}
