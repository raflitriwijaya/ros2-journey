#include "rclcpp/rclcpp.hpp"
#include "irrigation_msgs/srv/irrigation_control.hpp"

using IrrigationControl = irrigation_msgs::srv::IrrigationControl;

class IrrigationClient : public rclcpp::Node
{
public:
  IrrigationClient() : Node("irrigation_service_client")
  {
    client_ = this->create_client<IrrigationControl>("irrigation_control");
  }

  void send_request(const std::string & field_name, double duration_seconds, double moisture_threshold)
  {
    while (!client_->wait_for_service(std::chrono::seconds(1))) {
        RCLCPP_WARN(this->get_logger(), "Waiting for service to be available...");
    }

    auto request = std::make_shared<IrrigationControl::Request>();
    request->field_name = field_name;
    request->duration_seconds = duration_seconds;
    request->moisture_threshold = moisture_threshold;

    RCLCPP_INFO(this->get_logger(), "Sending request: field=%s duration=%.1fs threshold=%.1f%%",
      request->field_name.c_str(), request->duration_seconds, request->moisture_threshold);

    auto future = client_->async_send_request(request);

    // Wait for response
    if (rclcpp::spin_until_future_complete(this->get_node_base_interface(), future) ==
        rclcpp::FutureReturnCode::SUCCESS)
    {
      auto response = future.get();
      if (response->success) {
        RCLCPP_INFO(this->get_logger(), "Irrigation successful: applied %.1f liters. Message: %s",
          response->water_applied_liters, response->message.c_str());
      } else {
        RCLCPP_ERROR(this->get_logger(), "Irrigation failed: %s", response->message.c_str());
      }
    } else {
      RCLCPP_ERROR(this->get_logger(), "Failed to call service irrigation_control");
    }
  }

private:
    rclcpp::Client<IrrigationControl>::SharedPtr client_;
};

int main(int argc, char **argv)
{
  rclcpp::init(argc, argv);
  auto client_node = std::make_shared<IrrigationClient>();

  // Example request parameters
  client_node->send_request("Field_A", 10.0, 45.0);

  rclcpp::shutdown();
  return 0;
}