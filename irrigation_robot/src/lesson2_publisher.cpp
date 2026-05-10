#include <chrono>
#include <string>
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

using namespace std::chrono_literals;

class SoilPublisher : public rclcpp::Node
{
public:
  SoilPublisher() : Node("soil_publisher")
  {
    publisher_ = create_publisher<std_msgs::msg::String>("/soil_moisture", 10);
    timer_ = create_timer(1s, std::bind(&SoilPublisher::timer_callback, this));
    RCLCPP_INFO(get_logger(), "Soil publisher started!");
  }

private:
  void timer_callback()
  {
    auto msg = std_msgs::msg::String();
    msg.data = "moisture: 65%";
    publisher_->publish(msg);
    RCLCPP_INFO(get_logger(), "Published: '%s'", msg.data.c_str());
  }

  rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;
  rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<SoilPublisher>());
  rclcpp::shutdown();
  return 0;
}
