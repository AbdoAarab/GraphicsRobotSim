// CollisionSystem.h
#ifndef COLLISION_SYSTEM_H
#define COLLISION_SYSTEM_H

#include "Robot.h"
#include "Circle.h"
#include "DrawingSystem.h"

#include <vector>

class CollisionSystem
{
public:
  
  // Constructor
  CollisionSystem(DrawingSystem &drawing_system, Robot &robot, std::vector<Circle> &obstacles, std::vector<Circle> &goals);
  
  // Member functions
  void update();
  bool checkCirclesCollision(Circle circle, Circle other);
  void checkObstaclesCollision(double robot_x, double robot_y, double robot_radius);
  void checkGoalsCollision(double robot_x, double robot_y, double robot_radius);
  void checkBorderCollsion(double robot_x, double robot_y, double robot_radius, double window_width, double window_height, double window_border);

private:
  
  DrawingSystem &drawing_system;
  Robot &robot;
  std::vector<Circle> &obstacles;
  std::vector<Circle> &goals;
};

#endif // COLLISION_SYSTEM_H