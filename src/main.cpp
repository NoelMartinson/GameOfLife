/*
Raylib example file.
This is an example main file for a simple raylib project.
Use this as a starting point or replace it with your code.

by Jeffery Myers is marked with CC0 1.0. To view a copy of this license, visit https://creativecommons.org/publicdomain/zero/1.0/

*/

#include "raylib.h"
#include "resource_dir.h"	// utility header for SearchAndSetResourceDir
#include <vector>

#include <ctime>


const int screenWidth = 800;
const int screenHeight = 800;

const int gridWidth = 64;
const int gridHeight = 64;

const float cellWidth = (float)screenWidth / (float)gridWidth;
const float cellHeight = (float)screenHeight / (float)gridHeight;

using Cells = std::vector<bool>;

bool ReadCell(int x, int y, const Cells& cells)
{
	if (x < 0 || x >= gridWidth || y < 0 || y >= gridHeight)
		return false;
	return cells[(y * gridWidth) + x];
}

void WriteCell(int x, int y, bool value, Cells& cells)
{
    if (x < 0) x = gridWidth - 1;
    else if (x >= gridWidth) x = 0;

    if (y < 0) y = gridHeight - 1;
    else if (y >= gridHeight) y = 0;

    cells[(y * gridWidth) + x] = value;
}

void RandomizeCells(Cells& cells)
{
	for (int i = 0; i < cells.size(); i++)
	{
		cells[i] = (GetRandomValue(0, 1) == 1);
	}
}

int CountLiveNeighbors(int x, int y, const Cells& cells)
{
	int count = 0;

	if (ReadCell(x - 1, y - 1, cells)) count++;
	if (ReadCell(x - 1, y + 0, cells)) count++;
	if (ReadCell(x - 1, y + 1, cells)) count++;
	if (ReadCell(x + 0, y - 1, cells)) count++;
	if (ReadCell(x + 0, y + 1, cells)) count++;
	if (ReadCell(x + 1, y - 1, cells)) count++;
	if (ReadCell(x + 1, y + 0, cells)) count++;
	if (ReadCell(x + 1, y + 1, cells)) count++;

	return count;
}

int main()
{
    SetRandomSeed((unsigned int)time(NULL));
    SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);
    InitWindow(screenWidth, screenHeight, "Game of Life");
    SearchAndSetResourceDir("resources");

    SetTargetFPS(60);

    Cells currentGeneration(gridWidth * gridHeight);
    Cells nextGeneration(gridWidth * gridHeight);

    RandomizeCells(currentGeneration);

    while (!WindowShouldClose())
    {
        std::fill(nextGeneration.begin(), nextGeneration.end(), false);

        if (GetKeyPressed() == KEY_SPACE) {
            RandomizeCells(currentGeneration);
        }

        for (int y = 1; y < gridHeight - 1; y++) {
            for (int x = 1; x < gridWidth - 1; x++) {

                int count = CountLiveNeighbors(x, y, currentGeneration);
                bool isAlive = ReadCell(x, y, currentGeneration);

                if (isAlive) {
                    if (count == 2 || count == 3) {
                        WriteCell(x, y, true, nextGeneration);
                    }
                }
                else {
                    if (count == 3) {
                        WriteCell(x, y, true, nextGeneration);
                    }
                }
            }
        }

        BeginDrawing();
        ClearBackground(BLACK);

        for (int y = 0; y < gridHeight; y++) {
            for (int x = 0; x < gridWidth; x++) {
                if (ReadCell(x, y, nextGeneration)) {
                    DrawRectangle(
                        (int)(x * cellWidth),
                        (int)(y * cellHeight),
                        (int)cellWidth,
                        (int)cellHeight,
                        RAYWHITE
                    );
                }
            }
        }

        DrawText("SPACE: Randomize", 10, 10, 20, GRAY);
        DrawFPS(10, 40);
        EndDrawing();

        currentGeneration = nextGeneration;
    }

    CloseWindow();
    return 0;
}
