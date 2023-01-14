// ControllerSystem.cpp
#include "ControllerSystem.h"

#include <iostream>
using namespace std;

#define MC_F_SPEED 0.2 // Forward/Backwards Speed Increments
#define MC_T_SPEED 0.01 // Turning Speed Increments

deque<vector<double>> states;

ControllerSystem::ControllerSystem(DrawingSystem &drawing_system, Robot &robot, vector<Circle> &obstacles, vector<Circle> &goals) : drawing_system(drawing_system), robot(robot), obstacles(obstacles), goals(goals)
{
}

void ControllerSystem::update()
{
  if (isPlayBackMode())
  {
    // In playback mode, the wheel velocities are controlled by the values int he states file
    updatePlayBack();
  }
  else if (isAutomaticMode())
  {
    // In automatic mode, set the wheel velocities to move towards the goal
    drawing_system.setBorder(0);
    updateAutomatic();
  }
  else
  {
    // In manual mode, the wheel velocities are controlled by the user
    updateManual();
  }
}

void ControllerSystem::updatePlayBack()
{
  // In playback mode, the wheel velocities are controlled by the values int he states file
  if (!states.empty())
  {

    // Get the robot's data
    double robot_x = states.front()[0],
           robot_y = states.front()[1],
           robot_orientation = states.front()[2],
           robot_left_velocity = states.front()[3],
           robot_right_velocity = states.front()[4];

    // Apply the x,y positions and the orientation
    robot.setXPosition(robot_x);
    robot.setYPosition(robot_y);
    robot.setOrientation(robot_orientation);

    // Apply the RW, LW velocities
    robot.setLeftWheelVelocity(robot_left_velocity);
    robot.setRightWheelVelocity(robot_right_velocity);

    // Check for change to the obstacles and goals
    int isAdded = states.front()[5];

    if (isAdded != 0)
    {

      // Get the added (Obstacles/Goals) data
      double circle_x = states.front()[6],
             circle_y = states.front()[7],
             circle_radius = states.front()[8];

      if (isAdded == -1) // If a goal is added
        goals.push_back(Circle(circle_x, circle_y, circle_radius));
      if (isAdded == 1) // If a goal is added
        obstacles.push_back(Circle(circle_x, circle_y, circle_radius));
    }

    // Remove the current state from the queue
    states.pop_front();

    // Update current state number
    state_number++;

    // Update Mode Status
    mode_status = MODE_PLAYING;
  }
  else
  {
    // Apply the RW, LW velocities
    robot.setLeftWheelVelocity(0);
    robot.setRightWheelVelocity(0);

    // Update Mode Status
    mode_status = MODE_ENDED;
  }
}

void ControllerSystem::updateAutomatic()
{
  // In automatic mode, set the wheel velocities to move towards the goal

  if (goals.empty()) // If there's no goals stop the robot
  {
    // Update the actual angular velocity of each wheel
    robot.setLeftWheelVelocity(0);
    robot.setRightWheelVelocity(0);

    // Update Mode Status
    mode_status = MODE_ENDED;
    return;
  }

  int world_width = drawing_system.getWidth(),
      world_height = drawing_system.getHeight(),
      border_size = drawing_system.getBorder();

  double sensor_distance = robot.getSensorMaxDistance(),
         sensor_max_angle = robot.getSensorMaxAngle();
  double moving_speed = 0.5;
  double damping_factor = 0.4;

  // Calculate the desired angular velocity of each wheel
  double desired_left_wheel_angular_velocity = 0;
  double desired_right_wheel_angular_velocity = 0;

  double robot_x = robot.getXPosition(),
         robot_y = robot.getYPosition(),
         robot_radius = robot.getRadius() * drawing_system.getScale(),
         robot_orientation = robot.getOrientation(),
         robot_delta_t = robot.getDeltaTime(),
         robot_lw_velocity = robot.getLeftWheelVelocity(),
         robot_rw_velocity = robot.getRightWheelVelocity();

  double goal_x = goals.back().getXPosition(),
         goal_y = goals.back().getYPosition(),
         goal_radius = goals.back().getRadius() * drawing_system.getScale();


  // Calculate the distance and angle between the robot and the goal
  double dx = goal_x - robot_x;
  double dy = goal_y - robot_y;
  double goal_distance = sqrt(dx * dx + dy * dy);
  double goal_angle = atan2(dy, dx);

  // Check if there is an obstacle within a certain distance in front of the robot
  bool obstacle_detected = false;
  double obstacle_distance = 0;
  double obstacle_angle = 0;

  for (const Circle &obstacle : obstacles)
  {
    double obstacle_x = obstacle.getXPosition(),
           obstacle_y = obstacle.getYPosition(),
           obstacle_radius = obstacle.getRadius() * drawing_system.getScale();

    double dx = obstacle_x - robot_x;
    double dy = obstacle_y - robot_y;
    double distance = sqrt(dx * dx + dy * dy);
    double angle = atan2(dy, dx);

    double min_distance = (robot_radius + obstacle_radius) + sensor_distance;

    double angle_range = (robot_orientation + goal_angle) - angle;


    // Check if the obstacle is within a certain distance and angle in front of the robot
    if (distance <= min_distance && abs(angle_range) <= sensor_max_angle)
    {
      obstacle_detected = true;
      obstacle_distance = distance;
      obstacle_angle = angle;

      drawing_system.setObstacleDetected(true, obstacle);

      break;
    }
  }

  // cout<< "++++++++++++++++++++++++++++++++++++++++++++++++++ "<<endl;
  // cout<< "goal_angle= "<<goal_angle <<endl;
  // cout<< "obstacle_angle= "<<obstacle_angle <<endl;

  // Calculate the angle theta for all cases
  double theta = 0;

  // // Calculate the distance between the robot and the border
  // double border_distance = min(robot_x - border_size - robot_radius, world_width - (robot_x + border_size + robot_radius));
  // border_distance = min(border_distance, robot_y - border_size - robot_radius);
  // border_distance = min(border_distance, world_height - (robot_y + border_size + robot_radius));

  // if (border_distance < robot_radius)
  // {
  //   // The robot is too close to the border, so rotate away from the border
  //   double border_angle = atan2(-robot_y, robot_x);

  //   if (border_angle < 0)
  //   {
  //     border_angle += 2 * M_PI;
  //   }
  //   theta = border_angle;
  // }

  // If an obstacle is detected, calculate the desired angular velocity of each wheel to go around it
  if (obstacle_detected)
  {
    double theta_obstacle = obstacle_angle - robot_orientation; // Angle to face the obstacle
    theta = -(1 / theta_obstacle);
  }
  // If no obstacle is detected, calculate the desired angular velocity of each wheel to turn towards the goal
  else
  {
    double theta_goal = goal_angle - robot_orientation; // Angle to face the goal
    theta = theta_goal;
  }

  // Calculate the desired angular velocity of each wheel to go around the obstacle
  desired_left_wheel_angular_velocity = robot_lw_velocity + damping_factor * theta / 2;
  desired_right_wheel_angular_velocity = robot_rw_velocity - damping_factor * theta / 2;

  // If the angle is small, reduce the angular velocity of the wheels to smooth the transition
  if (abs(theta) < 0.1)
  {
    desired_left_wheel_angular_velocity *= 0.1 / abs(theta);
    desired_right_wheel_angular_velocity *= 0.1 / abs(theta);
  }

  // std::cout << "theta_obstacle: " << theta_obstacle << std::endl;
  // std::cout << "theta_goal: " << theta_goal << std::endl;

  // Update the actual angular velocity of each wheel
  robot.setLeftWheelVelocity(moving_speed + robot_delta_t * desired_left_wheel_angular_velocity);
  robot.setRightWheelVelocity(moving_speed + robot_delta_t * desired_right_wheel_angular_velocity);

  // Update Mode Status
  mode_status = MODE_PLAYING;
}

