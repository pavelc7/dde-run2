/*******************************************************************
	File: dde_run.c
	Author: Michael Polyakov
	Email: myp@andrew.cmu.edu mike.polyakov@gmail.com
	Website: www.angelfire.com/linux/myp
	Date: 1/18/2003
	Description: Connects to specified server with specified topic
		through DDE and executes up to 10 specified commands on the 
		server.
********************************************************************/




#include <windows.h>
#include <stdio.h>
#include <string.h>

void PrintUsage();
HDDEDATA CALLBACK DdeCallback(UINT uType,UINT uFmt,
							  HCONV hconv,HSZ hsz1,
							  HSZ hsz2,HDDEDATA hdata,
							  DWORD dwData1,DWORD dwData2);
#define COMMAND_MAX 10

int main(int argc, char* argv[])
{
	int i, ret=0, cmdI=0;
	char* szService = NULL;
	char* szTopic = NULL;
	char* szName = NULL;
	char* szCommand[COMMAND_MAX];
	UINT err;
	DWORD idInst = 0;
	HCONV hConv;
	HSZ hszService, hszTopic;
	HDDEDATA hData;

	for (i=1; i < argc; i++) {
		if (strcmp(argv[i], "-s") == 0)
			szService = argv[++i];
		else if (strcmp(argv[i], "-t") == 0)
			szTopic = argv[++i];
		else if (strcmp(argv[i], "-n") == 0)
			szName = argv[++i];
		else if (cmdI < COMMAND_MAX && strcmp(argv[i], "-c") == 0)
			szCommand[cmdI++] = argv[++i];
		else
			PrintUsage();
	}

	printf("DDE_run.exe - basic DDE execute - by Michael Polyakov\n");

	if (szService == NULL || szTopic == NULL || cmdI == 0) {
		PrintUsage();
		return 0;
	}

	if ((err = DdeInitialize(&idInst, 
					DdeCallback, 
					APPCLASS_STANDARD | 
					APPCMD_CLIENTONLY, 
					0)) != DMLERR_NO_ERROR) {
		printf("DDE initialization failed!\n");
		return 0;
	}
	
	printf("Connecting to %s...\n", szService);
	hszService = DdeCreateStringHandle(idInst, szService, CP_WINANSI);
	hszTopic = DdeCreateStringHandle(idInst, szTopic, CP_WINANSI);
	if ((hConv = DdeConnect(idInst, hszService, hszTopic, NULL)) == 0L) {
		printf("Could not connect to %s. Running program %s...\n", szService, szName);
		if (szName) {
			if (WinExec(szName,SW_SHOWNA) <= 31) {
				printf("Could not start program %s\n", szName);
				goto Program_Exit;
			} else if ((hConv = DdeConnect(idInst, hszService, hszTopic, NULL)) == 0L) {
				printf("Service name %s with topic %s was not found.\n", szService, szTopic);
				goto Program_Exit;
			}
		}
	}
	printf("Connected to service %s with topic %s.\n", szService, szTopic);
	
	for (i=0; i < cmdI; i++) {
		printf("Sending %s.........", szCommand[i]);
		if ((hData = DdeClientTransaction((LPBYTE)szCommand[i], strlen(szCommand[i])+1,hConv,
											NULL,0,XTYP_EXECUTE,3000,&err)) == 0)
			printf("Error 0x%x\n",DdeGetLastError(idInst));
		else {
			printf("Sent. Result: 0x%x\n",err);
			DdeFreeDataHandle(hData);
		}
	}
	
Program_Exit:
	DdeFreeStringHandle(idInst, hszService);
	DdeFreeStringHandle(idInst, hszTopic);
	DdeUninitialize(idInst);
	return ret;
}




HDDEDATA CALLBACK DdeCallback(UINT uType,UINT uFmt,HCONV hconv,
    HSZ hsz1,HSZ hsz2,HDDEDATA hdata,DWORD dwData1,DWORD dwData2)
{	return 0;	}


void PrintUsage()
{
	printf("Usage:\n\t-s  Specify service name\n"\
		"\t-t  Specify topic\n\t-n  Specify the name of program to run "\
		"if connection fails\n\t-c  Specify command to execute on server. Up to 10 allowed\n");
}