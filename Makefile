CXX      = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2
SRC      = src/main.cpp src/FoodEntry.cpp src/Tracker.cpp

nutritiontracker: $(SRC) src/FoodEntry.h src/Tracker.h
	$(CXX) $(CXXFLAGS) -o nutritiontracker $(SRC)

clean:
	rm -f nutritiontracker
