#ifndef CIRCLE_H
#define CIRCLE_H

class Circle {
 public:
  // Constructor
  Circle(double x_position, double y_position, double radius);
  Circle();
  // Getters
  double getXPosition() const;
  double getYPosition() const;
  double getRadius() const;
  
  // Setters
  void setPosition(double x_position, double y_position);
  void setRadius(double radius);
  
 private:
  double x_position;
  double y_position;
  double radius;
};

#endif
