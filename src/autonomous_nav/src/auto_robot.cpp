#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <sensor_msgs/LaserScan.h>
#include <nav_msgs/Odometry.h>
#include <tf/transform_datatypes.h>

ros::Publisher cmdVelPub;
ros::Subscriber laserSub, odomSub;

bool obstacleDetected = false;
bool cuttingSquare = false;

double currentX = 0.0;
double currentY = 0.0;
double currentYaw = 0.0;

void laserCallback(const sensor_msgs::LaserScan::ConstPtr& msg) {
    for (float range : msg->ranges) {
        if (!std::isinf(range) && range < 0.5) {
            obstacleDetected = true;
            return;
        }
    }
    obstacleDetected = false;
}

void odomCallback(const nav_msgs::Odometry::ConstPtr& msg) {
    currentX = msg->pose.pose.position.x;
    currentY = msg->pose.pose.position.y;
    tf::Quaternion quat;
    tf::quaternionMsgToTF(msg->pose.pose.orientation, quat);
    currentYaw = tf::getYaw(quat);
}

void moveRobot(const geometry_msgs::Twist& moveMsg) {
    cmdVelPub.publish(moveMsg);
    ros::Duration(0.5).sleep();
}

void avoidObstacle() {
    ROS_INFO("Obstacle detected. Avoiding...");

    // Stop the robot
    geometry_msgs::Twist stopMsg;
    moveRobot(stopMsg);

    // Move right
    geometry_msgs::Twist avoidMsg;
    avoidMsg.linear.x = 0.0;  // Stop linear movement
    avoidMsg.angular.z = -0.5;  // Angular velocity for turning right
    moveRobot(avoidMsg);

    // Allow time to turn
    ros::Duration(2.0).sleep();

    // Stop turning
    moveRobot(stopMsg);
}

void cutGrassSquare() {
    ROS_INFO("Cutting grass in a square pattern...");

    int numRepetitions = 8;

    for (int i = 0; i < numRepetitions; ++i) {
        // Move forward for a certain duration
        geometry_msgs::Twist forwardMsg;
        forwardMsg.linear.x = 0.2;  // Linear velocity for moving forward
        moveRobot(forwardMsg);
        ros::Duration(50.0).sleep();  // Adjust duration as needed

        // Stop and turn right
        geometry_msgs::Twist turnMsg;
        turnMsg.linear.x = 0.0;  // Stop linear movement
        turnMsg.angular.z = -0.5;  // Angular velocity for turning right
        moveRobot(turnMsg);
        ros::Duration(50.0).sleep();  // Adjust duration as needed

        // Stop turning after turning right
        geometry_msgs::Twist stopTurningMsg;
        moveRobot(stopTurningMsg);
    }

    ROS_INFO("Finished cutting grass in a square pattern.");
}
    // // Move forward for a certain duration
    // geometry_msgs::Twist forwardMsg;
    // forwardMsg.linear.x = 0.2;  // Linear velocity for moving forward
    // moveRobot(forwardMsg);
    // ros::Duration(50.0).sleep();  // Adjust duration as needed

    // // Stop and turn right
    // geometry_msgs::Twist turnMsg;
    // turnMsg.linear.x = 0.0;  // Stop linear movement
    // turnMsg.angular.z = -0.5;  // Angular velocity for turning right
    // moveRobot(turnMsg);
    // ros::Duration(50.0).sleep();  // Adjust duration as needed

    // // Stop turning
    // geometry_msgs::Twist stopMsg;
    // moveRobot(stopMsg);

    // // Repeat the process to cut grass in a square pattern
    //}

int main(int argc, char** argv) {
    ros::init(argc, argv, "grass_cutter_node");
    ros::NodeHandle nh;

    cmdVelPub = nh.advertise<geometry_msgs::Twist>("/cmd_vel", 10);
    laserSub = nh.subscribe<sensor_msgs::LaserScan>("/scan", 10, laserCallback);
    odomSub = nh.subscribe<nav_msgs::Odometry>("/odom", 10, odomCallback);

    ros::Rate rate(10);

    while (ros::ok()) {
        ros::spinOnce();

        if (obstacleDetected) {
            avoidObstacle();
        } 
        else {
            cutGrassSquare();
        }

        ROS_INFO("Current Pose: x=%f, y=%f, Yaw=%f", currentX, currentY, currentYaw);

        rate.sleep();
    }

    return 0;
}
