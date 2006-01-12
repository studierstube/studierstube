/* ========================================================================
 * Copyright (C) 2004-2005  Graz University of Technology
 *
 * This framework is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This framework is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this framework; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * For further information please contact Dieter Schmalstieg under
 * <schmalstieg@icg.tu-graz.ac.at> or write to Dieter Schmalstieg,
 * Graz University of Technology, Institut für Maschinelles Sehen und Darstellen,
 * Inffeldgasse 16a, 8010 Graz, Austria.
 * ========================================================================
 * PROJECT: Studierstube Application Manager (StbAM, SAM)
 * ======================================================================== */
/** @author   Daniel Wagner
 *
 * $Id: main.cpp 131 2005-11-03 20:53:09Z daniel $
 * @file                                                                   */
/* ======================================================================= */


/// AppStarter downloads the application master DLL, imports and execetutes it
/**
 *  The application master DLL is the first code that is downloaded from the server.
 *  AppStarter.exe file is the only code that can not be retrieved from the server
 *  since it contains the basic (and very simple) boot strapping implementation.
 *  AppStarter does not use ACE for downloading file. Therefore everything is done
 *  directly via SOCKET programming.
 */

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#include <windows.h>
#include <winsock.h>
#include <stdio.h>

#include "../AppMaster/ApplicationMaster.h"
#include "../AppMaster/PlatformTools.h"

#include <pocketknife/ProcessInfo.h>

#define PORT_NUM 80

typedef SAM::ApplicationMaster* (CREATEFUNC)();


static char debugFileName[1024];
static WCHAR wdebugFileName[1024];


SAM::ApplicationMaster::StartupInfo startupInfo;


unsigned int getFreeMemory(bool nVirt = true);
MEMORYSTATUS memInfo;

unsigned int
getFreeMemory(bool nVirt)
{
	memInfo.dwLength = sizeof(MEMORYSTATUS);
	GlobalMemoryStatus(&memInfo);

	if(nVirt)
		return (unsigned int)memInfo.dwAvailVirtual;
	else
		return (unsigned int)memInfo.dwAvailPhys;
}


const char*
skipFirstSlash(const char* nPath)
{
	return nPath[0]=='/' ? nPath+1 : nPath;
}

const WCHAR*
makeDebugFileName(const WCHAR* nFileName)
{
#if defined(_DEBUG) || defined(DEBUG)
	size_t len = wcslen(nFileName);
	if(wcsncmp(nFileName+len-3, L"dll", 3)==0)
	{
		memcpy(wdebugFileName, nFileName, 2*(len-4));
		wdebugFileName[len-4] = 'D';
		wdebugFileName[len-3] = '.';
		memcpy(wdebugFileName+len-2, L"dll", 2*3);
		wdebugFileName[len+1] = 0;
		return wdebugFileName;
	}
#endif //_DEBUG

	return nFileName;
}


const char*
makeDebugFileName(const char* nFileName)
{
#if defined(_DEBUG) || defined(DEBUG)
	size_t len = strlen(nFileName);
	if(strncmp(nFileName+len-3, "dll", 3)==0)
	{
		memcpy(debugFileName, nFileName, len-4);
		debugFileName[len-4] = 'D';
		debugFileName[len-3] = '.';
		memcpy(debugFileName+len-2, "dll", 3);
		debugFileName[len+1] = 0;
		return debugFileName;
	}
#endif //_DEBUG

	return nFileName;
}


WCHAR*
getStartupPath()
{
	const int maxLen = 512;
	WCHAR* filename = new WCHAR[maxLen+1];

	::GetModuleFileNameW(NULL, filename, maxLen);

	for(size_t len=wcslen(filename)-1; len>=0; len--)
		if(filename[len]=='\\' || filename[len]=='/')
		{
			filename[len+1] = 0;
			break;
		}

	return filename;
}


