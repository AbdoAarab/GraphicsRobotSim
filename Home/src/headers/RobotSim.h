// RobotSim.h
#ifndef ROBOT_SIM_H
#define ROBOT_SIM_H

#include "DrawingSystem.h"
#include "CollisionSystem.h"
#include "ControllerSystem.h"
#include "Robot.h"
#include "Circle.h"

#include <string>
#include <vector>
#include <deque>
#include <random>
#include <ctime>
#include <iostream>

using namespace std;

// DEBUG Constants
#define ALL_DEBUG -1
#define NO_DEBUG 0
#define TEXT_DEBUG 1
#define AXIS_DEBUG 2
#define DISTANCE_DEBUG 3

// WINDOW CONSTANTS
#define DELAY 10
#define WINDOW_WIDTH 1360
#define WINDOW_HEIGHT 700
#define WINDOW_BORDER 10

// OBSTACLES & GOALS CONSTANTS
#define DEFAULT_RADIUS 0.01

#define MAX_OBSTACLES 10
#define MIN_OBSTACLES 1

#define MAX_GOALS 3
#define MIN_GOALS 1

// Files
const string obstacles_filePath = "Obstacles.obs";
const string goals_filePath = "Goals.gol";
const string states_filePath = "States.pts";
const string config_filePath = "Initial.config";

class RobotSim
{
public:
    // Constructor
    RobotSim(DrawingSystem &drawing_system, CollisionSystem &collision_system, ControllerSystem &controller_system, Robot &robot, vector<Circle> &obstacles, vector<Circle> &goals, int mode);
    RobotSim(int w_width, int w_height, int w_border, int robot_x, int robot_y);
    RobotSim(int w_width, int w_height, int w_border);
    RobotSim(int robot_x, int robot_y);
    RobotSim();

    // Getters
    DrawingSystem getDrawingSystem();
    CollisionSystem getCollisionSystem();
    ControllerSystem getControllerSystem();
    Robot getRobot();
    vector<Circle> getObstacles();
    vector<Circle> getGoals();

    // Setters
    void setDrawingSystem(DrawingSystem &drawingSystem);
    void setCollisionSystem(CollisionSystem &collisionSystem);
    void setControllerSystem(ControllerSystem &controllerSystem);
    void setRobot(Robot &rob);
    void setObstacles(vector<Circle> &obs);
    void setGoals(vector<Circle> &gos);
    void setMode(int mode);
    void setDebug(int debug_lvl);

    // Member functions
    void run();

    // Setup functions
    void showSetupMenu();
    int getSelectedMenuOption(string main_instruction, vector<vector<string>> &options, int init_x, int init_y, int button_width, int button_height, int margin);
    int getSelectedNumber(string main_instruction, string entity_name, int min_number, int max_number, int init_x, int init_y, int button_width, int button_height, int margin);

    // Random circle creation functions
    vector<Circle> createRandomCircles(int numCircles, double maxCircleRadius);
    vector<Circle> createRandomCircles(int numCircles, double maxCircleRadius, int offset_x, int offset_y);
    // State functions
    void logStates(Robot &robot);
    deque<vector<double>> loadStates(string filePath) const;

    // Circles functions
    void saveCircles(string filePath, vector<Circle> &circles);
    vector<Circle> loadCircles(string filePath) const;

    // Initial Configuration functions
    bool saveConfig();
    bool loadConfig();

    // Keyboard & mouse event handlers
    int addCircleFromMouse();
    bool handleKeyboard();

    // Preview Functions - to preview the robot and number of obstacles & goals at the setup
    void previewRobot(Robot &robot);
    void previewCircles(vector<Circle> &circles, int numCircles);

private:
    DrawingSystem drawing_system;
    CollisionSystem collision_system;
    ControllerSystem controller_system;
    Robot robot;
    vector<Circle> obstacles;
    vector<Circle> goals;
    deque<vector<double>> states;
    int debug_level = 0;
};

#endif // ROBOT_SIM_H