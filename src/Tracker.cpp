#include "Tracker.h"
#include <algorithm>
#include <fstream>

Tracker::Tracker(const std::string& foodFile, const std::string& goalFile)
    : foodFile_(foodFile), goalFile_(goalFile) {}

void Tracker::load() {
    entries_.clear();
    nextId_ = 1;

    std::ifstream in(foodFile_);
    std::string line;
    while (std::getline(in, line)) {
        FoodEntry e(0, "", "", "", 0, 0, 0);
        if (FoodEntry::fromFileLine(line, e)) {
            entries_.push_back(e);
            nextId_ = std::max(nextId_, e.getId() + 1);
        }
    }

    std::ifstream gin(goalFile_);
    int c; double p, f;
    if (gin >> c >> p >> f) { goals_.calories = c; goals_.protein = p; goals_.fiber = f; }
}

void Tracker::save() const {
    std::ofstream out(foodFile_);
    for (const FoodEntry& e : entries_) out << e.toFileLine() << '\n';
    std::ofstream gout(goalFile_);
    gout << goals_.calories << ' ' << goals_.protein << ' ' << goals_.fiber << '\n';
}

void Tracker::addEntry(const std::string& date, const std::string& meal, const std::string& name,
                       int calories, double protein, double fiber) {
    entries_.emplace_back(nextId_++, date, meal, name, calories, protein, fiber);
}

bool Tracker::deleteEntry(int id) {
    auto it = std::remove_if(entries_.begin(), entries_.end(),
                             [id](const FoodEntry& e) { return e.getId() == id; });
    if (it == entries_.end()) return false;
    entries_.erase(it, entries_.end());
    return true;
}

std::vector<FoodEntry> Tracker::entriesFor(const std::string& date) const {
    std::vector<FoodEntry> result;
    for (const FoodEntry& e : entries_)
        if (e.getDate() == date) result.push_back(e);
    return result;
}

Totals Tracker::totalsFor(const std::string& date) const {
    Totals t;
    for (const FoodEntry& e : entries_) {
        if (e.getDate() != date) continue;
        t.calories += e.getCalories();
        t.protein += e.getProtein();
        t.fiber += e.getFiber();
    }
    return t;
}

std::map<std::string, Totals> Tracker::allDailyTotals() const {
    std::map<std::string, Totals> days;
    for (const FoodEntry& e : entries_) {
        Totals& t = days[e.getDate()];
        t.calories += e.getCalories();
        t.protein += e.getProtein();
        t.fiber += e.getFiber();
    }
    return days;
}
