/*
Refrence  https://msdn.microsoft.com/en-us/library/windows/desktop/ms676882(v=vs.85).aspx
*/

#pragma once
#include "Header.h"

class CActiveDirectoryUsers
{
public:
	CActiveDirectoryUsers(void);
	~CActiveDirectoryUsers(void);
	bool GetADUsers(vector<CString>&m_UserArray);
private:
	bool FindUsers(vector<CString>*pUserArray,IDirectorySearch *pContainerToSearch);
};
