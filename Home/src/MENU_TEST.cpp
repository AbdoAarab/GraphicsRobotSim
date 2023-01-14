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

#include <string>
#include <cstring>
#include <bits/stdc++.h>

using namespace std;

// DEBUG CONSTANTS
#define ALL_DEBUG -1
#define NO_DEBUG 0
#define TEXT_DEBUG 1
#define AXIS_DEBUG 2
#define DISTANCE_DEBUG 3

// WINDOW CONSTANTS
#define DELAY 10
#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 700
#define WINDOW_BORDER 10

// OBSTACLES CONSTANTS
#define DEFAULT_OBSTACLE_RADIUS 0.01
#define MAX_OBSTACLES 3
#define MIN_OBSTACLES 1

// GOALS CONSTANTS
#define MAX_GOALS 1
#define MIN_GOALS 1

std::vector<Circle> obstacles;
std::vector<Circle> goals;

// Functions declaration
std::vector<Circle> loadObjects(string fileName);

vector<Circle> createCircles(DrawingSystem &drawing_system, Robot &robot, int numCircles, double maxCircleRadius);
bool handleMouseInput(vector<Circle> &obstacles, vector<Circle> &goals);

void showSetupMenu(DrawingSystem &drawingSystem, ControllerSystem &controllerSystem);
int getSelectedMenuOption(string instruction, std::vector<std::string> &options, int init_x, int init_y, int button_width, int button_height, int margin);
void drawButton(string text, int x, int y, int button_width, int button_height, int font, int font_size, int justify_horiz, int justify_vert, int edge_style, int edge_color, int edge_thickness);

int main()
{

    // Load Obstacles from file
    std::vector<Circle> obstacles;

    // Load Goals from file
    std::vector<Circle> goals;

 
    // Main loop
    // while (true)
    // {
    //     // Clear the screen
    //     cleardevice();

    //     // Handle mouse input
    //     handleMouseInput(obstacles, goals);

    //     // Smooth animation
    //     swapbuffers();

    //     // Wait for a short time
    //     Sleep(10);
    // }

    Robot robot(100, 100);
    DrawingSystem drawing_system(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_BORDER);
    ControllerSystem controller_system(drawing_system, robot, obstacles, goals);

    showSetupMenu(drawing_system, controller_system);

    closegraph();

    closegraph();
    return 0;
}

// void showSetupMenu(int &mode)
// {
//     // Set up the menu options
//     std::vector<std::string> options = {"Playback mode", "Automatic mode", "Manual mode", "Creative mode"};
//     std::vector<std::string> suboptions = {"Random obstacles/goals", "Load from file"};

//     // Set up the menu window
//     int window_width = 300;
//     int window_height = 200;
//     int window_x = (getmaxx() - window_width) / 2;
//     int window_y = (getmaxy() - window_height) / 2;
//     initwindow(window_width, window_height);

//     // Display the menu options
//     settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
//     setcolor(BLACK);
//     int y = 50;
//     for (const auto &option : options)
//     {
//         const int length = option.length();
//         // declaring character array (+1 for null terminator)
//         char *char_array = new char[length + 1];
//         strcpy(char_array, option.c_str());

//         outtextxy(50, y, char_array);
//         y += 30;
//     }

//     // Wait for the user to make a selection
//     int x, y;
//     while (true)
//     {
//         if (ismouseclick(WM_LBUTTONDOWN))
//         {
//             getmouseclick(WM_LBUTTONDOWN, x, y);
//             // Check which option was clicked
//             int index = (y - 50) / 30;
//             if (index >= 0 && index < options.size())
//             {
//                 // Set the mode based on the selected option
//                 mode = index;
//                 if (mode == 0 || mode == 1 || mode == 2)
//                 {
//                     // Show the suboptions for modes 0, 1, and 2
//                     settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
//                     setcolor(BLACK);
//                     y = 50;
//                     for (const auto &suboption : suboptions)
//                     {
//                         const int length = suboption.length();
//                         // declaring character array (+1 for null terminator)
//                         char *char_array = new char[length + 1];
//                         strcpy(char_array, suboption.c_str());

//                         outtextxy(50, y, char_array);
//                         y += 30;
//                     }

