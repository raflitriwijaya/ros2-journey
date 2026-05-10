#include <chrono>
#include "rclcpp/rclcpp.hpp"

using namespace std::chrono_literals;

class MyNode : public rclcpp::Node
{
public:
  MyNode() : Node("my_first_node")
  {
    RCLCPP_INFO(get_logger(), "Node is alive!");
    timer_ = create_timer(1s, std::bind(&MyNode::timer_callback, this));
  }

private:
  void timer_callback()
  {
    RCLCPP_INFO(get_logger(), "1 second passed...");
  }

  rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<MyNode>());
  rclcpp::shutdown();
  return 0;
}