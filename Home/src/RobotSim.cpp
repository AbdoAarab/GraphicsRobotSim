// RobotSim.cpp
#include "RobotSim.h"

#include <fstream>

ofstream log_file;
char temp_text[1000];

RobotSim::RobotSim(DrawingSystem &drawing_system, CollisionSystem &collision_system, ControllerSystem &controller_system, Robot &robot, std::vector<Circle> &obstacles, std::vector<Circle> &goals, int mode) : drawing_system(drawing_system),
                                                                                                                                                                                                                collision_system(collision_system),
                                                                                                                                                                                                                controller_system(controller_system),
                                                                                                                                                                                                                robot(robot), obstacles(obstacles),
                                                                                                                                                                                                                goals(goals), debug_level(debug_level)
{
    // Set Mode
    setMode(mode);
    // Save Initial Config = Initial State of the world, robot and simulation
    saveConfig();
}
RobotSim::RobotSim(int w_width, int w_height, int w_border, int robot_x, int robot_y) : robot(robot_x, robot_y),
                                                                                        drawing_system(w_width, w_height, w_border),
                                                                                        collision_system(drawing_system, robot, obstacles, goals),
                                                                                        controller_system(drawing_system, robot, obstacles, goals),
                                                                                        debug_level(0)
{
    // Save Initial Config = Initial State of the world, robot and simulation
    saveConfig();
}
RobotSim::RobotSim(int w_width, int w_height, int w_border) : robot(100, 100),
                                                              drawing_system(w_width, w_height, w_border),
                                                              collision_system(drawing_system, robot, obstacles, goals),
                                                              controller_system(drawing_system, robot, obstacles, goals), debug_level(0)

{
    // Save Initial Config = Initial State of the world, robot and simulation
    saveConfig();
}
RobotSim::RobotSim(int robot_x, int robot_y) : robot(robot_x, robot_y),
                                               drawing_system(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_BORDER),
                                               collision_system(drawing_system, robot, obstacles, goals),
                                               controller_system(drawing_system, robot, obstacles, goals),
                                               debug_level(0)
{
    // Save Initial Config = Initial State of the world, robot and simulation
    saveConfig();
}

RobotSim::RobotSim() : robot(100, 100),
                       drawing_system(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_BORDER),
                       collision_system(drawing_system, robot, obstacles, goals),
                       controller_system(drawing_system, robot, obstacles, goals), debug_level(0)

{
    // Save Initial Config = Initial State of the world, robot and simulation
    saveConfig();
}

void RobotSim::run()
{

    // Show the setup menu
    showSetupMenu();

    clearmouseclick(WM_LBUTTONDOWN); // Clear the Left Mouse Event
    clearmouseclick(WM_RBUTTONDOWN); // Clear the Right Mouse Event

    // Main loop
    while (true)
    {
        // Clear the screen
        cleardevice();

        // Handle keyboard input
        handleKeyboard();

        // While not in playback mode
        if (!controller_system.isPlayBackMode())
        {
            logStates(robot); // Log states
        }

        // Update the controller
        controller_system.update();

        // Update the robot's position and orientation
        robot.update();

        // Check for collisions
        collision_system.update();

        // Draw the border
        drawing_system.drawBorder();

        // Draw the debug
        if ((debug_level >= -1) && (debug_level <= 3))
            drawing_system.drawDebug(robot, obstacles, goals, debug_level);

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
        Sleep(DELAY);
    }

    log_file.close();
    closegraph();
}

