// CollisionSystem.cpp
#include "CollisionSystem.h"

#include <cmath>

CollisionSystem::CollisionSystem(DrawingSystem &drawing_system, Robot &robot, std::vector<Circle> &obstacles, std::vector<Circle> &goals) : drawing_system(drawing_system), robot(robot), obstacles(obstacles), goals(goals)
{
}

void CollisionSystem::update()
{
  double window_border = drawing_system.getBorder(),
         window_width = drawing_system.getWidth(),
         window_height = drawing_system.getHeight();
  double robot_x = robot.getXPosition(),
         robot_y = robot.getYPosition(),
         robot_radius = robot.getRadius() * drawing_system.getScale();

  checkObstaclesCollision(robot_x, robot_y, robot_radius);
  checkGoalsCollision(robot_x, robot_y, robot_radius);

  if(drawing_system.getBorder()>0)
  checkBorderCollsion(robot_x, robot_y, robot_radius, window_width, window_height, window_border);
}

bool CollisionSystem::checkCirclesCollision(Circle circle, Circle other){

// Check for collisions between two circles

    double circle_x = circle.getXPosition(),
           circle_y = circle.getYPosition(),
           circle_radius = circle.getRadius() * drawing_system.getScale();

    double other_x = other.getXPosition(),
           other_y = other.getYPosition(),
           other_radius = other.getRadius() * drawing_system.getScale();

    double distance = std::sqrt((circle_x - other_x) * (circle_x - other_x) +
                                (circle_y - other_y) * (circle_y - other_y));

    double min_distance = circle_radius + other_radius;

    // Returns true if the circles have collided with each other
    return distance <= min_distance;
}

void CollisionSystem::checkObstaclesCollision(double robot_x, double robot_y, double robot_radius)
{
  // Check for collisions with each obstacle
  for (const Circle &obstacle : obstacles)
  {

    double obstacle_x = obstacle.getXPosition(),
           obstacle_y = obstacle.getYPosition(),
           obstacle_radius = obstacle.getRadius() * drawing_system.getScale();

    double distance = std::sqrt((robot_x - obstacle_x) * (robot_x - obstacle_x) +
                                (robot_y - obstacle_y) * (robot_y - obstacle_y));

    double min_distance = obstacle_radius + robot_radius;

    if (distance <= min_distance)
    {
      // The robot has collided with the obstacle, push it back to the edge of the obstacle
      robot.setXPosition(obstacle_x + ((robot_x - obstacle_x) / distance) * min_distance);
      robot.setYPosition(obstacle_y + ((robot_y - obstacle_y) / distance) * min_distance);

      robot.setLeftWheelVelocity(0);
      robot.setRightWheelVelocity(0);
    }
  }
}

void CollisionSystem::checkGoalsCollision(double robot_x, double robot_y, double robot_radius)
{

  // Check for collisions with each goal
  int i = 0;
  for (const Circle &goal : goals)
  {
    double goal_x = goal.getXPosition();
    double goal_y = goal.getYPosition();
    double goal_radius = goal.getRadius() * drawing_system.getScale();

    double distance = std::sqrt((robot_x - goal_x) * (robot_x - goal_x) +
                                (robot_y - goal_y) * (robot_y - goal_y));

    double min_distance = goal_radius + robot_radius;

    if (distance <= min_distance)
    {
      // The robot has collided with the goal, push it back to the edge of the goal
      robot.setXPosition(goal_x + ((robot_x - goal_x) / distance) * min_distance);
      robot.setYPosition(goal_y + ((robot_y - goal_y) / distance) * min_distance);

      robot.setLeftWheelVelocity(0);
      robot.setRightWheelVelocity(0);

      /// Goal reached - Destroy goal
      goals.erase(goals.begin() + i);
    }
    i++;
  }
}

void CollisionSystem::checkBorderCollsion(double robot_x, double robot_y, double robot_radius, double window_width, double window_height, double window_border)
{

  bool is_touching_left_border = (robot_x - robot_radius) < window_border;
  bool is_touching_right_border = (robot_x + robot_radius) > (window_width - window_border);
  bool is_touching_top_border = (robot_y - robot_radius) < window_border;
  bool is_touching_down_border = (robot_y + robot_radius) > (window_height - window_border);
  bool is_touching_border = is_touching_left_border || is_touching_right_border || is_touching_top_border || is_touching_down_border;

  if (is_touching_border)
  {
    // The robot has collided with the border, push it back to the edge of the border

    robot.setLeftWheelVelocity(0);
    robot.setRightWheelVelocity(0);

    if (is_touching_left_border)
    {
      robot.setXPosition(window_border + robot_radius + 1);
    }
    if (is_touching_right_border)
    {
      robot.setXPosition(robot_x - 1);
    }
    if (is_touching_top_border)
    {
      robot.setYPosition(window_border + robot_radius + 1);
    }
    if (is_touching_down_border)
    {
      robot.setYPosition(robot_y - 1);
    }
  }
}
