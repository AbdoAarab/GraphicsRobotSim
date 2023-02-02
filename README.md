# GraphicsRobotSim

This project is a simulation of a 2-wheeled robot moving in a 2D environment. The robot can be controlled in three different modes: automatic, manual, and creative. In the automatic mode, the robot will navigate from a starting point to a goal, avoiding obstacles along the way. In the manual mode, the robot can be controlled using keyboard input. In the creative mode, the user can add or remove obstacles to the environment. Finally, in the playback mode, the user can replay the previous simulation.

## Features

- Realistic movement and rotation based on the angular velocity of the wheels
- Keyboard control in manual mode
- Automatic navigation from starting point to goal while avoiding obstacles
- Obstacle avoidance using a simulated sensor in front of the robot
- Obstacles & goals can be generated randomly or manually
- Obstacles (Right Mouse Button) & goals (Left Mouse Button) can be added in the form of circles
- Free positioning and movement of the robot in creative mode

## Modes

There are four different modes available in this simulation: automatic, manual, creative, and playback.

### Automatic Mode

In the automatic mode, the robot will navigate from the starting point to the goal while avoiding obstacles along the way. The robot uses a simulated sensor in front of it to detect obstacles and navigate around them.

### Manual Mode

In the manual mode, the user can control the robot using keyboard input. The user can apply a velocity to the robot's wheels, causing it to move forward or backward.

### Creative Mode

In the creative mode, the user can add or remove obstacles to the environment. The user can also modify the starting point and the goal.

### Playback Mode

In the playback mode, the user can replay the previous simulation.

## Screenshots

### Mode Select / Home Screen

![Mode Select / Home Screen](https://github.com/AbdoAarab/GraphicsRobotSim/blob/Media/Mode_Select.png)

### Obstacles & Goals Creation

![Obstacles & Goals Creation](https://github.com/AbdoAarab/GraphicsRobotSim/blob/Media/Obstacles_Goals_Creation.png)

### Number of Random Obstacles Selection

![Number of Random Obstacles Selection](https://github.com/AbdoAarab/GraphicsRobotSim/blob/Media/Number_of_Obstacles.png)

### Number of Random Goals Selection

![Number of Random Goals Selection](https://github.com/AbdoAarab/GraphicsRobotSim/blob/Media/Number_of_Goals.png)

### Automatic Mode (All Debug)

![Automatic Mode (All Debug)](https://github.com/AbdoAarab/GraphicsRobotSim/blob/Media/Automatic_Mode_AllDebug.png)

### Automatic Mode (No Debug)

![Automatic Mode (No Debug)](https://github.com/AbdoAarab/GraphicsRobotSim/blob/Media/Automatic_Mode_NoDebug.png)

## Built With

- C++
- Graphics.h library

## Requirements

- Graphics.h library

## Getting Started

These instructions will get you a copy of the project up and running on your local machine for development and testing purposes.

### Prerequisites

You will need the following software installed on your computer to run this project:

- A C++ compiler
- The Graphics.h library

### Installing

Clone the repository to your local machine:

    git clone https://github.com/AbdoAarab/GraphicsRobotSim.git

Navigate to the project directory:

    cd GraphicsRobotSim/Home/src

Compile the code using your preferred C++ compiler:

    g++ main.cpp -o simulation -lgraph

## How to Run

Compile and run the code on a C++ compiler to start the simulation. Use the keyboard to control the robot in manual mode, or select automatic or creative mode from the options menu.

## Future Work

- Add more advanced obstacle avoidance algorithms
- Implement a way for the robot to dynamically change its velocity based on the proximity of obstacles
- Add a way to change the size and shape of the robot
- Add more customization options for the simulation environment

## Conclusion

This simulation provides a simple and intuitive way to understand the movements of a 2-wheeled robot. By controlling the robot in different modes, the user can gain a deeper understanding of the robot's behavior and the challenges it faces in navigating a 2D environment.

## Contributions

Feel free to contribute to this project by submitting pull requests or by reporting bugs.

## Acknowledgments

- https://www.programmingsimplified.com/c/graphics.h
- https://home.cs.colorado.edu/~main/bgi/doc/bgi.html
- https://github.com/ullaskunder3/Solution-to-graphics.h
