#include "rclcpp/rclcpp.hpp"
#include "std_srvs/srv/set_bool.hpp"

using SetBool = std_srvs::srv::SetBool;

class PumpSwitch : public rclcpp::Node
{
public:
  PumpSwitch() : Node("pump_switch")
  {
    client_ = create_client<SetBool>("/pump");
    while (!client_->wait_for_service(std::chrono::seconds(1))) {
      RCLCPP_INFO(get_logger(), "Waiting for pump controller...");
    }
  }

  void send_command(bool turn_on)
  {
    auto request = std::make_shared<SetBool::Request>();
    request->data = turn_on;

    auto future = client_->async_send_request(request);

    if (rclcpp::spin_until_future_complete(shared_from_this(), future) ==
      rclcpp::FutureReturnCode::SUCCESS)
    {
      RCLCPP_INFO(get_logger(), "Response: %s", future.get()->message.c_str());
    }
  }

private:
  rclcpp::Client<SetBool>::SharedPtr client_;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  auto node = std::make_shared<PumpSwitch>();
  node->send_command(true);
  node->send_command(false);
  rclcpp::shutdown();
  return 0;
}
