// DrawingSystem.h
#ifndef DRAWING_SYSTEM_H
#define DRAWING_SYSTEM_H

#include <vector>
#include "Robot.h"
#include "Circle.h"

#include "graphics.h"

#include <string>

#define SCALE 1000

class DrawingSystem
{
public:
    // Constructor
    DrawingSystem(int width, int height, int border);
    DrawingSystem();

    // Getters
    int getWidth() const;
    int getHeight() const;
    int getBorder() const;
    int getScale() const;

    // Setters
    void setBorder(int size);
    void setScale(int size);
    void setObstacleDetected(bool isObstacleDetected, Circle detectedObstacle);

    // Member functions
    void drawLine(
        int line_x1, int line_y1, int line_x2, int line_y2,
        int line_style, int line_color, int line_thickness);

    void drawCircle(
        int ellipse_x, int ellipse_y, int ellipse_radius,
        int edge_style, int edge_color, int edge_thickness,
        int fill_style, int fill_color);

    void drawCircles(
        std::vector<Circle> &circles,
        int edge_style, int edge_color, int edge_thickness,
        int fill_style, int fill_color);

    void drawShape(
        std::vector<int> &shape,
        int edge_style, int edge_color, int edge_thickness,
        int fill_style, int fill_color);

    void drawText(
        char *text,
        int text_x, int text_y,
        int font, int font_size, int text_color, int text_orientation);

    void drawString(
        std::string text,
        int x, int y,
        int font, int font_size, int font_color, int justify_horiz, int justify_vert);

    void drawButton(
        std::string text,
        int x, int y, int button_width, int button_height,
        int font, int font_size, int font_color, int justify_horiz, int justify_vert,
        int edge_style, int edge_color, int edge_thickness);

    void drawRobot(Robot &robot);
    void drawObstacles(std::vector<Circle> &obstacles);
    void drawGoals(std::vector<Circle> &goals);
    void drawBorder();
    void drawModeText(int mode, bool modeEnded, int state_number);
    void drawDebug(Robot &robot, std::vector<Circle> &obstacles, std::vector<Circle> &goals, int debug_level);
    void drawSensor(Robot &robot, std::vector<Circle> &goals);
    std::vector<int> rotateShape(std::vector<int> &shape, int pivotX, int pivotY, double angle);

private:
    int width;
    int height;
    int border;
    int scale = SCALE;
};

#endif // DRAWING_SYSTEM_H
