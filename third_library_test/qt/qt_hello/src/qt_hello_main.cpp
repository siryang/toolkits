#include "qt_hello_stdafx.h"

int WinMain( __in HINSTANCE hInstance, __in_opt HINSTANCE hPrevInstance, __in_opt LPSTR lpCmdLine, __in int nShowCmd)
{
	hInstance,hPrevInstance,lpCmdLine,nShowCmd;
	int ret = 0;
	
	ret = QApplication::exec();

	cout << "Hi" << endl;
	return ret;
}
