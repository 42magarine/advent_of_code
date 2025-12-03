#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib> // exit

using namespace std;

// Parse input file where each line contains single digits (0-9)
vector<vector<int>> parseInput(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Could not open file: " << filename << endl;
        exit(1);
    }

    vector<vector<int>> grid;
    string line;

    while (getline(file, line)) {
        vector<int> row;

        for (char c : line) {
            // ASCII char to int conversion (implicit cast from char to int)
            row.push_back(c - '0');
        }
        grid.push_back(row);
    }

    return grid;
}

// Find position of the maximum value in the row (excluding the last position)
size_t posFirstBattery(const vector<int>& row) {
    int maxValue = 0;
    size_t maxPos = 0;

    for (size_t i = 0; i < row.size() - 1; i++) {
        if (row[i] > maxValue) {
            maxValue = row[i];
            maxPos = i;
        }
    }

    return maxPos;
}

// Find position of the maximum value from start + 1 to the end of the row
size_t posSecondBattery(const vector<int>& row, size_t start) {
    int maxValue = 0;
    size_t maxPos = start + 1;

    for (size_t i = start + 1; i < row.size(); i++) {
        if (row[i] > maxValue) {
            maxValue = row[i];
            maxPos = i;
        }
    }

    return maxPos;
}

// Calculate sum of voltages by combining first and second max digits
int sumOfVoltage(const vector<vector<int>>& grid) {
    const string boldOn = "\033[1m";
    const string boldOff = "\033[0m";

    int sum = 0;

    for (const auto& row : grid) {
        size_t firstPos = posFirstBattery(row);
        size_t secondPos = posSecondBattery(row, firstPos);

        // Print row with highlighted positions
        for (size_t i = 0; i < row.size(); i++) {
            if (i == firstPos || i == secondPos) {
                cout << boldOn << row[i] << boldOff;
            }
            else {
                cout << row[i];
            }
        }
        cout << endl;

        // Combine first and second digit to form a two-digit number
        sum += (row[firstPos] * 10 + row[secondPos]);
    }

    return sum;
}

int main(int argc, char** argv) {
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <input_file>" << endl;
        return 1;
    }

    vector<vector<int>> banksOfbatteries = parseInput(argv[1]);
    int result = sumOfVoltage(banksOfbatteries);

    cout << "Sum of the maximum voltage: " << result << endl;

    return 0;
}
