#ifndef FOODENTRY_H
#define FOODENTRY_H

#include <string>

// One logged food item.
class FoodEntry {
public:
    FoodEntry(int id, const std::string& date, const std::string& meal,
              const std::string& name, int calories, double protein, double fiber);

    int getId() const { return id_; }
    const std::string& getDate() const { return date_; }   // YYYY-MM-DD
    const std::string& getMeal() const { return meal_; }
    const std::string& getName() const { return name_; }
    int getCalories() const { return calories_; }
    double getProtein() const { return protein_; }          // grams
    double getFiber() const { return fiber_; }              // grams

    std::string toFileLine() const;
    static bool fromFileLine(const std::string& line, FoodEntry& out);

private:
    int id_;
    std::string date_, meal_, name_;
    int calories_;
    double protein_, fiber_;
};

#endif
