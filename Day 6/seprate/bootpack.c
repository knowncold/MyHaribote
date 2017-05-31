void io_hlt(void);
void io_cli(void);
void io_out8(int port, int data);
int io_load_eflags(void);
void io_store_eflags(int eflags);

void rectangle(unsigned char* vram, int xsize, unsigned char c, int x0, int y, int x1, int y1);
void init_palette(void);
void set_palette(int start, int end, unsigned char *rgb);
void init_screen(char *vram, int x, int y);
void putfont8(char *vram, int xsize, int x, int y, char c, char *font);
void putfont8_asc(char *vram, int xsize, int x, int y, char c, unsigned char *s);
void init_mouse_cursor8(char *mouse, char bc);
void putblock8_8(char *vram, int vxsize, int pxsize,
	int pysize, int px0, int py0, char *buf, int bxsize);

// 定义各种颜色的序号
#define COL8_000000		0
#define COL8_FF0000		1
#define COL8_00FF00		2
#define COL8_FFFF00		3
#define COL8_0000FF		4
#define COL8_FF00FF		5
#define COL8_00FFFF		6
#define COL8_FFFFFF		7
#define COL8_C6C6C6		8
#define COL8_840000		9
#define COL8_008400		10
#define COL8_848400		11
#define COL8_000084		12
#define COL8_840084		13
#define COL8_008484		14
#define COL8_848484		15

struct BOOTINFO{	//启动时的结构体信息，包含了屏幕的数据
	char cyls, leds, vmode, reserve;
	short scrnx, scrny;
	char *vram;
};

void init_gdtidt(void);
void set_segmdesc(struct SEGMENT_DESCRIPTOR *sd, unsigned int limit, int base, int ar);
void set_gatedesc(struct GATE_DESCRIPTOR *gd, int offset, int selector, int ar);
void load_gdtr(int limit, int addr);
void load_idtr(int limit, int addr);

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

	// xsize = *((short *) 0x0ff4);
	// ysize = *((short *) 0x0ff6);
	// vram = (char *) *((int *) 0x0ff8);

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

void init_mouse_cursor8(char *mouse, char bc){
	static char cursor[16][16] = {
		"**************..",
		"*OOOOOOOOOOO*...",
		"*OOOOOOOOOO*....",
		"*OOOOOOOOO*.....",
		"*OOOOOOOO*......",
		"*OOOOOOO*.......",
		"*OOOOOOO*.......",
		"*OOOOOOOO*......",
		"*OOOO**OOO*.....",
		"*OOO*..*OOO*....",
		"*OO*....*OOO*...",
		"*O*......*OOO*..",
		"**........*OOO*.",
		"*.....O....*OOO*",
		"............*OO*",
		".............***"
	};
	int x, y;

	for (y = 0; y < 16; y++) {
		for (x = 0; x < 16; x++) {
			if (cursor[y][x] == '*') {
				mouse[y * 16 + x] = COL8_000000;
			}
			if (cursor[y][x] == 'O') {
				mouse[y * 16 + x] = COL8_FFFFFF;
			}
			if (cursor[y][x] == '.') {
				mouse[y * 16 + x] = bc;
			}
		}
	}
	return;
}