void RobotSim::showSetupMenu()
{

    // Set up the menu options
    vector<vector<string>> modes = {
        {"Automatic Mode", "Reach the goal while avoiding the obstacles"},
        {"Manual Mode", "Control the robot manually"},
        {"Playback Mode", "Playback the robot's last actions"},
        {"Creative Mode", "Create the environement for the robot"},
        {"Q-Learning Mode", "Navigate to the goal using Q-Learning"}};

    vector<vector<string>> options = {
        {"Random Obstacles & Goals", "Create random obstacles & goals"},
        {"Load Obstacles & Goals From File", "Load the saved obstacles & goals"}};

    int init_x = 50,
        init_y = 250;

    int x = init_x,
        y = init_y,
        button_width = 200,
        button_height = 40,
        margin = 40;

    // Wait for the user to select a mode
    int selected_mode = getSelectedMenuOption("Please Select a Mode:", modes, init_x, init_y, button_width, button_height, margin);
    Sleep(200);
    int selected_option = 0;

    // Set the mode based on the selected option
    switch (selected_mode)
    {
    case 1: // AUTO MODE
        setMode(AUTO_MODE);
        // Wait for the user to select an option
        selected_option = getSelectedMenuOption("Please Select a Mode:", options, init_x, init_y, button_width + 200, button_height, margin);
        Sleep(200);
        break;
    case 2: // MANUAL MODE
        setMode(MANUAL_MODE);
        // Wait for the user to select an option
        selected_option = getSelectedMenuOption("Please Select an Option:", options, init_x, init_y, button_width + 200, button_height, margin);
        Sleep(200);
        break;
    case 3: // PLAYBACK MODE
        setMode(PLAYBACK_MODE);
        selected_option = 2;
        break;

    default:
        setMode(MANUAL_MODE);
        break;
    }

    // Set the obstacle/goals loading based on the selected option
    switch (selected_option)
    {
    case 1: // Random Obstacles & Goals
    {
        // Wait for the user to input the number of entities
        int numObstacles = getSelectedNumber("Please Select The Number of Obstacles:", "Obstacles", MIN_OBSTACLES, MAX_OBSTACLES, init_x, init_y, button_width + 20, button_height, margin);
        Sleep(200);
        int numGoals = getSelectedNumber("Please Select The Number of Goals:", "Goals", MIN_GOALS, MAX_GOALS, init_x, init_y, button_width + 20, button_height, margin);
        Sleep(200);

        // Create the Entitites randomly and seperate them into obstacles and goals
        vector<Circle> circles = createRandomCircles(numObstacles + numGoals, 0.05);
        obstacles = {circles.begin(), circles.begin() + numObstacles};
        goals = {circles.end() - numGoals, circles.end()};

        // Save the obstacles and goals to file
        saveCircles(obstacles_filePath, obstacles);
        saveCircles(goals_filePath, goals);

        break;
    }
    case 2: // Load Obstacles & Goals From File
    {
        obstacles = loadCircles(obstacles_filePath);
        goals = loadCircles(goals_filePath);
        break;
    }
    default: // Load empty vectors of Obstacles & Goals
    {
        vector<Circle> empty_vector;
        obstacles = empty_vector;
        goals = empty_vector;
        break;
    }
    }
}

