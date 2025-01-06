#ifndef LABIRYNTH_H
#define LABIRYNTH_H

#include <vector>
#include <algorithm>
#include <ranges>
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <random>
#include <fstream>

class Labirynth {
private:
	const int moveX[4] = {1, 0, -1, 0};
	const int moveY[4] = {0, 1, 0, -1};
	int WIDTH;
	int HEIGHT;
	std::vector<std::vector<char>> labirynth{};
	std::vector<std::vector<char>> drawnPath{};
	std::vector<int *> path{};
	int start[2]{};
	int end[2]{};

	bool isValid(int x, int y) const;
	void generateMaze(int x, int y);
	void randomStartEndPoints();
	void makePath(int x, int y, int xz, int yz);

public:
	Labirynth(int WIDTH, int HEIGHT);
	void printLabirynth() const;
	void saveLabirynth(const std::string &filename) const;
	void readLabirynth(const std::string &filename);
	void solveLabirynth();
};


#endif //LABIRYNTH_H