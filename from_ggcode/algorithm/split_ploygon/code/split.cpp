#include "stdafx.h"
#include "split.h"

///< 直线和多边形的交点
/*
	1.交点位置
*/
enum CoordinateDirection
{
	CoordinateDirection_x,
	CoordinateDirection_y
};

struct IntersectionOfEdge
{
	int begin; // 第一个和边相交的扫描线位置
	int end;
	double delt; // 步长
	double insPos;	// 交点在扫描线上的位置
	size_t edgeIndex; // 交点信息对应的边序号
	bool forward; // 交点是否从线段的起点到终点
};
typedef vector<IntersectionOfEdge> IntersectionArray;

struct IntersectionPosition
{
	int edgeIdx; // 交点所在的边
	Point position; // 交点坐标
};

bool xLess(const IntersectionPosition& left, const IntersectionPosition& right)
{
	return left.position.x < right.position.x;
}

bool yLess(const IntersectionPosition& left, const IntersectionPosition& right)
{
	return left.position.y < right.position.y;
}

struct CutLine
{
	int linePos; // 当前扫描线位置
	vector<IntersectionPosition> insPos; // 扫描线上的所有交点
	bool operator = (const int _pos)
	{
		return linePos == _pos;
	}
};

inline bool IntersectionOfEdge_lessByBegin(const IntersectionOfEdge& left, const IntersectionOfEdge& right)
{
	return left.begin < right.begin;
}

void IntersectionArray_step( IntersectionArray &intersections, int gridsize) 
{
	int minBegin = intersections[0].begin;
	for (size_t i = 0; i < intersections.size();)
	{
		IntersectionOfEdge& insEdge = intersections[i];
		if (insEdge.begin != minBegin)
		{
			return;
		}

		insEdge.begin = (int)((double)insEdge.begin + gridsize);
		if (insEdge.begin < insEdge.end)
		{
			insEdge.insPos += insEdge.delt;
			i++;
		}
		else
		{
			// 遍历完毕，删除当前这条边
			intersections.erase(intersections.begin() + i);
		}
	}
}

void _Polygon_lineIntersection(const Polygon* o, int* left, int* right, IntersectionArray& intersections, int gridSize, CoordinateDirection lineDirection);

CutLine _Polygon_getOneCutLine(const IntersectionArray& intersections, int gridSize, CoordinateDirection direction) 
{
	CutLine cutLine;
	int minBegin = intersections[0].begin;
	cutLine.linePos = minBegin;
	for (size_t i = 0; i < intersections.size(); i++)
	{
		const IntersectionOfEdge& edge = intersections[i];
		if(edge.begin != minBegin)
		{
			break;
		}

		IntersectionPosition pos;
		pos.edgeIdx = edge.edgeIndex;
		if (direction == CoordinateDirection_x)
		{
			pos.position.y = (int)edge.insPos;
			pos.position.x = minBegin;
		}
		else //CoordinateDirection_y
		{
			pos.position.y = minBegin;
			pos.position.x = (int)edge.insPos;
		}
		cutLine.insPos.push_back(pos);	
	}

	if (direction == CoordinateDirection_x)
	{
		sort(cutLine.insPos.begin(), cutLine.insPos.end(), yLess);
	}
	else //CoordinateDirection_y
	{
		sort(cutLine.insPos.begin(), cutLine.insPos.end(), xLess);
	}

	// 按照网格切分
	return cutLine;
}

void _Polygon_getCutLine( const Polygon* poly, int gridSize, vector<CutLine> &cutlines, CoordinateDirection direction ) 
{
	IntersectionArray intersections;
	int left, right;
	_Polygon_lineIntersection(poly, &left, &right, intersections, gridSize, direction);
	sort(intersections.begin(), intersections.end(), IntersectionOfEdge_lessByBegin);
	
	if (left % gridSize != 0)
	{
		CutLine first;
		first.linePos = left / gridSize * gridSize;
		cutlines.push_back(first);
	}

	while(!intersections.empty())
	{
		CutLine cutLine = _Polygon_getOneCutLine(intersections, gridSize, direction);
		if (!cutLine.insPos.empty())
		{
			cutlines.push_back(cutLine);
		}
		IntersectionArray_step(intersections, gridSize);
	}

	if (right % gridSize != 0)
	{
		CutLine last;
		last.linePos = right / gridSize * gridSize + gridSize;
		cutlines.push_back(last);
	}
}