//                     // Wait for the user to make a selection
//                     while (true)
//                     {
//                         if (ismouseclick(WM_LBUTTONDOWN))
//                         {
//                             getmouseclick(WM_LBUTTONDOWN, x, y);
//                             // Check which suboption was clicked
//                             int index = (y - 50) / 30;
//                             if (index == 0)
//                             {
//                                 // Random obstacles/goals
//                                 int num_obstacles = rand() % MAX_OBSTACLES + MIN_OBSTACLES;
//                                 int num_goals = rand() % MAX_GOALS + MIN_GOALS;
//                                 obstacles.clear();
//                                 for (int i = 0; i < num_obstacles; i++)
//                                 {
//                                     obstacles.emplace_back(rand() % WINDOW_WIDTH, rand() % WINDOW_HEIGHT, DEFAULT_OBSTACLE_RADIUS);
//                                     goals.emplace_back(rand() % WINDOW_WIDTH, rand() % WINDOW_HEIGHT, DEFAULT_OBSTACLE_RADIUS)
//                                 }
//                                 goal = { rand() % WINDOW_WIDTH, rand() % WINDOW_HEIGHT
void showSetupMenu(DrawingSystem &drawingSystem, ControllerSystem &controllerSystem)
{
    // First, clear the screen and draw the menu background
    cleardevice();
    //   drawMenuBackground();

    // Set up the menu options
    std::vector<std::string> modes = {"Automatic Mode", "Manual Mode", "Playback Mode", "Creative Mode"};
    std::vector<std::string> options = {"Random Obstacles & Goals", "Load Obstacles & Goals From File"};

    int init_x = 50,
        init_y = 250;

    int x = init_x,
        y = init_y,
        button_width = 200,
        button_height = 40,
        margin = 50;

    // Wait for the user to select a mode
    int selected_mode = getSelectedMenuOption("Please Select a Mode:", modes, init_x, init_y, button_width, button_height, margin);
    // Wait for the user to select an option
    int selected_option = getSelectedMenuOption("Please Select an Option:", options, init_x, init_y, button_width + 200, button_height, margin);

    
    // Set the mode based on the selected option
    switch (selected_mode)
    {
    case 1: // Auto_mode
        controllerSystem.setAutomaticMode();
        break;
    case 2: // Manual_mode
        controllerSystem.setManualMode();
        break;
    case 3: // Playback_mode
        // controllerSystem.setPlayBackMode(states);
        break;
    case 4: // Creative_mode
        controllerSystem.setAutomaticMode();
        break;
    }

    // Set the obstacle/goals loading based on the selected option
    switch (selected_option)
    {
    case 1: // Random Obstacles & Goals
        //controllerSystem.setAutomaticMode();
        break;
    case 2: // Load Obstacles & Goals From File
        //controllerSystem.setManualMode();
        break;
    }
}

int getSelectedMenuOption(string instruction, std::vector<std::string> &options, int init_x, int init_y, int button_width, int button_height, int margin)
{

    int x = init_x,
        y = init_y;

    int numOptions = options.size();
    int selectedOption = 1;
    bool isOptionSelected = false;

    do
    {
        int i = 1;
        y = init_y;

        if (GetKeyState(VK_UP) & 0x8000)
        {
            // Move the selection up
            selectedOption--;
            if (selectedOption < 1)
            {
                selectedOption = numOptions;
            }
        }
        else if (GetKeyState(VK_DOWN) & 0x8000)
        {
            // Move the selection down
            selectedOption++;
            if (selectedOption > numOptions)
            {
                selectedOption = 1;
            }
        }
        else if (GetKeyState(VK_SPACE) | GetKeyState(VK_RETURN) & 0x8000)
        {
            // Return the selected option
            isOptionSelected = true;
        }

        // Redraw the menu with the updated selection
        cleardevice();

        drawButton("ROBOT SIMULATION", init_x, init_y - margin - 100, 0, 0, COMPLEX_FONT, 6, LEFT_TEXT, CENTER_TEXT, SOLID_LINE, BLACK, 0);
        drawButton("Robotics Mini-Project MIAAD - By: Aarab Abderrahmane & Walid Fajri", init_x, init_y - margin - 70, 0, 0, COMPLEX_FONT, 1, LEFT_TEXT, CENTER_TEXT, SOLID_LINE, BLACK, 0);
        drawButton(instruction, init_x, init_y - margin, 0, 0, COMPLEX_FONT, 3, LEFT_TEXT, CENTER_TEXT, SOLID_LINE, CYAN, 0);

        // Display the menu options
        for (const string &option : options)
        {
            if (i == selectedOption)
            {
                drawButton(option, x, y, button_width, button_height, COMPLEX_FONT, 1, CENTER_TEXT, CENTER_TEXT, SOLID_LINE, CYAN, 3);
                y += button_height + margin;
            }
            else
            {
                // Draw Menu Buttons
                drawButton(option, x, y, button_width, button_height, COMPLEX_FONT, 1, CENTER_TEXT, CENTER_TEXT, SOLID_LINE, BLACK, 1);
                y += button_height + margin;
            }

            i++;
        }

        // Smooth animation
        swapbuffers();

        // Wait for a short time
        Sleep(100);

    } while (!isOptionSelected);

    // Return the index of the selected option
    return selectedOption;
}

void drawButton(string text, int x, int y, int button_width, int button_height, int font, int font_size, int justify_horiz, int justify_vert, int edge_style, int edge_color, int edge_thickness)
{

    const int length = text.length();
    // declaring character array (+1 for null terminator)
    char *char_array = new char[length + 1];
    strcpy(char_array, text.c_str());

    // Draw button
    setcolor(edge_color);
    setlinestyle(edge_style, edge_color, edge_thickness);
    rectangle(x, y, x + button_width, y + button_height);
    // floodfill(x + edge_thickness + 1, y + edge_thickness + 1, fill_color);

    settextstyle(font, HORIZ_DIR, font_size);
    settextjustify(justify_horiz, justify_vert);
    outtextxy(x + button_width / 2, y + (button_height + 6) / 2, char_array);
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

bool handleMouseInput()
{
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
