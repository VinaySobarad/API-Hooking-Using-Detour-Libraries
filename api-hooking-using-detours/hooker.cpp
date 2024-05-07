#include <stdio.h>
#include <windows.h>
#include "detours.h"
#pragma comment(lib, "user32.lib")

//-- pointer to the original MessageBox function
int (WINAPI * pOrigMessageBox)(HWND hWnd, LPCTSTR lpText, LPCTSTR lpCaption, UINT uType) = MessageBox;

//-- hook and unhook function prototypes
BOOL HookTarget(void);
BOOL UnHookTarget(void);

//-- the modified MessageBox function
int ModifiedMessageBox(HWND hWnd, LPCTSTR lpText, LPCTSTR lpCaption, UINT uType) {
	
	//printf("ModifiedMessageBox() called. No MessageBox popup on screen!\n");
	
	//pOrigMessageBox(hWnd, lpText, lpCaption, uType);
	
	if(strcmp(lpCaption, "2nd MessageBox")==0)
		pOrigMessageBox(hWnd, "I have been modified", lpCaption, MB_OK | MB_ICONERROR);
	
	return IDOK;
}

//-- Set hooks on the MessageBox function
BOOL HookTarget(void) {

    LONG hookingResult;

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&(PVOID&)pOrigMessageBox, ModifiedMessageBox);
	hookingResult = DetourTransactionCommit();

	printf("MessageBox() hooked and hooking ressult = %d\n", hookingResult);
	
	return TRUE;
}

//-- Undo the hooks and revert all changes to original code
BOOL UnHookTarget(void) {
	
	LONG unhookingResult;
	
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourDetach(&(PVOID&)pOrigMessageBox, ModifiedMessageBox);
	unhookingResult = DetourTransactionCommit();

	printf("Hook removed from MessageBox() with un-hooking result = %d\n", unhookingResult);
	
	return TRUE;
}

BOOL WINAPI DllMain(HINSTANCE hinst, DWORD dwReason, LPVOID reserved) {

    switch (dwReason)  {
		case DLL_PROCESS_ATTACH:
			HookTarget();
			break;
			
		case DLL_THREAD_ATTACH:
			break;
			
		case DLL_THREAD_DETACH:
			break;
			
		case DLL_PROCESS_DETACH:
			UnHookTarget();
			break;
	}
	
    return TRUE;
}

