// DrawingSystem.cpp
#include "DrawingSystem.h"

#include <cmath>

static char debug_text[1000];
static char mode_text[1000];

bool is_obstacle_detected = false;
Circle detected_obstacle;

DrawingSystem::DrawingSystem(int width, int height, int border) : width(width), height(height), border(border)
{
  initwindow(width, height, "Robotics Mini Project MIAAD - By: Abderrahmane Aarab & Walid Fajri", 0, 0, true, true);
  setbkcolor(WHITE);
}
DrawingSystem::DrawingSystem() : width(GetSystemMetrics(SM_CXSCREEN)), height(GetSystemMetrics(SM_CYSCREEN)), border(10)
{
  initwindow(width, height, "Robotics Mini Project MIAAD - By: Abderrahmane Aarab & Walid Fajri", 0, 0, true, true);
  setbkcolor(WHITE);
}

void DrawingSystem::drawRobot(Robot &robot)
{
  int x_position = robot.getXPosition();
  int y_position = robot.getYPosition();
  double orientation = robot.getOrientation(); // Angle in Radiants
  int radius = robot.getRadius() * scale;
  int wheel_distance = robot.getWheelDistance() * scale;
  int wheel_radius = robot.getWheelRadius() * scale;

  // Define the pivot point and angle of rotation
  int pivotX = x_position;
  int pivotY = y_position;
  double angle = orientation;

  // Save the current graphics context
  std::vector<int> frame_graphic = {x_position, y_position - radius,
                                    x_position + radius, y_position,
                                    x_position, y_position + radius,
                                    x_position, y_position - radius};

  std::vector<int> right_wheel_graphic = {x_position - wheel_radius, (y_position - (wheel_distance / 2)),
                                          x_position + wheel_radius, (y_position - (wheel_distance / 2)),
                                          x_position + wheel_radius, (y_position - (3 * wheel_distance / 5)),
                                          x_position - wheel_radius, (y_position - (3 * wheel_distance / 5)),
                                          x_position - wheel_radius, (y_position - (wheel_distance / 2))};

  std::vector<int> left_wheel_graphic = {x_position - wheel_radius, (y_position + (3 * wheel_distance / 5)),
                                         x_position + wheel_radius, (y_position + (3 * wheel_distance / 5)),
                                         x_position + wheel_radius, (y_position + (wheel_distance / 2)),
                                         x_position - wheel_radius, (y_position + (wheel_distance / 2)),
                                         x_position - wheel_radius, (y_position + (3 * wheel_distance / 5))};

  // Draw the robot body as a circle
  drawCircle(x_position, y_position, radius, SOLID_LINE, BLACK, 1, SOLID_FILL, WHITE);

  // Rotate the frame
  std::vector<int> rotatedFrame = rotateShape(frame_graphic, pivotX, pivotY, angle);

  // Draw the robot's triangle
  drawShape(rotatedFrame, SOLID_LINE, BLACK, 1, SOLID_FILL, CYAN);

  // Rotate the wheels
  std::vector<int> rotatedLeftWheel = rotateShape(right_wheel_graphic, pivotX, pivotY, angle);
  std::vector<int> rotatedRightWheel = rotateShape(left_wheel_graphic, pivotX, pivotY, angle);

  // Draw the robot's wheels as rectangle
  drawShape(rotatedLeftWheel, SOLID_LINE, BLACK, 1, SOLID_FILL, YELLOW);
  drawShape(rotatedRightWheel, SOLID_LINE, BLACK, 1, SOLID_FILL, YELLOW);
}

void DrawingSystem::drawObstacles(std::vector<Circle> &obstacles)
{
  // Draw the obstacles
  drawCircles(obstacles, SOLID_LINE, LIGHTRED, 3, SOLID_FILL, RED);
}

void DrawingSystem::drawGoals(std::vector<Circle> &goals)
{
  // Draw the goals
  drawCircles(goals, SOLID_LINE, LIGHTGREEN, 3, SOLID_FILL, GREEN);
}

void DrawingSystem::drawCircles(std::vector<Circle> &circles,
                                int edge_style, int edge_color, int edge_thickness,
                                int fill_style, int fill_color)
{
  for (const Circle &circle : circles)
  {
    double circle_x = circle.getXPosition();
    double circle_y = circle.getYPosition();
    double circle_radius = circle.getRadius() * scale;

    // Draw the circle
    drawCircle(circle_x, circle_y, circle_radius, edge_style, edge_color, edge_thickness, fill_style, fill_color);
  }
}

