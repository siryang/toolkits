#include "gtest/gtest.h"
#include "stdio.h"
extern "C"
{
#include "maze_escape.h"
}

/* Enter your code here. Read input from STDIN. Print output to STDOUT */
const static char* g_grid[10] = {
	"#######",
	"#--#b-#",
	"#--#--#",
	"#--#--#",
	"e-----#",
	"#-----#",
	"#######",
};

void getInput(const char* grid[10], char adjency[3][3], int c, int r, Direction dir)
{
	const static Step steps[] = {
		{-1, 1, 1, -1}, // left
		{-1, 1, -1, 1}, // top
		{1, -1, -1, 1}, // right
		{1, -1, 1, -1}, // bottom
	};

	if(dir == left || dir == right)
	{
		const Step& step = steps[dir];
		for (int y = 0, xOffset = step.xStart
			; y <= 2; xOffset += step.xStep, y++)
		{
			for (int x = 0, yOffset = step.yStart
				; x <= 2; yOffset += step.yStep, x++)
			{
				adjency[y][x] = grid[c + yOffset][r + xOffset];
				if (adjency[y][x] == 'b')
					adjency[y][x] = '-';
			}
		}
	}
	else if (dir == top || dir == bottom)
	{
		const Step& step = steps[dir];
		for (int yOffset = step.yStart, y = 0
			; y <= 2; yOffset += step.yStep, y++)
		{
			for (int xOffset = step.xStart, x = 0
				; x <= 2; xOffset += step.xStep, x++)
			{
				adjency[y][x] = grid[c + yOffset][r + xOffset];
				if (adjency[y][x] == 'b')
					adjency[y][x] = '-';
			}
		}
	}
}

TEST(MazeEscape, testInput)
{
	char adjency[3][3];
	getInput(g_grid, adjency, 1, 5, left);
	EXPECT_EQ(strncmp(adjency[0], "--#", 3), 0);
	EXPECT_EQ(strncmp(adjency[1], "--#", 3), 0);
	EXPECT_EQ(strncmp(adjency[2], "###", 3), 0);

	getInput(g_grid, adjency, 1, 5, top);
	EXPECT_EQ(strncmp(adjency[0], "###", 3), 0);
	EXPECT_EQ(strncmp(adjency[1], "--#", 3), 0);
	EXPECT_EQ(strncmp(adjency[2], "--#", 3), 0);

	getInput(g_grid, adjency, 1, 5, right);
	EXPECT_EQ(strncmp(adjency[0], "###", 3), 0);
	EXPECT_EQ(strncmp(adjency[1], "#--", 3), 0);
	EXPECT_EQ(strncmp(adjency[2], "#--", 3), 0);

	getInput(g_grid, adjency, 1, 5, bottom);
	EXPECT_EQ(strncmp(adjency[0], "#--", 3), 0);
	EXPECT_EQ(strncmp(adjency[1], "#--", 3), 0);
	EXPECT_EQ(strncmp(adjency[2], "###", 3), 0);

}

void getNewPosition(Direction dir, int& x, int& y)
{
	switch (dir)
	{
	case left:
		x -= 1; break;
	case top:
		y -= 1; break;
	case right:
		x += 1; break;
	case bottom:
		y += 1; break;
	}
}

void showAjency(char adj[3][3])
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
			printf("%c", adj[i][j]);
		printf("\n");
	}

}

TEST(MazeEscape, test) {
	char adjency[3][3];
	int x = 4, y = 1;
	Direction currentDir = right;
	while (g_grid[y][x] != 'e')
	{
		getInput(g_grid, adjency, y, x, currentDir);
		showAjency(adjency);
		Direction newDir = mazeEscape(adjency);
		currentDir = (Direction)((currentDir + newDir - top + 4) % 4);
		getNewPosition(currentDir, x, y);
	}
}
