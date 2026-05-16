#include "rclcpp/rclcpp.hpp"
#include "irrigation_msgs/msg/soil_data.hpp"  // header pesan custom

class SoilDataPublisher : public rclcpp::Node
{
public:
  SoilDataPublisher()
  : Node("soil_data_publisher"), count_(0)
  {
    // Publisher dengan tipe message yang benar
    publisher_ = this->create_publisher<irrigation_msgs::msg::SoilData>("soil_data", 10);

    // Timer setiap 1 detik
    timer_ = this->create_wall_timer(
      std::chrono::seconds(1),
      std::bind(&SoilDataPublisher::timer_callback, this));

    RCLCPP_INFO(this->get_logger(), "Soil Data Publisher has been started.");
  }

private:
  void timer_callback()
  {
    auto msg = irrigation_msgs::msg::SoilData();   // namespace yang benar
    msg.moisture_percent = 42.5 + (count_ % 10);    // 42.5 - 51.5
    msg.temperature_celsius = 28.3;                 // typo diperbaiki, titik koma ditambah
    msg.field_name = "Field A";
    msg.pump_active = (msg.moisture_percent < 45.0); // aktif jika moisture < 45%
    msg.timestamp = this->now();                     // builtin_interfaces/Time

    publisher_->publish(msg);

    RCLCPP_INFO(this->get_logger(),
      "[%s] moisture: %.2f%%, temperature: %.2fC, pump_active: %s",
      msg.field_name.c_str(),
      msg.moisture_percent,
      msg.temperature_celsius,
      msg.pump_active ? "ON" : "OFF");

    ++count_;
  }

  // Deklarasi anggota kelas
  rclcpp::Publisher<irrigation_msgs::msg::SoilData>::SharedPtr publisher_;
  rclcpp::TimerBase::SharedPtr timer_;
  int count_;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<SoilDataPublisher>());
  rclcpp::shutdown();
  return 0;
}