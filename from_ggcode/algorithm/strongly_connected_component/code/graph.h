#include <map>
using namespace std;

typedef __int64 ObjectId;
struct GraphLink;
struct GraphNode;
class GraphCache;

struct GraphNode
{
	ObjectId m_nodeId;
	vector<ObjectId> m_links;

	string toString() const; 
	void fromString(const char* nodeStr);
};

struct GraphLink
{
	enum GraphLinkDirection
	{
		GraphLinkDirection_forward = 1, // 01
		GraphLinkDirection_backward = 2, // 10
		GraphLinkDirection_double = 3, //11
	};

	ObjectId m_linkId;
	ObjectId m_sNodeId;
	ObjectId m_eNodeId;
	GraphLinkDirection m_direction;

public:
	GraphLink();

	void fromString(const char* linkStr);

	inline string toString() const
	{
		ostringstream ostr;
		ostr << m_linkId << "," << m_sNodeId << "," << m_eNodeId << "," << (int)m_direction;
		return ostr.str();
	}
};

class Graph
{
public:
	void simple2();

	void addLink(const char* linkInfo);

	void addNode(const char* nodeInfo);

	// Link: linkId, sNodeId, eNodeId, dir
	// Node: nodeId, linkNum, link1-link2-link3
	/// record a graph to file.
	bool record(const char* destFName);

	/// parser graph from file.
	bool parser(const char* destFName);
	
public:
	typedef map<ObjectId, GraphNode*> NodeMap;
	typedef map<ObjectId, GraphLink*> LinkMap;

	NodeMap m_nodes;
	LinkMap m_links;

private:
	static GraphCache* m_graphCache;
};