class PolygonFinder
{
public:
	const Polygon* srcPoly;
	int gridSize;
	vector<Polygon*> result;

	enum SearchStatus
	{
		SearchStatus_start,
		SearchStatus_match,
		//SearchStatus_xTopSingle,
		//SearchStatus_xBottomSingle,
		//SearchStatus_yLeftSingle,
		//SearchStatus_yRightSingle,
		SearchStatus_vertex,
		SearchStatus_complete // 找到一个多边形
	};
	SearchStatus status;
	vector<CutLine> xCutLines;
	vector<CutLine> yCutLines;

	// 遍历Grid
	int xScannerIdx;
	int yScannerIdx;

	// 每个Grid找出x/y上所有交点，并按逆时针排序
	enum GridEdgeIntersectionType
	{
		GridEdgeIntersectionType_none = 0,
		GridEdgeIntersectionType_right,
		GridEdgeIntersectionType_top,
		GridEdgeIntersectionType_left,
		GridEdgeIntersectionType_bottom = 4,

		GridEdgeIntersectionType_rightBottom,
		GridEdgeIntersectionType_rightTop,
		GridEdgeIntersectionType_leftTop,
		GridEdgeIntersectionType_leftBottom
	};

	struct IntersectionOnGridEdge
	{
		const IntersectionPosition* pos;
		GridEdgeIntersectionType type;
	};

	list<IntersectionOnGridEdge> insPositions;
	IntersectionOnGridEdge firstVectect;
	list<IntersectionOnGridEdge>::const_iterator iterNextInsPos;
	int lastEdgeId;


	PolygonFinder()
	{
		status = SearchStatus_start;
		xScannerIdx = 0;
		yScannerIdx = -1;
	}

	Point gridEdgeIntersectionType2Position(GridEdgeIntersectionType type)
	{
		Point temp;
		temp.x = xCutLines[xScannerIdx + 1].linePos; // right
		temp.y = yCutLines[yScannerIdx + 1].linePos; // top

		switch (type)
		{
		case PolygonFinder::GridEdgeIntersectionType_rightBottom:
			temp.y -= gridSize;
			break;
		case PolygonFinder::GridEdgeIntersectionType_leftTop:
			temp.x -= gridSize;
			break;
		case PolygonFinder::GridEdgeIntersectionType_leftBottom:
			temp.y -= gridSize;
			temp.x -= gridSize;
			break;
		}

		return temp;
	}

	// 将Grid边界上的点，连接起来
	void pushIntersectionToListX(const CutLine* line, int begin, int end)
	{
		int firstIdx = INVALID_VALUE;
		const vector<IntersectionPosition>& insOnLine = line->insPos;

		// find begin
		if (begin < end)
		{
			for (size_t i = 0; i < insOnLine.size(); i++)
			{
				if (insOnLine[i].position.y >= begin)
				{
					firstIdx = i;
					break;
				}
			}

			if (firstIdx == INVALID_VALUE)
			{
				return;
			}

			if(firstIdx % 2 != 0 && insOnLine[firstIdx].position.y != begin)
			{
				IntersectionOnGridEdge ins;
				ins.type = GridEdgeIntersectionType_rightBottom;
				insPositions.push_back(ins);
			}

			for (size_t i = firstIdx; i < insOnLine.size(); i++)
			{
				if (insOnLine[i].position.y >= end)
				{
					break;
				}
				IntersectionOnGridEdge ins;
				ins.pos = &insOnLine[i];
				ins.type = GridEdgeIntersectionType_right;
				insPositions.push_back(ins);
			}
		}
		else if (begin > end)
		{
			for (int i = insOnLine.size() - 1; i >= 0; i--)
			{
				if (insOnLine[i].position.y <= begin)
				{
					firstIdx = i;
					break;
				}
			}

			if (firstIdx == INVALID_VALUE)
			{
				return;
			}

			if(firstIdx % 2 == 0 && insOnLine[firstIdx].position.y != begin)
			{
				IntersectionOnGridEdge ins;
				ins.type = GridEdgeIntersectionType_leftTop;
				insPositions.push_back(ins);
			}

			for (int i = firstIdx; i >= 0; i--)
			{
				if (insOnLine[i].position.y <= end)
				{
					break;
				}
				IntersectionOnGridEdge ins;
				ins.pos = &insOnLine[i];
				ins.type = GridEdgeIntersectionType_left;
				insPositions.push_back(ins);
			}
		}
	}

