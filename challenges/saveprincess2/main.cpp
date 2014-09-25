// url https://www.hackerrank.com/challenges/saveprincess2
/**
In this version of "Bot saves princess", Princess Peach and bot's position are randomly set. Can you save the princess?

Task

Complete the function nextMove which takes in 4 parameters - an integer N, integers r and c indicating the row & column 
position of the bot and the character array grid - and outputs the next move the bot makes to rescue the princess.

Input Format

The first line of the input is N (<100), the size of the board (NxN). The second line of the input contains two space separated integers,
which is the position of the bot.

Grid is indexed using Matrix Convention

The position of the princess is indicated by the character 'p' and the position of the bot is indicated 
by the character 'm' and each cell is denoted by '-' (ascii value: 45).

Output Format

Output only the next move you take to rescue the princess. Valid moves are LEFT, RIGHT, UP or DOWN

Sample Input

5
2 3
-----
-----
p--m-
-----
-----
Sample Output

LEFT
Scoring 
Your score for every testcase would be (NxN minus number of moves made to rescue the princess)/10 where N is 
the size of the grid (5x5 in the sample testcase). Maximum score is 17.5
*/

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <math.h>


void nextMove(int n, int r, int c, char grid[101][101])
{
	int left, right, top, bottom;
	int x, y;
	for (int step = 1; step < n; step++)
	{
		// left
		left = c - step;
		right = c + step;
		top = r - step;
		bottom = r + step;

		for (int x = left; x < right; x++)
		{
			if (grid[top][x] == 'p')
			{
				printf("TOP");
				return;
			}

			if (grid[bottom][x] == 'p')
			{
				printf("BOTTOM");
				return;
			}
		}

		for (int y = top; y < bottom; y++)
		{
			if (grid[y][left] == 'p')
			{
				printf("LEFT");
				return;
			}

			if (grid[y][right] == 'p')
			{
				printf("RIGHT");
				return;
			}
		}
	}
}
int main(void) {

	int n, r, c;

#if defined(USE_TEST_CASE)
	n = 5;
	r = 3;
	c = 1;

	char grid[101][101] = {
		{"-----"},
		{"-----"},
		{"-----"},
		{"-m--p"},
		{"-----"},
	};
#else
	scanf("%d", &n);
	scanf("%d", &r);
	scanf("%d", &c);



	for(int i=0; i<n; i++) {
		scanf("%s[^\n]%*c", grid[i]);
	}
#endif
	nextMove(n, r, c, grid);
	return 0;
}