# Lesson 5 — Parameters (C++)

**Phase:** P1 — ROS2 Fundamentals  
**Status:** ⏳ Upcoming  
**Concepts:** declare_parameter, get_parameter, YAML config, param callbacks, rcl_interfaces

---

## 🧠 Concept

**Parameters** are named configuration values owned by a node.  
Instead of hardcoding constants, declare them as parameters so they can be changed at runtime — without recompiling.

### Thesis analogy

| Hardcoded (bad) | Parameter (good) |
|----------------|-----------------|
| `float threshold = 30.0f;` in source | `declare_parameter("moisture_threshold", 30.0)` |
| Recompile to change value | `ros2 param set /soil_sensor moisture_threshold 45.0` |
| Fixed publish rate | `declare_parameter("publish_rate", 1.0)` |

---

## 📄 Code

**`src/param_node.cpp`**

```cpp
#include <chrono>
#include "rclcpp/rclcpp.hpp"

using namespace std::chrono_literals;

class SoilSensor : public rclcpp::Node
{
public:
  SoilSensor() : Node("soil_sensor")
  {
    // Declare parameters with default values
    declare_parameter("publish_rate", 1.0);
    declare_parameter("moisture_threshold", 30.0);
    declare_parameter("field_name", std::string("field_A"));

    // Read values
    double rate      = get_parameter("publish_rate").as_double();
    threshold_       = get_parameter("moisture_threshold").as_double();
    std::string field = get_parameter("field_name").as_string();

    RCLCPP_INFO(get_logger(), "Field: %s | Rate: %.1fs | Threshold: %.1f%%",
      field.c_str(), rate, threshold_);

    auto period = std::chrono::duration<double>(rate);
    timer_ = create_timer(period,
      std::bind(&SoilSensor::timer_callback, this));
  }

private:
  void timer_callback()
  {
    double moisture = 45.0;  // replace with real sensor read
    if (moisture < threshold_) {
      RCLCPP_WARN(get_logger(), "LOW MOISTURE: %.1f%% < %.1f%%",
        moisture, threshold_);
    } else {
      RCLCPP_INFO(get_logger(), "Moisture OK: %.1f%%", moisture);
    }
  }

  double threshold_;
  rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<SoilSensor>());
  rclcpp::shutdown();
  return 0;
}
```

---

## 📄 YAML Config File

**`config/sensor_params.yaml`**

```yaml
soil_sensor:
  ros__parameters:
    publish_rate: 0.5
    moisture_threshold: 35.0
    field_name: "field_B"
```

> ⚠️ Note the double underscore in `ros__parameters` — this is required by ROS2.

---

## 🔧 CMakeLists.txt

```cmake
find_package(rclcpp REQUIRED)

add_executable(param_node src/param_node.cpp)
ament_target_dependencies(param_node rclcpp)

# Install the config directory
install(DIRECTORY config/
  DESTINATION share/${PROJECT_NAME}/config
)

install(TARGETS param_node
  DESTINATION lib/${PROJECT_NAME}
)
```

---

## 🔨 Build & Run

```bash
cd /root/ros2_ws
colcon build --packages-select irrigation_robot
source install/setup.bash

# With default parameters
ros2 run irrigation_robot param_node

# With YAML config file
ros2 run irrigation_robot param_node \
  --ros-args --params-file src/irrigation_robot/config/sensor_params.yaml

# Override a single parameter at launch
ros2 run irrigation_robot param_node \
  --ros-args -p moisture_threshold:=50.0
```

---

## 🔍 Inspect

```bash
ros2 param list /soil_sensor
ros2 param get /soil_sensor moisture_threshold
ros2 param set /soil_sensor moisture_threshold 50.0
ros2 param dump /soil_sensor
```

---

## 💡 Key C++ Concepts

| Code | What it does |
|------|-------------|
| `declare_parameter("name", default)` | Register param with a typed default |
| `get_parameter("name").as_double()` | Read as double |
| `get_parameter("name").as_string()` | Read as std::string |
| `get_parameter("name").as_int()` | Read as int64_t |
| `get_parameter("name").as_bool()` | Read as bool |
| `--ros-args --params-file file.yaml` | Load params from YAML |
| `--ros-args -p name:=value` | Override a single param at launch |

---

## 🔗 Navigation

[← Lesson 4 — Services](../lesson4_services/) &nbsp;|&nbsp; [Lesson 6 — Launch Files →](../lesson6_launch_files/)
