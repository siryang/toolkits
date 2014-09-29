/**
Maze Escape is a popular 1 player game where a bot is trapped in a maze and is expected to find its way out. 
In this version of the game, 2 bots whose positions are not visible to each other are placed 
at random points in the maze. the first bot to find its way out of the maze wins the game.

The visibility of the bot is restricted to its 8 adjacent cells as shown in the figure below

---
-b-
---
where b is the bot. Bot's can be facing any of the 4 directions. To make this game more interesting, 
the orientation of both the bots are randomly chosen at the start of the game and 
the map visible to them also changes according to the orientation.

If the actual map is as shown below,

#######
#--#-b#
#--#--#
#--#--#
------#
#-----#
#######
and your bot is positioned at (1,5) and is facing the RIGHT side of the maze, the input will be

###
#b-
#--
If your bot is facing UP, your input will be

###
-b#
--#
If your bot is facing LEFT, your input will be

--#
-b#
###
And if your bot is facing DOWN, your input will be

#--
#b-
###
It is to be noted that your bot faces the direction in which it chooses to make its next move.

Input Format

The walls are represented by character # ( ascii value 35), empty cells are represented by - (ascii value 45), 
the maze door which is the indication of the way out is represented by e (ascii value 101)

The input contains 4 lines, the first line being the player id (1 or 2) and followed by 3 lines, 
each line containing 3 of the above mentioned characters which represents the 8 adjacent cells of the bot. 
The visible maze given as input always has the bot at the center.

Constraints

5 <= r,c <= 30 where r, c is the number of rows and columns of the maze.

Output Format

Output UP, DOWN, LEFT or RIGHT which is the next move of the bot.

Sample Input

1
###
#--
#--
Sample Output

RIGHT
Explanation

Considering the maze given above. If the input is as given below with the bot initially facing the RIGHT side of the maze, 
if the bot chooses to go RIGHT, the new position of the bot in the maze would be

#######
#--#--#
#--#-b#
#--#--#
------#
#-----#
#######
The bot moves 1 step DOWN w.r.t the maze. As the bot is facing DOWN side of the maze, his next input would be

#--
#--
#--
with the bot at the center.
*/

typedef enum Direction
{
	left, top, right, bottom
} Direction;

typedef struct Step
{
	int xStart, xStep, yStart, yStep;
} Step;

typedef struct Point
{
	int x, y;
} Point;

#if defined UPLOAD_HANDRANK
void mazeEscape(char adjency[3][3]);
#else
Direction mazeEscape(char adjency[3][3]);
#endif