	void pushIntersectionToListY(const CutLine* line, int begin, int end)
	{
		int firstIdx = INVALID_VALUE;
		const vector<IntersectionPosition>& insOnLine = line->insPos;

		// find begin
		if (begin < end)
		{
			for (size_t i = 0; i < insOnLine.size(); i++)
			{
				if (insOnLine[i].position.x >= begin)
				{
					firstIdx = i;
					break;
				}
			}

			if (firstIdx == INVALID_VALUE)
			{
				return;
			}

			if(firstIdx % 2 != 0 && insOnLine[firstIdx].position.x != begin)
			{
				IntersectionOnGridEdge ins;
				ins.type = GridEdgeIntersectionType_leftBottom;
				insPositions.push_back(ins);
			}

			for (size_t i = firstIdx; i < insOnLine.size(); i++)
			{
				if (insOnLine[i].position.x >= end)
				{
					break;
				}
				IntersectionOnGridEdge ins;
				ins.pos = &insOnLine[i];
				ins.type = GridEdgeIntersectionType_bottom;
				insPositions.push_back(ins);
			}
		}
		else if (begin > end)
		{
			for (int i = insOnLine.size() - 1; i >= 0; i--)
			{
				if (insOnLine[i].position.x <= begin)
				{
					firstIdx = i;
					break;
				}
			}


			if (firstIdx == INVALID_VALUE)
			{
				return;
			}
			else if(/*(firstIdx % 2 != 0 && insOnLine[firstIdx].position.x < end)
				||*/ firstIdx % 2 == 0 && insOnLine[firstIdx].position.x != begin)
			{
				IntersectionOnGridEdge ins;
				ins.type = GridEdgeIntersectionType_rightTop;
				insPositions.push_back(ins);
			}

			for (int i = firstIdx; i >= 0; i--)
			{
				if (insOnLine[i].position.x <= end)
				{
					break;
				}
				IntersectionOnGridEdge ins;
				ins.pos = &insOnLine[i];
				ins.type = GridEdgeIntersectionType_top;
				insPositions.push_back(ins);
			}
		}
	}

	bool moveToNextGrid()
	{
		// 将格子四边上的交点放到链表中
		if (yScannerIdx == yCutLines.size() - 2)
		{
			if (xScannerIdx == xCutLines.size() - 2)
			{
				return false;
			}
			xScannerIdx++;
			yScannerIdx = 0;
		}
		else
		{
			yScannerIdx++;
		}

		const CutLine* xLineLeft = &xCutLines[xScannerIdx];
		const CutLine* xLineRight = &xCutLines[xScannerIdx + 1];
		const CutLine* yLineBottom = &yCutLines[yScannerIdx];
		const CutLine* yLineTop = &yCutLines[yScannerIdx + 1];
		pushIntersectionToListX(xLineRight, yLineBottom->linePos, yLineTop->linePos);
		pushIntersectionToListY(yLineTop, xLineRight->linePos, xLineLeft->linePos);
		pushIntersectionToListX(xLineLeft, yLineTop->linePos, yLineBottom->linePos);
		pushIntersectionToListY(yLineBottom, xLineLeft->linePos, xLineRight->linePos);
		return true;
	}

