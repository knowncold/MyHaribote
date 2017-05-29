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

struct SEGMENT_DESCRIPTOR {
	short limit_low, base_low;
	char base_mid, access_right;
	char limit_high, base_high;
};

struct GATE_DESCRIPTOR {
	short offset_low, selector;
	char dw_count, access_right;
	short offset_high;
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

void rectangle(unsigned char* vram, int xsize, unsigned char color, int x0, int y0, int x1, int y1){	//画矩形
	int x,y;
	for (x=x0; x <=x1; x++){
		for (y=y0; y<=y1; y++){
			vram[ y * xsize + x] = color;
		}
	}
}

void init_palette(void){	//初始化调色板需要配置的颜色
	static unsigned char table_rgb[16 * 3] = {
		0x00, 0x00, 0x00,	/*  0:黑 */
		0xff, 0x00, 0x00,	/*  1:亮红 */
		0x00, 0xff, 0x00,	/*  2:亮绿 */
		0xff, 0xff, 0x00,	/*  3:亮黄 */
		0x00, 0x00, 0xff,	/*  4:亮蓝 */
		0xff, 0x00, 0xff,	/*  5:亮紫 */
		0x00, 0xff, 0xff,	/*  6:浅亮蓝 */
		0xff, 0xff, 0xff,	/*  7:白 */
		0xc6, 0xc6, 0xc6,	/*  8:亮灰 */
		0x84, 0x00, 0x00,	/*  9:暗红 */
		0x00, 0x84, 0x00,	/* 10:暗绿 */
		0x84, 0x84, 0x00,	/* 11:暗黄 */
		0x00, 0x00, 0x84,	/* 12:暗青 */
		0x84, 0x00, 0x84,	/* 13:暗紫 */
		0x00, 0x84, 0x84,	/* 14:浅暗蓝 */
		0x84, 0x84, 0x84	/* 15:暗灰 */
	};
	set_palette(0, 15, table_rgb);
	return;
}

void set_palette(int start, int end, unsigned char *rgb){	//调色板配置
	int i, eflags;
	eflags = io_load_eflags();
	io_cli();
	io_out8(0x03c8, start);
	for (i = start; i <= end; i++) {
		io_out8(0x03c9, rgb[0] / 4);
		io_out8(0x03c9, rgb[1] / 4);
		io_out8(0x03c9, rgb[2] / 4);
		rgb += 3;
	}
	io_store_eflags(eflags);
	return;
}

void init_screen(char *vram, int x, int y){	//初始化背景
	rectangle(vram, x, COL8_008484,  0,     0,      x -  1, y - 29);
	rectangle(vram, x, COL8_C6C6C6,  0,     y - 28, x -  1, y - 28);
	rectangle(vram, x, COL8_FFFFFF,  0,     y - 27, x -  1, y - 27);
	rectangle(vram, x, COL8_C6C6C6,  0,     y - 26, x -  1, y -  1);

	rectangle(vram, x, COL8_FFFFFF,  3,     y - 24, 59,     y - 24);
	rectangle(vram, x, COL8_FFFFFF,  2,     y - 24,  2,     y -  4);
	rectangle(vram, x, COL8_848484,  3,     y -  4, 59,     y -  4);
	rectangle(vram, x, COL8_848484, 59,     y - 23, 59,     y -  5);
	rectangle(vram, x, COL8_000000,  2,     y -  3, 59,     y -  3);
	rectangle(vram, x, COL8_000000, 60,     y - 24, 60,     y -  3);

	rectangle(vram, x, COL8_848484, x - 47, y - 24, x -  4, y - 24);
	rectangle(vram, x, COL8_848484, x - 47, y - 23, x - 47, y -  4);
	rectangle(vram, x, COL8_FFFFFF, x - 47, y -  3, x -  4, y -  3);
	rectangle(vram, x, COL8_FFFFFF, x -  3, y - 24, x -  3, y -  3);
	return;
}

void putfont8(char *vram, int xsize, int x, int y, char c, char *font){	//显示8x16的字体
	int i;
	char *p, d /* data */;
	for (i = 0; i < 16; i++) {
		p = vram + (y + i) * xsize + x;
		d = font[i];
		if ((d & 0x80) != 0) { p[0] = c; }
		if ((d & 0x40) != 0) { p[1] = c; }
		if ((d & 0x20) != 0) { p[2] = c; }
		if ((d & 0x10) != 0) { p[3] = c; }
		if ((d & 0x08) != 0) { p[4] = c; }
		if ((d & 0x04) != 0) { p[5] = c; }
		if ((d & 0x02) != 0) { p[6] = c; }
		if ((d & 0x01) != 0) { p[7] = c; }
	}
	return;
}

void putfont8_asc(char *vram, int xsize, int x, int y, char c, unsigned char *s){
	extern char hankaku[4096];
	for(; *s != 0x00; s++){
		putfont8(vram, xsize, x, y, c, hankaku + *s * 16);
		x += 8;
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

void putblock8_8(char *vram, int vxsize, int pxsize,
	int pysize, int px0, int py0, char *buf, int bxsize){
	int x, y;
	for (y = 0; y < pysize; y++) {
		for (x = 0; x < pxsize; x++) {
			vram[(py0 + y) * vxsize + (px0 + x)] = buf[y * bxsize + x];
		}
	}
	return;
}

void init_gdtidt(void){
	struct SEGMENT_DESCRIPTOR *gdt = (struct SEGMENT_DESCRIPTOR *) 0x00270000;
	struct GATE_DESCRIPTOR    *idt = (struct GATE_DESCRIPTOR    *) 0x0026f800;
	int i;

	for (i = 0; i < 8192; i++) {
		set_segmdesc(gdt + i, 0, 0, 0);
	}
	set_segmdesc(gdt + 1, 0xffffffff, 0x00000000, 0x4092);
	set_segmdesc(gdt + 2, 0x0007ffff, 0x00280000, 0x409a);
	load_gdtr(0xffff, 0x00270000);

	for (i = 0; i < 256; i++) {
		set_gatedesc(idt + i, 0, 0, 0);
	}
	load_idtr(0x7ff, 0x0026f800);

	return;
}

void set_segmdesc(struct SEGMENT_DESCRIPTOR *sd, unsigned int limit, int base, int ar){
	if (limit > 0xfffff) {
		ar |= 0x8000; /* G_bit = 1 */
		limit /= 0x1000;
	}
	sd->limit_low    = limit & 0xffff;
	sd->base_low     = base & 0xffff;
	sd->base_mid     = (base >> 16) & 0xff;
	sd->access_right = ar & 0xff;
	sd->limit_high   = ((limit >> 16) & 0x0f) | ((ar >> 8) & 0xf0);
	sd->base_high    = (base >> 24) & 0xff;
	return;
}

void set_gatedesc(struct GATE_DESCRIPTOR *gd, int offset, int selector, int ar){
	gd->offset_low   = offset & 0xffff;
	gd->selector     = selector;
	gd->dw_count     = (ar >> 8) & 0xff;
	gd->access_right = ar & 0xff;
	gd->offset_high  = (offset >> 16) & 0xffff;
	return;
}
