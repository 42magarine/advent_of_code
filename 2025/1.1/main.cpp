#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <utility>

using namespace std;

int main(int argc, char** argv) {
    (void) argc;

    ifstream file(argv[1]);
    string line;
    vector<pair<char, int>> input;

    while (getline(file, line)) {
        pair<char, int> rotation;
        rotation.first = line[0]; // 'L' or 'R'
        rotation.second = stoi(line.substr(1)); // number 0 - 99
        input.push_back(rotation);
    }

    file.close();

    int count = 0;
    int start = 50;
    for (auto rotation : input) {
        if (rotation.first == 'L') {
            start = (((start - rotation.second) % 100) + 100) % 100;
        }
        else if (rotation.first == 'R') {
            start = (((start + rotation.second) % 100) + 100) % 100;
        }
        else {
            cout << "error" << endl;
        }
        cout << start << " ";
        if (start == 0) {
            count++;
        }
    }

    std::cout<< "count: " << count << endl;
    return 0;
}
