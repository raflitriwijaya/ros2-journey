# Lesson 1 — Minimal Node (C++)

**Phase:** P1 — ROS2 Fundamentals  
**Status:** 🔄 In Progress  
**Concepts:** rclcpp::Node, RCLCPP_INFO, create_timer, spin, SharedPtr

---

## 🧠 Concept

A **node** is one compiled executable in the ROS2 graph.  
Every node does exactly one job. They communicate through topics and services — never by calling each other directly.

### Python vs C++ side by side

| Python | C++ |
|--------|-----|
| `class MyNode(Node):` | `class MyNode : public rclcpp::Node` |
| `super().__init__('name')` | `: Node("name")` in constructor initializer list |
| `self.get_logger().info()` | `RCLCPP_INFO(get_logger(), ...)` |
| `self.create_timer(1.0, cb)` | `create_timer(1s, std::bind(&MyNode::cb, this))` |
| `rclcpp.spin(node)` | `rclcpp::spin(std::make_shared<MyNode>())` |
| Run: `python3 node.py` | Run: `colcon build` → `ros2 run pkg node` |

---

## 📄 Code

**`src/minimal_node.cpp`**

```cpp
#include <chrono>
#include "rclcpp/rclcpp.hpp"

using namespace std::chrono_literals;   // lets us write 1s, 500ms, 100ms

class MyNode : public rclcpp::Node
{
public:
  MyNode() : Node("my_first_node")      // registers name in ROS2 graph
  {
    RCLCPP_INFO(get_logger(), "Node is alive!");
    timer_ = create_timer(1s, std::bind(&MyNode::timer_callback, this));
  }

private:
  void timer_callback()
  {
    RCLCPP_INFO(get_logger(), "1 second passed...");
  }

  rclcpp::TimerBase::SharedPtr timer_;  // keeps the timer alive
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);                     // start ROS2
  rclcpp::spin(std::make_shared<MyNode>());     // create + keep alive
  rclcpp::shutdown();                           // clean up on Ctrl+C
  return 0;
}
```

---

## 🔧 CMakeLists.txt

Add inside `irrigation_robot/CMakeLists.txt`:

```cmake
find_package(rclcpp REQUIRED)

add_executable(minimal_node src/minimal_node.cpp)
ament_target_dependencies(minimal_node rclcpp)

install(TARGETS
  minimal_node
  DESTINATION lib/${PROJECT_NAME}
)
```

---

## 🔨 Build & Run

```bash
cd /root/ros2_ws
colcon build --packages-select irrigation_robot
source install/setup.bash
ros2 run irrigation_robot minimal_node
```

**Expected output:**
```
[INFO] [timestamp] [my_first_node]: Node is alive!
[INFO] [timestamp] [my_first_node]: 1 second passed...
[INFO] [timestamp] [my_first_node]: 1 second passed...
```

---

## 🔍 Inspect

```bash
ros2 node list
# /my_first_node

ros2 node info /my_first_node
# Subscribers:   /parameter_events
# Publishers:    /rosout, /parameter_events
# Service Servers: /my_first_node/get_parameters ...
```

---

## 💡 Key C++ Concepts

| Code | What it does |
|------|-------------|
| `public rclcpp::Node` | Inherit from Node — your class IS a ROS2 node |
| `: Node("name")` | Constructor initializer list — calls parent constructor |
| `RCLCPP_INFO(get_logger(), "msg")` | ROS2 logger macro — never use `std::cout` in ROS2 |
| `create_timer(1s, ...)` | Timer fires every 1 second |
| `std::bind(&MyNode::cb, this)` | Bind member function as a callback |
| `rclcpp::TimerBase::SharedPtr` | Smart pointer — timer stays alive as long as the node does |
| `std::make_shared<MyNode>()` | Create node on the heap, managed by shared pointer |

---

## ⚠️ Common C++ Mistakes

```cpp
// ❌ Wrong — raw pointer, timer gets destroyed immediately
rclcpp::TimerBase * timer_ = create_timer(...);

// ✅ Correct — SharedPtr keeps it alive
rclcpp::TimerBase::SharedPtr timer_ = create_timer(...);
```

```cpp
// ❌ Wrong — std::cout bypasses ROS2 logging
std::cout << "hello" << std::endl;

// ✅ Correct — shows timestamp + node name
RCLCPP_INFO(get_logger(), "hello");
```

---

## 🔗 Next Lesson

[Lesson 2 — Publisher →](../lesson2_publisher/)
