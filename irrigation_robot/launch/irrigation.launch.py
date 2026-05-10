from launch import LaunchDescription
from launch_ros.actions import Node
from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration

def generate_launch_description():
    return LaunchDescription([

        DeclareLaunchArgument(
            'moisture_threshold',
            default_value='30.0',
            description='Moisture level that triggers a warning'
        ),

        Node(
            package='irrigation_robot',
            executable='lesson2_publisher',
            name='soil_publisher',
            output='screen',
        ),

        Node(
            package='irrigation_robot',
            executable='lesson3_subscriber',
            name='soil_listener',
            output='screen',
        ),

        Node(
            package='irrigation_robot',
            executable='lesson5_param_node',
            name='soil_sensor',
            parameters=[{
                'moisture_threshold': LaunchConfiguration('moisture_threshold'),
                'publish_rate': 1.0,
                'field_name': 'field_A',
            }],
            output='screen',
        ),

        Node(
            package='irrigation_robot',
            executable='lesson4_service_server',
            name='pump_controller',
            output='screen',
        ),

    ])
