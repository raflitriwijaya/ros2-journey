# Lesson 2 — Publisher (C++)

**Phase:** P1 — ROS2 Fundamentals  
**Status:** ⏳ Upcoming  
**Concepts:** create_publisher, std_msgs, publish, message types, queue size

---

## 🧠 Concept

A **publisher** broadcasts data on a named topic every time it fires.  
The publisher does not know — or care — who is listening.

### Thesis analogy

| Irrigation Thesis | ROS2 C++ equivalent |
|------------------|---------------------|
| ESP32 sends moisture bytes over WiFiUDP | `publisher_->publish(msg)` on `/soil_moisture` |
| Fixed destination IP | Topic name — any subscriber can listen |
| Custom byte struct | `std_msgs::msg::String` message type |
| Repeats every loop cycle | `create_timer(1s, callback)` |

---

## 📄 Code

**`src/publisher_node.cpp`**

```cpp
#include <chrono>
#include <string>
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

using namespace std::chrono_literals;

class SoilPublisher : public rclcpp::Node
{
public:
  SoilPublisher() : Node("soil_publisher")
  {
    publisher_ = create_publisher<std_msgs::msg::String>("/soil_moisture", 10);
    timer_ = create_timer(1s, std::bind(&SoilPublisher::timer_callback, this));
    RCLCPP_INFO(get_logger(), "Soil publisher started!");
  }

private:
  void timer_callback()
  {
    auto msg = std_msgs::msg::String();
    msg.data = "moisture: 65%";
    publisher_->publish(msg);
    RCLCPP_INFO(get_logger(), "Published: '%s'", msg.data.c_str());
  }

  rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;
  rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<SoilPublisher>());
  rclcpp::shutdown();
  return 0;
}
```

---

## 🔧 CMakeLists.txt

```cmake
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)

add_executable(publisher_node src/publisher_node.cpp)
ament_target_dependencies(publisher_node rclcpp std_msgs)

install(TARGETS
  publisher_node
  DESTINATION lib/${PROJECT_NAME}
)
```

---

## 🔨 Build & Run

```bash
cd /root/ros2_ws
colcon build --packages-select irrigation_robot
source install/setup.bash
ros2 run irrigation_robot publisher_node
```

**Expected output:**
```
[INFO] [timestamp] [soil_publisher]: Soil publisher started!
[INFO] [timestamp] [soil_publisher]: Published: 'moisture: 65%'
[INFO] [timestamp] [soil_publisher]: Published: 'moisture: 65%'
```

---

## 🔍 Inspect

```bash
ros2 topic list
# /soil_moisture

ros2 topic echo /soil_moisture
# data: 'moisture: 65%'
# ---

ros2 topic info /soil_moisture
# Type: std_msgs/msg/String
# Publisher count: 1
# Subscription count: 0

ros2 topic hz /soil_moisture
# average rate: 1.000
```

---

## 💡 Key C++ Concepts

| Code | What it does |
|------|-------------|
| `create_publisher<T>("/topic", qos)` | Creates a typed publisher |
| `std_msgs::msg::String` | ROS2 C++ message type (note the `msg::` namespace) |
| `auto msg = std_msgs::msg::String()` | Stack-allocated message object |
| `msg.data = "text"` | Set the message field |
| `publisher_->publish(msg)` | Send the message |
| `msg.data.c_str()` | Convert std::string to C string for RCLCPP_INFO |
| `rclcpp::Publisher<T>::SharedPtr` | Typed smart pointer for the publisher |

---

## ⚠️ Common C++ Mistakes

```cpp
// ❌ Wrong — message type missing msg:: namespace
rclcpp::Publisher<std_msgs::String>::SharedPtr pub_;

// ✅ Correct
rclcpp::Publisher<std_msgs::msg::String>::SharedPtr pub_;
```

```cpp
// ❌ Wrong — RCLCPP_INFO doesn't accept std::string directly
RCLCPP_INFO(get_logger(), msg.data);

// ✅ Correct — use format string + .c_str()
RCLCPP_INFO(get_logger(), "Data: '%s'", msg.data.c_str());
```

---

## 🔗 Navigation

[← Lesson 1 — Nodes](../lesson1_nodes/) &nbsp;|&nbsp; [Lesson 3 — Subscriber →](../lesson3_subscriber/)