	bool step()
	{
		while (insPositions.size() < 2)
		{
			insPositions.clear();
			if (!moveToNextGrid())
				return false;
		}

		iterNextInsPos = insPositions.begin();

		Polygon* poly = new Polygon;
		// 取第一个点，进行配对，配对结束则找顶点，顶点找顶点，如果找到的是边上的点，则配对
		firstVectect = *iterNextInsPos++;
		insPositions.pop_front();
		if (firstVectect.type > GridEdgeIntersectionType_bottom)
			poly->push_back(gridEdgeIntersectionType2Position(firstVectect.type));
		else
			poly->push_back(firstVectect.pos->position);

		status = SearchStatus_match;
		while (status != SearchStatus_complete)
		{
			if (status == SearchStatus_match)
			{
				popIntersection(poly);
			}

			if (status == SearchStatus_vertex)
			{
				findVectex(poly);
			}
		}
		result.push_back(poly);
		return true;
	}

	// 查找顶点
	void findVectex(Polygon* poly)
	{
		int edgeIndex = lastEdgeId;
		int right = xCutLines[xScannerIdx + 1].linePos;
		int top = yCutLines[yScannerIdx + 1].linePos;

		while(true)
		{
			// 下一个顶点是最后一个顶点
			if (firstVectect.type != GridEdgeIntersectionType_none && firstVectect.type <= GridEdgeIntersectionType_bottom 
				&& edgeIndex == firstVectect.pos->edgeIdx)
			{
				status = SearchStatus_complete;
				return;
			}

			list<IntersectionOnGridEdge>::iterator iterPosition = insPositions.begin();
			for (; iterPosition != insPositions.end(); iterPosition++)
			{
				if (iterPosition->type <= GridEdgeIntersectionType_bottom && iterPosition->pos->edgeIdx == edgeIndex)
				{
					poly->push_back(iterPosition->pos->position);
					iterNextInsPos = insPositions.erase(iterPosition);
					status = SearchStatus_match;
					return;
				}
			}

			Point p = srcPoly->at(edgeIndex);
			Point pre = srcPoly->at((edgeIndex == 0? srcPoly->size(): edgeIndex) - 1);
			if (p != poly->back() && right - gridSize <= p.x && p.x < right 
				&& top - gridSize <= p.y && p.y < top)
			{
				if (!(right - gridSize == p.x) && !(top - gridSize == p.y))
				{
					poly->push_back(p);
				}
				edgeIndex = edgeIndex + 1;
				if (edgeIndex == srcPoly->size())
					edgeIndex = 0;
			}
			else if (pre != poly->back() && right - gridSize <= pre.x && pre.x < right 
				&& top - gridSize <= pre.y && pre.y < top)
			{
				if (!(right - gridSize == pre.x) && !(top - gridSize == pre.y))
				{
					poly->push_back(pre);
				}
				if (edgeIndex == 0)
				{
					edgeIndex = srcPoly->size() - 1;
				}
				else
				{
					edgeIndex = edgeIndex - 1;
				}
			}
			else
			{
				status = SearchStatus_match;
				return;
			}
		}
	}

	// 配对
	void popIntersection(Polygon* poly)
	{
		list<IntersectionOnGridEdge>::const_iterator iterBeginer = iterNextInsPos;
		for(; iterNextInsPos != insPositions.end() && iterNextInsPos->type > GridEdgeIntersectionType_bottom;
			iterNextInsPos++) 
		{
			poly->push_back(gridEdgeIntersectionType2Position(iterNextInsPos->type));
		};

		if (iterNextInsPos != insPositions.end())
		{
			poly->push_back(iterNextInsPos->pos->position);
			status = SearchStatus_vertex;
			lastEdgeId = iterNextInsPos->pos->edgeIdx;
			iterNextInsPos++;
			iterNextInsPos = insPositions.erase(iterBeginer, iterNextInsPos);
		}
		else
		{
			// 和起点配对
			status = SearchStatus_complete;
			return;
		}
	}
};

