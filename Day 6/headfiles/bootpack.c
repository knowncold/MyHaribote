#include "bootback.h"

void HariMain(void){	//主函数
	char *vram;
	int xsize, ysize;
	struct BOOTINFO *bootinfo;
	init_palette();
	extern char hankaku[4096];
	char s[40], mcursor[256];
	// 获得屏幕信息
	bootinfo = (struct BOOTINFO *) 0xff0;	//恰好内存中的结构体内部的变量就是这么一个结构
	xsize = bootinfo->scrnx;
	ysize = bootinfo->scrny;
	vram = bootinfo->vram;

	init_screen(vram, xsize, ysize);
	rectangle(vram, xsize, 12, 0, 0, xsize, ysize);
	rectangle(vram, xsize, 5, 0, ysize-13, xsize, ysize);	// 制造任务栏上面的条纹
	rectangle(vram, xsize, 6, 0, ysize-12, xsize, ysize);	// 任务栏的绘制
	rectangle(vram, xsize, 5, 1, ysize-10, 24, ysize-2);	// 左下方块的左上框
	rectangle(vram, xsize, 7, 2, ysize-9, 25, ysize-1);		// 左下方块的右下框
	rectangle(vram, xsize, 8, 2, ysize-9, 24, ysize-2);		// 左下方块的中间

	putfont8(vram, xsize, 10, 10, COL8_FFFFFF, hankaku + 'A' * 16);
	putfont8(vram, xsize, 18, 10, COL8_FFFFFF, hankaku + 'B' * 16);
	putfont8_asc(vram, xsize, 26, 10, COL8_FFFFFF, "KnownCold");
	init_mouse_cursor8(mcursor, 12);
	putblock8_8(vram, xsize, 16, 16, 100, 100, mcursor, 16);
	for (;;) {	//HTL循环
		io_hlt();
	}
}
