#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib> // exit

using namespace std;

struct Rotation {
    char dir;
    int value;
};

vector<Rotation> parseInput(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Could not open file: " << filename << endl;
        exit(1);
    }

    vector<Rotation> input;
    string line;

    while (getline(file, line)) {
        Rotation r;
        r.dir = line[0];
        r.value = stoi(line.substr(1));
        input.push_back(r);
    }

    return input;
}

int computePassword(const vector<Rotation>& input) {
    int dial = 50;
    int count = 0;

    cout << "The dial starts by pointing at " << dial << "." << endl;

    for (const auto& r : input) {
        int pointAtZero = 0;

        if (r.dir == 'L') {
            // Calculate how many full -100 cycles we complete
            pointAtZero = abs((dial - r.value) / 100);

            // Check if we cross or land on 0 during the rotation
            // Don't count if we start at 0 (dial != 0)
            // Count if the result is 0 or negative (dial - r.value <= 0)
            if (dial != 0 && dial - r.value <= 0) {
                pointAtZero++;
            }

            // Normalize dial position to range 0-99
            // Double modulo ensures positive result even for negative numbers
            dial = (((dial - r.value) % 100) + 100) % 100;
        }
        else if (r.dir == 'R') {
            // Calculate how many full 100 cycles we complete
            pointAtZero = (dial + r.value) / 100;

            // Normalize dial position to range 0-99
            dial = (dial + r.value) % 100;
        }

        count += pointAtZero;

        cout << "The dial is rotated " << r.dir << r.value
             << " to point at " << dial
             << "; during this rotation, it points at 0 " << pointAtZero
             << "-times." << endl;
    }

    return count;
}


int main(int argc, char** argv) {
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <input_file>" << endl;
        return 1;
    }

    vector<Rotation> input = parseInput(argv[1]);
    int password = computePassword(input);

    cout << "Password: " << password << endl;

    return 0;
}
