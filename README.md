# GraphicsRobotSim

This project is a 2D top-down simulation of a 2-wheeled robot that moves and rotates based on the angular velocity of its wheels. The robot can be controlled through keyboard input in manual mode, navigates from a starting point to a goal while avoiding obstacles in automatic mode, or be freely positioned and moved in creative mode.

## Features

- Realistic movement and rotation based on the angular velocity of the wheels
- Keyboard control in manual mode
- Automatic navigation from starting point to goal while avoiding obstacles
- Obstacle avoidance using a simulated sensor in front of the robot
- Obstacles & goals can be generated randomly or manually
- Obstacles (Right Mouse Button) & goals (Left Mouse Button) can be added in the form of circles
- Free positioning and movement of the robot in creative mode

## Manual Mode

In manual mode, the robot can be controlled through keyboard input. Use the arrow keys to move the robot in the desired direction. The speed of the robot can be adjusted by changing the `VELOCITY` constant in the code.

## Automatic Mode

In automatic mode, the robot will navigate from the starting point to the goal while avoiding obstacles. The starting point and goal can be set by clicking on the desired locations in the simulation window. Obstacles can be added by clicking and dragging to create circles. The robot uses a simulated sensor in front of it to detect obstacles and navigate around them.

## Creative Mode

In creative mode, the robot can be freely positioned and moved in the simulation window. This mode can be used for testing and experimentation. The position and orientation of the robot can be changed by clicking and dragging.

## Screenshots

Insert screenshots here to give an idea of what the simulation looks like.

## GIFs

Insert GIFs here to show the different modes and how the robot moves and navigates.

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
    
    ```git clone https://github.com/AbdoAarab/GraphicsRobotSim.git```

Navigate to the project directory:
    
    ```cd GraphicsRobotSim/Home/src```

Compile the code using your preferred C++ compiler:

    ```g++ main.cpp -o simulation -lgraph```

## How to Run

Compile and run the code on a C++ compiler to start the simulation. Use the keyboard to control the robot in manual mode, or select automatic or creative mode from the options menu.

## Future Work

- Add more advanced obstacle avoidance algorithms
- Implement a way for the robot to dynamically change its velocity based on the proximity of obstacles
- Add a way to change the size and shape of the robot
- Add more customization options for the simulation environment

## Contributions

Feel free to contribute to this project by submitting pull requests or by reporting bugs.

## Author

Abdo Aarab

## Acknowledgments

- https://www.programmingsimplified.com/c/graphics.h
- https://home.cs.colorado.edu/~main/bgi/doc/bgi.html
- https://github.com/ullaskunder3/Solution-to-graphics.h
