#!/bin/ruby
=begin
[0,4,2]
[1,3,5]
[6,7,8]

# DOWN
[1,4,2]
[0,3,5]
[6,7,8]

# RIGHT
[1,4,2]
[3,0,5]
[6,7,8]

# UP
[1,0,2]
[3,4,5]
[6,7,8]

# LEFT
[0,1,2]
[3,4,5]
[6,7,8]
=end

class Node
	def initialize(edge, grids)
		@edge = edge
		@grids = grids
		@key = generateKey
	end

	def generateKey

	end

	def hCost
		cost = 0
		(0..@edge).each do |y|
			(0..@edge).each do |x|
				v = @grids[][]
				itemCost = ( % @edge - r).abs + (d / @edge - c).abs
				cost += itemCost				
			end
		end
	end

	def show
		puts @grids
	end
end

=begin
	astar:	cost, fcost, nodeKey, node, openSet, closeSet
=end
class NodeAstar

	def initialize(startNode)

		@openNodes
		 = startNode
	end

	def step
	end
end


node = Node.new( 3,
	[[0,4,2],
	[1,3,5],
	[6,7,8]])

#node.show
node.hCost

