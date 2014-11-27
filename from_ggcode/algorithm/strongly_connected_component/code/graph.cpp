#include "scc_stdafx.h"
#include <sstream>
#include "graph.h"

class GraphCache
{
public:
	GraphNode* allocNode()
	{
		assert(m_nodeNum < 1000);
		return m_nodePool + m_nodeNum++;
	}

	GraphLink* allocLink()
	{
		assert(m_linkNum < 1000);
		return m_linkPool + m_linkNum++;
	}

	inline size_t getNodeNum() const
	{
		return m_nodeNum;
	}

	inline size_t getLinkNum() const
	{
		return m_linkNum;
	}

	inline const GraphNode* getNodePool() const
	{
		return m_nodePool;
	}

	inline const GraphLink* getLinkPool() const
	{
		return m_linkPool;
	}

private:
	GraphCache();

	~GraphCache()
	{
		delete[] m_nodePool;
		delete[] m_linkPool;
	}

public:
	static GraphCache* m_cache;

private:
	size_t m_nodeNum;
	size_t m_linkNum;
	GraphNode* m_nodePool;
	GraphLink* m_linkPool;
};

GraphCache* GraphCache::m_cache = new GraphCache;
GraphCache* Graph::m_graphCache = GraphCache::m_cache;

GraphLink::GraphLink() : m_linkId(0)
, m_direction(GraphLinkDirection_double)
, m_sNodeId(NULL)
, m_eNodeId(NULL)
{

}

void GraphLink::fromString( const char* linkStr )
{
	// linkId, sNodeId, eNodeId, dir
	const char* p = linkStr;
	m_linkId = atoi(p);

	p = strchr(p, ',');
	m_sNodeId = atoi(++p);

	p = strchr(p, ',');
	m_eNodeId = atoi(++p);

	p = strchr(p, ',');
	m_direction = (GraphLinkDirection)atoi(++p);
}

GraphCache::GraphCache()
{
	m_linkNum = 0;
	m_nodeNum = 0;
	m_nodePool = new GraphNode[1000];
	m_linkPool = new GraphLink[1000];
}


void Graph::addLink( const char* linkInfo )
{
	GraphLink* link = m_graphCache->allocLink();
	link->fromString(linkInfo);
	m_links.insert(LinkMap::value_type(link->m_linkId, link));
}

void Graph::addNode( const char* nodeInfo )
{
	GraphNode* node = m_graphCache->allocNode();
	node->fromString(nodeInfo);
	m_nodes.insert(NodeMap::value_type(node->m_nodeId, node));
}

bool Graph::record(const char* destFile)
{
	string nodeFName = string(destFile) + ".node";
	string linkFNaem = string(destFile) + ".link";

	ofstream nodeFs(nodeFName.c_str());

	for (NodeMap::const_iterator iterNode = m_nodes.begin();
		iterNode != m_nodes.end(); iterNode++)
	{
		nodeFs << iterNode->second->toString() << endl;
	}

	nodeFs.close();

	// write link info to file.
	ofstream linkFs(linkFNaem.c_str());
	for (LinkMap::const_iterator iterLink = m_links.begin();
		iterLink != m_links.end(); iterLink++)
	{
		linkFs << iterLink->second->toString() << endl;
	}
	linkFs.close();
	return true;
}

bool Graph::parser(const char* nodeStr)
{
	ifstream nodeFs((string(nodeStr) + ".node").c_str());
	ifstream linkFs((string(nodeStr) + ".link").c_str());

	if (!nodeFs.is_open() || !linkFs.is_open())
	{
		return false;
	}

	char buffer[1024];
	while(nodeFs.getline(buffer, element_of(buffer)))
	{
		addNode(buffer);
	}

	while(linkFs.getline(buffer, element_of(buffer)))
	{
		addLink(buffer);
	}

	nodeFs.close();
	linkFs.close();
	return true;
}

void GraphNode::fromString(const char* nodeStr)
{
	// nodeId, linkNum, link1-link2-link3
	const char* p = nodeStr;
	m_nodeId = atoi(p);
	p = strchr(p, ',');
	
	size_t linkNum = atoi(++p);
	p = strchr(p, ',');
	m_links.resize(linkNum);
	for (size_t i = 0; i < linkNum; i++)
	{
		m_links[i] = atoi(++p);
		p = strchr(p, '-');
	}
}

std::string GraphNode::toString() const
{
	ostringstream ostr;
	ostr << m_nodeId << "," << m_links.size() << ",";

	for (size_t j = 0; j < m_links.size(); j++)
	{
		ostr << m_links[j];
		if (j != m_links.size() - 1)
		{
			ostr << "-";
		}
	}
	string temp = ostr.str();
	return ostr.str();
}

void Graph::simple2()
{
	addLink("0,0,1,3");
	addLink("1,1,2,1");
	addLink("2,2,3,1");

	addNode("0,1,0");
	addNode("1,2,0-1");
	addNode("2,2,1-2");
	addNode("3,1,2");
}