void ControllerSystem::updateManual()
{
  // In manual mode, the wheel velocities are controlled by the user

  double forward_speed = MC_F_SPEED;
  double turn_speed = MC_T_SPEED;

  // Get robot data
  double robot_right_velocity = robot.getRightWheelVelocity(),
         robot_left_velocity = robot.getLeftWheelVelocity(),
         robot_delta_t = robot.getDeltaTime();

  // Get input & Calculate the desired angular velocity of each wheel
  if (GetAsyncKeyState(VK_UP))
  {

    robot_left_velocity += robot_delta_t * forward_speed;
    robot_right_velocity += robot_delta_t * forward_speed;
  }
  if (GetAsyncKeyState(VK_DOWN))
  {

    robot_left_velocity -= robot_delta_t * forward_speed;
    robot_right_velocity -= robot_delta_t * forward_speed;
  }
  if (GetAsyncKeyState(VK_LEFT))
  {
    robot_left_velocity -= robot_delta_t * turn_speed;
    robot_right_velocity += robot_delta_t * turn_speed;
  }
  if (GetAsyncKeyState(VK_RIGHT))
  {
    robot_left_velocity += robot_delta_t * turn_speed;
    robot_right_velocity -= robot_delta_t * turn_speed;
  }

  robot.setLeftWheelVelocity(robot_left_velocity);
  robot.setRightWheelVelocity(robot_right_velocity);

  // Update Mode Status
  mode_status = MODE_PLAYING;
}

void ControllerSystem::setAutomaticMode()
{
  mode = AUTO_MODE;
  mode_status = MODE_PLAYING;
}

void ControllerSystem::setManualMode()
{
  mode = MANUAL_MODE;
  mode_status = MODE_PLAYING;
}

void ControllerSystem::setPlayBackMode(deque<vector<double>> &states_queue)
{
  mode = PLAYBACK_MODE;
  mode_status = MODE_PLAYING;
  state_number = 0;
  states = states_queue;
}

int ControllerSystem::getMode() { return mode; }
bool ControllerSystem::getModeStatus() { return mode_status; }
int ControllerSystem::getStatesNumber() { return state_number; }

bool ControllerSystem::isAutomaticMode() const { return mode == AUTO_MODE; }
bool ControllerSystem::isManualMode() const { return mode == MANUAL_MODE; }
bool ControllerSystem::isPlayBackMode() const { return mode == PLAYBACK_MODE; }

bool ControllerSystem::isKeyPressed(int key)
{
  return GetKeyState(key) & 0x8000;
}

bool ControllerSystem::handleMouseInput(int mouseButton, int &x, int &y)
{
  // Check for mouse input

  if (ismouseclick(mouseButton))
  {
    // Mouse button was clicked, store coordinates in x & y
    getmouseclick(mouseButton, x, y);
    return true;
  }

  return false;
}