int RobotSim::getSelectedMenuOption(string main_instruction, vector<vector<string>> &options, int init_x, int init_y, int button_width, int button_height, int margin)
{
    Robot preview_bot(drawing_system.getWidth() - 300, drawing_system.getHeight() - 300);

    int x = init_x, // Initial position of buttons
        y = init_y;

    int numOptions = options.size(); // Number of options
    int selectedOption = 1;          // index of currently selected option
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

        previewRobot(preview_bot); // Draw a preview of the robot

        // Draw the Title, subTitle an Instruction
        drawing_system.drawButton("ROBOT SIMULATION", init_x, init_y - margin - 100, 0, 0, COMPLEX_FONT, 6, CYAN, LEFT_TEXT, CENTER_TEXT, SOLID_LINE, CYAN, 0);
        drawing_system.drawButton("Robotics Mini-Project MIAAD - By: Aarab Abderrahmane & Walid Fajri", init_x, init_y - margin - 70, 0, 0, COMPLEX_FONT, 1, BLACK, LEFT_TEXT, CENTER_TEXT, SOLID_LINE, BLACK, 0);
        drawing_system.drawButton(main_instruction, init_x, init_y - margin, 0, 0, COMPLEX_FONT, 3, CYAN, LEFT_TEXT, CENTER_TEXT, SOLID_LINE, CYAN, 0);

        // Display the menu options
        for (const vector<string> &option : options)
        {
            string option_string = option[0];
            string instruction_string = option[1];

            if (i == selectedOption)
            {
                drawing_system.drawButton(option_string, x, y, button_width, button_height, COMPLEX_FONT, 1, CYAN, CENTER_TEXT, CENTER_TEXT, SOLID_LINE, CYAN, 3);
                drawing_system.drawString(instruction_string, x + button_width + margin / 2, y + 2 + button_height / 2, COMPLEX_FONT, 1, DARKGRAY, LEFT_TEXT, CENTER_TEXT);
                y += button_height + margin;
            }
            else
            {
                // Draw Menu Buttons
                drawing_system.drawButton(option_string, x, y, button_width, button_height, COMPLEX_FONT, 1, BLACK, CENTER_TEXT, CENTER_TEXT, SOLID_LINE, BLACK, 1);
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

int RobotSim::getSelectedNumber(string main_instruction, string entity_name, int min_number, int max_number, int init_x, int init_y, int button_width, int button_height, int margin)
{
    // Create Random circles
    vector<Circle> circles = createRandomCircles(max_number, 0.05, (drawing_system.getWidth() / 2) - 150, (drawing_system.getHeight() / 2) - 150);

    int number = min_number; // Mininum possible number of entities

    int x = init_x,
        y = init_y;

    vector<string> options = {"+", entity_name + ": ", "-"}; // Options to diplay
    int numOptions = options.size();                         // Number of options
    bool isOptionSelected = false;

    do
    {
        int i = 1;
        int selectedOption = 0;
        y = init_y;

        if (GetKeyState(VK_UP) & 0x8000)
        {
            // Move the selection up
            selectedOption = 1;

            // Increase number
            number < max_number ? number++ : number;
        }
        else if (GetKeyState(VK_DOWN) & 0x8000)
        {
            // Move the selection down
            selectedOption = 3;

            // Decrease number
            number > min_number ? number-- : number;
        }
        else if (GetKeyState(VK_SPACE) | GetKeyState(VK_RETURN) & 0x8000)
        {
            // Return the selected option
            isOptionSelected = true;
        }

        // Redraw the menu with the updated selection
        cleardevice();

        previewCircles(circles, number); // Draw a preview of the number of entities

        // Draw the Title, subTitle an Instruction
        drawing_system.drawButton("ROBOT SIMULATION", init_x, init_y - margin - 100, 0, 0, COMPLEX_FONT, 6, CYAN, LEFT_TEXT, CENTER_TEXT, SOLID_LINE, CYAN, 0);
        drawing_system.drawButton("Robotics Mini-Project MIAAD - By: Aarab Abderrahmane & Walid Fajri", init_x, init_y - margin - 70, 0, 0, COMPLEX_FONT, 1, BLACK, LEFT_TEXT, CENTER_TEXT, SOLID_LINE, BLACK, 0);
        drawing_system.drawButton(main_instruction, init_x, init_y - margin, 0, 0, COMPLEX_FONT, 3, CYAN, LEFT_TEXT, CENTER_TEXT, SOLID_LINE, CYAN, 0);

        // Display the menu Options
        for (const string &option : options)
        {

            if (i == selectedOption)
            {
                drawing_system.drawButton(option, x, y, button_width, button_height, COMPLEX_FONT, 1, CYAN, CENTER_TEXT, CENTER_TEXT, SOLID_LINE, CYAN, 3);
                y += button_height + margin;
            }
            else
            {
                if (i == 2)
                {
                    string temp_string;
                    temp_string = (number == min_number) ? (option + "Min " + to_string(number)) : (number == max_number) ? (option + "Max " + to_string(number))
                                                                                                                          : (option + to_string(number));

                    drawing_system.drawButton(temp_string, x, y, button_width, button_height, COMPLEX_FONT, 1, CYAN, CENTER_TEXT, CENTER_TEXT, SOLID_LINE, CYAN, 4);
                    y += button_height + margin;
                }
                else
                {
                    // Draw Menu Buttons
                    drawing_system.drawButton(option, x, y, button_width, button_height, COMPLEX_FONT, 1, BLACK, CENTER_TEXT, CENTER_TEXT, SOLID_LINE, BLACK, 1);
                    y += button_height + margin;
                }
            }

            i++;
        }

        // Smooth animation
        swapbuffers();

        // Wait for a short time
        Sleep(100);

    } while (!isOptionSelected);

    // Return the index of the selected option
    return number;
}

void RobotSim::setMode(int mode)
{
    switch (mode)
    {
    case PLAYBACK_MODE: // PLAYBACK MODE

        // Check if the previous mode was AUTO_MODE then disable the border
        drawing_system.setBorder(controller_system.getMode() == AUTO_MODE ? 0 : drawing_system.getBorder());

        states = loadStates(states_filePath); // Load the states from file

        controller_system.setPlayBackMode(states);

        break;
    case MANUAL_MODE: // MANUAL MODE
        log_file.open(states_filePath, ios::trunc & ios::app);
        controller_system.setManualMode();
        break;
    case AUTO_MODE: // AUTO MODE
        log_file.open(states_filePath, ios::trunc & ios::app);
        controller_system.setAutomaticMode();
        break;

    default: // AUTO MODE is default
        log_file.open(states_filePath, ios::trunc & ios::app);
        controller_system.setAutomaticMode();
        break;
    }
    clearmouseclick(WM_LBUTTONDOWN); // Clear the Left Mouse Event
    clearmouseclick(WM_RBUTTONDOWN); // Clear the Right Mouse Event
}

vector<Circle> RobotSim::createRandomCircles(int numCircles, double maxCircleRadius)
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
           robot_radius = robot.getRadius() * drawing_system.getScale(),
           sensor_max_distance = robot.getSensorMaxDistance();

    double max_circle_radius = maxCircleRadius * drawing_system.getScale();

    double min_PosX = abs(windowBorder + max_circle_radius + robot_x),
           max_PosX = abs(windowWidth - windowBorder - max_circle_radius);

    double min_PosY = abs(windowBorder + max_circle_radius + robot_y),
           max_posY = abs(windowHeight - windowBorder - max_circle_radius);

    uniform_real_distribution<> rad(0.01, max_circle_radius / drawing_system.getScale());
    uniform_real_distribution<> xPos(min_PosX, max_PosX);
    uniform_real_distribution<> yPos(min_PosY, max_posY);

    int i = 0;
    while (i < numCircles)
    {

        Circle circle(xPos(gen), yPos(gen), rad(gen));
        double circle_x = circle.getXPosition(),
               circle_y = circle.getYPosition(),
               circle_radius = circle.getRadius() * drawing_system.getScale();

        // check if circle is within window and does not overlap with any other circles
        bool valid = true;
        for (Circle other : circles)
        {
            double other_x = other.getXPosition(),
                   other_y = other.getYPosition(),
                   other_radius = other.getRadius() * drawing_system.getScale();

            bool isInBounds = (circle_x + circle_radius < windowWidth) && (circle_x - circle_radius > 0) && (circle_y + circle_radius < windowHeight) && (circle_y - circle_radius > 0);

            double distance = std::sqrt((circle_x - other_x) * (circle_x - other_x) +
                                        (circle_y - other_y) * (circle_y - other_y));

            double min_distance = circle_radius + other_radius + 2 * robot_radius + 2 * sensor_max_distance;

            if (!isInBounds || distance <= min_distance)
            {
                valid = false;
                break;
            }
        }
        if (valid)
        {
            circles.push_back(circle);
            i++;

            // Progress Text
            sprintf(temp_text, "Creating Entities: %d/%d Completed", i, numCircles);
            drawing_system.drawText(temp_text, 10, drawing_system.getHeight() - 10, COMPLEX_FONT, 1, CYAN, HORIZ_DIR);

            // Smooth animation
            swapbuffers();
        }
    }

    cleardevice();

    return circles;
}

vector<Circle> RobotSim::createRandomCircles(int numCircles, double maxCircleRadius, int offset_x, int offset_y)
{
    vector<Circle> circles; // initialize Circle vector

    // Create random number generator
    random_device rd;
    mt19937 gen(rd());
    gen.seed(time(0)); // seed generator with current time

    double windowWidth = drawing_system.getWidth(),
           windowHeight = drawing_system.getHeight(),
           windowBorder = drawing_system.getBorder();

    double max_circle_radius = maxCircleRadius * drawing_system.getScale();

    double min_PosX = abs(windowBorder + max_circle_radius + offset_x),
           max_PosX = abs(windowWidth - windowBorder - max_circle_radius);

    double min_PosY = abs(windowBorder + max_circle_radius + offset_y),
           max_posY = abs(windowHeight - windowBorder - max_circle_radius);

    uniform_real_distribution<> rad(0.01, max_circle_radius / drawing_system.getScale());
    uniform_real_distribution<> xPos(min_PosX, max_PosX);
    uniform_real_distribution<> yPos(min_PosY, max_posY);

    int i = 0;
    while (i < numCircles)
    {

        Circle circle(xPos(gen), yPos(gen), rad(gen));
        double circle_x = circle.getXPosition(),
               circle_y = circle.getYPosition(),
               circle_radius = circle.getRadius() * drawing_system.getScale();

        // check if circle is within window and does not overlap with any other circles
        bool valid = true;
        for (Circle other : circles)
        {
            double other_x = other.getXPosition(),
                   other_y = other.getYPosition(),
                   other_radius = other.getRadius() * drawing_system.getScale();

            bool isInBounds = (circle_x + circle_radius < windowWidth) && (circle_x - circle_radius > 0) && (circle_y + circle_radius < windowHeight) && (circle_y - circle_radius > 0);

            double distance = std::sqrt((circle_x - other_x) * (circle_x - other_x) +
                                        (circle_y - other_y) * (circle_y - other_y));

            double min_distance = circle_radius + other_radius;

            if (!isInBounds || distance <= min_distance)
            {
                valid = false;
                break;
            }
        }
        if (valid)
        {
            circles.push_back(circle);
            i++;
        }
    }

    cleardevice();

    return circles;
}

void RobotSim::logStates(Robot &robot)
{
    // Record Robot's data and log it
    double robot_x = robot.getXPosition(),
           robot_y = robot.getYPosition(),
           robot_orientation = robot.getOrientation(),
           robot_left_velocity = robot.getLeftWheelVelocity(),
           robot_right_velocity = robot.getRightWheelVelocity();

    double circle_x = 0,
           circle_y = 0,
           circle_radius = 0;

    // If an obstacle or goal are added log them too
    int isAdded = addCircleFromMouse(); // Add Obstacles & Goals using the mouse

    if (isAdded != 0)
    {
        // Get the added (Obstacles/Goals) data
        if (isAdded == -1) // Means a goals was added
        {
            circle_x = goals.back().getXPosition(),
            circle_y = goals.back().getYPosition(),
            circle_radius = goals.back().getRadius();
        }
        if (isAdded == 1) // Means an Obstacle was added
        {
            circle_x = obstacles.back().getXPosition(),
            circle_y = obstacles.back().getYPosition(),
            circle_radius = obstacles.back().getRadius();
        }
    }

    log_file << robot_x << " " << robot_y << " " << robot_orientation << " " << robot_left_velocity << " " << robot_right_velocity << " "
             << " " << isAdded << " " << circle_x << " " << circle_y << " " << circle_radius << endl;
}

deque<vector<double>> RobotSim::loadStates(string filePath) const
{
    ifstream states_file;
    states_file.open(filePath);

    double robot_x,
        robot_y,
        robot_orientation,
        robot_right_velocity,
        robot_left_velocity;

    int isAdded;

    double circle_x,
        circle_y,
        circle_radius;

    deque<vector<double>> states;

    // Get the robot's data and save it to vector
    while (states_file >> robot_x >> robot_y >> robot_orientation >> robot_left_velocity >> robot_right_velocity >> isAdded >> circle_x >> circle_y >> circle_radius)
    {
        states.push_back(vector<double>{robot_x, robot_y, robot_orientation, robot_left_velocity, robot_right_velocity, (double)isAdded, circle_x, circle_y, circle_radius});
    };

    states_file.close();

    return states;
}

vector<Circle> RobotSim::loadCircles(string filePath) const
{
    ifstream objects_file;
    objects_file.open(filePath);

    double objects_x, objects_y, objects_radius;

    vector<Circle> objects;

    while (objects_file >> objects_x >> objects_y >> objects_radius)
    {
        objects.push_back(Circle(objects_x, objects_y, objects_radius));
    };

    objects_file.close();

    return objects;
}

void RobotSim::saveCircles(string filePath, vector<Circle> &circles)
{
    ofstream circles_file;
    circles_file.open(filePath);

    if (!circles.empty())
    {
        for (Circle &circle : circles)
        {
            double circle_x = circle.getXPosition(),
                   circle_y = circle.getYPosition(),
                   circle_radius = circle.getRadius();

            circles_file << circle_x << " " << circle_y << " " << circle_radius << endl;
        }
    }

    circles_file.close();
}

int RobotSim::addCircleFromMouse()
{
    // Check for mouse input
    int x, y;
    double radius = DEFAULT_RADIUS;

    if (controller_system.handleMouseInput(WM_RBUTTONDOWN, x, y))
    {
        // Right mouse button was clicked, set the goal position
        goals.push_back(Circle(x, y, radius));
        return -1;
    }
    else if (controller_system.handleMouseInput(WM_LBUTTONDOWN, x, y))
    {
        // Left mouse button was clicked, add a new obstacle
        obstacles.push_back(Circle(x, y, radius));
        return 1;
    }

    return 0;
}

bool RobotSim::handleKeyboard()
{
    if (kbhit())
    {
        if (controller_system.isKeyPressed(VK_ESCAPE))
        {
            // Press ESC to go back to the menu
            log_file.close(); // Close Log file
            loadConfig();     // Load intial configuration
            run();            // Rerun simulation
        }
        else if (controller_system.isKeyPressed(VK_F1) && controller_system.getMode() != AUTO_MODE)
        {
            // Press F1 to change to automatic mode
            log_file.close();   // Close Log file
            setMode(AUTO_MODE); // Set Automatic Mode
        }
        else if (controller_system.isKeyPressed(VK_F2) && controller_system.getMode() != MANUAL_MODE)
        {
            // Press F2 to change to manual mode
            drawing_system.setBorder(WINDOW_BORDER); // Reset the border
            log_file.close();                        // Close Log file
            setMode(MANUAL_MODE);                    // Set Manual Mode
        }
        else if (controller_system.isKeyPressed(VK_F3) && controller_system.getMode() != PLAYBACK_MODE)
        {
            // Press F3 to change to playback mode
            drawing_system.setBorder(WINDOW_BORDER); // Reset the border
            log_file.close();                        // Close Log file
            loadConfig();                            // Load initial configuration
            setMode(PLAYBACK_MODE);                  // Set Playback Mode
        }
        else if (controller_system.isKeyPressed(0x53))
        {
            // Press S to save the Obstacles & Goals to File
            // Save the obstacles and goals to file
            saveCircles(obstacles_filePath, obstacles);
            saveCircles(goals_filePath, goals);
        }
        else if (controller_system.isKeyPressed(0x42) && controller_system.getMode() != PLAYBACK_MODE && controller_system.getMode() != AUTO_MODE)
        {
            // Press B to toggle the border on and off
            // if the border size is already 0, then return the original value
            int border_size = drawing_system.getBorder() <= 0 ? WINDOW_BORDER : 0;
            drawing_system.setBorder(border_size);
            Sleep(200);
        }
        else if (controller_system.isKeyPressed(VK_OEM_PLUS) || controller_system.isKeyPressed(VK_OEM_MINUS))
        {
            // Press +/- to zoom in or out
            int scale_size = drawing_system.getScale();

            if (controller_system.isKeyPressed(VK_OEM_PLUS))
                scale_size += 50; // Press + to zoom in

            if (controller_system.isKeyPressed(VK_OEM_MINUS))
                scale_size -= 50; // Press - to zoom out

            drawing_system.setScale(max(500, min(scale_size, 2000)));
            Sleep(100);
        }
        else if (controller_system.isKeyPressed(0x30))
        {
            debug_level = NO_DEBUG; // Press 0 to show no debug
        }
        else if (controller_system.isKeyPressed(0x31))
        {
            debug_level = ALL_DEBUG; // Press 1 to show all debug
        }
        else if (controller_system.isKeyPressed(0x32))
        {
            debug_level = TEXT_DEBUG; // Press 2 to show text debug
        }
        else if (controller_system.isKeyPressed(0x33))
        {
            debug_level = AXIS_DEBUG; // Press 3 to show axis debug
        }
        else if (controller_system.isKeyPressed(0x34))
        {
            debug_level = DISTANCE_DEBUG; // Press 4 to show distance debug
        }
    }
}
bool RobotSim::saveConfig()
{
    ofstream config_file;
    config_file.open(config_filePath);

    // Get Intial State of the World/Simulation and save it
    int world_width = drawing_system.getWidth(),
        world_height = drawing_system.getHeight(),
        border_size = drawing_system.getBorder(),
        scale_size = drawing_system.getScale();

    // Get Intial State of the Robot and save it
    double robot_x = robot.getXPosition(),
           robot_y = robot.getYPosition(),
           robot_orientation = robot.getOrientation(),
           robot_left_velocity = robot.getLeftWheelVelocity(),
           robot_right_velocity = robot.getRightWheelVelocity();

    // Save World Data
    config_file << world_width << " " << world_height << " " << border_size << " " << scale_size << endl;
    // Save Robot Data
    config_file << robot_x << " " << robot_y << " " << robot_orientation << " " << robot_left_velocity << " " << robot_right_velocity << endl;

    config_file.close();
}

bool RobotSim::loadConfig()
{
    ifstream config_file;
    config_file.open(config_filePath);

    // Load Intial State of the World/Simulation
    int world_width,
        world_height,
        border_size,
        scale_size;

    // Load Intial State of the Robot
    double robot_x,
        robot_y,
        robot_orientation,
        robot_left_velocity,
        robot_right_velocity;

    config_file >> world_width >> world_height >> border_size >> scale_size;
    config_file >> robot_x >> robot_y >> robot_orientation >> robot_left_velocity >> robot_right_velocity;

    drawing_system.setBorder(border_size);
    drawing_system.setScale(scale_size);

    robot.setXPosition(robot_x);
    robot.setYPosition(robot_y);
    robot.setOrientation(robot_orientation);
    robot.setLeftWheelVelocity(robot_left_velocity);
    robot.setRightWheelVelocity(robot_right_velocity);

    config_file.close();
}

void RobotSim::previewRobot(Robot &preview_bot)
{
    // Update the wheels' angular velocity over time
    preview_bot.setLeftWheelVelocity(preview_bot.getLeftWheelVelocity() + 0.005);
    preview_bot.setRightWheelVelocity(preview_bot.getRightWheelVelocity() + 0.0055);

    // Update the preview robot's position and orientation
    preview_bot.update();

    // Draw the preview robot
    drawing_system.drawRobot(preview_bot);
}
void RobotSim::previewCircles(vector<Circle> &circles, int numCircles)
{
    vector<Circle> temp_circles = {circles.begin(), circles.begin() + numCircles};

    // Draw the circles
    drawing_system.drawCircles(temp_circles, SOLID_LINE, LIGHTCYAN, 3, SOLID_FILL, CYAN);
}

DrawingSystem RobotSim::getDrawingSystem() { return drawing_system; }
CollisionSystem RobotSim::getCollisionSystem() { return collision_system; }
ControllerSystem RobotSim::getControllerSystem() { return controller_system; }
Robot RobotSim::getRobot() { return robot; }
vector<Circle> RobotSim::getObstacles() { return obstacles; }
vector<Circle> RobotSim::getGoals() { return goals; }

// void RobotSim::setDrawingSystem(DrawingSystem &drawingSystem) { drawing_system = drawingSystem }
// void RobotSim::setCollisionSystem(CollisionSystem &collisionSystem) { collision_system = collisionSystem }
// void RobotSim::setControllerSystem(ControllerSystem &controllerSystem) { controller_system = controllerSystem }
// void RobotSim::setRobot(Robot &rob) {  robot=rob; }
// void RobotSim::setObstacles(vector<Circle> &obs) { obstacles = obs; }
// void RobotSim::setGoals(vector<Circle> &gos) { goals = gos; }
void RobotSim::setDebug(int debug_lvl) { debug_level = debug_lvl; }