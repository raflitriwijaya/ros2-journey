#include "rclcpp/rclcpp.hpp"
#include "irrigation_msgs/srv/irrigation_control.hpp"

using IrrigationControl = irrigation_msgs::srv::IrrigationControl;

class IrrigationServer : public rclcpp::Node
{
public:
  IrrigationServer() : Node("irrigation_service_server")
  {
    server_ = this->create_service<IrrigationControl>(
      "irrigation_control",
      std::bind(&IrrigationServer::handle_request, this, std::placeholders::_1, std::placeholders::_2));
    RCLCPP_INFO(this->get_logger(), "Irrigation Control Service Server is ready.");
  }

private:
  void handle_request(
    const IrrigationControl::Request::SharedPtr request,IrrigationControl::Response::SharedPtr response)
  {
    RCLCPP_INFO(this->get_logger(), 
      "received: field=%s duration=%.1fs threshold=%.1f%%",
      request->field_name.c_str(), 
      request->duration_seconds, 
      request->moisture_threshold);

      // Reject invalid requests
      if (request->duration_seconds <= 0.0) {
        response->success = false;
        response->message = "Duration must be positive.";
        RCLCPP_WARN(this->get_logger(), "Invalid request: %s", response->message.c_str());
        return;
      }

      // simulate irrigation: 0.5 liter per second
      const double flow_rate = 0.5; // liters per second
      response->water_applied_liters = request->duration_seconds * flow_rate;
      response->success = true;
      response->message = "irrigation completed on " + request->field_name;

      RCLCPP_INFO(this->get_logger(), "Irrigation completed: applied %.1f liters to %s",
        response->water_applied_liters, request->field_name.c_str());
  }

    rclcpp::Service<IrrigationControl>::SharedPtr server_;
};

int main(int argc, char **argv)
{
  rclcpp::init(argc, argv);
  auto node = std::make_shared<IrrigationServer>();
  rclcpp::spin(node);
  rclcpp::shutdown();
  return 0;
}
