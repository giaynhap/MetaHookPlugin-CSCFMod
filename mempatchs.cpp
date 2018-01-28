#include <metahook.h>
#include <net_api.h>
#include <cvardef.h>
#include "mempatchs.h"
#include "plugins.h"

void MemPatch_WideScreenLimit(void)
{
	if (g_dwEngineBuildnum >= 5953)
		return;

	unsigned char data[] = { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 };
	DWORD addr = (DWORD)g_pMetaHookAPI->SearchPattern((void *)g_dwEngineBase, g_dwEngineSize, "\x8B\x51\x08\x8B\x41\x0C\x8B\x71\x54\x8B\xFA\xC1\xE7\x04", 14);

	if (!addr)
	{
		MessageBox(NULL, "WideScreenLimit patch failed!", "[Giay Nhap] Error!", MB_ICONWARNING);
		return;
	}

	DWORD addr2 = addr + 11;
	DWORD addr3 =  (DWORD)g_pMetaHookAPI->SearchPattern((void *)addr, 0x60, "\xB1\x01\x8B\x7C\x24\x14", 6);

	if (addr3)
	{
		g_pMetaHookAPI->WriteMemory((void *)addr2, data, addr3 - addr2);
	}
}

void MemPatch_BlockSniperScope(void)
{
	if (g_dwEngineBuildnum >= 5953)
		return;

	unsigned char data[] = { 0xB8, 0x01, 0x00, 0x00, 0x00, 0xC2, 0x04, 0x00 };
	DWORD addr = (DWORD)g_pMetaHookAPI->SearchPattern((void *)g_pMetaSave->pExportFuncs->Initialize, 0x100000, "\x83\xEC\x50\xA1\x2A\x2A\x2A\x2A\x89\x4C\x24\x00\x85\xC0", 14);

	if (!addr)
	{
		MessageBox(NULL, "BlockSniperScope patch failed!", "[Giay Nhap] Error!", MB_ICONWARNING);
		return;
	}

	g_pMetaHookAPI->WriteMemory((void *)addr, data, sizeof(data));
}

void MemPatch_Start(MEMPATCH_STEP step)
{
	switch (step)
	{
		case MEMPATCH_STEP_LOADENGINE:
		{
			//g_pMetaHookAPI->WriteMemory((void *)0x1D5EFB3, (byte *)"\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90", 10);
			//MemPatch_WideScreenLimit();
			break;
		}

		case MEMPATCH_STEP_LOADCLIENT:
		{
			break;
		}

		case MEMPATCH_STEP_INITCLIENT:
		{
		//	MemPatch_BlockSniperScope();
			break;
		}
	}
}