void DrawingSystem::drawBorder()
{
  // Define the shapes as a list of points
  std::vector<int> border_graphic = {border, border,
                                     width - border, border,
                                     width - border, height - border,
                                     border, height - border,
                                     border, border};

  // Draw the border
  drawShape(border_graphic, SOLID_LINE, BLACK, 3, EMPTY_FILL, BLACK);
}

void DrawingSystem::drawDebug(Robot &robot, std::vector<Circle> &obstacles, std::vector<Circle> &goals, int debug_level = 0)
{
  double robot_x = robot.getXPosition(),
         robot_y = robot.getYPosition(),
         robot_orientation = robot.getOrientation(), // Angle in Radians (Angle in Text is in Degrees)
      robot_radius = robot.getRadius() * scale,
         robot_right_velocity = robot.getRightWheelVelocity() * 10,
         robot_left_velocity = robot.getLeftWheelVelocity() * 10,
         robot_displacement = robot.getDisplacement(),
         robot_velocity = robot.getVelocity(),
         robot_curvatureRadius = robot.getCurvatureRadius();

  double DistGoal = 0; // Calculate the distance fromt the robot to the goal
  if (!goals.empty())
    DistGoal = sqrt((robot_x + robot_radius - goals.back().getXPosition()) *
                        (robot_x + robot_radius - goals.back().getXPosition()) +
                    (robot_y + robot_radius - goals.back().getYPosition()) *
                        (robot_y + robot_radius - goals.back().getYPosition()));

  double axis_size = 50;
  std::vector<int> x_axis_lines = {(int)robot_x, (int)robot_y, (int)robot_x, (int)((robot_y + robot_radius) + axis_size)};
  std::vector<int> y_axis_lines = {(int)robot_x, (int)robot_y, (int)((robot_x + robot_radius) + axis_size), (int)robot_y};
  std::vector<int> rotatedShape;

  switch (debug_level)
  {
  case 0: // No Debug

    break;

  case 1: // Debug Text

    // Debug Text
    sprintf(debug_text, "X = %.f pxl, Y = %.f pxl, RW = %.2f rad/s, LW = %.2f rad/s, A = %.f deg, GD = %.f pxl, Dr = %.3f m, Vr = %.3f m/s", robot_x, robot_y, robot_right_velocity, robot_left_velocity, robot_orientation * (180 / M_PI), DistGoal, robot_displacement*10, robot_velocity);
    drawText(debug_text, 10, 20, COMPLEX_FONT, 1, BLACK, HORIZ_DIR);

    break;

  case 2: // World Axis lines

    // Debug Text
    sprintf(debug_text, "X = %.f pxl, Y = %.f pxl, RW = %.2f rad/s, LW = %.2f rad/s, A = %.f deg, GD = %.f pxl, Dr = %.3f m, Vr = %.3f m/s", robot_x, robot_y, robot_right_velocity, robot_left_velocity, robot_orientation * (180 / M_PI), DistGoal, robot_displacement*10, robot_velocity);
    drawText(debug_text, 10, 20, COMPLEX_FONT, 1, BLACK, HORIZ_DIR);

    // Debug - World Axis lines

    drawLine(border, (height / 2), (width - border), (height / 2), SOLID_LINE, LIGHTGRAY, 1);
    drawLine((width / 2), border, (width / 2), (height - border), SOLID_LINE, LIGHTGRAY, 1);

    // Debug - Robot x,y axis lines
    // X Axis
    rotatedShape = rotateShape(x_axis_lines, robot_x, robot_y, robot_orientation);
    drawShape(rotatedShape, SOLID_LINE, LIGHTGREEN, 2, EMPTY_FILL, GREEN);
    // Y Axis
    rotatedShape = rotateShape(y_axis_lines, robot_x, robot_y, robot_orientation);
    drawShape(rotatedShape, SOLID_LINE, LIGHTRED, 2, EMPTY_FILL, RED);

    break;

  case 3: // Distance lines

    // Debug Text
    sprintf(debug_text, "X = %.f pxl, Y = %.f pxl, RW = %.2f rad/s, LW = %.2f rad/s, A = %.f deg, GD = %.f pxl, Dr = %.3f m, Vr = %.3f m/s", robot_x, robot_y, robot_right_velocity, robot_left_velocity, robot_orientation * (180 / M_PI), DistGoal, robot_displacement*10, robot_velocity);
    drawText(debug_text, 10, 20, COMPLEX_FONT, 1, BLACK, HORIZ_DIR);

    // Debug - Robot position line
    drawLine(width / 2, height / 2, robot_x, robot_y, DASHED_LINE, BLUE, 1);

    // Debug - Distance from robot to obstacles
    for (const Circle &obstacle : obstacles)
    {
      double x_position = obstacle.getXPosition();
      double y_position = obstacle.getYPosition();

      drawLine(x_position, y_position, robot_x, robot_y, DOTTED_LINE, RED, 1);
    }

    // Debug - Distance from robot to goals
    for (const Circle &goal : goals)
    {
      double x_position = goal.getXPosition();
      double y_position = goal.getYPosition();

      drawLine(x_position, y_position, robot_x, robot_y, DOTTED_LINE, GREEN, 1);
    }
    break;

  default:
    // Debug Text
    sprintf(debug_text, "X = %.f pxl, Y = %.f pxl, RW = %.2f rad/s, LW = %.2f rad/s, A = %.f deg, GD = %.f pxl, Dr = %.3f m, Vr = %.3f m/s", robot_x, robot_y, robot_right_velocity, robot_left_velocity, robot_orientation * (180 / M_PI), DistGoal, robot_displacement*10, robot_velocity);
    drawText(debug_text, 10, 20, COMPLEX_FONT, 1, BLACK, HORIZ_DIR);

    // Debug - World Axis lines

    drawLine(border, (height / 2), (width - border), (height / 2), SOLID_LINE, LIGHTGRAY, 1);
    drawLine((width / 2), border, (width / 2), (height - border), SOLID_LINE, LIGHTGRAY, 1);

    // Debug - Robot x,y axis lines
    // X Axis
    rotatedShape = rotateShape(x_axis_lines, robot_x, robot_y, robot_orientation);
    drawShape(rotatedShape, SOLID_LINE, LIGHTGREEN, 2, EMPTY_FILL, GREEN);
    // Y Axis
    rotatedShape = rotateShape(y_axis_lines, robot_x, robot_y, robot_orientation);
    drawShape(rotatedShape, SOLID_LINE, LIGHTRED, 2, EMPTY_FILL, RED);

    // Debug - Robot position line

    drawLine(width / 2, height / 2, robot_x, robot_y, DASHED_LINE, BLUE, 1);

    // Debug - Distance from robot to obstacles
    for (const Circle &obstacle : obstacles)
    {
      double x_position = obstacle.getXPosition();
      double y_position = obstacle.getYPosition();

      drawLine(x_position, y_position, robot_x, robot_y, DOTTED_LINE, RED, 1);
    }

    // Debug - Distance from robot to goals
    for (const Circle &goal : goals)
    {
      double x_position = goal.getXPosition();
      double y_position = goal.getYPosition();

      drawLine(x_position, y_position, robot_x, robot_y, DOTTED_LINE, GREEN, 1);
    }
    break;
  }
}
void DrawingSystem::drawSensor(Robot &robot, std::vector<Circle> &goals)
{

  double robot_x = robot.getXPosition(),
         robot_y = robot.getYPosition();

  if (!goals.empty())
  {
    double goal_x = goals.back().getXPosition(),
           goal_y = goals.back().getYPosition();
    // Debug lines - line pointing to current goal to reach
    drawLine(robot_x, robot_y, goal_x, goal_y, SOLID_LINE, LIGHTGREEN, 4);
  }

  // Check if obstacle is detected
  if (is_obstacle_detected)
  {
    double obstacle_x = detected_obstacle.getXPosition(),
           obstacle_y = detected_obstacle.getYPosition();

    // Debug lines - line pointing to current obstacle
    drawLine(robot_x, robot_y, obstacle_x, obstacle_y, SOLID_LINE, LIGHTRED, 4);
  }

  is_obstacle_detected = false;
}

