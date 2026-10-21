#include <algorithm>
#include <cctype>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

struct Signal {
    string title, category;
    int impact, urgency, confidence;
    string nextStep;
};

string trim(string s) {
    while (!s.empty() && isspace(static_cast<unsigned char>(s.front()))) s.erase(s.begin());
    while (!s.empty() && isspace(static_cast<unsigned char>(s.back()))) s.pop_back();
    return s;
}
string lower(string s) {
    transform(s.begin(), s.end(), s.begin(), [](unsigned char c) {
        return static_cast<char>(tolower(c));
    });
    return s;
}
int score(const Signal& s) { return s.impact * 9 + s.urgency * 8 + s.confidence * 3; }
string priority(const Signal& s) {
    int n = score(s);
    if (n >= 75) return "CRITICAL";
    if (n >= 58) return "HIGH";
    if (n >= 40) return "MEDIUM";
    return "LOW";
}
string safeCell(string s) {
    replace(s.begin(), s.end(), '|', '/');
    replace(s.begin(), s.end(), '\n', ' ');
    replace(s.begin(), s.end(), '\r', ' ');
    return s;
}
bool save(const vector<Signal>& items, const string& file = "signals.csv") {
    ofstream out(file);
    if (!out) return false;
    for (const auto& s : items)
        out << safeCell(s.title) << '|' << safeCell(s.category) << '|'
            << s.impact << '|' << s.urgency << '|' << s.confidence << '|'
            << safeCell(s.nextStep) << '\n';
    return static_cast<bool>(out);
}
vector<Signal> load(const string& file = "signals.csv") {
    vector<Signal> items;
    ifstream in(file);
    string line;
    while (getline(in, line)) {
        stringstream ss(line);
        vector<string> p;
        string part;
        while (getline(ss, part, '|')) p.push_back(part);
        if (p.size() != 6) continue;
        try {
            Signal s{p[0], p[1], stoi(p[2]), stoi(p[3]), stoi(p[4]), p[5]};
            if (!s.title.empty() && s.impact >= 1 && s.impact <= 5 &&
                s.urgency >= 1 && s.urgency <= 5 &&
                s.confidence >= 1 && s.confidence <= 5) items.push_back(s);
        } catch (...) { }
    }
    return items;
}
int askNumber(const string& prompt, int low, int high) {
    while (true) {
        cout << prompt;
        string line;
        getline(cin, line);
        stringstream ss(line);
        int value; char extra;
        if ((ss >> value) && !(ss >> extra) && value >= low && value <= high) return value;
        cout << "Enter a whole number from " << low << " to " << high << ".\n";
    }
}
string askText(const string& prompt) {
    while (true) {
        cout << prompt;
        string s; getline(cin, s); s = trim(s);
        if (!s.empty()) return safeCell(s);
        cout << "This field cannot be empty.\n";
    }
}
void separator() { cout << string(78, '-') << '\n'; }
void dashboard(const vector<Signal>& items) {
    int critical = 0, high = 0, medium = 0, low = 0;
    for (const auto& s : items) {
        string p = priority(s);
        if (p == "CRITICAL") ++critical;
        else if (p == "HIGH") ++high;
        else if (p == "MEDIUM") ++medium;
        else ++low;
    }
    cout << "\n"; separator(); cout << " SIGNALDESK / DECISION DASHBOARD\n"; separator();
    cout << " Signals tracked : " << items.size() << "\n Critical        : " << critical
         << "\n High            : " << high << "\n Medium          : " << medium
         << "\n Low             : " << low << '\n';
    if (items.empty()) cout << "\nNo signals yet. Add one or load demo data.\n";
    else {
        auto ranked = items;
        stable_sort(ranked.begin(), ranked.end(), [](const Signal& a, const Signal& b) {
            return score(a) > score(b);
        });
        cout << "\n TOP RECOMMENDED ACTION\n"; separator();
        cout << ' ' << ranked.front().title << " [" << priority(ranked.front()) << "]\n"
             << " Next step: " << ranked.front().nextStep << "\n Score: "
             << score(ranked.front()) << "/100\n";
    }
    separator();
}
void listSignals(vector<Signal> items) {
    stable_sort(items.begin(), items.end(), [](const Signal& a, const Signal& b) {
        return score(a) > score(b);
    });
    cout << "\n PRIORITIZED SIGNALS\n"; separator();
    if (items.empty()) { cout << "No signals to display.\n"; return; }
    cout << left << setw(4) << "#" << setw(26) << "SIGNAL" << setw(15) << "CATEGORY"
         << setw(12) << "PRIORITY" << setw(8) << "SCORE" << "NEXT STEP\n";
    separator();
    for (size_t i = 0; i < items.size(); ++i)
        cout << left << setw(4) << i + 1 << setw(26) << items[i].title.substr(0, 24)
             << setw(15) << items[i].category.substr(0, 13) << setw(12) << priority(items[i])
             << setw(8) << score(items[i]) << items[i].nextStep.substr(0, 45) << '\n';
    separator();
}
void addSignal(vector<Signal>& items) {
    cout << "\n NEW SIGNAL\n"; separator();
    Signal s;
    s.title = askText("Title: ");
    s.category = askText("Category (study / career / project / personal): ");
    s.impact = askNumber("Impact (1 low - 5 high): ", 1, 5);
    s.urgency = askNumber("Urgency (1 low - 5 high): ", 1, 5);
    s.confidence = askNumber("Confidence (1-5): ", 1, 5);
    s.nextStep = askText("Smallest useful next step: ");
    items.push_back(s);
    cout << "Added. Score: " << score(s) << "/100 (" << priority(s) << ").\n";
}
void searchSignals(const vector<Signal>& items) {
    string q = lower(askText("Search title, category, or next step: "));
    int found = 0; cout << "\n SEARCH RESULTS\n"; separator();
    for (const auto& s : items)
        if (lower(s.title).find(q) != string::npos || lower(s.category).find(q) != string::npos ||
            lower(s.nextStep).find(q) != string::npos) {
            cout << "- " << s.title << " [" << priority(s) << ", score " << score(s) << "]\n"
                 << "  Category: " << s.category << "\n  Next step: " << s.nextStep << '\n';
            ++found;
        }
    if (!found) cout << "No matching signals found.\n";
    separator();
}
void demo(vector<Signal>& items) {
    items = {
        {"Finish C++ mini project", "Project", 5, 5, 5, "Build the smallest working version"},
        {"Apply for internship", "Career", 5, 4, 4, "Tailor resume to one role and apply"},
        {"Revise pointers", "Study", 4, 4, 5, "Solve three pointer exercises"},
        {"Polish GitHub README", "Career", 3, 2, 5, "Add setup steps and one screenshot"},
        {"Prepare club presentation", "Project", 4, 3, 3, "Draft the first five slides"}
    };
    cout << "Demo signals loaded. Sample data only; no external services used.\n";
}
void exportReport(const vector<Signal>& items) {
    auto ranked = items;
    stable_sort(ranked.begin(), ranked.end(), [](const Signal& a, const Signal& b) {
        return score(a) > score(b);
    });
    ofstream out("signal_report.txt");
    if (!out) { cout << "Could not create report. Check folder permissions.\n"; return; }
    out << "SIGNALDESK - PRIORITY REPORT\n============================\n\n"
        << "Transparent heuristic, not an AI prediction.\n\n";
    for (size_t i = 0; i < ranked.size(); ++i)
        out << i + 1 << ". " << ranked[i].title << " [" << priority(ranked[i]) << "]\n"
            << "   Category: " << ranked[i].category << "\n   Score: " << score(ranked[i])
            << "/100\n   Impact: " << ranked[i].impact << "/5, urgency: " << ranked[i].urgency
            << "/5, confidence: " << ranked[i].confidence << "/5\n   Next step: "
            << ranked[i].nextStep << "\n\n";
    cout << "Exported " << ranked.size() << " signal(s) to signal_report.txt.\n";
}
int main(int argc, char* argv[]) {
    vector<Signal> items;
    if (argc > 1 && string(argv[1]) == "--demo") {
        demo(items); dashboard(items); listSignals(items); return 0;
    }
    items = load();
    separator();
    cout << " SIGNALDESK | TURN SCATTERED TASKS INTO CLEAR NEXT STEPS\n"
         << " Offline C++17 decision-support tool\n";
    separator();
    while (true) {
        cout << "\n 1. Dashboard\n 2. View ranked signals\n 3. Add signal\n"
             << " 4. Search signals\n 5. Save signals\n 6. Load saved signals\n"
             << " 7. Export text report\n 8. Load demo data\n 9. Exit\n";
        int choice = askNumber(" Select an option: ", 1, 9);
        switch (choice) {
            case 1: dashboard(items); break;
            case 2: listSignals(items); break;
            case 3: addSignal(items); break;
            case 4: searchSignals(items); break;
            case 5: cout << (save(items) ? "Saved to signals.csv.\n" : "Save failed.\n"); break;
            case 6: items = load(); cout << "Loaded " << items.size() << " signal(s).\n"; break;
            case 7: exportReport(items); break;
            case 8: demo(items); break;
            case 9: cout << "SignalDesk closed. Make the next step small and clear.\n"; return 0;
        }
    }
}
