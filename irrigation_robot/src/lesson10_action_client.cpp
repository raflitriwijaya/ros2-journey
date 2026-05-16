#include "rclcpp/rclcpp.hpp"
#include "rclcpp_action/rclcpp_action.hpp"
#include "irrigation_msgs/action/irrigation_cycle.hpp"

using IrrigationCycle = irrigation_msgs::action::IrrigationCycle;
using GoalHandle      = rclcpp_action::ClientGoalHandle<IrrigationCycle>;

class IrrigationActionClient : public rclcpp::Node
{
public:
  IrrigationActionClient()
  : Node("irrigation_action_client")
  {
    client_ = rclcpp_action::create_client<IrrigationCycle>(
      this, "/irrigation_cycle");
  }

  void send_goal(
    const std::string & field_name,
    double target_moisture,
    double max_duration)
  {
    if (!client_->wait_for_action_server(std::chrono::seconds(5))) {
      RCLCPP_ERROR(this->get_logger(), "Action server not available.");
      return;
    }

    auto goal = IrrigationCycle::Goal();
    goal.field_name               = field_name;
    goal.target_moisture_percent  = target_moisture;
    goal.max_duration_seconds     = max_duration;

    auto send_goal_options = rclcpp_action::Client<IrrigationCycle>::SendGoalOptions();

    // Feedback callback — called every second by the server
    send_goal_options.feedback_callback =
      [this](GoalHandle::SharedPtr,
             const std::shared_ptr<const IrrigationCycle::Feedback> feedback)
      {
        RCLCPP_INFO(this->get_logger(),
          "↳ FEEDBACK [%.0fs] moisture=%.1f%%  water=%.1fL  — %s",
          feedback->elapsed_seconds,
          feedback->current_moisture_percent,
          feedback->water_applied_liters,
          feedback->status_message.c_str());
      };

    // Result callback — called once when server finishes
    send_goal_options.result_callback =
      [this](const GoalHandle::WrappedResult & wrapped_result)
      {
        auto result = wrapped_result.result;
        RCLCPP_INFO(this->get_logger(),
          "✔ RESULT — success=%s  final_moisture=%.1f%%  water=%.1fL  msg='%s'",
          result->success ? "true" : "false",
          result->final_moisture_percent,
          result->total_water_liters,
          result->message.c_str());
      };

    RCLCPP_INFO(this->get_logger(),
      "Sending goal → field=%s  target=%.1f%%  max=%.1fs",
      field_name.c_str(), target_moisture, max_duration);

    client_->async_send_goal(goal, send_goal_options);
  }

private:
  rclcpp_action::Client<IrrigationCycle>::SharedPtr client_;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  auto node = std::make_shared<IrrigationActionClient>();

  node->send_goal("Field_A", 55.0, 30.0);  // target 55% moisture, max 30 seconds

  rclcpp::spin(node);
  rclcpp::shutdown();
  return 0;
}