#include <windows.h>
#include <stdint.h>
#include <stdio.h>
#include <conio.h>

/* Banner generated at manytools.org*/

void banner(){
	
	puts("\n\n");
	puts("	 d8b 888                        888                  ");
	puts("         Y8P 888                        888                  ");
	puts("             888                        888      ");            
	puts(" .d8888b 888 888  .d88b.   8888b.   .d88888  .d88b.  888d888 ");
	puts("d88P\"    888 888 d88\"\"88b     \"88b d88\" 888 d8P  Y8b 888P\"   ");
	puts("888      888 888 888  888 .d888888 888  888 88888888 888     ");
	puts("Y88b.    888 888 Y88..88P 888  888 Y88b 888 Y8b.     888     ");
	puts(" \"Y8888P 888 888  \"Y88P\" \"Y888888  \"Y88888  \"Y8888   888    "); 
	puts("\n\t\t\tCoded by: Aleksandar");
	puts("\t\t\tVisit:\t  https://github.com/revengsmK");
	puts("\n");
	
}

int main(void){
	
	unsigned char buffer[7];
	unsigned char patch_byte[] = { 0x01 };
	int i;
	
	PROCESS_INFORMATION procInfo;
	STARTUPINFO startInfo;
	
	banner();
	
	GetStartupInfo(&startInfo);

	if(!CreateProcess(NULL,"ciprotect.exe -unset block_everything -unset block_harmful_sites -unset block_selected_resources -unset block_time_change",
					  NULL,NULL,FALSE,CREATE_SUSPENDED | CREATE_NO_WINDOW | NORMAL_PRIORITY_CLASS,NULL,NULL,&startInfo,&procInfo))
	{
		printf("\nFailed to create process!\n\n");
		
		if(GetLastError() == 2 )
			printf("Cannot find ciprotect.exe!\n\nCopy loader in the same directory where ciprotect.exe is located and try again.\n");
		
		getch();
		return -1;
	}
	
	printf("\nciprotect.exe PID: %d\n\n",(unsigned int)procInfo.dwProcessId);
	
	// No ASLR, use hardcoded addresses
	
	if(!ReadProcessMemory(procInfo.hProcess,(char*)0x004533C3,buffer,7,NULL))
		puts("Failed to read process memory!\n");
	
	else 
	{
		printf("-> Reading bytes at 0x004533C3: ");
		
		for(i = 0 ; i < 7 ; i++)
		   printf("0x%02X ", buffer[i]); 
	}
		
	puts("\n\n-> Attempting to write byte to a running process...\n");
	
	// Patch ciprotect.exe in memory
	
	if(!WriteProcessMemory(procInfo.hProcess,(char*)0x004533C9,patch_byte,1,NULL))
		puts("Failed to write to process memory!\n");
	else
	{
		puts("-> Success! All Surfblocker protections disabled.\n\n");
		FlushInstructionCache(procInfo.hProcess,NULL,0);
	}
	
	// Continue execution after patching
	
	ResumeThread(procInfo.hThread);
	
	CloseHandle(procInfo.hThread);	
	CloseHandle(procInfo.hProcess);

	puts("Press any key to quit....\n");
	getch();
	
	return 0;
}