// Function to rotate a shape around a pivot point by a given angle
std::vector<int> DrawingSystem::rotateShape(std::vector<int> &shape, int pivotX, int pivotY, double angle)
{

  // Create a result vector to store the rotated points
  std::vector<int> rotatedShape;

  double radian = angle;

  // Iterate over each point in the shape
  for (int i = 0; i < shape.size(); i += 2)
  {

    // Get the x and y coordinates of the current point
    double x = shape[i];
    double y = shape[i + 1];

    // Calculate the rotated x and y coordinates using the pivot point as the origin
    double rotatedX = (x - pivotX) * cos(radian) - (y - pivotY) * sin(radian);
    double rotatedY = (x - pivotX) * sin(radian) + (y - pivotY) * cos(radian);

    // Add the rotated point to the result vector, with the pivot point as the origin
    rotatedShape.push_back(rint(pivotX + rotatedX));
    rotatedShape.push_back(rint(pivotY + rotatedY));
  }

  return rotatedShape;
}

void DrawingSystem::drawLine(
    int line_x1, int line_y1, int line_x2, int line_y2,
    int line_style, int line_color, int line_thickness)
{
  setcolor(line_color);
  setlinestyle(line_style, line_color, line_thickness);
  line(line_x1, line_y1, line_x2, line_y2);
}
void DrawingSystem::drawCircle(
    int ellipse_x, int ellipse_y, int ellipse_radius,
    int edge_style, int edge_color, int edge_thickness,
    int fill_style, int fill_color)
{
  setcolor(edge_color);
  setlinestyle(edge_style, edge_color, edge_thickness);
  setfillstyle(fill_style, fill_color);
  fillellipse(ellipse_x, ellipse_y, ellipse_radius, ellipse_radius);
}
void DrawingSystem::drawShape(
    std::vector<int> &shape,
    int edge_style, int edge_color, int edge_thickness,
    int fill_style, int fill_color)
{
  setcolor(edge_color);
  setlinestyle(edge_style, edge_color, edge_thickness);
  setfillstyle(fill_style, fill_color);
  fillpoly(shape.size() / 2, &shape[0]);
}
void DrawingSystem::drawText(
    char *text,
    int text_x, int text_y,
    int font, int font_size, int text_color, int text_orientation)
{
  setcolor(text_color);
  settextstyle(font, text_orientation, font_size);
  settextjustify(LEFT_TEXT, CENTER_TEXT);
  outtextxy(text_x, text_y, text);
}
void DrawingSystem::drawModeText(int mode, bool modeStatus, int state_number)
{
  std::string buttons_guide = " | ESC: Menu, F1: Auto, F2: Manual, F3: Playback, S: Save, 0-4: Debug, B: Border, +/-: Zoom";

  std::string playback_playing = "PLAYBACK MODE: STATE = %d" + buttons_guide;
  std::string manual_playing = "MANUAL MODE: CONTROLLING" + buttons_guide;
  std::string auto_playing = "AUTOMATIC MODE: SEARCHING" + buttons_guide;

  std::string playback_ended = "PLAYBACK MODE: ENDED" + buttons_guide;
  std::string auto_ended = "AUTOMATIC MODE: NO GOAL" + buttons_guide;

  if (modeStatus)
  {
    switch (mode)
    {
    case -1:
      // Debug - PLAYBACK MODE PLAYING & States Text
      sprintf(mode_text, &playback_playing[0], state_number);
      break;
    case 0:
      // Debug - MANUAL MODE PLAYING Text
      sprintf(mode_text, &manual_playing[0]);
      break;
    case 1:
      // Debug - AUTOMATIC MODE PLAYING Text
      sprintf(mode_text, &auto_playing[0]);
      break;
    }
  }
  else
  {
    switch (mode)
    {
    case -1:
      // Debug - PLAYBACK MODE ENDED Text
      sprintf(mode_text, &playback_ended[0]);
      break;
    case 1:
      // Debug - AUTOMATIC MODE ENDED Text
      sprintf(mode_text, &auto_ended[0]);
      break;
    }
  }
  drawText(mode_text, 10, height - 8, COMPLEX_FONT, 1, CYAN, HORIZ_DIR);
}

