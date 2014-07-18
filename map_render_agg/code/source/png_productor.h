#pragma once
#include "mapr_basic_type.h"
extern "C"
{
	#include "util/nds_db.h"
}

typedef uint32 GridId;
class PngProductor
{
public:
	PngProductor();
	bool open(const wchar_t* sourceDb, const wchar_t* pngDb);
	bool getImage(GridId gridId, ImageInfo& image);
	void close();

private:
	bool getImageFromDb(GridId gridId, ImageInfo& image);
	bool writeImageToDb(GridId gridId, const ImageInfo& image);
	bool readSourceData(GridId gridId, std::vector<char>& data);
	bool parse(GridId gridId, std::vector<char>& data, ImageInfo& image);

private:
	NdsDb *m_sourceDb, *m_pngDb;
};