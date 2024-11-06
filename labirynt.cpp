#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <ctime>

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
		int* ReadSizeFromFile(string fileName)
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
				int* size = new int[2];
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
		char** ReadLabirynthFromFile(string fileName, int horizontalSize, int verticalSize)
		{
			ifstream file;
			file.open(fileName.c_str());
			if (file.is_open())
			{
				char** labirynth = new char* [verticalSize];
				int iterator = 0;
				string line;
				while (getline(file, line))
				{
					char* lineOfLab = new char[line.length() + 1];
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
		char** writeLabirynthToFile(char** labirynth, int horizontalSize, int verticalSize)
		{
			ofstream file;
			fileName = "labirynthCopy.txt";
			file.open(fileName.c_str(), ios::trunc);
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
				return labirynth;
			}
			else
			{
				cout << "File not found" << endl;
				return NULL;
			}
		}
};

class Labirynth : File
{
	private:
		int verticalSize, horizontalSize, startX, startY, endX, endY;
		char **labirynth;
		vector<int*> path;

		int dx[4] = { 0, 0, -2, 2 };
		int dy[4] = { -2, 2, 0, 0 };

		// Sprawdza, czy komórka jest w granicach
		bool isInBounds(int x, int y) {
			return x > 0 && x < verticalSize - 1 && y > 0 && y < horizontalSize - 1;
		}

		// Funkcja generująca labirynt metodą DFS
		void generateMaze(int x, int y) {
			// Mieszamy kolejność kierunków, aby uzyskać losowość
			int directions[4] = { 0, 1, 2, 3 };
			shuffle(directions, directions + 4);

			for (int i = 0; i < 4; i++) {
				int nx = x + dx[directions[i]];
				int ny = y + dy[directions[i]];

				// Sprawdzenie, czy można przebić się w wybranym kierunku
				if (isInBounds(nx, ny) && labirynth[nx][ny] == 'x') {
					labirynth[nx][ny] = ' ';
					labirynth[x + dx[directions[i]] / 2][y + dy[directions[i]] / 2] = ' ';
					generateMaze(nx, ny);
				}
			}
		}
    public:
        Labirynth(int verticalSize, int horizontalSize, int startX, int startY, int endX, int endY)
        {
			this->verticalSize = verticalSize;
			this->horizontalSize = horizontalSize;
			this->startX = startX;
			this->startY = startY;
			this->endX = endX;
			this->endY = endY;
			labirynth = new char*[verticalSize];
			for (int i = 0; i < verticalSize; i++)
			{
				labirynth[i] = new char[horizontalSize];
				for (int j = 0; j < horizontalSize; j++)
				{
					labirynth[i][j] = 'o';
				}
			}
			for (int i = 0; i < verticalSize; i++)
			{
				labirynth[0][i] = 'x';
			}
			for (int i = 0; i < verticalSize; i++)
			{
				labirynth[verticalSize - 1][i] = 'x';
			}
			for (int i = 0; i < horizontalSize; i++)
			{
				labirynth[i][0] = 'x';
			}
			for (int i = 0; i < horizontalSize; i++)
			{
				labirynth[i][horizontalSize - 1] = 'x';
			}
			labirynth[startX][startY] = 'S';
			labirynth[endX][endY] = 'E';
			// tu dopisz swoj algorytm
		}
		Labirynth()
		{
			int* size = ReadSizeFromFile(getFileName());
			if (size != NULL)
			{
				verticalSize = size[0];
				horizontalSize = size[1];
				labirynth = ReadLabirynthFromFile(getFileName(), horizontalSize, verticalSize);
				for (int i = 0; i < verticalSize; i++)
				{
					for (int j = 0; j < horizontalSize; j++)
					{
						if (labirynth[i][j] == 'S')
						{
							startX = i;
							startY = j;
						}
						if (labirynth[i][j] == 'E')
						{
							endX = i;
							endY = j;
						}
					}
				}
			}
			else
			{
				cout << "File not found" << endl;
				exit(1);
			}
		}
		~Labirynth()
		{
		}
		int getVerticalSize()
		{
			return verticalSize;
		}
		int getHorizontalSize()
		{
			return horizontalSize;
		}
		int getStartX()
		{
			return startX;
		}
		int getStartY()
		{
			return startY;
		}
		int getEndX()
		{
			return endX;
		}
		int getEndY()
		{
			return endY;
		}
		char** getLabirynth()
		{
			return labirynth;
		}
		int setVerticalSize(int verticalSize)
		{
			this->verticalSize = verticalSize;
		}
		int setHorizontalSize(int horizontalSize)
		{
			this->horizontalSize = horizontalSize;
		}
		int setStartX(int startX)
		{
			this->startX = startX;
		}
		int setStartY(int startY)
		{
			this->startY = startY;
		}
		int setEndX(int endX)
		{
			this->endX = endX;
		}
		int setEndY(int endY)
		{
			this->endY = endY;
		}
		void setLabirynth(char** labirynth)
		{
			this->labirynth = labirynth;
		}
		void setWall(int x, int y)
		{
			labirynth[x][y] = 'x';
		}
		void setPath(int x, int y)
		{
			labirynth[x][y] = ' ';
		}
		void printLabirynth()
		{
			for (int i = 0; i < verticalSize; i++)
			{
				for (int j = 0; j < horizontalSize; j++)
				{
					cout << labirynth[i][j] << " ";
				}
				cout << endl;
			}
		}
		void makePath(int x, int y, int xz, int yz)
		{
			int currentPos[2] = { x, y };
			int previousPos[2] = { xz, yz };
			path.push_back(currentPos);
			if (currentPos[0] == endX && currentPos[1] == endY)
			{
				cout << "You are already at the end of the labirynth" << endl;
				char** drawnPath = new char*[verticalSize];
				for (int i = 0; i < verticalSize; i++)
				{
					drawnPath[i] = new char[horizontalSize];
					for (int j = 0; j < horizontalSize; j++)
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
				writeLabirynthToFile(drawnPath, horizontalSize, verticalSize);
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
			if (currentPos[0] + 1 < verticalSize)
			{
				if (labirynth[currentPos[0] + 1][currentPos[1]] == ' ' || labirynth[currentPos[0] + 1][currentPos[1]] == 'E')
				{
					if (currentPos[0] + 1 != previousPos[0])
					{
						makePath(currentPos[0] + 1, currentPos[1], currentPos[0], currentPos[1]);
					}
				}
			}
			if (currentPos[1] + 1 < horizontalSize)
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
};

int main()
{
	Labirynth lab = Labirynth(10, 10, 0, 1, 9, 8);
	lab.printLabirynth();
	//lab.makePath(lab.getStartX(), lab.getStartY(), lab.getStartX(), lab.getStartY());
	return 0;
}