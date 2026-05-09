# ROS2 Journey 🤖

Learning ROS2 from zero toward a Master's degree at **KTH Royal Institute of Technology**
in Systems, Control & Robotics.

**Background:** Agricultural Engineering (Universitas Padjadjaran) — irrigation automation
with STM32, CAN Bus, ESP32, and SIM900A.

---

## Progress

| Phase | Topic | Status |
|-------|-------|--------|
| P1 | ROS2 Fundamentals | 🔄 In Progress |
| P2 | Packages & Workspace | ⏳ Upcoming |
| P3 | Robot Modeling & Visualization | ⏳ Upcoming |
| P4 | Perception & Sensors | ⏳ Upcoming |
| P5 | Autonomous Navigation (Nav2) | ⏳ Upcoming |
| P6 | Embedded & Hardware (micro-ROS) | ⏳ Upcoming |
| P7 | Manipulation with MoveIt2 | ⏳ Upcoming |
| P8 | Autonomous Vehicles (Autoware) | ⏳ Upcoming |
| P9 | Research & AI Integration | ⏳ Upcoming |

---

## Phase 1 — ROS2 Fundamentals

### Lesson 1 — Nodes
A node is one running process in the ROS2 graph. Each does one job.

### Lesson 2 — Publisher
Broadcasts data on a topic. Built a soil moisture publisher
simulating the sensor node from my irrigation thesis.

### Lesson 3 — Subscriber  
Listens to a topic and reacts. Built a listener that receives
moisture readings without the publisher knowing it exists.

### Lesson 4 — Services
Request/response communication. Built a pump controller
server that turns a pump ON/OFF on command.

---

## Setup

```bash
# Pull ROS2 Jazzy via Docker (Xubuntu 26)
docker pull osrf/ros:jazzy-desktop

# Launch container
~/ros2.sh

# Inside container — source ROS2
source /opt/ros/jazzy/setup.bash
```