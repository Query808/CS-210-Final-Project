#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <iomanip>
#include <limits>

using namespace std;

/*
  ItemTracker
  -----------
  Loads item data from CS210_Project_Three_Input_File.txt, builds a frequency table,
  writes frequency.dat once at start, and provides menu-driven queries.
*/
class ItemTracker {
private:
    map<string, int> freq_;                 // item -> count
    string inputFile_ = "CS210_Project_Three_Input_File.txt";
    string outputFile_ = "frequency.dat";

public:
    // Load records from the input file.
    bool loadData() {
        ifstream in(inputFile_);
        if (!in) return false;
        string token;
        while (in >> token) {
            ++freq_[token];
        }
        return true;
    }

    // Write the frequency table to the backup file.
    bool saveToFile() const {
        ofstream out(outputFile_);
        if (!out) return false;
        for (const auto& p : freq_) {
            out << p.first << ' ' << p.second << '\n';
        }
        return true;
    }

    // Allows lookup of a specific item from original list. Spelling must be exact.
    int getItemFrequency(const string& item) const {
        auto it = freq_.find(item);
        return (it == freq_.end()) ? 0 : it->second;
    }

    // Print "item count" list for items included in original file.
    void printFrequencyList() const {
        for (const auto& p : freq_) {
            cout << p.first << ' ' << p.second << '\n';
        }
    }

    // Print a text histogram for all items included in original file.
    void printHistogram() const {
        size_t maxLen = 0;
        for (const auto& p : freq_) {
            if (p.first.size() > maxLen) maxLen = p.first.size();
        }
        for (const auto& p : freq_) {
            cout << left << setw(static_cast<int>(maxLen) + 1) << p.first << ' ';
            for (int i = 0; i < p.second; ++i) cout << '*';
            cout << '\n';
        }
    }
};

// Read a validated menu choice in [1..4] and display the menu each time.
int readMenuChoice() {
    while (true) {
        cout << "\nMenu Options:\n"
            << "1. Check frequency of an item\n"
            << "2. Print frequency list\n"
            << "3. Print histogram\n"
            << "4. Exit\n"
            << "Enter your choice (1-4): ";
        int choice;
        if (cin >> choice && choice >= 1 && choice <= 4) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return choice;
        }
        cout << "Invalid input. Please enter a number 1-4.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

int main() {
    ItemTracker tracker;

    // Load once and create backup once at program start.
    if (!tracker.loadData()) {
        cout << "ERROR: Unable to open input file 'CS210_Project_Three_Input_File.txt'.\n"
            "Make sure the file is in the same folder as the program and try again.\n";
        return 1;
    }
    if (!tracker.saveToFile()) {
        cerr << "Warning: Could not write frequency.dat\n";
    }

    // Menu loop
    for (;;) {
        int choice = readMenuChoice();
        if (choice == 1) {
            cout << "Must use exact spelling and capitalization!\n Enter item to check:  ";
            string item;
            getline(cin >> ws, item);              // allow spaces in queries
            cout << tracker.getItemFrequency(item) << '\n';
        }
        else if (choice == 2) {
            tracker.printFrequencyList();
        }
        else if (choice == 3) {
            tracker.printHistogram();
        }
        else { // 4
            break;
        }
    }
    return 0;
}
