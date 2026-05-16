#include "rclcpp/rclcpp.hpp"
#include "irrigation_msgs/msg/soil_data.hpp"
#include "irrigation_msgs/srv/irrigation_control.hpp"

using SoilData        = irrigation_msgs::msg::SoilData;
using IrrigationControl = irrigation_msgs::srv::IrrigationControl;

class IrrigationExecutorNode : public rclcpp::Node
{
public:
    IrrigationExecutorNode() : Node("irrigation_executor")
    {
        // Callback Groups
        // Reentrant: subscriber and service can be executed in parallel
        cb_group_sensor_ = this->create_callback_group(rclcpp::CallbackGroupType::Reentrant);

        // MutuallyExclusive: slow timer runs isolated
        cb_group_control_ = this->create_callback_group(rclcpp::CallbackGroupType::MutuallyExclusive);

        // Subscriber - soil data (fast, frequent)
        auto sub_options = rclcpp::SubscriptionOptions();
        sub_options.callback_group = cb_group_sensor_;

        subscriber_ = this->create_subscription<SoilData>(
            "/soil_data", 10, 
            std::bind(&IrrigationExecutorNode::on_soil_data, this,
            std::placeholders::_1), sub_options);

        // Service - irrigation control (slow, infrequent)
        server_ = this->create_service<IrrigationControl>(
            "/irrigation_control_ex",
            std::bind(&IrrigationExecutorNode::handle_service, this,
            std::placeholders::_1, std::placeholders::_2), 
            rclcpp::ServicesQoS(),
            cb_group_sensor_
        );

        timer_ = this->create_timer(
            std::chrono::seconds(5),
            std::bind(&IrrigationExecutorNode::compute_crop_requirement, this),
            cb_group_control_); //isolated group
        
        RCLCPP_INFO(this->get_logger(), "Irrigation Executor Node has been started.");
        RCLCPP_INFO(this->get_logger(), "MultiThreadedExecutor: subscriber + service run in parallel with timer.");
    }

private:
    // Fast Callback - runs in Reentrant group
    void on_soil_data(const SoilData::SharedPtr msg)
    {
        RCLCPP_INFO(this->get_logger(),
            "[SENSOR] field=%s moisture=%.1f%% pump=%s",
            msg->field_name.c_str(),
            msg->moisture_percent,
            msg->pump_active ? "ON" : "OFF"
        );
    }

    void handle_service(
        const IrrigationControl::Request::SharedPtr request,
        IrrigationControl::Response::SharedPtr response)
    {
        RCLCPP_INFO(this->get_logger(),
          "[SERVICE] Received: field=%s duration=%.1fs",
          request->field_name.c_str(),
          request->duration_seconds);

        // Simulate blocking work (e.g., hardware handshake)
        std::this_thread::sleep_for(std::chrono::seconds(2));

        response->success = true;
        response-> water_applied_liters = request->duration_seconds * 0.5;
        response->message = "Done: " + request->field_name;

        RCLCPP_INFO(this->get_logger(),
          "[SERVICE] Completed: field=%s applied=%.1fL",
          request->field_name.c_str(),
          response->water_applied_liters);
    }

    // Slow timer - runs in MutuallyExclusive group (isolated)
    void compute_crop_requirement()
    {
        RCLCPP_INFO(this->get_logger(),
          "[TIMER] Computing crop water requirement... (slow, 5s interval)");

        // Simulate heavy computation
        std::this_thread::sleep_for(std::chrono::milliseconds(500));

        RCLCPP_INFO(this->get_logger(),
          "[TIMER] Crop water requirement: 4.2 mm/day");
    }

    // Callback Groups
    rclcpp::CallbackGroup::SharedPtr cb_group_sensor_;
    rclcpp::CallbackGroup::SharedPtr cb_group_control_;

    // Node Componens
    rclcpp::Subscription<SoilData>::SharedPtr subscriber_;
    rclcpp::Service<IrrigationControl>::SharedPtr server_;
    rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<IrrigationExecutorNode>();

    // Use MultiThreadedExecutor to allow parallel execution of callbacks
    rclcpp::executors::MultiThreadedExecutor executor(rclcpp::ExecutorOptions(), 3); // 4 threads
    executor.add_node(node);
    executor.spin();

    rclcpp::shutdown();
    return 0;
}


