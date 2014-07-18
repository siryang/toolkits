#include "png_productor.h"
#include "grid_data_parser.h"
extern "C"
{
	#include "util/nds.h"
};

using namespace std;

PngProductor::PngProductor()
{
}
  
bool PngProductor::open( const wchar_t* sourceDb, const wchar_t* pngDb )
{
	m_sourceDb = NdsDb_alloc(sourceDb, L"qvf", DbOpenFlag_openForRead, NULL);
	m_pngDb = NdsDb_alloc(pngDb, L"png", DbOpenFlag_openForReadWrite, L"@gridId:int, width:int, height:int, png:blob");
	return m_sourceDb != NULL && m_pngDb != NULL;
}

void PngProductor::close()
{
	NdsDb_free(m_sourceDb);
	NdsDb_free(m_pngDb);
}

bool PngProductor::getImage( GridId gridId, ImageInfo& image )
{
	if (getImageFromDb(gridId, image)){
		return true;
	}

	vector<char> source;
	if (!readSourceData(gridId, source)){
		return false;
	}

	parse(gridId, source, image);
	writeImageToDb(gridId, image);
	return true;
}

bool PngProductor::getImageFromDb(GridId gridId, ImageInfo& image)
{
	NdsBlob pngData;
	size_t maxDataSize = NdsDb_getMaxFieldSize(m_pngDb, L"png");
	image.data.resize(maxDataSize);
	pngData.data = image.data.empty()? NULL: &image.data[0];
	if (NdsDb_readEntry32(m_pngDb, gridId, &image.width, &image.height, &pngData))
	{
		image.data.resize(pngData.size);
		return true;
	}
	else
	{
		return false;
	}
}

bool PngProductor::writeImageToDb(GridId gridId, const ImageInfo& image)
{
	NdsBlob pngData;
	pngData.size = image.data.size();
	pngData.data = (void*)&image.data[0];
	NdsDb_writeEntry32(m_pngDb, gridId, image.width, image.height, &pngData);
	return true;
}

bool PngProductor::readSourceData(GridId gridId, std::vector<char>& data)
{
	data.resize(NdsDb_getMaxGridSize(m_sourceDb));
	size_t len = NdsDb_read(m_sourceDb, gridId, &data[0], data.size());
	if (len != 0)
	{
		data.resize(len);
		return true;
	}
	else
	{
		return false;
	}

}

bool PngProductor::parse(GridId gridId, std::vector<char>& data, ImageInfo& image)
{
	GridDataParser parser;
	return parser.parse(gridId, image, &data[0], data.size());
	return true;
}
