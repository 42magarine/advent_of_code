#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <array>
#include <cstdlib> // exit

using namespace std;

constexpr int NUMBER_OF_BATTERIES = 12;

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

// Find position of maximum value in the specified range [begin, end] (inclusive)
size_t posBattery(const vector<int>& row, size_t begin, size_t end) {
    int maxValue = 0;
    size_t maxPos = begin;

    for (size_t i = begin; i <= end; i++) {
        if (row[i] > maxValue) {
            maxValue = row[i];
            maxPos = i;
        }
    }

    return maxPos;
}

// Calculate sum of voltages by greedily selecting the maximum digits
// Strategy: For each of N batteries, find the max digit in a sliding window
// that ensures enough digits remain for subsequent batteries
long long sumOfVoltage(const vector<vector<int>>& grid) {
    long long sum = 0;

    for (const auto& row : grid) {
        array<size_t, NUMBER_OF_BATTERIES> batteryPositions;
        long long voltage = 0;

        size_t begin = 0;
        // Initial search window: can't go past (row.size - NUMBER_OF_BATTERIES)
        // to leave room for remaining batteries
        size_t end = row.size() - NUMBER_OF_BATTERIES;

        for (int i = 0; i < NUMBER_OF_BATTERIES; i++) {
            // Find max in current window [begin, end]
            batteryPositions[i] = posBattery(row, begin, end);

            // Next search starts after current max position
            begin = batteryPositions[i] + 1;

            // Window expands by 1 since we need one fewer battery now
            end++;

            // Build voltage by appending current digit
            voltage = voltage * 10 + row[batteryPositions[i]];
        }

        sum += voltage;
    }

    return sum;
}

int main(int argc, char** argv) {
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <input_file>" << endl;
        return 1;
    }

    vector<vector<int>> banksOfbatteries = parseInput(argv[1]);
    long long result = sumOfVoltage(banksOfbatteries);

    cout << "Sum of the maximum voltage: " << result << endl;

    return 0;
}
