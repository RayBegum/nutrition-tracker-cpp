// Nutrition Goal Tracker (C++)
// Log foods and see daily progress toward calorie, protein, and fiber goals.

#include "Tracker.h"
#include <cctype>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <string>

namespace {

std::string todayDate() {
    std::time_t now = std::time(nullptr);
    char buf[11];
    std::strftime(buf, sizeof(buf), "%Y-%m-%d", std::localtime(&now));
    return buf;
}

bool readLine(const std::string& prompt, std::string& out) {
    std::cout << prompt;
    return static_cast<bool>(std::getline(std::cin, out));
}

// Reads a number between lo and hi (inclusive). Returns false if input ends.
bool readNumber(const std::string& prompt, double lo, double hi, double& value) {
    std::string s;
    while (readLine(prompt, s)) {
        try {
            std::size_t pos = 0;
            double v = std::stod(s, &pos);
            if (pos == s.size() && v >= lo && v <= hi) { value = v; return true; }
        } catch (...) {}
        std::cout << std::defaultfloat << "  Please enter a number from " << lo << " to " << hi << ".\n";
    }
    return false;
}

bool hasLetter(const std::string& s) {
    for (unsigned char c : s) if (std::isalpha(c)) return true;
    return false;
}

std::string bar(double value, double goal) {
    const int width = 20;
    double pct = goal <= 0 ? 0 : value / goal;
    int filled = static_cast<int>(std::min(pct, 1.0) * width);
    return "[" + std::string(filled, '#') + std::string(width - filled, '-') + "]";
}

void showProgressLine(const std::string& label, double value, double goal, const std::string& unit, int digits) {
    double pct = goal <= 0 ? 0 : value / goal * 100.0;
    std::cout << std::left << std::setw(10) << label << bar(value, goal) << " "
              << std::right << std::fixed << std::setprecision(1) << std::setw(6) << pct << "%   "
              << std::setprecision(digits) << value << " / " << goal << " " << unit << "\n";
}

void showMenu() {
    std::cout << "\n=== Nutrition Goal Tracker ===\n"
              << " 1. Today's progress\n"
              << " 2. Log food\n"
              << " 3. View today's food log\n"
              << " 4. Delete a food entry\n"
              << " 5. Set daily goals\n"
              << " 6. History (daily totals)\n"
              << " 7. Save and exit\n";
}

void showProgress(const Tracker& t, const std::string& today) {
    Totals tot = t.totalsFor(today);
    const Goals& g = t.goals();
    std::cout << "\nProgress for " << today << "\n\n";
    showProgressLine("Calories", tot.calories, g.calories, "kcal", 0);
    showProgressLine("Protein", tot.protein, g.protein, "g", 1);
    showProgressLine("Fiber", tot.fiber, g.fiber, "g", 1);
}

void logFood(Tracker& t, const std::string& today) {
    std::string name;
    if (!readLine("Food name: ", name)) return;
    if (!hasLetter(name)) { std::cout << "  Please include at least one letter in the name.\n"; return; }
    if (name.find('|') != std::string::npos) { std::cout << "  Please do not use the | character.\n"; return; }

    double meal = 0, cal = 0, pro = 0, fib = 0;
    std::cout << "Meal: 1) Breakfast  2) Lunch  3) Dinner  4) Snack\n";
    if (!readNumber("Choose meal (1-4): ", 1, 4, meal)) return;
    if (!readNumber("Calories (kcal): ", 0, 10000, cal)) return;
    if (!readNumber("Protein (g): ", 0, 1000, pro)) return;
    if (!readNumber("Fiber (g): ", 0, 1000, fib)) return;

    static const char* meals[] = {"", "Breakfast", "Lunch", "Dinner", "Snack"};
    t.addEntry(today, meals[static_cast<int>(meal)], name, static_cast<int>(cal), pro, fib);
    std::cout << "Logged.\n";
}

void showLog(const Tracker& t, const std::string& today) {
    auto entries = t.entriesFor(today);
    if (entries.empty()) { std::cout << "\nNothing logged today yet.\n"; return; }
    std::cout << "\n" << std::left << std::setw(4) << "ID" << std::setw(11) << "Meal"
              << std::setw(24) << "Food" << std::setw(7) << "kcal" << std::setw(10) << "Protein"
              << "Fiber\n" << std::string(62, '-') << "\n";
    for (const FoodEntry& e : entries) {
        std::cout << std::left << std::setw(4) << e.getId() << std::setw(11) << e.getMeal()
                  << std::setw(24) << e.getName().substr(0, 22) << std::setw(7) << e.getCalories()
                  << std::fixed << std::setprecision(1) << std::setw(10) << e.getProtein()
                  << e.getFiber() << "\n";
    }
}

void setGoals(Tracker& t) {
    double c = 0, p = 0, f = 0;
    std::cout << "\nEnter your daily goals (these starter values are only defaults).\n";
    if (!readNumber("Calories goal (kcal): ", 1, 10000, c)) return;
    if (!readNumber("Protein goal (g): ", 1, 1000, p)) return;
    if (!readNumber("Fiber goal (g): ", 1, 1000, f)) return;
    t.goals().calories = static_cast<int>(c);
    t.goals().protein = p;
    t.goals().fiber = f;
    std::cout << "Goals updated.\n";
}

void showHistory(const Tracker& t) {
    auto days = t.allDailyTotals();
    if (days.empty()) { std::cout << "\nNo history yet.\n"; return; }
    std::cout << "\n" << std::left << std::setw(12) << "Date" << std::setw(8) << "kcal"
              << std::setw(11) << "Protein" << "Fiber\n" << std::string(40, '-') << "\n";
    for (const auto& [date, tot] : days) {
        std::cout << std::left << std::setw(12) << date << std::setw(8) << tot.calories
                  << std::fixed << std::setprecision(1) << std::setw(11) << tot.protein
                  << tot.fiber << "\n";
    }
}

}  // namespace

int main() {
    Tracker tracker("foods.txt", "goals.txt");
    tracker.load();
    const std::string today = todayDate();

    while (true) {
        showMenu();
        double choice = 0;
        if (!readNumber("Choose an option: ", 1, 7, choice)) break;
        switch (static_cast<int>(choice)) {
            case 1: showProgress(tracker, today); break;
            case 2: logFood(tracker, today); break;
            case 3: showLog(tracker, today); break;
            case 4: {
                double id = 0;
                if (!readNumber("Entry ID to delete: ", 1, 1e9, id)) break;
                std::cout << (tracker.deleteEntry(static_cast<int>(id)) ? "Deleted.\n" : "No entry with that ID.\n");
                break;
            }
            case 5: setGoals(tracker); break;
            case 6: showHistory(tracker); break;
            default: tracker.save(); std::cout << "Saved. Goodbye!\n"; return 0;
        }
    }
    tracker.save();
    std::cout << "Saved. Goodbye!\n";
    return 0;
}
