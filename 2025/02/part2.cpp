#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <cstdlib> // exit

using namespace std;

struct IdRange {
    string firstId;
    string lastId;
};

// Parse input file containing comma-separated ranges like "11-22,95-115,998-1012"
vector<IdRange> parseInput(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Could not open file: " << filename << endl;
        exit(1);
    }

    vector<IdRange> ranges;
    string line;

    if (getline(file, line)) {
        stringstream ss(line);
        string range;

        while (getline(ss, range, ',')) {
            size_t dashPos = range.find('-');
            if (dashPos != string::npos) {
                ranges.push_back({
                    range.substr(0, dashPos),
                    range.substr(dashPos + 1)
                });
            }
        }
    }

    return ranges;
}

// Check if a string consists of a repeating pattern
// Uses mathematical trick:
// S is repeating if S appears in (S+S) before position |S|
bool isRepeated(const string& idAsString) {
    if (idAsString.size() < 2) {
        return false;
    }

    string doubled = idAsString + idAsString;

    // Search for the original string starting from position 1
    size_t pos = doubled.find(idAsString, 1);

    // If we find it before position size(), it means there's a repeating pattern
    return pos < idAsString.size();
}

// Compute sum of all invalid IDs across all ranges
long long computeSumOfInvalidIds(const vector<IdRange>& ranges) {
    long long sum = 0;

    for (const auto& range : ranges) {
        long long start = stoll(range.firstId);
        long long end = stoll(range.lastId);

        // Check each ID in the range
        for (long long i = start; i <= end; i++) {
            if (isRepeated(to_string(i))) {
                cout << range.firstId << "-" << range.lastId
                     << " has invalid ID " << i << "." << endl;
                sum += i;
            }
        }
    }

    return sum;
}

int main(int argc, char** argv) {
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <input_file>" << endl;
        return 1;
    }

    vector<IdRange> ranges = parseInput(argv[1]);
    long long result = computeSumOfInvalidIds(ranges);

    cout << "Sum of all invalid IDs: " << result << endl;

    return 0;
}
