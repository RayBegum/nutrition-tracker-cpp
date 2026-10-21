#include "FoodEntry.h"
#include <sstream>

FoodEntry::FoodEntry(int id, const std::string& date, const std::string& meal,
                     const std::string& name, int calories, double protein, double fiber)
    : id_(id), date_(date), meal_(meal), name_(name),
      calories_(calories), protein_(protein), fiber_(fiber) {}

// File format: id|date|meal|calories|protein|fiber|name
std::string FoodEntry::toFileLine() const {
    std::ostringstream out;
    out << id_ << '|' << date_ << '|' << meal_ << '|' << calories_ << '|'
        << protein_ << '|' << fiber_ << '|' << name_;
    return out.str();
}

bool FoodEntry::fromFileLine(const std::string& line, FoodEntry& out) {
    std::istringstream in(line);
    std::string id, date, meal, cal, pro, fib, name;
    if (!std::getline(in, id, '|') || !std::getline(in, date, '|') ||
        !std::getline(in, meal, '|') || !std::getline(in, cal, '|') ||
        !std::getline(in, pro, '|') || !std::getline(in, fib, '|') ||
        !std::getline(in, name)) {
        return false;
    }
    try {
        out = FoodEntry(std::stoi(id), date, meal, name,
                        std::stoi(cal), std::stod(pro), std::stod(fib));
    } catch (...) {
        return false;
    }
    return true;
}
