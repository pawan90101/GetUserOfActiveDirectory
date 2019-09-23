
#include "activedirectoryusers.h"
#include <conio.h>
#include "GetIpAddress.h"
#include "GetMacAddress.h"
int main(int args, char *argv[])
{
	vector<CString> User;
	CActiveDirectoryUsers *ad = new CActiveDirectoryUsers();
	ad->GetADUsers(User);
	
	for (vector<CString>::iterator it = User.begin(); it != User.end(); it++)
	{
		_tprintf(_T("%s \n"), *it);
		string ips, sMac;
		wstring temp = CString(*it).GetBuffer();
		const std::string sHost(temp.begin(), temp.end());
		CGetIpAddress::GetIpAddress(sHost, ips);
		printf("%s \n", ips.c_str());
		CGetMacAddress::GetMacAddress(ips, sMac);
		printf("%s \n", sMac.c_str());
	}
	delete ad;
	_getch();
	return 0;
}