bool
checkDLL(const WCHAR* nDLL, bool nMakeDebug, bool nAddPath)
{
	const WCHAR* tmpName = nMakeDebug ? makeDebugFileName(nDLL) : nDLL;
	WCHAR* fullPath = getStartupPath();
	WCHAR str[256];

	if(nAddPath)
		wcscat(fullPath, tmpName);
	else
		wcscpy(fullPath, tmpName);

	HANDLE hFile = 0;

	if(!nAddPath)
	{
#ifdef _WIN32_WCE
		// under WinCE we try the system folder too...
		//
		wcscpy(fullPath, L"\\Windows\\");
		wcscat(fullPath, tmpName);

		hFile = ::CreateFileW(fullPath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
		wcscpy(fullPath, tmpName);
#endif
	}

	if(nAddPath)
	{
		if(hFile == INVALID_HANDLE_VALUE || hFile==0)
			hFile = ::CreateFileW(fullPath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	}

	if(hFile == INVALID_HANDLE_VALUE)
	{
		LPVOID lpMsgBuf;
		FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM|FORMAT_MESSAGE_IGNORE_INSERTS, NULL, GetLastError(), 0, (LPWSTR)&lpMsgBuf, 0, NULL);

		swprintf(str, L"Missing DLL:\n\n%s\n\ndoes not exist.\n\nWindows reports: %s", fullPath, lpMsgBuf);
		MessageBoxW(NULL, str, L"STARTUP ERROR", MB_OK);

		LocalFree(lpMsgBuf);
		delete fullPath;
		return false;
	}

	CloseHandle(hFile);


	HMODULE hLib = ::LoadLibraryW(fullPath);
	if(!hLib)
	{
		LPVOID lpMsgBuf;
		DWORD lastErr = GetLastError();
		FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM|FORMAT_MESSAGE_IGNORE_INSERTS, NULL, lastErr, 0, (LPWSTR)&lpMsgBuf, 0, NULL);

		swprintf(str, L"Failed to load\n\n%s\n\nDLL exists but can not be loaded.\n\nWindows reports: %s (code: %d)", fullPath, lpMsgBuf, lastErr);

		MessageBoxW(NULL, str, L"STARTUP ERROR", MB_OK);
		LocalFree(lpMsgBuf);
		delete fullPath;
		return false;
	}

	delete fullPath;
	return true;
}


bool checkBaseDLLs()
{
	const unsigned int num = 10;
	size_t freeMem=0, freeMem0 = getFreeMemory();
	PN::ProcessInfo pi;

	startupInfo.freePhysicalBeforeModuless = memInfo.dwAvailPhys;
	startupInfo.freeVirtualBeforeModuless = memInfo.dwAvailVirtual;

	pi.takeSnapshot(PN::ProcessInfo::INFO_ALL);
	PN::ProcessInfo::Modules *piMods = pi.getModules(true);
	PN::ProcessInfo::Modules::SUMMARY summary0 = piMods->getSummary();
	pi.releaseSnapshot();


	freeMem = getFreeMemory();


#ifdef REAL_OPENGL

	OutputDebugStringW(L">>> OpenGL32.dll\n");
	if(!checkDLL(L"OpenGL32.dll", false, false))
		return false;

	freeMem = getFreeMemory();

	OutputDebugStringW(L">>> coin2.dll\n");
	if(!checkDLL(L"coin2.dll", true, false))
		return false;

#else

	OutputDebugStringW(L">>> KlimtES.dll\n");
	if(!checkDLL(L"KlimtES.dll", true, false))
		return false;

	freeMem = getFreeMemory();

	OutputDebugStringW(L">>> coin2es.dll\n");
#  ifdef _WIN32_WCE
	if(!checkDLL(L"coin2es.dll", true, false))
		return false;
#  else
	if(!checkDLL(L"Coin2ES.dll", true, false))
		return false;
#  endif

#endif //REAL_OPENGL

	///////////////////////////////////////////
	//
	//      test start
/*
	freeMem = getFreeMemory();

	if(!checkDLL(L"KlimtES.dll", false, false))
		return false;

	freeMem = getFreeMemory();

	if(!checkDLL(L"coin2esklimt.dll", false, true))
		return false;

	freeMem = getFreeMemory();

	if(!checkDLL(L"ace.dll", true, true))
		return false;

	freeMem = getFreeMemory();

	if(!checkDLL(L"ace.dll", false, true))
		return false;


	unsigned char* memBuf[num];
	for(i=0; i<num; i++)
		memBuf[i] = 0;
	for(i=0; i<num; i++)
	{
		memBuf[i] = new unsigned char[510*1024];
		if(!memBuf[i])
			break;
	}
*/
	//
	//      test end
	///////////////////////////////////////////



	freeMem = getFreeMemory();

	OutputDebugStringW(L">>> XMLClient.dll\n");
	if(!checkDLL(L"XMLClient.dll", true, false))
		return false;

	freeMem = getFreeMemory();

	OutputDebugStringW(L">>> TinyXML_Mod.dll\n");
	if(!checkDLL(L"TinyXML_Mod.dll", true, false))
		return false;

	freeMem = getFreeMemory();

	OutputDebugStringW(L">>> ARToolKitPlusDll.dll\n");
	if(!checkDLL(L"ARToolKitPlusDll.dll", false, false))
		return false;

	freeMem = getFreeMemory();

#ifdef _WIN32_WCE
	OutputDebugStringW(L">>> FMODCE.dll\n");
	if(!checkDLL(L"fmodce.dll", false, false))
		return false;
#else
	OutputDebugStringW(L">>> FMOD.dll\n");
	if(!checkDLL(L"fmod.dll", false, false))
		return false;
#endif

	freeMem = getFreeMemory();


#ifdef REAL_OPENGL

	OutputDebugStringW(L">>> AppMasterSAGL.dll\n");
#  ifdef SAM_STANDALONE
	if(!checkDLL(L"ApplicationMaster/AppMasterSAGL.dll", true, true))
		return false;
#  else
	if(!checkDLL(L"ApplicationMaster/AppMasterGL.dll", true, true))
		return false;
#  endif

#else

	OutputDebugStringW(L">>> AppMasterSA.dll\n");
#  ifdef SAM_STANDALONE
	if(!checkDLL(L"ApplicationMaster/AppMasterSA.dll", true, true))
	//if(!checkDLL(L"AppMasterSA.dll", true, false))
		return false;
#  else
	if(!checkDLL(L"ApplicationMaster/AppMaster.dll", true, true))
		return false;
#  endif

#endif //REAL_OPENGL

	//freeMem = getFreeMemory();
	//if(!checkDLL(L"Applications/Flash/AppFlash.dll", true, true))
	//	return false;


	OutputDebugStringW(L">>> Coin2ESExt.dll\n");
	if(!checkDLL(L"Coin2ESExt.dll", true, false))
		return false;


	freeMem = getFreeMemory();

	pi.takeSnapshot(PN::ProcessInfo::INFO_ALL);
	piMods = pi.getModules(true);
	PN::ProcessInfo::Modules::SUMMARY summary1 = piMods->getSummary();
	pi.releaseSnapshot();

	// NOTE: this value makes only sense under WinCE where DLLs are loaded in
	// order top-to-down from the 32MB process border...
	//
	int memUsageByDLLs = (char*)summary0.lowestBaseAddress - (char*)summary1.lowestBaseAddress;
	int memLeftForGeneralUsage = (int)summary1.lowestBaseAddress;

	startupInfo.freePhysicalAfterModuless = memInfo.dwAvailPhys;
	startupInfo.freeVirtualAfterModuless = memInfo.dwAvailVirtual;
	startupInfo.lowestModuleBeforeModules = (int)summary0.lowestBaseAddress;
	startupInfo.lowestModuleAfterModules = (int)summary1.lowestBaseAddress;

	return true;
}


