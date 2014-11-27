int PreMain1(void)
{
	cout << "PreMain1" << endl;
	return 0;
}

int PreMain2(void)
{
	cout << "PreMain2" << endl;
	return 0;
}

int PostMain1(void)
{
	cout << "PostMain1" << endl;
	return 0;
}

int PostMain2(void)
{
	cout << "PostMain2" << endl;
	return 0;
}

typedef int cb(void);

#pragma data_seg(".CRT$XIU")
static cb *autostart[] = { PreMain1, PreMain2 };

#pragma data_seg(".CRT$XPU")
static cb *autoexit[] = { PostMain1, PostMain2 };

#pragma data_seg()
