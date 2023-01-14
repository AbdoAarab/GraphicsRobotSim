#include "Goal.h"
#include "graphics.h"

// Constructor
Goal::Goal(double x_position, double y_position, double radius) : x_position(x_position), y_position(y_position), radius(radius) {
}

// Set the position of the goal
void Goal::setPosition(double x_position, double y_position) {
  this->x_position = x_position;
  this->y_position = y_position;
}

// Set the radius of the goal
void Goal::setRadius(double radius) {
  this->radius = radius;
}
