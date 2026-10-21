# Nutrition Goal Tracker (C++)

A console app for logging food and tracking daily progress toward calorie, protein, and fiber goals.
Inspired by my JavaFX Daily To-Do List Planner (goal + progress tracking), rebuilt in C++17.

## Features
- Set your own daily goals for calories, protein (g), and fiber (g)
- Log foods by meal (Breakfast, Lunch, Dinner, Snack)
- Progress bars with percent complete for each goal
- View and delete today's entries
- History of daily totals
- Data is saved to text files and reloaded on the next run

## Concepts used
- Object-oriented programming (`FoodEntry`, `Tracker` classes)
- STL containers and algorithms (`vector`, `map`, `remove_if`)
- File input/output and date handling (`<ctime>`)
- Input validation

## Build and run
Requires a C++17 compiler (g++ or clang++).

```bash
make
./nutritiontracker
```

On Windows without `make`:

```bash
g++ -std=c++17 -o nutritiontracker src/main.cpp src/FoodEntry.cpp src/Tracker.cpp
nutritiontracker.exe
```

## Project structure
```
src/
  main.cpp              menu, input, progress display
  FoodEntry.h / .cpp    one logged food
  Tracker.h / .cpp      goals, entries, totals, saving and loading
Makefile
```

## Note
Starter goals (2000 kcal, 50 g protein, 28 g fiber) are only general defaults. Set your own goals.
This app is a tracking tool, not medical or dietary advice.

## Author
Rahenuma Begum — Computer Science, Webster University
