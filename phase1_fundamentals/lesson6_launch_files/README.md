# Lesson 6 — Launch Files (C++)

**Phase:** P1 — ROS2 Fundamentals  
**Status:** ⏳ Upcoming  
**Concepts:** Python launch API, multi-node startup, arguments, remapping, YAML params

---

## 🧠 Concept

A **launch file** starts multiple compiled C++ nodes with one command, each configured correctly.  
Even though the nodes are C++, launch files are always written in **Python** — this is standard ROS2.

### Without launch file

```bash
# Terminal 1
ros2 run irrigation_robot publisher_node

# Terminal 2
ros2 run irrigation_robot subscriber_node

# Terminal 3
ros2 run irrigation_robot service_server

# Terminal 4 — with parameters
ros2 run irrigation_robot param_node --ros-args -p moisture_threshold:=40.0
```

### With launch file

```bash
ros2 launch irrigation_robot irrigation.launch.py moisture_threshold:=40.0
# All nodes start in one command, correctly configured
```

---

## 📄 Code

**`launch/irrigation.launch.py`**

```python
from launch import LaunchDescription
from launch_ros.actions import Node
from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration

def generate_launch_description():
    return LaunchDescription([

        # Declare CLI arguments
        DeclareLaunchArgument(
            'moisture_threshold',
            default_value='30.0',
            description='Moisture level that triggers a warning'
        ),

        # C++ node 1 — soil moisture publisher
        Node(
            package='irrigation_robot',
            executable='publisher_node',
            name='soil_publisher',
            output='screen',
        ),

        # C++ node 2 — subscriber
        Node(
            package='irrigation_robot',
            executable='subscriber_node',
            name='soil_listener',
            output='screen',
        ),

        # C++ node 3 — configurable sensor with parameters
        Node(
            package='irrigation_robot',
            executable='param_node',
            name='soil_sensor',
            parameters=[{
                'moisture_threshold': LaunchConfiguration('moisture_threshold'),
                'publish_rate': 0.5,
                'field_name': 'field_A',
            }],
            output='screen',
        ),

        # C++ node 4 — pump controller service server
        Node(
            package='irrigation_robot',
            executable='service_server',
            name='pump_controller',
            output='screen',
            # Remap topic names for multi-field setups
            remappings=[
                ('/pump', '/field_A/pump'),
            ],
        ),
    ])
```

---

## 🔧 CMakeLists.txt

```cmake
# Install the launch directory
install(DIRECTORY launch/
  DESTINATION share/${PROJECT_NAME}/launch
)
```

---

## 🔨 Build & Run

```bash
mkdir -p /root/ros2_ws/src/irrigation_robot/launch
# (create irrigation.launch.py in this folder via VS Code)

cd /root/ros2_ws
colcon build --packages-select irrigation_robot
source install/setup.bash

# Launch with default parameters
ros2 launch irrigation_robot irrigation.launch.py

# Override threshold at launch
ros2 launch irrigation_robot irrigation.launch.py moisture_threshold:=45.0
```

---

## 🔍 Inspect

```bash
# After launch — all nodes visible at once
ros2 node list

# All topics from all nodes
ros2 topic list

# Dry run — see what would launch without starting it
ros2 launch irrigation_robot irrigation.launch.py --show-args
```

---

## 💡 Key Concepts

| Code | What it does |
|------|-------------|
| `Node(package, executable, name)` | Launch a compiled C++ node |
| `parameters=[{...}]` | Pass parameters to the node |
| `remappings=[('/old', '/new')]` | Rename a topic for this node only |
| `DeclareLaunchArgument(name, default)` | Expose a CLI argument |
| `LaunchConfiguration('arg')` | Read the CLI argument value |
| `output='screen'` | Print logs to the terminal |

---

## 🔗 Navigation

[← Lesson 5 — Parameters](../lesson5_parameters/) &nbsp;|&nbsp; [Phase 2 — Packages & Workspace →](../../phase2_packages/)
