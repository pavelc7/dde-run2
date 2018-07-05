//
//	File: dde_run2.cpp
//	Author of first version: Michael Polyakov
//	Modified by: Pavel Ch
//	Email: pavelch@gmail.com mike.polyakov@gmail.com
//	Website: www.angelfire.com/linux/myp
//	Website: https://code.google.com/p/dde-run2/
//	Date: 1/18/2003
//	Date of modifying: 12 Dec 2014
//	Description: Connects to specified server with specified topic
//		through DDE and executes up to 5 specified commands on the 
//		server.
//


#include <tchar.h>
#include <string.h>
#include <windows.h>
#include <stdio.h>

#define MAXCOMMANDS 5

HDDEDATA CALLBACK DdeCallback(
  _In_  UINT uType,
  _In_  UINT uFmt,
  _In_  HCONV hconv,
  _In_  HSZ hsz1,
  _In_  HSZ hsz2,
  _In_  HDDEDATA hdata,
  _In_  ULONG_PTR dwData1,
  _In_  ULONG_PTR dwData2
)
{	
	return 0;
}


void usage()
{
	printf("Usage:\n"\
		"\t-s  Specify service name\n"\
		"\t-t  Specify topic\n"\
		"\t-p  Specify the path name to program if connection fails\n"\
		"\t-r  Specify request to execute on server\n"\
		"\t-c  Specify command to execute on server. Up to %d allowed\n",MAXCOMMANDS);
}
int _tmain(int argc, _TCHAR* argv[])
{
	int i, ret=0, cmdI=0;
	TCHAR* szService = NULL;
	TCHAR* szTopic = NULL;
	TCHAR* szProgram = NULL;
	TCHAR* szCommand[MAXCOMMANDS];
	TCHAR* szRequest = NULL;
	DWORD err = 0;
	DWORD idInst = 0;
	HCONV hConv;
	HSZ hszService, hszTopic,hszRequest;
	HDDEDATA hData;	
	int bExit=0;
	for (i=1; i < argc; i++) {
		if (_tcscmp(argv[i], _T("-s")) == 0)
			szService = argv[++i];
		else if (_tcscmp(argv[i], _T("-t")) == 0)
			szTopic = argv[++i];
		else if (_tcscmp(argv[i], _T("-p")) == 0)
			szProgram = argv[++i];
		else if (_tcscmp(argv[i], _T("-r")) == 0)
			szRequest = argv[++i];
		else if (cmdI < MAXCOMMANDS && _tcscmp(argv[i], _T("-c")) == 0)
			szCommand[cmdI++] = argv[++i];
		else
			usage();
	}

	//printf("dde_run2.exe\n");

	if (szService == NULL || szTopic == NULL || cmdI == 0) {
		usage();
		return 0;
	}

	if ((err = DdeInitialize(&idInst, 
					DdeCallback, 
					APPCLASS_STANDARD | 
					APPCMD_CLIENTONLY, 
					0)) != DMLERR_NO_ERROR) {
		printf("Error: DDE initialization failed!\n");
		return 0;
	}
	
	//printf("Connecting to %s\n", szService);
	hszService = DdeCreateStringHandle(idInst, szService, CP_WINANSI);
	hszTopic = DdeCreateStringHandle(idInst, szTopic, CP_WINANSI);
	if ((hConv = DdeConnect(idInst, hszService, hszTopic, NULL)) == 0L) {
		printf("Error: Could not connect to %s. Running program %s\n", szService, szProgram);
		if (szProgram) {
			if (WinExec(szProgram,SW_SHOWNA) <= 31) {
				printf("Error: Could not start program %s\n", szProgram);
				bExit=1;
			} else if ((hConv = DdeConnect(idInst, hszService, hszTopic, NULL)) == 0L) {
				printf("Error: Service name %s with topic %s was not found.\n", szService, szTopic);
				bExit=1;
			}
		}
	}
	//printf("Connected to service %s with topic %s.\n", szService, szTopic);
	if(!bExit)
	{
		for (i=0; i < cmdI; i++) {
			//printf("Sending %s", szCommand[i]);
			if ((hData = DdeClientTransaction((LPBYTE)szCommand[i], strlen(szCommand[i])+1,hConv,
												NULL,0,XTYP_EXECUTE,3000,&err)) == 0)
			{
				printf("Error 0x%x\n",DdeGetLastError(idInst));				
			}
			else {
				DdeFreeDataHandle(hData);
			}
		}
		if (szRequest) {						
			hszRequest = DdeCreateStringHandle(idInst, szRequest, CP_WINANSI);
			if ((hData = DdeClientTransaction(0, 0,hConv,
									hszRequest,CF_TEXT,XTYP_REQUEST,3000,&err)) == 0)
				{
					printf("Error 0x%x\n",DdeGetLastError(idInst));				
				}
				else {					
					DWORD data_len;
					char*pstr = (char*)DdeAccessData( hData, &data_len );
        			printf("%s",pstr);
					DdeUnaccessData( hData );        			
					DdeFreeDataHandle(hData);
				}
		}
	}

	DdeFreeStringHandle(idInst, hszService);
	DdeFreeStringHandle(idInst, hszTopic);
	DdeUninitialize(idInst);
	return ret;
}




