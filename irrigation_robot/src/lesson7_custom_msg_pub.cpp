#include "rclcpp/rclcpp.hpp"
#include "irrigation_msgs/msg/soil_data.hpp"

class SoilDataPublisher : public rclcpp::Node
{
private:
    void timer_callback()
    {
        auto msg = irrigation_msgs::msgs::msg::SoilData();
        msg.moisture_percent    = 42.5 + (count_ % 10) // fluctuates 42.5-51.5
        msg.temperature_celcius = 28.3
        msg.field_name          = "Field A"
        msg.pump_active         = (msg.moisture_percent < 45.0) // pump active if moisture is below 45%

        publisher_->publish(msg);

        RC
        
    }
public:
    SoilDataPublisher : public rclcpp::Node(/* args */);
    ~SoilDataPublisher : public rclcpp::Node();
};

SoilDataPublisher : public rclcpp::Node::SoilDataPublisher : public rclcpp::Node(/* args */)
{
}

SoilDataPublisher : public rclcpp::Node::~SoilDataPublisher : public rclcpp::Node()
{
}
