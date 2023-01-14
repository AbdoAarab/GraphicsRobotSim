#include "Robot.h"
#include "Circle.h"
#include "CollisionSystem.h"
#include "DrawingSystem.h"
#include "ControllerSystem.h"

#include <fstream>
#include <iostream>
#include <deque>
#include <vector>
#include "graphics.h"

#include <random>
#include <ctime>

using namespace std;

// DEBUG Constants
#define ALL_DEBUG -1
#define NO_DEBUG 0
#define TEXT_DEBUG 1
#define AXIS_DEBUG 2
#define DISTANCE_DEBUG 3


// WINDOW Constants
#define DELAY 10
#define W_WIDTH 1000
#define W_HEIGHT 700
#define W_BORDER 10

// Functions declaration
void logStates(ofstream &logFile, Robot &robot);
std::vector<Circle> loadObjects(string fileName);
std::deque<vector<double>> loadStates(string fileName);

vector<Circle> createCircles(DrawingSystem &drawing_system, Robot &robot, int numCircles, double maxCircleRadius);
bool handleMouseInput(vector<Circle> &obstacles, vector<Circle> &goals);

int main()
{

  // Load Obstacles from file
  std::vector<Circle> obstacles = loadObjects("Obstacles.obs");

  // Load Goals from file
  std::vector<Circle> goals = loadObjects("Goals.gol");

  // Load States from file
  std::deque<vector<double>> states = loadStates("States.pts");

  // Create the robot
  Robot robot(100, 100);

  // Create the systems
  DrawingSystem drawing_system(W_WIDTH, W_HEIGHT, W_BORDER);

  //vector<Circle> obstacles = createCircles(drawing_system, robot, 10, 0.05);

  CollisionSystem collision_system(drawing_system, robot, obstacles, goals);
  ControllerSystem controller_system(drawing_system, robot, obstacles, goals);

  
  // Log file
  ofstream log_file;
  if (controller_system.isPlayBackMode())
    log_file.open("States.pts", ios::trunc & ios::app);


  // Select Mode
  if (PLAYBACK_MODE)
    controller_system.setPlayBackMode(states);
  else if (AUTO_MODE)
    controller_system.setAutomaticMode();
  else
    controller_system.setManualMode();

      // Main loop
    while (true)
    {
        // Clear the screen
        cleardevice();

        // Handle mouse input
        handleMouseInput(obstacles, goals);

        // Update the controller
        controller_system.update();

        // Update the robot's position and orientation
        robot.update();

        // Check for collisions
        collision_system.update();

        // Log states while not in playback mode
        if (!controller_system.isPlayBackMode())
            logStates(log_file,robot);

        // Draw the border
        drawing_system.drawBorder();
        
        // Draw the debug
        drawing_system.drawDebug(robot, obstacles, goals, -1);

        // Draw the sensor lines for the automatic mode
        if (controller_system.isAutomaticMode())
            drawing_system.drawSensor(robot, goals);

        // Draw the robot, obstacles, goals
        drawing_system.drawRobot(robot);
        drawing_system.drawObstacles(obstacles);
        drawing_system.drawGoals(goals);

        // Draw Text showing the current mode
        drawing_system.drawModeText(controller_system.getMode(), controller_system.getModeStatus(), controller_system.getStatesNumber());

        // Smooth animation
        swapbuffers();

        // Wait for a short time
        Sleep(10);
    }

    log_file.close();
    closegraph();

  closegraph();
  return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
void logStates(ofstream &logFile, Robot &robot)
{

    double robot_x = robot.getXPosition();
    double robot_y = robot.getYPosition();
    double robot_orientation = robot.getOrientation();
    double robot_right_velocity = robot.getRightWheelVelocity();
    double robot_left_velocity = robot.getLeftWheelVelocity();

    logFile << robot_x << " " << robot_y << " " << robot_orientation << " " << robot_left_velocity << " " << robot_right_velocity << " " << endl;
  
}

std::vector<Circle> loadObjects(string fileName)
{
  ifstream objects_file;
  objects_file.open(fileName);

  double objects_x, objects_y, objects_radius;

  int objects_number = 0;

  std::vector<Circle> objects;

  while (objects_file >> objects_x >> objects_y >> objects_radius)
  {
    objects.push_back(Circle(objects_x, objects_y, objects_radius));
    objects_number++;
  };

  objects_file.close();

  return objects;
}

std::deque<vector<double>> loadStates(string fileName)
{
  ifstream states_file;
  states_file.open(fileName);

  double robot_x,
      robot_y,
      robot_orientation,
      robot_right_velocity,
      robot_left_velocity;

  std::deque<vector<double>> states;

  // Get the robot's data and save it to vector
  while (states_file >> robot_x >> robot_y >> robot_orientation >> robot_left_velocity >> robot_right_velocity)
  {
    states.push_back(vector<double>{robot_x, robot_y, robot_orientation, robot_left_velocity, robot_right_velocity});
  };

  states_file.close();

  return states;
}

vector<Circle> createCircles(DrawingSystem &drawing_system, Robot &robot, int numCircles, double maxCircleRadius)
{
  vector<Circle> circles; // initialize Circle vector

  // Create random number generator
  random_device rd;
  mt19937 gen(rd());
  gen.seed(time(0)); // seed generator with current time

  double windowWidth = drawing_system.getWidth(),
         windowHeight = drawing_system.getHeight(),
         windowBorder = drawing_system.getBorder();

  double robot_x = robot.getYPosition(),
         robot_y = robot.getYPosition(),
         robot_radius = robot.getRadius() * drawing_system.getScale();

  double max_circle_radius = maxCircleRadius * drawing_system.getScale();

  double min_PosX = abs(windowBorder + max_circle_radius + robot_x),
         max_PosX = abs(windowWidth - windowBorder - max_circle_radius);

  double min_PosY = abs(windowBorder + max_circle_radius + robot_y),
         max_posY = abs(windowHeight - windowBorder - max_circle_radius);

  uniform_real_distribution<> rad(0.005, max_circle_radius / drawing_system.getScale());
  uniform_real_distribution<> xPos(min_PosX, max_PosX);
  uniform_real_distribution<> yPos(min_PosY, max_posY);

  for (int i = 0; i < numCircles; i++)
  {
    Circle circle(xPos(gen), yPos(gen), rad(gen));
    double circle_x = circle.getXPosition(),
           circle_y = circle.getYPosition(),
           circle_radius = circle.getRadius();

    // check if circle is within window and does not overlap with any other circles
    bool valid = true;
    for (Circle other : circles)
    {
      if ((circle_x + circle_radius > windowWidth || circle_x - circle_radius < 0 || circle_y + circle_radius > windowHeight || circle_y - circle_radius < 0) || (pow(circle_x - other.getXPosition(), 2) + pow(circle_y - other.getYPosition(), 2) <= pow(circle_radius + other.getRadius(), 2)))
      {
        valid = false;
        break;
      }
    }
    if (valid)
    {
      circles.push_back(circle);
    }
  }
  return circles;
}

bool handleMouseInput(vector<Circle> &obstacles, vector<Circle> &goals)
{
  double DEFAULT_OBSTACLE_RADIUS = 0.01;

  // Check for mouse input
  int x, y;
  double radius = DEFAULT_OBSTACLE_RADIUS;
  // int mouse_wheel = getmousestatus(WHEEL_DELTA);
  // if (mouse_wheel != 0)
  // {
  //   radius += mouse_wheel;
  // }

  if (ismouseclick(WM_LBUTTONDOWN))
  {
    // Left mouse button was clicked, add a new obstacle
    getmouseclick(WM_LBUTTONDOWN, x, y);
    obstacles.push_back(Circle(x, y, radius));
    return true;
  }
  else if (ismouseclick(WM_RBUTTONDOWN))
  {
    // Right mouse button was clicked, set the goal position
    getmouseclick(WM_RBUTTONDOWN, x, y);
    goals.push_back(Circle(x, y, radius));

    return true;
  }

  return false;
}
