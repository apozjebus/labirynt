#include "labirynth.h"

bool Labirynth::isValid(const int x, const int y) const {
    return x > 0 && x < WIDTH - 1 && y > 0 && y < HEIGHT - 1 && labirynth[y][x] == 'x';
}

void Labirynth::generateMaze(const int x, const int y) {
    std::vector<int> dirs = {0, 1, 2, 3};
    std::ranges::shuffle(dirs, std::mt19937(std::random_device()()));

    for (int i = 0; i < 4; i++)
    {
        const int nx = x + moveX[dirs[i]] * 2;
        const int ny = y + moveY[dirs[i]] * 2;

        if (isValid(nx, ny))
        {
            labirynth[ny][nx] = ' ';
            labirynth[y + moveY[dirs[i]]][x + moveX[dirs[i]]] = ' ';

            generateMaze(nx, ny);
        }
    }
}

void Labirynth::randomStartEndPoints() {
    bool set_start = false;
    while (!set_start)
    {
        if (const int random = rand() % 2; random == 0)
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
        if (const int random = rand() % 2; random == 0)
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

void Labirynth::makePath(const int x, const int y, const int xz, const int yz) {
    int currentPos[2] = {x, y};
    const int previousPos[2] = {xz, yz};
    path.push_back(currentPos);
    if (currentPos[0] == end[1] && currentPos[1] == end[0])
    {
        for (int i = 0; i < HEIGHT; i++)
        {
            std::vector<char> row(WIDTH, 'x');
            drawnPath.push_back(row);
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
        saveLabirynth("solved_labirynth.txt");
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

void Labirynth::printLabirynth() const {
    for (int y = 0; y < HEIGHT; y++)
    {
        for (int x = 0; x < WIDTH; x++)
        {
            std::cout << labirynth[y][x];
        }
        std::cout << std::endl;
    }
}

Labirynth::Labirynth(const int WIDTH, const int HEIGHT) : WIDTH(WIDTH), HEIGHT(HEIGHT) {
    for (int i = 0; i < HEIGHT; i++)
    {
        std::vector<char> row(WIDTH, 'x');
        labirynth.push_back(row);
    }
    labirynth[1][1] = ' ';
    generateMaze(1, 1);
    randomStartEndPoints();
}

void Labirynth::saveLabirynth(const std::string &filename) const {
    std::ofstream file(filename);
    if (file.is_open())
    {
        for (int y = 0; y < HEIGHT; y++)
        {
            for (int x = 0; x < WIDTH; x++)
            {
                file << drawnPath[y][x];
            }
            file << std::endl;
        }
        file.close();
    }
}

void Labirynth::readLabirynth(const std::string &filename) {
    std::ifstream file(filename);
    if (file.is_open())
    {
        std::string line;
        int y = 0;
        while (std::getline(file, line))
        {
            for (int x = 0; x < WIDTH; x++)
            {
                labirynth[y][x] = line[x];
            }
            y++;
        }
        file.close();
    }
}

void Labirynth::solveLabirynth() {
    makePath(start[1], start[0], start[1], start[0]);
}