///< 每条边，保存一个切割点数组
void _Polygon_lineIntersection(const Polygon* o, int* boundingLeft, int* boundingRight, IntersectionArray& intersections, int gridSize, CoordinateDirection lineDirection)
{
	size_t polyPosNum = o->size();
	const Point* p = &o->at(0);
	const Point* pre = p + polyPosNum - 1;
	const Point* pEnd = p + polyPosNum;
	
	if (lineDirection == CoordinateDirection_x)
	{
		*boundingLeft = p->x;
		*boundingRight = p->x;
	}
	else
	{
		*boundingLeft = p->y;
		*boundingRight = p->y;
	}

	intersections.reserve(polyPosNum);
	for (; p != pEnd; pre = p, p++)
	{
		IntersectionOfEdge edge;
		if (lineDirection == CoordinateDirection_x)
		{
			*boundingLeft = min(*boundingLeft, p->x);
			*boundingRight = max(*boundingRight, p->x);
			int left = pre->x / gridSize * gridSize;
			int right = p->x / gridSize * gridSize;
			edge.forward = left < right;
			edge.begin = left + gridSize;
			edge.end = right + gridSize;
			if(edge.begin == edge.end)
			{
				continue;
			}
			else
			{
				edge.delt = gridSize * (p->y - pre->y) / (double)(p->x - pre->x);
				if (!edge.forward)
				{
					swap(edge.begin, edge.end);
					edge.insPos = p->y - edge.delt * (p->x - right) / double(gridSize) + edge.delt;
				}
				else
				{
					// 如果求线上的第一个交点，且left是起点左侧的扫描线，则加上edge.delt
					edge.insPos = pre->y - edge.delt * (pre->x - left) / double(gridSize) + edge.delt;
				}
			}
		}
		else if (lineDirection == CoordinateDirection_y)
		{
			*boundingLeft = min(*boundingLeft, p->y);
			*boundingRight = max(*boundingRight, p->y);
			int left = pre->y / gridSize * gridSize;
			int right = p->y / gridSize * gridSize;
			edge.forward = left < right;
			edge.begin = left + gridSize;
			edge.end = right + gridSize;
			if(edge.begin == edge.end)
			{
				continue;
			}
			else
			{
				edge.delt = gridSize * (p->x - pre->x) / (double)(p->y - pre->y);
				if (!edge.forward)
				{
					swap(edge.begin, edge.end);
					edge.insPos = p->x - edge.delt * (p->y - right) / double(gridSize) + edge.delt;
				}
				else
				{
					edge.insPos = pre->x - edge.delt * (pre->y- left) / double(gridSize) + edge.delt;
				}
			}
		}
		edge.edgeIndex = polyPosNum - (pEnd - p);
		intersections.push_back(edge);
	}
}

void split(const Polygon* poly, int gridSize, Polygon** pieces, size_t* num)
{
	poly, gridSize, num, pieces, poly;

	PolygonFinder finder;
	finder.srcPoly = poly;
	finder.gridSize = gridSize;
	// 分别获取每个切割线和多边形在X/Y上的交点，并排序
	_Polygon_getCutLine(poly, gridSize, finder.xCutLines, CoordinateDirection_x);
	_Polygon_getCutLine(poly, gridSize, finder.yCutLines, CoordinateDirection_y);
	// 如果一条yCutLine上，两个连续切点对应的边连续，则这两个切点和中间的顶点，组成多边形。
	while(finder.step())
	{}
	*num = finder.result.size();
	*pieces = new Polygon[*num];
	for (size_t i = 0; i < *num; i++)
	{
		(*pieces)[i] = *finder.result[i];
		delete finder.result[i];
	}

}

void output(const Polygon* polygons, size_t num, const char* dest)
{
	string midFname, mifFname;
	midFname = string(dest) + ".mid";
	mifFname = string(dest) + ".mif";

	ofstream midFs(midFname.c_str());
	ofstream mifFs(mifFname.c_str());

	mifFs << "Version 300\n"
		"Charset \"WindowsSimpChinese\"\n"
		"Delimiter \",\"\n"
		"CoordSys Earth\n"
		"Projection 1,0\n"
		"Columns 1\n"
		"ID Char(10)\n"
		" Data\n" << endl;

	for (size_t i = 0; i < num; i++)
	{
		midFs << "\"" << i << "\"" << endl;
		const Polygon& poly = polygons[i];
		mifFs << "Region 1" << endl;
		mifFs << " " << poly.size() << endl;
		for (size_t j = 0; j < poly.size(); j++)
		{
			mifFs << poly[j].x / float(1E2) << " " << poly[j].y / float(1E2) << endl;
		}
	}
	
	midFs.close();
	mifFs.close();
}
