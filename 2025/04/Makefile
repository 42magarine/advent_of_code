CPP			= g++
CPPFLAGS	= -Wall -Wextra -Werror -std=c++17

AOC1		= aoc1
AOC2		= aoc2

SRC1		= part1.cpp
SRC2		= part2.cpp

all: $(AOC1) $(AOC2)

$(AOC1): $(SRC1)
	$(CPP) $(CPPFLAGS) $(SRC1) -o $(AOC1)

$(AOC2): $(SRC2)
	$(CPP) $(CPPFLAGS) $(SRC2) -o $(AOC2)

clean:
	rm -f $(AOC1) $(AOC2)
