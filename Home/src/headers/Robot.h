// Robot.h
#ifndef ROBOT_H
#define ROBOT_H

#include <cmath>

class Robot
{
public:
  Robot(double x_position, double y_position);

  // Setters
  void setLeftWheelVelocity(double velocity);
  void setRightWheelVelocity(double velocity);
  void setXPosition(double x);
  void setYPosition(double y);
  void setOrientation(double angle);

  // Getters
  double getXPosition() const;
  double getYPosition() const;
  double getOrientation() const;
  double getRadius() const;
  double getMaxAngularVelocity() const;
  double getWheelRadius() const;
  double getWheelDistance() const;
  double getDeltaTime() const;
  double getCurvatureRadius() const;

  double getDisplacement();
  double getReOrientation();
  double getLeftDistance();
  double getRightDistance();
  double getVelocity();
  double getLeftWheelVelocity();
  double getRightWheelVelocity();

  double getSensorMaxDistance();
  double getSensorMaxAngle();

  // Member functions
  void update();
  void displayDataInTerminal();

private:
  // Constants
  static constexpr double delta_t = 0.1;                  // Dt = 1 s - Delta time in seconds (time between each update to the position & orientation)
  static constexpr double radius = 0.05;                  // Rr = 0.05 m - Radius of the robot in meters
  static constexpr double wheel_radius = 0.02;            // R0 = 0.02 m - Radius of each wheel in meters
  static constexpr double wheel_distance = 0.05;          // D = 0.05 m - Distance between the wheels in meters
  static constexpr double max_angular_velocity = 10.0;    // w0Max = 10 rad/s - The wheels' maximum angular velocity in rad/s
  static constexpr double max_angular_acceleration = 2.0; // Dw0Max = 2 rad/s2 - The wheels' maximum angular acceleration in rad/s

  double x_position;                 // Xr coordinate of the robot's position in pixels
  double y_position;                 // Yr coordinate of the robot's position in pixels
  double orientation;                // Alpha angle representing the orientation of the robot in radians
  double left_wheel_velocity = 0.0;  // wg - Angular velocity of the left wheel in rad/s
  double right_wheel_velocity = 0.0; // wd - Angular velocity of the right wheel in rad/s

  double displacement;       // Dr - Displacement of the robot during Dt in meters
  double reorientation;      // Dalpha - Reorientation during Dt in radians
  double left_displacement;  // Dg - Displacement of the left wheel during Dt in meters
  double right_displacement; // Dd - Displacement of the right wheel during Dt in meters
  double velocity;           // Vr - Velocity of the robot
  double curvature_radius;   // Rc - Curvature radius of the robot's trajectory in meters

  double sensor_max_distance = 10; // The maximum distance where the sensor can detect an object
  double sensor_max_angle = M_PI;  // The maximum angle where the sensor can detect an object
};

#endif // ROBOT_H
