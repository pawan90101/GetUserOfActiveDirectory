/*
File Name:		GetMacAddress.cpp
Created By:		Pawan Kumar
Date:			12-2-2018  11:30 AM
Last Modified:
Desc:
Usage:
Copyright (c)2018 Pawan90101@gmail.com.
*/

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif


#include "GetMacAddress.h"
#include <WinSock2.h>
#include <string>
#include "iphlpapi.h"   //For SendARP
#pragma comment(lib , "iphlpapi.lib") //For iphlpapi
#pragma comment(lib,"ws2_32.lib")



void CGetMacAddress::GetMacAddress(string sIp, string &sMac)
{
	unsigned char mac[6];
	struct in_addr destip;
	WSADATA firstsock;

	if (WSAStartup(MAKEWORD(2, 2), &firstsock) != 0)
	{
		return;
	}

	destip.s_addr = inet_addr(sIp.c_str());

	//Get mac addresses of the ip
	DWORD ret;
	IPAddr srcip;
	ULONG MacAddr[2];
	ULONG PhyAddrLen = 6;  /* default to length of six bytes */
	int i;
	srcip = 0;

	//Send an arp packet
	ret = SendARP((IPAddr)destip.S_un.S_addr, srcip, MacAddr, &PhyAddrLen);

	//Prepare the mac address
	if (ret == NO_ERROR)
	{
		BYTE *bMacAddr = (BYTE *)& MacAddr;
		for (i = 0; i < (int)PhyAddrLen; i++)
		{
			mac[i] = (char)bMacAddr[i];
		}
		char ip_address[32];
		sprintf_s(ip_address, 32, "%.2X-%.2X-%.2X-%.2X-%.2X-%.2X", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
		sMac = ip_address;
	}
	WSACleanup();
}