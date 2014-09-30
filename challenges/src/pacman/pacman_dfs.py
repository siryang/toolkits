import heapq

def distance(x1, y1, x2, y2):
	return abs(x1-x2) + abs(y1-y2)


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
		
class NodeHeap:
	def __init__(self, rowNum, colNum, yStart, xStart, yEnd, xEnd, data):
		self.m_colNum = colNum
		self.m_rowNum = rowNum

		self.xEnd = xEnd
		self.yEnd = yEnd

		self.m_xStart = xStart
		self.m_yStart = yStart

		self.m_data = data

		self.m_closed = dict()
		self.m_openHeap = [] # cost, position
		heapq.heappush(self.m_openHeap, (0, (xStart, yStart, 0, 0, 0)))

		self.m_arrival = False

	def detect(self, xParent, yParent, x, y, fCost):
		if x >= self.m_colNum or y >= self.m_rowNum or (x, y) in self.m_closed:
			return
 
		if self.m_data[y][x] == "%":
			self.m_closed[(x, y)] = (xParent, yParent)
			return

		hCost = distance(self.xEnd, self.yEnd, x, y)
		heapq.heappush(self.m_openHeap, (fCost + hCost, (x, y, xParent, yParent, fCost)))

	def step(self):
		(cost, (x, y, xP, yP, pFCost)) = heapq.heappop(self.m_openHeap)
		# close Node
		self.m_closed[(x, y)] = (xP, yP)
		if x == self.xEnd and y == self.yEnd:
			self.m_arrival = True
			return

		++pFCost
		self.detect(x, y, x+1, y, pFCost)
		self.detect(x, y, x-1, y, pFCost)
		self.detect(x, y, x, y+1, pFCost)
		self.detect(x, y, x, y-1, pFCost)

	def collect(self):
		x = self.xEnd
		y = self.yEnd
		step = list()
		while not (self.m_xStart == x and self.m_yStart == y):
			(xP, yP) = self.m_closed[(x, y)]
			step.append(direction(xP, yP, x, y))
			(x, y) = (xP, yP)

		step.reverse()
		print step

def dfsAStar(r, c, pacman_r, pacman_c, food_r, food_c, grid):
	nodeHeap = NodeHeap(r, c, pacman_r, pacman_c, food_r, food_c, grid)
	while not nodeHeap.m_arrival:
		nodeHeap.step()
	nodeHeap.collect()

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

	dfsAStar(r, c, pacman_r, pacman_c, food_r, food_c, grid)

if __name__ == '__main__':
	main()