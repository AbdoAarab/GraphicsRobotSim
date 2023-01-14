// Robot.cpp
#include "Robot.h"

#include <algorithm>
#include <iostream>

#define DISPLACEMENT_SCALE 2000

Robot::Robot(double x_position, double y_position) : x_position(x_position), y_position(y_position)
{
  orientation = 0;
  left_wheel_velocity = 0;
  right_wheel_velocity = 0;
}

void Robot::update()
{
  // Limit the angular velocities to the maximum allowed value
  left_wheel_velocity = std::max(-max_angular_velocity / 10, std::min(max_angular_velocity / 10, left_wheel_velocity));
  right_wheel_velocity = std::max(-max_angular_velocity / 10, std::min(max_angular_velocity / 10, right_wheel_velocity));

  // Calculate the distance traveled by each wheel
  left_displacement = left_wheel_velocity * delta_t * wheel_radius;
  right_displacement = right_wheel_velocity * delta_t * wheel_radius;

  // Calculate curveture radius
  if (left_displacement != right_displacement)
    curvature_radius = wheel_distance * (left_displacement + right_displacement) / (2 * (left_displacement - right_displacement));

  // Calculate the displacement along the front axis of the robot and the reorientation
  displacement = (left_displacement + right_displacement) / 2;
  reorientation = (left_displacement - right_displacement) / wheel_distance;

  // Update the orientation of the robot
  orientation += reorientation * (180 / M_PI);

  // Contrain the orientation between [-PI,+PI]
  orientation = remainder(orientation, 2 * M_PI);

  // Update the position  of the robot
  x_position += displacement * cos(orientation) * DISPLACEMENT_SCALE;
  y_position += displacement * sin(orientation) * DISPLACEMENT_SCALE;

  velocity = displacement / delta_t;

  //displayDataInTerminal();
}

void Robot::displayDataInTerminal()
{
  std::cout<<"\33[2K\r" << "Robot's Data {" << std::endl<< std::flush;
  // std::cout<<"\33[2K\r" << "| Constant Data {" << std::endl<< std::flush;
  // std::cout<<"\33[2K\r" << "|  | Dt = " << delta_t * 10 << " s" << std::endl<< std::flush;
  // std::cout<<"\33[2K\r" << "|  | Rr = " << radius << " m" << std::endl<< std::flush;
  // std::cout<<"\33[2K\r" << "|  | R0 = " << wheel_radius << " m" << std::endl<< std::flush;
  // std::cout<<"\33[2K\r" << "|  | D = " << wheel_distance << " m" << std::endl<< std::flush;
  // std::cout<<"\33[2K\r" << "|  | w0Max = " << max_angular_velocity <<" rad/s"<< std::endl<< std::flush;
  // std::cout<<"\33[2K\r" << "|  | Dw0Max = " << max_angular_acceleration <<" rad/s2"<< std::endl<< std::flush;
  // std::cout<<"\33[2K\r" << "| }" << std::endl<< std::flush;
  std::cout<<"\33[2K\r" << "| Variable Data {" << std::endl<< std::flush;
  std::cout<<"\33[2K\r" << "|  | Xr = " << (int)x_position << ",  Yr = " << (int)y_position << std::endl<< std::flush;
  std::cout<<"\33[2K\r" << "|  | Wg = " << left_wheel_velocity*10 << ",  Wd = " << right_wheel_velocity*10 << std::endl<< std::flush;
  std::cout<<"\33[2K\r" << "|  | Alpha = " << orientation << std::endl<< std::flush;
  std::cout<<"\33[2K\r" << "|  | Dalpha = " << reorientation << std::endl<< std::flush;
  std::cout<<"\33[2K\r" << "|  | Dr = " << displacement <<" m"<< " - (Dg = " << left_displacement << ",  Dd = " << right_displacement << ")"<< std::endl<< std::flush;
  std::cout<<"\33[2K\r" << "|  | Rc = " << curvature_radius << std::endl<< std::flush;
  std::cout<<"\33[2K\r" << "|  | Vr= " << velocity<<" m/s" << std::endl<< std::flush;
  std::cout<<"\33[2K\r" << "| }" << std::endl<< std::flush;
  std::cout<<"\33[2K\r" << "}" << std::endl<< std::flush;
}

void Robot::setXPosition(double x) { x_position = x; }
void Robot::setYPosition(double y) { y_position = y; }
void Robot::setOrientation(double angle) { orientation = angle; }
void Robot::setLeftWheelVelocity(double velocity) { left_wheel_velocity = velocity; }
void Robot::setRightWheelVelocity(double velocity) { right_wheel_velocity = velocity; }

double Robot::getXPosition() const { return x_position; }
double Robot::getYPosition() const { return y_position; }
double Robot::getOrientation() const { return orientation; }
double Robot::getMaxAngularVelocity() const { return max_angular_velocity; }
double Robot::getRadius() const { return radius; }
double Robot::getWheelRadius() const { return wheel_radius; }
double Robot::getWheelDistance() const { return wheel_distance; }
double Robot::getLeftWheelVelocity() { return left_wheel_velocity; }
double Robot::getRightWheelVelocity() { return right_wheel_velocity; }
double Robot::getDeltaTime() const { return delta_t; }
double Robot::getCurvatureRadius() const { return curvature_radius; }
double Robot::getDisplacement() { return displacement; }
double Robot::getReOrientation() { return reorientation; }
double Robot::getLeftDistance() { return left_displacement; }
double Robot::getRightDistance() { return right_displacement; }
double Robot::getVelocity() { return velocity; }

double Robot::getSensorMaxDistance() { return sensor_max_distance; }
double Robot::getSensorMaxAngle() { return sensor_max_angle; }