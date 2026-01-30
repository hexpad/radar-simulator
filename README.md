# Simple C++ Radar Sim

![simulator](https://github.com/user-attachments/assets/86f10bb1-0fd7-46a8-8f74-1891ef896b3d)


This is a small project I put together using C++ and Raylib.

I wanted to see if I could recreate how a basic radar sweep works—detecting moving dots on a screen and logging that data somewhere useful.


# How it works

Basically, there's a rotating line (the radar beam) and some random targets moving around. A target only shows up when the line actually hits it. It’s not just a visual trick; the app checks for intersections in real-time.

Tracking: If you see a target, you can click it with the mouse to "lock" on. It'll show you the distance and speed of that specific target.

Logging: Every time the radar hits something, it saves the details (ID, coordinates, distance) into a radar_log.csv file. I added this so I could check the data later in Excel.

Movement: The targets move on their own at different speeds.


# Requirements

You just need Raylib to get this running.

Link: www.raylib.com


# Controls

Left Click: Select a target you want to follow.

ESC: Close the window.


# What I used for the logs

<img width="486" height="705" alt="logs" src="https://github.com/user-attachments/assets/3d29d295-65db-4f09-af68-7cd0bdad90ab" />

In the radar_log.csv, it saves:

Time: When it was spotted.

ID: Which target it was.

Position: X and Y spots.

Math stuff: Distance from the center and the angle of the radar line.
