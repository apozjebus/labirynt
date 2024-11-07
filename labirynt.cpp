#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <fstream>
#include <string>
#include <cstring>

using namespace std;

class File
{
private:
	string fileName = "labirynth.txt";

protected:
	File()
	{
	}
	File(string fileName)
	{
		this->fileName = fileName;
	}
	~File()
	{
	}
	string getFileName()
	{
		return fileName;
	}
	int *ReadSizeFromFile(string fileName)
	{
		ifstream file;
		file.open(fileName.c_str());
		if (file.is_open())
		{
			string line;
			int horizontalSize = 0;
			int verticalSize = 0;
			while (getline(file, line))
			{
				verticalSize++;
				if (horizontalSize < line.length())
				{
					horizontalSize = line.length();
				}
			}
			file.close();
			int *size = new int[2];
			size[0] = verticalSize;
			size[1] = horizontalSize;
			return size;
		}
		else
		{
			cout << "File not found" << endl;
			return NULL;
		}
	}
	char **ReadLabirynthFromFile(string fileName, int horizontalSize, int verticalSize)
	{
		ifstream file;
		file.open(fileName.c_str());
		if (file.is_open())
		{
			char **labirynth = new char *[verticalSize];
			int iterator = 0;
			string line;
			while (getline(file, line))
			{
				char *lineOfLab = new char[line.length() + 1];
				strcpy_s(lineOfLab, line.length() + 1, line.c_str());
				labirynth[iterator] = lineOfLab;
				iterator++;
			}
			file.close();
			return labirynth;
		}
		else
		{
			cout << "File not found" << endl;
			return NULL;
		}
	}
	void writeLabirynthToFile(char **labirynth, int horizontalSize, int verticalSize)
	{
		ofstream file;
		file.open("labirynthCopy.txt", ios::trunc);
		if (file.is_open())
		{
			for (int i = 0; i < verticalSize; i++)
			{
				for (int j = 0; j < horizontalSize; j++)
				{
					file << labirynth[i][j];
				}
				file << endl;
			}
			file.close();
		}
		else
		{
			cout << "File not found" << endl;
		}
	}
};

class Labirynth : File
{
private:
	const int moveX[4] = {1, 0, -1, 0};
	const int moveY[4] = {0, 1, 0, -1};
	int WIDTH;
	int HEIGHT;
	vector<vector<char>> labirynth;
	vector<int *> path;
	int start[2];
	int end[2];

	bool isValid(int x, int y)
	{
		return x > 0 && x < WIDTH - 1 && y > 0 && y < HEIGHT - 1 && labirynth[y][x] == 'x';
	}

	void generateMaze(int x, int y)
	{
		vector<int> dirs = {0, 1, 2, 3};
		random_shuffle(dirs.begin(), dirs.end());

		for (int i = 0; i < 4; i++)
		{
			int nx = x + moveX[dirs[i]] * 2;
			int ny = y + moveY[dirs[i]] * 2;

			if (isValid(nx, ny))
			{
				labirynth[ny][nx] = ' ';
				labirynth[y + moveY[dirs[i]]][x + moveX[dirs[i]]] = ' ';

				generateMaze(nx, ny);
			}
		}
	}

	void randomStartEndPoints()
	{
		bool set_start = false;
		while (!set_start)
		{
			int random = rand() % 2;
			if (random == 0)
			{
				start[0] = rand() % (HEIGHT - 1);
				start[1] = 0;
				if (labirynth[start[1] + 1][start[0]] != 'x')
				{
					labirynth[start[1]][start[0]] = 'S';
					set_start = true;
				}
			}
			else
			{
				start[0] = 0;
				start[1] = rand() % (WIDTH - 1);
				if (labirynth[start[1]][start[0] + 1] != 'x')
				{
					labirynth[start[1]][start[0]] = 'S';
					set_start = true;
				}
			}
		}

		bool set_end = false;
		while (!set_end)
		{
			int random = rand() % 2;
			if (random == 0)
			{
				end[0] = rand() % (HEIGHT - 1);
				end[1] = WIDTH - 1;
				if (labirynth[end[1]][end[0] - 1] != 'x')
				{
					labirynth[end[1]][end[0]] = 'E';
					set_end = true;
				}
			}
			else
			{
				end[0] = HEIGHT - 1;
				end[1] = rand() % (WIDTH - 1);
				if (labirynth[end[1]][end[0] - 1] != 'x')
				{
					labirynth[end[1]][end[0]] = 'E';
					set_end = true;
				}
			}
		}
	}