void DrawingSystem::drawString(std::string text, int x, int y, int font, int font_size, int font_color, int justify_horiz, int justify_vert)
{
  // Convert string to char array
  const int length = text.length();
  // declaring character array (+1 for null terminator)
  char *char_array = new char[length + 1];
  strcpy(char_array, text.c_str());

  // Draw Text
  setcolor(font_color);
  settextstyle(font, HORIZ_DIR, font_size);
  settextjustify(justify_horiz, justify_vert);
  outtextxy(x, y, char_array);
}

void DrawingSystem::drawButton(std::string text, int x, int y, int button_width, int button_height, int font, int font_size, int font_color, int justify_horiz, int justify_vert, int edge_style, int edge_color, int edge_thickness)
{

  // Draw button Rectangle
  setcolor(edge_color);
  setlinestyle(edge_style, edge_color, edge_thickness);
  rectangle(x, y, x + button_width, y + button_height);
  // floodfill(x + edge_thickness + 1, y + edge_thickness + 1, fill_color);

  // Draw Text
  drawString(text, (x + button_width / 2), (y + (button_height + 6) / 2), font, font_size, font_color, justify_horiz, justify_vert);
}

void DrawingSystem::setObstacleDetected(bool isObstacleDetected, Circle detectedObstacle)
{
  is_obstacle_detected = isObstacleDetected;
  detected_obstacle = detectedObstacle;
}

void DrawingSystem::setBorder(int size) { border = size; }
void DrawingSystem::setScale(int size) { scale = size; }

int DrawingSystem::getWidth() const { return width; }
int DrawingSystem::getHeight() const { return height; }
int DrawingSystem::getBorder() const { return border; }
int DrawingSystem::getScale() const { return scale; }
