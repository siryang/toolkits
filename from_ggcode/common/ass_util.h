size_t getFileLength(FILE* fp)
{
	size_t tell = ftell(fp);
	size_t length = (fseek(fp, 0, SEEK_END), ftell(fp));
	fseek(fp, tell, SEEK_SET);
	return length;
}