	void makePath(int x, int y, int xz, int yz)
	{
		int currentPos[2] = {x, y};
		int previousPos[2] = {xz, yz};
		path.push_back(currentPos);
		if (currentPos[0] == end[1] && currentPos[1] == end[0])
		{
			cout << "You are already at the end of the labirynth" << endl;
			char **drawnPath = new char *[HEIGHT];
			for (int i = 0; i < HEIGHT; i++)
			{
				drawnPath[i] = new char[WIDTH];
				for (int j = 0; j < WIDTH; j++)
				{
					drawnPath[i][j] = 'x';
				}
			}
			for (int i = 0; i < path.size(); i++)
			{
				if (i == 0)
				{
					drawnPath[path[i][0]][path[i][1]] = 'S';
				}
				else if (i == path.size() - 1)
				{
					drawnPath[path[i][0]][path[i][1]] = 'E';
				}
				else
				{
					drawnPath[path[i][0]][path[i][1]] = ' ';
				}
			}
			writeLabirynthToFile(drawnPath, WIDTH, HEIGHT);
		}
		if (currentPos[0] - 1 >= 0)
		{
			if (labirynth[currentPos[0] - 1][currentPos[1]] == ' ' || labirynth[currentPos[0] - 1][currentPos[1]] == 'E')
			{
				if (currentPos[0] - 1 != previousPos[0])
				{
					makePath(currentPos[0] - 1, currentPos[1], currentPos[0], currentPos[1]);
				}
			}
		}
		if (currentPos[1] - 1 >= 0)
		{
			if (labirynth[currentPos[0]][currentPos[1] - 1] == ' ' || labirynth[currentPos[0]][currentPos[1] - 1] == 'E')
			{
				if (currentPos[1] - 1 != previousPos[1])
				{
					makePath(currentPos[0], currentPos[1] - 1, currentPos[0], currentPos[1]);
				}
			}
		}
		if (currentPos[0] + 1 < HEIGHT)
		{
			if (labirynth[currentPos[0] + 1][currentPos[1]] == ' ' || labirynth[currentPos[0] + 1][currentPos[1]] == 'E')
			{
				if (currentPos[0] + 1 != previousPos[0])
				{
					makePath(currentPos[0] + 1, currentPos[1], currentPos[0], currentPos[1]);
				}
			}
		}
		if (currentPos[1] + 1 < WIDTH)
		{
			if (labirynth[currentPos[0]][currentPos[1] + 1] == ' ' || labirynth[currentPos[0]][currentPos[1] + 1] == 'E')
			{
				if (currentPos[1] + 1 != previousPos[1])
				{
					makePath(currentPos[0], currentPos[1] + 1, currentPos[0], currentPos[1]);
				}
			}
		}
		path.pop_back();
	}

public:
	void printLabirynth()
	{
		for (int y = 0; y < HEIGHT; y++)
		{
			for (int x = 0; x < WIDTH; x++)
			{
				cout << labirynth[y][x];
			}
			cout << endl;
		}
	}

	Labirynth(int WIDTH, int HEIGHT) : WIDTH(WIDTH), HEIGHT(HEIGHT)
	{
		for (int i = 0; i < HEIGHT; i++)
		{
			vector<char> row(WIDTH, 'x');
			labirynth.push_back(row);
		}
		labirynth[1][1] = ' ';
		generateMaze(1, 1);
		randomStartEndPoints();
		makePath(start[1], start[0], start[1], start[0]);
		for (int i = 0; i < path.size(); i++)
		{
			cout << path[i][0] << " " << path[i][1] << endl;
		}
	}
};

int main()
{
	srand(time(NULL));
	Labirynth labirynth(21, 21);
	labirynth.printLabirynth();
	return 0;
}
