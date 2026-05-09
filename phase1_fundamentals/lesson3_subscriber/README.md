# Lesson 3 — Subscriber (C++)

**Phase:** P1 — ROS2 Fundamentals  
**Status:** ⏳ Upcoming  
**Concepts:** create_subscription, callback with message reference, loose coupling

---

## 🧠 Concept

A **subscriber** listens on a named topic and fires a callback every time a message arrives.  
It does not know who publishes — only the topic name and message type matter.

### Loose coupling

Change the publisher → subscriber reacts instantly.  
**Zero changes to subscriber code.** This is the core power of ROS2 topics.

```
[SoilPublisher] ──── /soil_moisture ────► [SoilListener]
  publishes every 1s                        callback fires on arrival
  doesn't know who listens                  doesn't know who publishes
```

---

## 📄 Code

**`src/subscriber_node.cpp`**

```cpp
#include <memory>
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

class SoilListener : public rclcpp::Node
{
public:
  SoilListener() : Node("soil_listener")
  {
    subscription_ = create_subscription<std_msgs::msg::String>(
      "/soil_moisture",
      10,
      std::bind(&SoilListener::listener_callback, this, std::placeholders::_1)
    );
    RCLCPP_INFO(get_logger(), "Listening for soil moisture data...");
  }

private:
  void listener_callback(const std_msgs::msg::String & msg)
  {
    RCLCPP_INFO(get_logger(), "Received: '%s'", msg.data.c_str());
  }

  rclcpp::Subscription<std_msgs::msg::String>::SharedPtr subscription_;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<SoilListener>());
  rclcpp::shutdown();
  return 0;
}
```

---

## 🔧 CMakeLists.txt

```cmake
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)

add_executable(subscriber_node src/subscriber_node.cpp)
ament_target_dependencies(subscriber_node rclcpp std_msgs)

install(TARGETS
  subscriber_node
  DESTINATION lib/${PROJECT_NAME}
)
```

---

## 🔨 Build & Run

Open two terminals (both inside Docker):

**Terminal 1 — run publisher:**
```bash
ros2 run irrigation_robot publisher_node
```

**Terminal 2 — run subscriber:**
```bash
ros2 run irrigation_robot subscriber_node
```

**Expected subscriber output:**
```
[INFO] [timestamp] [soil_listener]: Listening for soil moisture data...
[INFO] [timestamp] [soil_listener]: Received: 'moisture: 65%'
[INFO] [timestamp] [soil_listener]: Received: 'moisture: 65%'
```

---

## 🔍 Inspect

```bash
ros2 topic info /soil_moisture
# Publisher count: 1
# Subscription count: 1    ← now it's 1

ros2 node list
# /soil_publisher
# /soil_listener
```

---

## 💡 Key C++ Concepts

| Code | What it does |
|------|-------------|
| `create_subscription<T>(topic, qos, cb)` | Register a typed subscriber |
| `std::bind(&Class::cb, this, std::placeholders::_1)` | Bind member callback with one argument |
| `const std_msgs::msg::String & msg` | Const reference — avoids copying the message |
| `std::placeholders::_1` | Placeholder for the message argument |
| `rclcpp::Subscription<T>::SharedPtr` | Typed smart pointer for the subscription |

---

## ⚠️ Common C++ Mistakes

```cpp
// ❌ Wrong — missing std::placeholders::_1 for the message argument
std::bind(&SoilListener::listener_callback, this)

// ✅ Correct
std::bind(&SoilListener::listener_callback, this, std::placeholders::_1)
```

```cpp
// ❌ Wrong — takes message by value (unnecessary copy)
void listener_callback(std_msgs::msg::String msg)

// ✅ Correct — const reference, zero copy
void listener_callback(const std_msgs::msg::String & msg)
```

---

## 🧪 Experiment

1. Change `msg.data` in `publisher_node.cpp` to `"moisture: 23% — CRITICAL LOW"`
2. Rebuild: `colcon build --packages-select irrigation_robot`
3. Restart only the publisher
4. Watch the subscriber react — **no rebuild needed on subscriber side**

---

## 🔗 Navigation

[← Lesson 2 — Publisher](../lesson2_publisher/) &nbsp;|&nbsp; [Lesson 4 — Services →](../lesson4_services/)
