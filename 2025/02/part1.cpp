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

// Extract first half of a number string
// Examples: "11" -> "1", "998" -> "9", "1010" -> "10"
string getRangeStart(const string& num) {
    if (num.size() == 1) {
        return num;
    }

    return num.substr(0, num.size() / 2);
}

// Extract first half of a number string
// Examples: "11" -> "1", "998" -> "99", "1010" -> "10"
string getRangeEnd(const string& num) {
    if (num.size() == 1) {
        return num;
    }

    return num.substr(0, (num.size() + 1) / 2);
}

// Generate all potential invalid IDs within a range
// Strategy: Extract first halves of start/end, iterate through that range,
// then double each number to create invalid IDs (e.g., 10 -> "1010")
vector<string> findInvalidCandidates(const IdRange& range) {
    vector<string> candidates;

    // Get the first halves to determine the search space
    string startHalf = getRangeStart(range.firstId);
    string endHalf = getRangeEnd(range.lastId);

    // Convert to numbers for iteration
    long long startNum = stoll(startHalf);
    long long endNum = stoll(endHalf);

    // Generate all invalid IDs by doubling each number in range
    for (long long i = startNum; i <= endNum; i++) {
        string half = to_string(i);
        candidates.push_back(half + half);
    }

    return candidates;
}

long long computeSumOfInvalidIds(const vector<IdRange>& ranges) {
    long long sum = 0;

    for (const auto& range : ranges) {
        long long start = stoll(range.firstId);
        long long end = stoll(range.lastId);

        vector<string> candidates = findInvalidCandidates(range);

        for (const string& candidate : candidates) {
            long long candidateNum = stoll(candidate);

            // Check if candidate is within the actual numeric range
            if (candidateNum >= start && candidateNum <= end) {
                cout << range.firstId << "-" << range.lastId
                     << " has invalid ID " << candidateNum << "." << endl;
                sum += candidateNum;
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
