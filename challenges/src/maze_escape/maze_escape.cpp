#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include "maze_escape.h"

namespace MazeEscape
{
	enum NextStepStaus
	{
		shouldNotGo = 0,
		wait = 1,
		justGo,
	};

	NextStepStaus getDirStatus(char adjency[3][3], Direction dir)
	{
		const static Point positions[] = {
			{0, 1},
			{1, 0},
			{2, 1},
		};

		const Point& p = positions[dir];
		switch(adjency[p.y][p.x])
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


	Direction mazeEscape(char adjency[3][3])
	{
		NextStepStaus leftStatus = getDirStatus(adjency, left);
		if (leftStatus == justGo){
			return left;
		}

		NextStepStaus topStatus = getDirStatus(adjency, top);
		if (topStatus == justGo){
			return top;
		}

		NextStepStaus rightStatus = getDirStatus(adjency, right);
		if (rightStatus == justGo){
			return right;
		}

		int choiceNum = leftStatus + rightStatus + topStatus;
#define checkAndReturn(status, dir) {if(status == wait) return (dir);}
		switch (choiceNum)
		{
		case 0:
			return bottom;
		case 1:
			checkAndReturn(leftStatus, left);
			checkAndReturn(rightStatus, right);
			checkAndReturn(topStatus, top);
		default:
			bool doorLT = adjency[0][0] == 'e';
			bool doorRT = adjency[0][2] == 'e';
			if (doorLT){
				checkAndReturn(topStatus, top);
				checkAndReturn(leftStatus, left);
			}
			if (doorRT){
				checkAndReturn(topStatus, top);
				checkAndReturn(rightStatus, right);
			}

			int randWay = rand() % choiceNum;
			if (choiceNum == 3)
				return (Direction)randWay;
			if (choiceNum == 2)
			{
				checkAndReturn(rightStatus, right);
				checkAndReturn(topStatus, top);
				checkAndReturn(leftStatus, left);
			}
		}
		return top;
	}
}