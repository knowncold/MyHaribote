void io_hlt(void);
void io_cli(void);
void io_out8(int port, int data);
int io_load_eflags(void);
void io_store_eflags(int eflags);

void rectangle(unsigned char* vram, int xsize, unsigned char c, int x0, int y, int x1, int y1);
void init_palette(void);
void set_palette(int start, int end, unsigned char *rgb);

void HariMain(void){
	char *vram;
	int xsize = 320, ysize = 200;
	init_palette();
	vram = (char *) 0xa0000;

	rectangle(vram, xsize, 12, 0, 0, xsize, ysize);
	rectangle(vram, xsize, 5, 0, ysize-13, xsize, ysize);	//制造任务栏上面的条纹
	rectangle(vram, xsize, 6, 0, ysize-12, xsize, ysize);
	rectangle(vram, xsize, 5, 1, ysize-10, 24, ysize-2);	// 左下方块的左上框
	rectangle(vram, xsize, 7, 2, ysize-9, 25, ysize-1);
	rectangle(vram, xsize, 8, 2, ysize-9, 24, ysize-2);

	for (;;) {
		io_hlt();
	}
}

void rectangle(unsigned char* vram, int xsize, unsigned char color, int x0, int y0, int x1, int y1){
	int x,y;
	for (x=x0; x <=x1; x++){
		for (y=y0; y<=y1; y++){
			vram[ y * xsize + x] = color;
		}
	}
}

void init_palette(void){
	static unsigned char table_rgb[16 * 3] = {
		0x00, 0x00, 0x00,	/*  0:�� */
		0xff, 0x00, 0x00,	/*  1:���邢�� */
		0x00, 0xff, 0x00,	/*  2:���邢�� */
		0xff, 0xff, 0x00,	/*  3:���邢���F */
		0x00, 0x00, 0xff,	/*  4:���邢�� */
		0xff, 0x00, 0xff,	/*  5:���邢�� */
		0x00, 0xff, 0xff,	/*  6:���邢���F */
		0xff, 0xff, 0xff,	/*  7:�� */
		0xc6, 0xc6, 0xc6,	/*  8:���邢�D�F */
		0x84, 0x00, 0x00,	/*  9:�Â��� */
		0x00, 0x84, 0x00,	/* 10:�Â��� */
		0x84, 0x84, 0x00,	/* 11:�Â����F */
		0x00, 0x00, 0x84,	/* 12:�Â��� */
		0x84, 0x00, 0x84,	/* 13:�Â��� */
		0x00, 0x84, 0x84,	/* 14:�Â����F */
		0x84, 0x84, 0x84	/* 15:�Â��D�F */
	};
	set_palette(0, 15, table_rgb);
	return;

	/* static char ���߂́A�f�[�^�ɂ����g���Ȃ�����DB���ߑ��� */
}

void set_palette(int start, int end, unsigned char *rgb){
	int i, eflags;
	eflags = io_load_eflags();	/* ���荞�݋��t���O�̒l���L�^���� */
	io_cli(); 					/* ���t���O��0�ɂ��Ċ��荞�݋֎~�ɂ��� */
	io_out8(0x03c8, start);
	for (i = start; i <= end; i++) {
		io_out8(0x03c9, rgb[0] / 4);
		io_out8(0x03c9, rgb[1] / 4);
		io_out8(0x03c9, rgb[2] / 4);
		rgb += 3;
	}
	io_store_eflags(eflags);	/* ���荞�݋��t���O�����ɖ߂� */
	return;
}
