
#include <string.h>
#include <gccore.h>
#include "cheat.hpp"
#include "loader/fat.h"
#include "text.hpp"

#define FILEDIR	"sd:/codes/%s.gct"
#define FILEDIR_USB	"usb:/codes/%s.gct"

extern const DISC_INTERFACE __io_wiisd;
extern const DISC_INTERFACE __io_usbstorage;

void loadCheatFile(SmartBuf &buffer, u32 &size, const char *gameId)
{
	FILE *fp = 0;
	u32 fileSize;
	SmartBuf fileBuf;

	buffer.release();
	size = 0;
	if (Fat_SDAvailable())
		fp = fopen(sfmt(FILEDIR, gameId).c_str(), "rb");
	if (fp == 0 && Fat_USBAvailable())
		fp = fopen(sfmt(FILEDIR_USB, gameId).c_str(), "rb");
	if (fp == 0)
		return;
	fseek(fp, 0, SEEK_END);
	fileSize = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	fileBuf = smartCoverAlloc(fileSize);
	if (!fileBuf)
	{
		fclose(fp);
		return;
	}
	if (fread(fileBuf.get(), 1, fileSize, fp) != fileSize)
	{
		fclose(fp);
		return;
	}
	fclose(fp);
	buffer = fileBuf;
	size = fileSize;
}

void loadCheat(const u8 *buffer, u32 size)
{
	memcpy((void*)0x800027E8, buffer, size);
	*(vu8 *)0x80001807 = 0x01;
}