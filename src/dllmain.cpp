/*
Shows NFS U2 chart in CV (cavalo-vapor) and Kgfm (Kilograma força-metro)

MIT License

Copyright (c) 2020 Fabio3rs

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
#include <cstdint>
// https://github.com/thelink2012/injector
#include <injector\injector.hpp>
#include <injector\assembly.hpp>
#include <injector\hooking.hpp>
#include <injector\calling.hpp>
#include <Windows.h>


// 0x0054AE3F

auto getText = injector::fastcall<char*(int, unsigned int)>::call<0x004FF9D0>;

//double &kwToBhp = *(double*)0x79BEF0;
//float &nmToftlb = *(float*)0x79BEFC;

double kwToBhp = 1.34102209;
float nmToftlb = 0.73756212;
double kwToCv = 1.35962;
double nmToKgfm = 0.101972;

double graphicTorqueMax = 813.49077 * 0.101972;
double graphicPowerMax = 650;

char modByFabio[] = { "Mod by Fabio - http://brmodstudio.forumeiros.com/" };

bool changeText()
{
	char *ch = getText(0, 0x5B81D5BAu);

	if (ch)
	{
		strcpy(ch, "cv");
	}

	char *ch2 = getText(0, 0x5B81D613u);

	if (ch2)
	{
		strcpy(ch2, "kgfm");
	}

	return true;
}

void changeTextSingle()
{
	static bool res = changeText();
}

void __declspec(naked) testHook()
{
	__asm
	{
		test al, al
		jnz metric
		fmul   kwToBhp
		ret

		metric:
		fmul   kwToCv
		pushad
		pushfd
			call changeTextSingle
		popfd
		popad
		ret
	}
}

void __declspec(naked) testHook1()
{
	__asm
	{
		cmp bl, 1
		jz metric1
		fmul   kwToBhp
		ret

		metric1:
		fmul   kwToCv
			pushad
			pushfd
			call changeTextSingle
			popfd
			popad
			ret
	}

}

void __declspec(naked) testHook2()
{
	__asm
	{
		test al, al
		jnz metric2
		fmul   nmToftlb
		ret

		metric2:
		fmul   nmToKgfm
			pushad
			pushfd
			call changeTextSingle
			popfd
			popad
			ret
	}
}

void __declspec(naked) testHook3()
{
	__asm
	{
		cmp bl, 1
		jz metric3
		fmul   nmToftlb
		ret

		metric3:
		fmul   nmToKgfm
			pushad
			pushfd
			call changeTextSingle
			popfd
			popad
			ret
	}
}

void __declspec(naked) testHook4()
{
	__asm
	{
		test al, al
		jz metric4
		fmul   kwToBhp
		ret

		metric4:
		fmul   kwToCv
			pushad
			pushfd
			call changeTextSingle
			popfd
			popad
			ret
	}
}

void __declspec(naked) testHook5()
{
	__asm
	{
		test al, al
		jz metric5
		fmul   nmToftlb
		ret

		metric5:
		fmul   nmToKgfm
			pushad
			pushfd
			call changeTextSingle
			popfd
			popad
			ret
	}
}

void makeHook(uintptr_t mem, size_t size)
{
	injector::MakeNOP(mem, size);
	injector::MakeCALL(mem, testHook);
}

void makeHook1(uintptr_t mem, size_t size)
{
	injector::MakeNOP(mem, size);
	injector::MakeCALL(mem, testHook1);
}

void makeHook2(uintptr_t mem, size_t size)
{
	injector::MakeNOP(mem, size);
	injector::MakeCALL(mem, testHook2);
}

void makeHook3(uintptr_t mem, size_t size)
{
	injector::MakeNOP(mem, size);
	injector::MakeCALL(mem, testHook3);
}

void *getStartTextOriginal = 0;

void __declspec(naked) hookStartGetText()
{
	__asm
	{
		pushad
		pushfd
		call changeText
		popfd
		popad

		jmp getStartTextOriginal
	}
}

void hook()
{
	makeHook(0x0054AE3F, (0x0054AE49 - 0x0054AE3F));
	makeHook(0x0054A73B, (0x0054A745 - 0x0054A73B));


	makeHook2(0x0054A664, (0x0054A66E - 0x0054A664));
	makeHook2(0x0054ACEF, (0x0054ACF9 - 0x0054ACEF));


	injector::MakeNOP(0x00562749, 3);
	makeHook1(0x0056274D, (0x00562755 - 0x0056274D));

	injector::MakeNOP(0x00562663, 3);
	makeHook3(0x0056266C, (0x00562674 - 0x0056266C));


	char fld[] = { 0xD9, 0x45, 0xE8 };
	injector::MakeNOP(0x0053D496, (0x0053D4A8 - 0x0053D496));
	injector::WriteMemoryRaw(0x0053D496, fld, 3, true);
	injector::MakeCALL(0x0053D496 + 3, testHook4);


	injector::MakeNOP(0x0053D3EF, (0x0053D3FC - 0x0053D3EF));
	injector::WriteMemoryRaw(0x0053D3EF, fld, 3, true);
	injector::MakeCALL(0x0053D3EF + 3, testHook5);


	injector::WriteMemory(0x0053D0D2 + 2, &graphicTorqueMax, true);
	injector::WriteMemory(0x0053D0EE + 2, &graphicPowerMax, true);

	getStartTextOriginal = injector::MakeCALL(0x005124C5, hookStartGetText).get();
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		hook();
		break;

	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;

	case 10:
		MessageBoxA(0, modByFabio, modByFabio, 0);
		break;
	}
	return TRUE;
}

