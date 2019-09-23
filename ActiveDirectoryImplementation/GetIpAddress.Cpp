/*
File Name:		GetIpAddress.Cpp
Created By:		Pawan Kumar
Date:			2-4-2018  10:34 AM
Last Modified:
Desc:
Usage:
Copyright (c)2018 Pawan90101@gmail.com.
*/
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif



#include "GetIpAddress.h"
#include <WinSock2.h>
#include <ws2tcpip.h>
#include <string>
#pragma comment(lib,"ws2_32.lib")


void CGetIpAddress::GetIpAddress(string sHostName, string &sIp)
{
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	IN_ADDR addr;
	HOSTENT* host; // To retrieve the IP Address 
	host = gethostbyname(sHostName.c_str());

	if (host != NULL)
	{
		memcpy(&addr.S_un.S_addr, host->h_addr, host->h_length);
		sIp = inet_ntoa(addr);
	}
	WSACleanup();
}