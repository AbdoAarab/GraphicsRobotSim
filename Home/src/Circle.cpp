#include "Circle.h"
#include "graphics.h"

// Constructor
Circle::Circle(double x_position, double y_position, double radius) : x_position(x_position), y_position(y_position), radius(radius) {
}
Circle::Circle() : x_position(0), y_position(0), radius(0) {
}

// Set the position of the obstacle
void Circle::setPosition(double x_position, double y_position) {
  this->x_position = x_position;
  this->y_position = y_position;
}

// Set the radius of the obstacle
void Circle::setRadius(double radius) {
  this->radius = radius;
}

double Circle::getXPosition() const {
  return x_position;
}

double Circle::getYPosition() const {
  return y_position;
}

double Circle::getRadius() const {
  return radius;
}

