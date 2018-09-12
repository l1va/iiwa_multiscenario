// Basic robot reactions

//	UNITS
// Length:       mm
// Angle:        rad
// Velocity:     mm/s
// Rad.velocity: rad/s

#include "kuka/ros_iiwa_link.h"

#include <iostream>
#include <fstream>

// tmp
#include "kuka/iiwa_kinematics.h"
#include "fsm/torque_particle_filter.h"

// save to file
//#define LOG_JOINT "joints.csv"
//#define LOG_TORQUE "torque.csv"

#define FREQ  25.0              // message frequency

int main(int argc, char **argv) 
{
   // initialise ROS objects
   ros::init(argc, argv, "MultiscenarioControl");
   ros::NodeHandle nh("~");
   ros::Rate rate(FREQ);
   
   // file for log
   std::ofstream* logJoints = 0, *logTorques = 0;
   // uncomment for logging joint angles
#ifdef LOG_JOINT 
   std::ofstream fileJoints;
   fileJoints.open(LOG_JOINT);
   logJoints = &fileJoints;
#endif

#ifdef LOG_TORQUE
   std::ofstream fileTorques;
   fileTorques.open(LOG_TORQUE);
   logTorques = &fileTorques;
#endif

   // interaction with ROS
   RosIiwaLink robot(nh, logJoints, logTorques);
   // using smart servo
   // double servoStiffness = 100;
   //robot.configureSmartServo(servoStiffness);
   
   // robot state exchange
   RobotState& rs = RobotState::getInstance();
/*
   // run
   while(ros::ok() && rs.isContinue) {
       if(rs.isRobotConnected) {          
          rate.sleep();
       }
       else {
          ROS_ERROR("Robot is not connected...");
          ros::Duration(3.0).sleep();
       }
   }
*/
   Eigen::Matrix<double,JOINT_NO,1> q;
   for(int i = 0; i < JOINT_NO; i++) { q(i) = i*0.1; }

   TorqueParticleFilter filter;
   filter.prepareForJacobian(q);
   std::cout << filter.findJacobian(LINK01+LINK23+LINK45+LINK67) << std::endl;

   std::cout << iiwa14::Jacobian(q) << std::endl;
   //std::cout << iiwa14::jointCoordinate(q) << std::endl;
   
   
   // stoping
   ROS_INFO("Bye!");

   if(logJoints) logJoints->close();
   if(logTorques) logTorques->close();

   return 0;
}