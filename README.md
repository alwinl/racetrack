# Racetrack
As a child I used to play this game on square grid paper and always retained a love for it. I knew it then as a game where you controlled a sailing boat.
At a turn you will have a choice of the next position of the boat based on your last movement. If you take your last movement as a vector, the center target is that movement vector based at your current position. Your choices are the end of that vector or any of the eight positions around that point.
The playing field is a lake drawn on the paper with a start and finish line. You should not sail your boat into the bank of the lake.

## Overview

My intent developing **racetrack** is two-fold. I want to learn OpenGL and learn how to develop a game.

## Building

### Dependencies
- **C++20 or newer**
- **CMake 3.21+**
- **GLFW**
- **GLAD**
- **glm**

### Clone the repo
```bash
git clone http://github.com/alwinl/racetrack
cd racetrack
```

### Build
```bash
cmake -B .build
cmake --build .build
```

### Run
```bash
.build/src/racetrack
```
## Features

**racetrack** does not have any features yet

## Libraries used

### GLFW

**Rendering, input, window handling.**
My platform is Linux. Most distributions will have a GLFW development package that you can install. If not you can download and install the library from github:
https://github.com/glfw/glfw

### GLAD

**Low level OpenGL function access.** You can create the source using a generator. Instructions and samples are on github: https://github.com/Dav1dde/glad

### glm

**Header only utility library** for vector and matrix opererations. https://github.com/icaven/glm

## Roadmap
### ✔ Done
- Setting up the project
- Built a basic framework that displays a window

### 🔜 In Progress
- Hand roll a simple ECS

### 🚀 Future Ideas
- Convert to EnTT

## Contributing

Pull requests are welcome — especially for patterns, UX improvements, or new tools.

## License

**racetrack** is licensed under the GPL Licence, see [LICENSE.txt](LICENSE.txt) for more information.
