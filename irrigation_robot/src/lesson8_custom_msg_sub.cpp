#include "rclcpp/rclcpp.hpp"
#include "irrigation_msgs/msg/soil_data.hpp"  // header pesan custom

class SoilDataSubscriber : public rclcpp::Node
{
public:
    SoilDataSubscriber() : Node("soil_data_subscriber")
    {
        // Membuat subscriber untuk pesan SoilData
        subscriber_ = this->create_subscription<irrigation_msgs::msg::SoilData>(
            "soil_data", 10,
            std::bind(&SoilDataSubscriber::on_soil_data, this, std::placeholders::_1));

        RCLCPP_INFO(this->get_logger(), "Soil Data Subscriber has been started.");
    }

private:
    void on_soil_data(const irrigation_msgs::msg::SoilData::SharedPtr msg)
    {
        // Menampilkan data yang diterima
        RCLCPP_INFO(this->get_logger(), 
            "[%s] moisture=%.1f%% temp=%.1fC pump_status=%s",
            msg->field_name.c_str(),
            msg->moisture_percent,
            msg->temperature_celsius,
            msg->pump_active ? "ON" : "OFF");

        // Decision Logic: Jika kelembaban di bawah 30%, nyalakan pompa
    if (msg->moisture_percent < 30.0) {
      RCLCPP_WARN(this->get_logger(),
        "⚠ CRITICAL: moisture %.1f%% is dangerously low on [%s]!",
        msg->moisture_percent,
        msg->field_name.c_str());
    }

        // log timestamp
        RCLCPP_DEBUG(this->get_logger(), "Timestamp: %d", msg->timestamp.sec);
    }

    rclcpp::Subscription<irrigation_msgs::msg::SoilData>::SharedPtr subscriber_;
};

int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<SoilDataSubscriber>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}