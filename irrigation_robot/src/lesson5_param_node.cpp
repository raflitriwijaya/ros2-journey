#include <chrono>
#include "rclcpp/rclcpp.hpp"

using namespace std::chrono_literals;

class SoilSensor : public rclcpp::Node
{
public:
  SoilSensor() : Node("soil_sensor")
  {
    declare_parameter("publish_rate", 1.0);
    declare_parameter("moisture_threshold", 30.0);
    declare_parameter("field_name", std::string("field_A"));

    double rate       = get_parameter("publish_rate").as_double();
    threshold_        = get_parameter("moisture_threshold").as_double();
    std::string field = get_parameter("field_name").as_string();

    RCLCPP_INFO(get_logger(), "Field: %s | Rate: %.1fs | Threshold: %.1f%%",
      field.c_str(), rate, threshold_);

    // React to parameter changes at runtime
    param_cb_ = add_on_set_parameters_callback(
      std::bind(&SoilSensor::on_param_change, this, std::placeholders::_1));

    auto period = std::chrono::duration<double>(rate);
    timer_ = create_timer(period, std::bind(&SoilSensor::timer_callback, this));
  }

private:
  void timer_callback()
  {
    double moisture = 45.0;
    if (moisture < threshold_) {
      RCLCPP_WARN(get_logger(), "LOW MOISTURE: %.1f%% < %.1f%%",
        moisture, threshold_);
    } else {
      RCLCPP_INFO(get_logger(), "Moisture OK: %.1f%%", moisture);
    }
  }

  rcl_interfaces::msg::SetParametersResult on_param_change(
    const std::vector<rclcpp::Parameter> & params)
  {
    for (const auto & param : params) {
      if (param.get_name() == "moisture_threshold") {
        threshold_ = param.as_double();
        RCLCPP_INFO(get_logger(), "Threshold updated to: %.1f%%", threshold_);
      }
    }
    rcl_interfaces::msg::SetParametersResult result;
    result.successful = true;
    return result;
  }

  double threshold_;
  rclcpp::TimerBase::SharedPtr timer_;
  rclcpp::node_interfaces::OnSetParametersCallbackHandle::SharedPtr param_cb_;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<SoilSensor>());
  rclcpp::shutdown();
  return 0;
}
