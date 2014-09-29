#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include "maze_escape.h"

typedef enum NextStepStaus
{
	shouldNotGo = 0,
	wait = 1,
	justGo
} NextStepStaus;


NextStepStaus getDirStatus(char adjency[3][3], Direction dir)
{
	const static Point positions[] = {
		{0, 1},
		{1, 0},
		{2, 1},
	};

	const Point* p = &positions[dir];
	switch(adjency[p->y][p->x])
	{
	case '-':
		return wait;
	case 'e':
		return justGo;
	case '#':
		return shouldNotGo;
	default:
		return shouldNotGo;
	}
}

const char* dirstring[] = {
	"LEFT", 
	"UP",
	"RIGHT",
	"DOWN"
};

#if defined UPLOAD_HANDRANK
#define RETURN_RESULT(dir) {printf("%s", dirstring[(dir)]); return;}
#else
#define RETURN_RESULT(dir) {return (dir);}
#endif

#if defined UPLOAD_HANDRANK
void mazeEscape(char adjency[3][3])
#else
Direction mazeEscape(char adjency[3][3])
#endif
{
	NextStepStaus leftStatus, topStatus, rightStatus;
	int choiceNum;
#define checkJustGoAndReturn(status, dir) {if(status == justGo){RETURN_RESULT(dir);}}
	leftStatus = getDirStatus(adjency, left);
	checkJustGoAndReturn(leftStatus, left);

	topStatus = getDirStatus(adjency, top);
	checkJustGoAndReturn(topStatus, top);

	rightStatus = getDirStatus(adjency, right);
	checkJustGoAndReturn(rightStatus, right);

	choiceNum = leftStatus + rightStatus + topStatus;
#define checkAndReturn(status, dir) {if(status == wait){RETURN_RESULT(dir);}}
	switch (choiceNum)
	{
	case 0:
		RETURN_RESULT(bottom);
	case 1:
		checkAndReturn(leftStatus, left);
		checkAndReturn(rightStatus, right);
		checkAndReturn(topStatus, top);
	default:
		{
			int randWay;
			int doorLT = (adjency[0][0] == 'e'? 1: 0);
			int doorRT = (adjency[0][2] == 'e'? 1: 0);
			if (doorLT){
				checkAndReturn(topStatus, top);
				checkAndReturn(leftStatus, left);
			}
			if (doorRT){
				checkAndReturn(topStatus, top);
				checkAndReturn(rightStatus, right);
			}

			srand(time());
			randWay =  rand() % choiceNum; // 0, 1, 2
			if (choiceNum == 3)
			{
				RETURN_RESULT(randWay);
			}
			if (randWay == 0)
			{
				checkAndReturn(rightStatus, right);
				checkAndReturn(topStatus, top);
				checkAndReturn(leftStatus, left);
			}
			else
			{
				checkAndReturn(leftStatus, left);
				checkAndReturn(topStatus, top);
				checkAndReturn(rightStatus, right);
			}
		}
	}
	RETURN_RESULT(top);
}

#if defined UPLOAD_HANDRANK
int main() {

	/* Enter your code here. Read input from STDIN. Print output to STDOUT */ 
	int i;
	int robId;
	char grid[3][3];
	scanf("%d", &robId);
	for(i=0; i<3; i++) {
		scanf("%s[^\n]%*c", grid[i]);
	}
	mazeEscape(grid);
	return 0;
}
#endif