#ifndef SAM_STANDALONE
int
downloadFile(const char* nServerAddr, const char* nPath, char* nDestBuffer)
{
	int index = 0,                      // Integer index
		iReturn,                        // Return value of recv function
		iSum = 0;						// Sum of all bytes received
	TCHAR szError[100];                 // Error message string
	SOCKET ServerSock = INVALID_SOCKET; // Socket bound to the server
	SOCKADDR_IN destination_sin;        // Server socket address
	PHOSTENT phostent = NULL;           // Points to the HOSTENT structure
										// of the server
	char buffer[2048];					// download buffer
	FILE* fp  = NULL;					// output file
	char http_command[1024];			// http command strinf


	// Create a TCP/IP socket that is bound to the server.
	if ((ServerSock = socket (AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
	{
		wsprintf(szError, TEXT("AppStarter: Allocating socket failed. Error: %d\nShutting down StbAM"), WSAGetLastError ());
		MessageBox(NULL, szError, TEXT("Error"), MB_OK);
		return -1;
	}

	// Fill out the server socket's address information.
	destination_sin.sin_family = AF_INET;

	// Retrieve the host information corresponding to the host name.
	if ((phostent = gethostbyname (nServerAddr)) == NULL) 
	{
		wsprintf(szError, TEXT("AppStarter: Unable to get the host name. Error: %d\nShutting down StbAM"), WSAGetLastError ());
		MessageBox(NULL, szError, TEXT("Error"), MB_OK);
		closesocket(ServerSock);
		return -1;
	}

	// Assign the socket IP address.
	memcpy((char FAR *)&(destination_sin.sin_addr), phostent->h_addr, phostent->h_length);

	// Convert to network ordering.
	destination_sin.sin_port = htons(PORT_NUM);      

	// Establish a connection to the server socket.
	if(connect(ServerSock, (PSOCKADDR) &destination_sin, sizeof (destination_sin)) == SOCKET_ERROR) 
	{
		wsprintf(szError, TEXT("AppStarter: Connecting to the server failed. Error: %d\nShutting down StbAM"), WSAGetLastError ());
		MessageBox(NULL, szError, TEXT("Error"), MB_OK);
		closesocket(ServerSock);
		return -1;
	}

	sprintf(http_command, "GET %s\n", nPath);

	// Send a string to the server.
	if(send(ServerSock, http_command, strlen(http_command), 0) == SOCKET_ERROR) 
	{
		wsprintf(szError, TEXT("AppStarter: Sending data to the server failed. Error: %d\nShutting down StbAM"), WSAGetLastError ());
		MessageBox(NULL, szError, TEXT("Error"), MB_OK);
		closesocket(ServerSock);
		return -1;
	}


	// Disable sending on ServerSock.
	shutdown(ServerSock, 0x01);

	if(!nDestBuffer)
	{

		// split the directory from the path
		//
		strcpy(buffer, nPath);
		for(int i=strlen(buffer); i>=0; i--)
			if(buffer[i]=='/')
			{
				buffer[i] = 0;
				break;
			}

		// create the directory
		//
		CreateDirectory(skipFirstSlash(buffer), NULL);


		fp = fopen(skipFirstSlash(nPath), "wb");
		if(!fp)
		{
			wsprintf(szError, TEXT("Could not open file '%s' for writing"), nPath);
			MessageBox(NULL, szError, TEXT("Error"), MB_OK);
			closesocket(ServerSock);
			return -1;
		}
	}

	for(;;)
	{
		// Receive data from the server socket.
		iReturn = recv(ServerSock, buffer, sizeof(buffer), 0);

		// Check if there is any data received. If there is, display it.
		if(iReturn == SOCKET_ERROR)
		{
			wsprintf(szError, TEXT("No data is received, recv failed.") TEXT(" Error: %d"), WSAGetLastError ());
			MessageBox (NULL, szError, TEXT("Client"), MB_OK);
			break;
		}
		else if(iReturn == 0)
		{
			break;
		}
		else
		{
			if(nDestBuffer)
				memcpy(nDestBuffer+iSum, buffer, iReturn);
			else
			{
				size_t numBytes = fwrite(buffer, 1, iReturn, fp);

				if(numBytes != (unsigned)iReturn)
				{
					wsprintf(szError, TEXT("Failed to write file '%s'"), nPath);
					MessageBox(NULL, szError, TEXT("Error"), MB_OK);
					closesocket(ServerSock);
					return -1;
				}
			}

			iSum += iReturn;
		}
	}

	if(fp)
		fclose(fp);

	if(nDestBuffer)
		nDestBuffer[iSum] = 0;

	// Disable receiving on ServerSock.
	shutdown(ServerSock, 0x00);

	// Close the socket.
	closesocket(ServerSock);


	return iSum;
}
#endif //!SAM_STANDALONE


bool
getNextLine(const char*& nInfo, char* nLine)
{
	int len = strlen(nInfo), i=0;

	if(len==0)
		return false;

	while(i<len)
		if(nInfo[i++]=='\n')
			break;
	i=i;

	if(nInfo[i-1]=='\n')
	{
		memcpy(nLine, nInfo, i-2);
		nLine[i-2] = 0;
		nInfo += i;
		return true;
	}
	else
		return false;
}


void
getStartupPath(char* nPath, int nMaxLen)
{
	const int maxLen = 256;
	WCHAR *filename = new WCHAR[maxLen+1];

	::GetModuleFileNameW(NULL, filename, maxLen);

	int i, len;

	for(len=wcslen(filename)-1; len>=0; len--)
		if(filename[len]=='\\' || filename[len]=='/')
			break;

	len++;

	len = len>nMaxLen-1 ? nMaxLen-1 : len;

	for(i=0; i<len; i++)
		nPath[i] = (char)filename[i];
	nPath[len] = 0;
}


#ifndef SAM_STANDALONE
bool
downloadApplicationMaster(const char* nServerAddr, const char* nPath)
{
	TCHAR szError[100];                 // Error message string
	WSADATA WSAData;                    // Contains details of the Winsocket implementation
	char info_txt[512], *curInfo = &info_txt[0], line[256];

	// Initialize Winsocket. 
	if(WSAStartup(MAKEWORD(1,1), &WSAData) != 0) 
	{
		wsprintf(szError, TEXT("WSAStartup failed. Error: %d"), WSAGetLastError ());
		MessageBox(NULL, szError, TEXT("Error"), MB_OK);
		return false;
	}

	if(downloadFile(nServerAddr, nPath, info_txt)<=0)
		return false;

	while(getNextLine(curInfo, line))
	{
		if(downloadFile(nServerAddr, makeDebugFileName(line), NULL)<=0)
			return false;
	}

	WSACleanup();

	return true;
}
#endif //SAM_STANDALONE


int
executeApplicationMaster(HINSTANCE nHInstance, const char* nPath)
{
	DWORD freeMem = getFreeMemory();

	//OutputDebugStringW(L">>> checkBaseDLLs()\n");
	//if(!checkBaseDLLs())
	//	return -1;

	const int maxLen = 1024;
	char* fullPath = new char[maxLen];

	getStartupPath(fullPath, 512);
	strcat(fullPath, skipFirstSlash(nPath));

	const TCHAR *tmpPath = CHAR_TO_NATIVE(fullPath);

	OutputDebugStringW(L">>> LoadLibrary()\n");
	HMODULE hLib = ::LoadLibrary(tmpPath);
	delete fullPath;

	if(!hLib)
	{
		DWORD errorID = GetLastError();
		::MessageBox(NULL, TEXT("Could not load Application Framework"), TEXT("ERROR"), MB_OK);
		return 1;
	}

//#if (_MSC_VER>=1400)
	CREATEFUNC *createFunc = (CREATEFUNC*)GetProcAddress(hLib, CHAR_TO_DLLFUNC_STR("createApplicationMaster"));
//#else
//	CREATEFUNC *createFunc = (CREATEFUNC*)GetProcAddress(hLib, TEXT("createApplicationMaster"));
//#endif

	if(!createFunc)
	{
		::MessageBox(NULL, TEXT("Getting DLL Entrypoints failed"), TEXT("ERROR"), MB_OK);
		::FreeLibrary(hLib);
		return 1;
	}

	OutputDebugStringW(L">>> createFunc()\n");
	SAM::ApplicationMaster* appMaster = createFunc();
	if(!appMaster)
	{
		::MessageBox(NULL, TEXT("Creating Application Master failed"), TEXT("ERROR"), MB_OK);
		::FreeLibrary(hLib);
		return 1;
	}


	appMaster->setStartupInfo(startupInfo);
	OutputDebugStringW(L">>> init()\n");
	if(!appMaster->init(nHInstance))
	{
		::MessageBox(NULL, TEXT("Application Master could not be initialized"), TEXT("ERROR"), MB_OK);
		::FreeLibrary(hLib);
		return 1;
	}

	OutputDebugStringW(L">>> run()\n");
	appMaster->run();

	//::FreeLibrary(hLib);
	return 0;
}


#ifndef SAM_STANDALONE
void readLine(FILE* nFP, char* nLine, int nMaxLen)
{
	char ch=0;
	int i=0;

	while(!feof(nFP) && i<nMaxLen-1)
	{
		ch = fgetc(nFP);
		if(ch=='\n')
			break;

		nLine[i++] = ch;
	}

	nLine[i] = 0;
}


const char* getHostAddress()
{
	static char hostAddr[256];
	char line[256];
	const char* searchStr = "initial-server:";
	int searchLen = strlen(searchStr);


	FILE* fp = fopen("client_setup.txt", "r");
	if(fp)
	{
		while(!feof(fp))
		{
			readLine(fp, line, 255);
			if(strncmp(line, searchStr, searchLen)==0)
			{
				int i=searchLen, lineLen=strlen(line);
				while(i<lineLen)
					if(line[i]==' ')
						i++;
					else
						break;

				strcpy(hostAddr, line+i);
				break;
			}

		}

		fclose(fp);
	}
	else
		strcpy(hostAddr, "127.0.0.1");

	return hostAddr;
}


#endif //SAM_STANDALONE


#include <pocketknife/Heap.h>


#ifdef _WIN32_WCE
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
#else
int  WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
#endif
{
//#ifdef REAL_OPENGL
//
//#  ifndef SAM_STANDALONE
//	if(!downloadApplicationMaster(getHostAddress(), "/applicationmaster_files.txt"))
//		return 1;
//
//	return executeApplicationMaster(hInstance, makeDebugFileName("/ApplicationMaster/AppMasterGL.dll"));
//
//#  else
//	return executeApplicationMaster(hInstance, makeDebugFileName("/ApplicationMaster/AppMasterSAGL.dll"));
//#  endif //SAM_STANDALONE
//
//#else
//
//#  ifndef SAM_STANDALONE
//	if(!downloadApplicationMaster(getHostAddress(), "/applicationmaster_files.txt"))
//		return 1;

	return executeApplicationMaster(hInstance, makeDebugFileName("/ApplicationMaster/AppMaster.dll"));

//#  else
//	return executeApplicationMaster(hInstance, makeDebugFileName("/ApplicationMaster/AppMasterSA.dll"));
//#  endif //SAM_STANDALONE
//
//#endif //REAL_OPENGL
}
