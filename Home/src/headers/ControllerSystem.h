// ControllerSystem.h
#ifndef CONTROLLER_SYSTEM_H
#define CONTROLLER_SYSTEM_H

#include "DrawingSystem.h"
#include "Robot.h"
#include "Circle.h"

#include <deque>
#include <vector>

// MODE Constants
#define PLAYBACK_MODE -1
#define MANUAL_MODE 0
#define AUTO_MODE 1

// MODE STATUS Constants
#define MODE_PLAYING true
#define MODE_ENDED false

class ControllerSystem
{
public:
  // Constructor
  ControllerSystem(DrawingSystem &drawing_system, Robot &robot, std::vector<Circle> &obstacles, std::vector<Circle> &goals);

  // Getters
  int getMode();
  bool getModeStatus();
  int getStatesNumber();

  // Setters
  void setAutomaticMode();
  void setManualMode();
  void setPlayBackMode(std::deque<std::vector<double>> &states_queue);

  // Member functions
  void update();
  void updatePlayBack();
  void updateAutomatic();
  void updateManual();
  bool isAutomaticMode() const;
  bool isManualMode() const;
  bool isPlayBackMode() const;
  bool isKeyPressed(int key);
  bool handleMouseInput(int mouseButton, int &x, int &y);

private:
  DrawingSystem &drawing_system;
  Robot &robot;
  std::vector<Circle> &obstacles;
  std::vector<Circle> &goals;
  int mode = AUTO_MODE;
  bool mode_status = MODE_PLAYING;
  int state_number = 0;
};

#endif // CONTROLLER_SYSTEM_H
