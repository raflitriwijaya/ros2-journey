#include "rclcpp/rclcpp.hpp"
#include "std_srvs/srv/set_bool.hpp"

using SetBool = std_srvs::srv::SetBool;

class PumpController : public rclcpp::Node
{
public:
  PumpController() : Node("pump_controller")
  {
    server_ = create_service<SetBool>(
      "/pump",
      std::bind(&PumpController::handle_request, this,
        std::placeholders::_1, std::placeholders::_2)
    );
    RCLCPP_INFO(get_logger(), "Pump controller ready.");
  }

private:
  void handle_request(
    const SetBool::Request::SharedPtr request,
    SetBool::Response::SharedPtr response)
  {
    if (request->data) {
      RCLCPP_INFO(get_logger(), "Pump -> ON");
      response->success = true;
      response->message = "Pump turned ON";
    } else {
      RCLCPP_INFO(get_logger(), "Pump -> OFF");
      response->success = true;
      response->message = "Pump turned OFF";
    }
  }

  rclcpp::Service<SetBool>::SharedPtr server_;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<PumpController>());
  rclcpp::shutdown();
  return 0;
}
