# Conway’s Game of Life – Qt C++

A Conway’s Game of Life simulator built using C++ and the Qt framework, featuring JSON-based pattern loading.

---

# Description

This project is a desktop implementation of Conway’s Game of Life created with Qt and C++.  
The application allows users to run simulations and import patterns using JSON files.

It is focused on visualizing cellular automata and experimenting with different initial configurations.

---

# Features

- Conway’s Game of Life simulation
- Interactive Qt GUI
- Start / pause / reset controls
- Adjustable simulation speed
- Load patterns from JSON files
- Save custom patterns to JSON
- Import predefined structures into the grid
- Dynamic grid rendering and real-time updates

---

# Simulation Rules

The simulation follows the classic Conway’s Game of Life rules:

- Any live cell with fewer than 2 neighbors dies  
- Any live cell with 2 or 3 neighbors survives  
- Any live cell with more than 3 neighbors dies  
- Any dead cell with exactly 3 neighbors becomes alive  

---

# User Interface

- Qt Widgets-based interface
- Interactive simulation grid
- Dark-themed UI
- Real-time visual updates of the grid

---

# Technologies Used

- C++
- Qt Widgets
- JSON file handling
- Qt Creator

---

# Screenshots

## Main Window
![Main Window](screenshots/main.png)

## Running Simulation
![Simulation](screenshots/gameplay.png)

---

# Project Status

Completed as a personal learning project.