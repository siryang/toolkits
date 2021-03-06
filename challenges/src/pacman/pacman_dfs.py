#!/usr/bin/python


def direction(x1, y1, x2, y2):
	'''
		Direction from (x1, y1) to (x2, y2)
	'''
	if x1 < x2:
		return "RIGHT"
	if x1 > x2:
		return "LEFT"
	if y1 < y2:
		return "DOWN"
	if y1 > y2:
		return "UP"

class NodeStack:
	def __init__(self, rowNum, colNum, yStart, xStart, yEnd, xEnd, data):
		self.m_colNum = colNum
		self.m_rowNum = rowNum

		self.xEnd = xEnd
		self.yEnd = yEnd

		self.m_xStart = xStart
		self.m_yStart = yStart

		self.m_data = data

		self.m_closed = set()
		self.m_closed.add((xStart, yStart))
		self.m_traveled = [(xStart, yStart, 0, 0)]
		self.m_arrival = False

		self.m_stepNum = 0

	def arrival(self):
		return self.m_arrival
    
	def detect(self, xTo, yTo, xFrom, yFrom, xP, yP):
		if xTo == xP and yTo == yP:
			return False

		if xTo >= self.m_colNum or yTo >= self.m_rowNum or self.m_data[yTo][xTo] == "%" or (xTo, yTo) in self.m_closed:
			return False
		else:
			self.m_closed.add((xTo, yTo))
			self.m_traveled.append((xTo, yTo, xFrom, yFrom))
			return True
 
	def step(self):
		# close Node
		(x, y, xP, yP) = self.m_traveled[-1]
		if x == self.xEnd and y == self.yEnd:
			self.m_arrival = True
			return

		if self.detect(x, y-1, x, y, xP, yP):
			return
		elif self.detect(x-1, y, x, y, xP, yP):
			return
		elif self.detect(x+1, y, x, y, xP, yP):
			return
		elif self.detect(x, y+1, x, y, xP, yP):
			return
		else:
			# go back
			self.m_traveled.pop();
		

	def collect(self):
		x = self.xEnd
		y = self.yEnd
		steps = list()
		while not (self.m_xStart == x and self.m_yStart == y):
			steps.append((x, y))
			(x, y, xP, yP) = self.m_traveled.pop()
			(x, y) = (xP, yP)
		steps.append((x, y))
		print len(steps) 
		steps.reverse()
		for step in steps :
			print "%d %d" % (step[1], step[0])
        
def dfs( r, c, pacman_r, pacman_c, food_r, food_c, grid):
	nodeHeap = NodeStack(r, c, pacman_r, pacman_c, food_r, food_c, grid)
	stepNum = 0
	while not nodeHeap.arrival():
		nodeHeap.step()
		stepNum = stepNum + 1
		if (stepNum) > 100:
			exit(0)
	
	nodeHeap.collect()

''' codes for commit to hankrank
pacman_r, pacman_c = [ int(i) for i in raw_input().strip().split() ]
food_r, food_c = [ int(i) for i in raw_input().strip().split() ]
r,c = [ int(i) for i in raw_input().strip().split() ]

grid = []
for i in xrange(0, r):
    grid.append(raw_input().strip())

dfs(r, c, pacman_r, pacman_c, food_r, food_c, grid)
'''

def format2IntStr(str):
	str = str.split(" ")
	return (int(str[0]), int(str[1]))

def test():
	pacman_r, pacman_c = [ int(i) for i in "3 9".strip().split() ]
	food_r, food_c = [ int(i) for i in "5 1".strip().split() ]
	r,c = [ int(i) for i in "7 20".strip().split() ]

	data = list()
	data.append('''%%%%%%%%%%%%%%%%%%%%''')
	data.append('''%--------------%---%''')
	data.append('''%-%%-%%-%%-%%-%%-%-%''')
	data.append('''%--------P-------%-%''')
	data.append('''%%%%%%%%%%%%%%%%%%-%''')
	data.append('''%.-----------------%''')
	data.append('''%%%%%%%%%%%%%%%%%%%%''')
	return (r, c, pacman_r, pacman_c, food_r, food_c, data)

def parseArgs():
	pacman_r, pacman_c = [ int(i) for i in raw_input().strip().split() ]
	food_r, food_c = [ int(i) for i in raw_input().strip().split() ]
	r,c = [ int(i) for i in raw_input().strip().split() ]

	grid = []
	for i in xrange(0, r):
	    grid.append(raw_input().strip())

	return (r, c, pacman_r, pacman_c, food_r, food_c, grid)

def main():
	#(r, c, pacman_r, pacman_c, food_r, food_c, grid) = parseArgs();
	(r, c, pacman_r, pacman_c, food_r, food_c, grid) = test();

	dfs(r, c, pacman_r, pacman_c, food_r, food_c, grid)

if __name__ == '__main__':
	main()