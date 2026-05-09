# Lesson 4 — Services (C++)

**Phase:** P1 — ROS2 Fundamentals  
**Status:** ⏳ Upcoming  
**Concepts:** create_service, create_client, request/response, async call, Future, SetBool

---

## 🧠 Concept

A **service** is a request → response pattern.  
Unlike topics (fire and forget), services wait for a reply — like a function call across node boundaries.

### Topics vs Services

| | Topic | Service |
|--|-------|---------|
| Pattern | Publish / Subscribe | Request / Response |
| Reply | None | Yes — waits for it |
| Use case | Continuous sensor data | Trigger action, query value |
| C++ type | `rclcpp::Publisher` / `rclcpp::Subscription` | `rclcpp::Service` / `rclcpp::Client` |

### Thesis analogy

| Irrigation Thesis | ROS2 Service |
|------------------|-------------|
| Web dashboard sends "Pump ON" HTTP request | Client sends `SetBool::Request{data: true}` |
| STM32 acts and replies "OK" | Server handles and returns `SetBool::Response{success: true}` |

---

## 📄 Code

**`src/service_server.cpp`** — receives pump commands

```cpp
#include "rclcpp/rclcpp.hpp"
#include "std_srvs/srv/set_bool.hpp"

using SetBool = std_srvs::srv::SetBool;

class PumpController : public rclcpp::Node
{
public:
  PumpController() : Node("pump_controller")
  {
    server_ = create_service<SetBool>(
      "/pump",
      std::bind(&PumpController::handle_request, this,
        std::placeholders::_1, std::placeholders::_2)
    );
    RCLCPP_INFO(get_logger(), "Pump controller ready.");
  }

private:
  void handle_request(
    const SetBool::Request::SharedPtr request,
    SetBool::Response::SharedPtr response)
  {
    if (request->data) {
      RCLCPP_INFO(get_logger(), "Pump → ON");
      response->success = true;
      response->message = "Pump turned ON";
    } else {
      RCLCPP_INFO(get_logger(), "Pump → OFF");
      response->success = true;
      response->message = "Pump turned OFF";
    }
  }

  rclcpp::Service<SetBool>::SharedPtr server_;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<PumpController>());
  rclcpp::shutdown();
  return 0;
}
```

---

**`src/service_client.cpp`** — sends pump commands

```cpp
#include "rclcpp/rclcpp.hpp"
#include "std_srvs/srv/set_bool.hpp"

using SetBool = std_srvs::srv::SetBool;

class PumpSwitch : public rclcpp::Node
{
public:
  PumpSwitch() : Node("pump_switch")
  {
    client_ = create_client<SetBool>("/pump");
    while (!client_->wait_for_service(std::chrono::seconds(1))) {
      RCLCPP_INFO(get_logger(), "Waiting for pump controller...");
    }
  }

  void send_command(bool turn_on)
  {
    auto request = std::make_shared<SetBool::Request>();
    request->data = turn_on;

    auto future = client_->async_send_request(request);

    if (rclcpp::spin_until_future_complete(shared_from_this(), future) ==
      rclcpp::FutureReturnCode::SUCCESS)
    {
      RCLCPP_INFO(get_logger(), "Response: %s", future.get()->message.c_str());
    }
  }

private:
  rclcpp::Client<SetBool>::SharedPtr client_;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  auto node = std::make_shared<PumpSwitch>();
  node->send_command(true);    // Pump ON
  node->send_command(false);   // Pump OFF
  rclcpp::shutdown();
  return 0;
}
```

---

## 🔧 CMakeLists.txt

```cmake
find_package(rclcpp REQUIRED)
find_package(std_srvs REQUIRED)

add_executable(service_server src/service_server.cpp)
ament_target_dependencies(service_server rclcpp std_srvs)

add_executable(service_client src/service_client.cpp)
ament_target_dependencies(service_client rclcpp std_srvs)

install(TARGETS
  service_server
  service_client
  DESTINATION lib/${PROJECT_NAME}
)
```

---

## 🔨 Build & Run

```bash
cd /root/ros2_ws
colcon build --packages-select irrigation_robot
source install/setup.bash
```

**Terminal 1 — server:**
```bash
ros2 run irrigation_robot service_server
```

**Terminal 2 — client:**
```bash
ros2 run irrigation_robot service_client
```

**Expected server output:**
```
[INFO] [pump_controller]: Pump controller ready.
[INFO] [pump_controller]: Pump → ON
[INFO] [pump_controller]: Pump → OFF
```

**Expected client output:**
```
[INFO] [pump_switch]: Response: Pump turned ON
[INFO] [pump_switch]: Response: Pump turned OFF
```

---

## 🔍 Inspect

```bash
ros2 service list
# /pump
# /pump_controller/...

ros2 service type /pump
# std_srvs/srv/SetBool

# Call manually without client code
ros2 service call /pump std_srvs/srv/SetBool "{data: true}"
ros2 service call /pump std_srvs/srv/SetBool "{data: false}"
```

---

## 💡 Key C++ Concepts

| Code | What it does |
|------|-------------|
| `create_service<T>(name, callback)` | Register a service server |
| `create_client<T>(name)` | Create a service client |
| `wait_for_service(timeout)` | Block until server is available |
| `client_->async_send_request(req)` | Send request, returns a std::future |
| `spin_until_future_complete(node, future)` | Wait for the response |
| `future.get()->message` | Access response fields |
| `shared_from_this()` | Get a SharedPtr to `this` node (requires inheriting enable_shared_from_this via Node) |
| `std::placeholders::_1, _2` | Placeholders for request and response in server callback |

---

## ⚠️ Common C++ Mistakes

```cpp
// ❌ Wrong — request is a SharedPtr, use -> not .
request.data = true;

// ✅ Correct
request->data = true;
```

```cpp
// ❌ Wrong — can't get SharedPtr to this without shared_from_this()
rclcpp::spin_until_future_complete(this, future);

// ✅ Correct
rclcpp::spin_until_future_complete(shared_from_this(), future);
```

---

## 🔗 Navigation

[← Lesson 3 — Subscriber](../lesson3_subscriber/) &nbsp;|&nbsp; [Lesson 5 — Parameters →](../lesson5_parameters/)
