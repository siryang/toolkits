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
	def __init__(self, colNum, rowNum, xStart, yStart, xEnd, yEnd, data):
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
		#print (len(self.m_openHeap))
		(cost, (x, y, xP, yP, pFCost)) = heapq.heappop(self.m_openHeap)
		self.m_closed[(x, y)] = (xP, yP)
		if x == self.xEnd and y == self.yEnd:
			self.m_arrival = True
			return

		#print "x:%d, y:%d" % (x, y)
		# close Node
		++pFCost
		self.detect(x, y, x+1, y, pFCost)
		self.detect(x, y, x-1, y, pFCost)
		self.detect(x, y, x, y+1, pFCost)
		self.detect(x, y, x, y-1, pFCost)

	def dfsAStar(self):
		while not self.m_arrival:
			self.step()
		self.collect()

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

# nodes = map(pos:cost)
	

def format2IntStr(str):
	str = str.split(" ")
	return (int(str[0]), int(str[1]))

def parseArgs():
	pacmanPos = format2IntStr(raw_input())
	foodPos = format2IntStr(raw_input())

def main():
	#parseArgs()	
	pacmanPos = format2IntStr("3 9")
	foodPos = format2IntStr("5 1")
	matrix = format2IntStr("7 20")
	print pacmanPos

	data = list()
	data.append('''%%%%%%%%%%%%%%%%%%%%''')
	data.append('''%--------------%---%''')
	data.append('''%-%%-%%-%%-%%-%%-%-%''')
	data.append('''%--------P-------%-%''')
	data.append('''%%%%%%%%%%%%%%%%%%-%''')
	data.append('''%.-----------------%''')
	data.append('''%%%%%%%%%%%%%%%%%%%%''')

	nodeHeap = NodeHeap(matrix[1], matrix[0], pacmanPos[1], pacmanPos[0], foodPos[1], foodPos[0], data)
	nodeHeap.dfsAStar()


if __name__ == '__main__':
	main()