// #include <ros/ros.h>
// #include <move_base_msgs/MoveBaseAction.h>
// #include <actionlib/client/simple_action_client.h>

// typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;

// int main(int argc, char **argv) {
//     ros::init(argc, argv, "autonomous_navigation_node");
//     MoveBaseClient ac("move_base", true);

//     while (!ac.waitForServer(ros::Duration(5.0))) {
//         ROS_INFO("Waiting for the move_base action server to come up");
//     }

//     move_base_msgs::MoveBaseGoal goal;

//     // Set the goal position and orientation here
//     goal.target_pose.header.frame_id = "map";
//     goal.target_pose.header.stamp = ros::Time::now();
//     goal.target_pose.pose.position.x = 1.0;
//     goal.target_pose.pose.position.y = 1.0;
//     goal.target_pose.pose.orientation.w = 0.0;

//     ROS_INFO("Sending goal");
//     ac.sendGoal(goal);

//     ac.waitForResult();

//     if (ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED) {
//         ROS_INFO("Hooray, the base reached the goal");
//     } else {
//         ROS_WARN("The base failed to move to the goal");
//     }

//     return 0;
// }
// #include <ros/ros.h>
// #include <geometry_msgs/Twist.h>
// #include <sensor_msgs/LaserScan.h>
// #include <move_base_msgs/MoveBaseAction.h>
// #include <actionlib/client/simple_action_client.h>

// typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;

// ros::Publisher cmdVelPub;
// MoveBaseClient* moveBaseClient;

// void laserCallback(const sensor_msgs::LaserScan::ConstPtr& msg) {
//     // Check for obstacles in the laser scan data
//     for (float range : msg->ranges) {
//         if (!std::isinf(range) && range < 0.5) {
//             // Obstacle detected, plan a new path
//             move_base_msgs::MoveBaseGoal goal;

//             // Set the new goal position randomly (you may use a more sophisticated approach)
//             goal.target_pose.header.frame_id = "odom";  // Adjust the frame_id as needed
//             goal.target_pose.header.stamp = ros::Time::now();
//             goal.target_pose.pose.position.x = 2.0;
//             goal.target_pose.pose.position.y = 0.0;
//             goal.target_pose.pose.orientation.w = 1.0;

//             ROS_INFO("Obstacle detected. Planning a new path.");
//             moveBaseClient->sendGoal(goal);

//             // Wait for the result with a timeout
//             moveBaseClient->waitForResult(ros::Duration(60.0));

//             if (moveBaseClient->getState() == actionlib::SimpleClientGoalState::SUCCEEDED) {
//                 ROS_INFO("New path planned successfully.");
//             } else {
//                 ROS_WARN("Failed to plan a new path.");
//             }

//             return;
//         }
//     }

//     // No obstacles, continue moving forward
//     geometry_msgs::Twist moveMsg;
//     moveMsg.linear.x = 0.2;  // Adjust the linear velocity as needed
//     cmdVelPub.publish(moveMsg);
// }

// int main(int argc, char** argv) {
//     ros::init(argc, argv, "grass_cutter");
//     ros::NodeHandle nh;

//     cmdVelPub = nh.advertise<geometry_msgs::Twist>("/cmd_vel", 10);
//     moveBaseClient = new MoveBaseClient("/move_base", true);

//     ros::Subscriber laserSub = nh.subscribe<sensor_msgs::LaserScan>("/scan", 10, laserCallback);

//     while (!moveBaseClient->waitForServer(ros::Duration(5.0))) {
//         ROS_INFO("Waiting for the move_base action server to come up");
//     }

//     ros::spin();

//     delete moveBaseClient;
//     return 0;
// }
