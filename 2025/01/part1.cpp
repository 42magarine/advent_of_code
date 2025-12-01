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
        int delta = (r.dir == 'L') ? -r.value : r.value;

        dial = (((dial + delta) % 100) + 100) % 100;

        if (dial == 0) {
            count++;
        }

        cout << "The dial is rotated " << r.dir << r.value
             << " to point at " << dial << "." << endl;
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
