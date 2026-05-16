#include "rclcpp/rclcpp.hpp"
#include "rclcpp_action/rclcpp_action.hpp"
#include "irrigation_msgs/action/irrigation_cycle.hpp"

using IrrigationCycle = irrigation_msgs::action::IrrigationCycle;
using GoalHandle      = rclcpp_action::ServerGoalHandle<IrrigationCycle>;

class IrrigationActionServer : public rclcpp::Node
{
public:
  IrrigationActionServer()
  : Node("irrigation_action_server")
  {
    server_ = rclcpp_action::create_server<IrrigationCycle>(
      this,
      "/irrigation_cycle",
      std::bind(&IrrigationActionServer::handle_goal,   this, std::placeholders::_1, std::placeholders::_2),
      std::bind(&IrrigationActionServer::handle_cancel, this, std::placeholders::_1),
      std::bind(&IrrigationActionServer::handle_accepted, this, std::placeholders::_1));

    RCLCPP_INFO(this->get_logger(), "IrrigationActionServer ready on /irrigation_cycle");
  }

private:
  rclcpp_action::Server<IrrigationCycle>::SharedPtr server_;

  // ── 1. Accept or reject the goal ──────────────────────────────────────────
  rclcpp_action::GoalResponse handle_goal(
    const rclcpp_action::GoalUUID &,
    std::shared_ptr<const IrrigationCycle::Goal> goal)
  {
    RCLCPP_INFO(this->get_logger(),
      "Goal received: field=%s  target=%.1f%%  max=%.1fs",
      goal->field_name.c_str(),
      goal->target_moisture_percent,
      goal->max_duration_seconds);

    if (goal->target_moisture_percent <= 0.0 || goal->max_duration_seconds <= 0.0) {
      RCLCPP_WARN(this->get_logger(), "Goal rejected — invalid parameters.");
      return rclcpp_action::GoalResponse::REJECT;
    }

    return rclcpp_action::GoalResponse::ACCEPT_AND_EXECUTE;
  }

  // ── 2. Handle cancel request ───────────────────────────────────────────────
  rclcpp_action::CancelResponse handle_cancel(
    const std::shared_ptr<GoalHandle> goal_handle)
  {
    RCLCPP_WARN(this->get_logger(), "Cancel request received.");
    (void)goal_handle;
    return rclcpp_action::CancelResponse::ACCEPT;
  }

  // ── 3. Execute the goal in a separate thread ───────────────────────────────
  void handle_accepted(const std::shared_ptr<GoalHandle> goal_handle)
  {
    std::thread{
      std::bind(&IrrigationActionServer::execute, this, std::placeholders::_1),
      goal_handle
    }.detach();
  }

  void execute(const std::shared_ptr<GoalHandle> goal_handle)
  {
    const auto goal     = goal_handle->get_goal();
    auto feedback       = std::make_shared<IrrigationCycle::Feedback>();
    auto result         = std::make_shared<IrrigationCycle::Result>();

    const double flow_rate   = 0.5;   // liters per second
    const double moisture_rise = 0.8; // moisture gain per second of irrigation
    double current_moisture  = 30.0;  // simulate starting moisture
    double elapsed           = 0.0;
    double water_applied     = 0.0;

    RCLCPP_INFO(this->get_logger(), "Executing irrigation cycle on [%s]...",
      goal->field_name.c_str());

    rclcpp::Rate rate(1);  // 1 Hz — one feedback per second

    while (rclcpp::ok()) {

      // ── Check cancel ──────────────────────────────────────────────────────
      if (goal_handle->is_canceling()) {
        result->success              = false;
        result->final_moisture_percent = current_moisture;
        result->total_water_liters   = water_applied;
        result->message              = "Irrigation cancelled.";
        goal_handle->canceled(result);
        RCLCPP_WARN(this->get_logger(), "Goal cancelled.");
        return;
      }

      // ── Simulate irrigation progress ──────────────────────────────────────
      elapsed          += 1.0;
      water_applied    += flow_rate;
      current_moisture += moisture_rise;

      // ── Publish feedback ──────────────────────────────────────────────────
      feedback->current_moisture_percent = current_moisture;
      feedback->water_applied_liters     = water_applied;
      feedback->elapsed_seconds          = elapsed;
      feedback->status_message           = "Irrigating " + goal->field_name;
      goal_handle->publish_feedback(feedback);

      RCLCPP_INFO(this->get_logger(),
        "[%.0fs] moisture=%.1f%%  water=%.1fL",
        elapsed, current_moisture, water_applied);

      // ── Check completion conditions ───────────────────────────────────────
      bool target_reached  = current_moisture >= goal->target_moisture_percent;
      bool time_exceeded   = elapsed          >= goal->max_duration_seconds;

      if (target_reached || time_exceeded) {
        result->success                = target_reached;
        result->final_moisture_percent = current_moisture;
        result->total_water_liters     = water_applied;
        result->message = target_reached
          ? "Target moisture reached on " + goal->field_name
          : "Max duration exceeded on "   + goal->field_name;
        goal_handle->succeed(result);
        RCLCPP_INFO(this->get_logger(), "✔ %s", result->message.c_str());
        return;
      }

      rate.sleep();
    }
  }
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<IrrigationActionServer>());
  rclcpp::shutdown();
  return 0;
}