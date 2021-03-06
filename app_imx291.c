#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <strings.h>



#define TEST_291 1
// full 1080p
#define IMX291_FRSEL 			0x02
#if TEST_291
#define IMX291_VMAX_LOW	0x46
//0x65
#else
#define IMX291_VMAX_LOW	0x46
//0x65
#endif

//0x04
#define IMX291_VMAX_HIGH	0x05
#define IMX291_HMAX_LOW		0x30
#define IMX291_HMAX_HIGH	0x11


typedef struct _ioctl_args{
	unsigned char rdwrFlag; //0: read, 1: write
	unsigned char regH;
	unsigned char regL;
	unsigned char val;
}ioctl_args_t;



#define IMX291_REG_END		0xffff
#define IMX291_REG_DELAY	0xfffe
typedef struct _regval_list{
		unsigned int reg_num;
		unsigned char value;
}regval_list_t;


#if 1

static regval_list_t imx291_init_regs_1920_1080_25fps[] = {
/* inclk 37.125M clk*/
	{0x3000,0x01},	//standby
	{IMX291_REG_DELAY, 0x14},	/* delay 20ms */
	{0x3005,0x01},	//12 bit AD conversion bits setting
	{0x3007,0x00},	//no inverted/Full HD 1080p
	{0x3009,0x02},	//default 2h, frame rate.
	{0x300a,0xf0},	//black level offset value
	{0x300f,0x00},	//set to
	{0x3010,0x21},	//set to
	{0x3012,0x64},	//set to
	{0x3013,0x00},	//
	{0x3016,0x09},	//set to
	{0x3018,0x65},	//number of operation lines(L) count from 1
	{0x3019,0x04},	//number of operation lines(M) count from 1
	{0x301a,0x00},	//number of operation lines(H) count from 1
	{0x301c,0xa0},	//number of operation clocks (L) count from 1
	{0x301d,0x14},	//number of operation clocks (H) count from 1
	{0x3046,0x01},	//12 bit, Number of output bit setting
	{0x304B,0x0A},	//VSYNC & HSYNC output
	{0x305c,0x18},	//INCKSEL1
	{0x305d,0x00},	//INCKSEL2
	{0x305e,0x20},	//INCKSEL3
	{0x305f,0x01},	//INCKSEL4
	{0x3070,0x02},	//set to
	{0x3071,0x11},	//set to
	{0x30a6,0x20},	//set to
	{0x30a8,0x20},	//set to
	{0x30aa,0x20},	//set to	
	{0x30ac,0x20},	//set to
	
	{0x310b,0x00},	//set to, add
	{0x3119,0x9e},	//set to
	{0x311e,0x08},	//set to
	{0x3128,0x05},	//set to
	{0x3134,0x0f},	//set to, add
	{0x313b,0x50},	//set to, add
	{0x313d,0x83},	//set to
	{0x315e,0x1A},	//INCKSET5, inck: 37.1215
	{0x3164,0x1A},	//INCKSET6, inck: 37.1215
	{0x317c,0x00},	//set to
	{0x317e,0x00},	//unknowd
	{0x317f,0x00},	//set to, add
	
	{0x32b8,0x50},
	{0x32b9,0x10},
	{0x32ba,0x00},
	{0x32bb,0x04},
	{0x32c8,0x50},
	{0x32c9,0x10},
	{0x32ca,0x00},
	{0x32cb,0x04},
	
	{0x332c,0xd3},	//set to
	{0x332d,0x10},	//set to
	{0x332e,0x0d},	//set to
	{0x3358,0x06},	//set to
	{0x3359,0xe1},	//set to
	{0x335a,0x11},	//set to
	{0x3360,0x1e},	//set to
	{0x3361,0x61},	//set to	
	{0x3362,0x10},	//set to
	{0x33b0,0x08},	//set to
	{0x33b1,0x30},	//set to
	{0x33b2,0x00},	//set to
	{0x33b3,0x04},	//set to
	
	{0x3418,0xd9},	//V direction effective pixel width setting[7:0]
	{0x3419,0x02},	//V direction effective pixel width setting[12:8]

	{0x3000,0x00},	//operating
	{0x3002,0x00},	//start masterMode

	{IMX291_REG_DELAY, 0x14},	/* END MARKER */
	{IMX291_REG_END, 0x00},	/* END MARKER */
};
#else
static regval_list_t imx291_init_regs_1920_1080_25fps[]={
	{0x0203, 0x1},
	{0x0200, 0x1},
	{0x0201, 0x0},
	{0x0202, 0x01}, //0x0,	master stop
	{0x0203, 0x0},
	{0x0204, 0x10},
//0x0205, 0x00, // 10bit
	{0x0205, 0x01}, //	00-10bit,01-12bit
	{0x0206, 0x0},
	{0x0207, 0x00}, //full 1080P
	{0x0208, 0xA0},
	{0x0209, IMX291_FRSEL}, //frame rate 1-60fps, 2-30fps
	{0x020a, 0xf0},
	{0x020b, 0x00},
	{0x020c, 0x0},
	{0x020d, 0x00},
	{0x020e, 0x01},
	{0x020f, 0x00},
	{0x0210, 0x01},
	{0x0211, 0x0},
	{0x0212, 0xf0},
	{0x0213, 0x00},
	{0x0214, 0x0},
	{0x0215, 0x0},
	{0x0216, 0x09},
	{0x0217, 0x00},
	{0x0218, IMX291_VMAX_LOW}, //0x65
	{0x0219, IMX291_VMAX_HIGH}, //0x04
	{0x021a, 0x0},
	{0x021b, 0x00}, //IMX136_HMAX_LOW,//0x021b,0x30,
	{0x021c, IMX291_HMAX_LOW}, //0x021c,0x11,
	{0x021d, IMX291_HMAX_HIGH}, //0x26,
	{0x021e, 0xb2}, //0x2,
	{0x021f, 0x01},
	{0x0220, 0x0},
	{0x0221, 0x0},
	{0x0222, 0x0},
	{0x0223, 0x0},
	{0x0224, 0x0},
	{0x0225, 0x0},
	{0x0226, 0x0},
	{0x0227, 0x0},
	{0x0228, 0x0},
	{0x0229, 0x0},
	{0x022a, 0x0},
	{0x022b, 0x0},
	{0x022c, 0x0},
	{0x022d, 0x0},
	{0x022e, 0x0},
	{0x022f, 0x0},
	{0x0230, 0x0},
	{0x0231, 0x0},
	{0x0232, 0x0},
	{0x0233, 0x0},
	{0x0234, 0x0},
	{0x0235, 0x0},
	{0x0236, 0x0},
	{0x0237, 0x0},
	{0x0238, 0x00}, //0x3c,
	{0x0239, 0x0},
	{0x023a, 0x0c}, //0x50,
	{0x023b, 0x00}, //0x04,
	{0x023c, 0x0},
	{0x023d, 0x0},
	{0x023e, 0x49}, //0x9c,
	{0x023f, 0x04}, //0x07,
	{0x0240, 0x00},
	{0x0241, 0x00},
	{0x0242, 0x9c},
	{0x0243, 0x07},
	{0x0244, 0x00}, // �������
	{0x0246, 0x01}, //0x00 10bit,  // ������� 12bit
//0x0246, 0x00, //0x00,	// 10bit
	{0x0247, 0x01}, //0x08,
#if TEST_291
	{0x0248, 0x30}, //0x00, //0x13,//change by xmx for test 2016.12.19
	{0x0249, 0x30}, //0x0a,
#else
	{0x0248, 0x00},
	{0x0249, 0x00}, //0x0a,
#endif
	{0x024a, 0x00},
	{0x024b, 0x0a}, //0x00,
	{0x025c, 0x18}, //0x2c,//0x20,
	{0x025d, 0x00}, //0x10, //0x06,
	{0x025e, 0x20}, //0x2c,//0x30,
	{0x025f, 0x01}, //0x10,//0x04,
	{0x0270, 0x02}, //0x1,
	{0x0271, 0x11},
	{0x02a6, 0x20},
	{0x02a7, 0x0},
	{0x02a8, 0x20},
	{0x02a9, 0x00},//0x10,
	{0x02aa, 0x20},
	{0x02ab, 0x00},//0x10,
	{0x02ac, 0x20},
	{0x030b, 0x00},
	{0x0319, 0x9e}, //0x2,
//0x031d,0x8,
	{0x031e, 0x08},//0x3,
	{0x0328, 0x05},
	{0x0334, 0x0f},//0x6,
//0x0339, 0x0,
	{0x033b, 0x50},
	{0x033c, 0x41},
	{0x033d, 0x83},
	{0x035e, 0x1a}, //0x0, ����INCK hz 37.125HZ
	{0x0364, 0x1a}, //0x0,
	{0x037c, 0x00},
	{0x037d, 0x00}, //0xc,
//0x037e, 0x0,
	{0x037f, 0x0},
	{0x04b8, 0x50}, //0x05,
	{0x04b9, 0x10}, //0xa5,
	{0x04ba, 0x00}, //0x51,
	{0x04bb, 0x04}, //0x13,
	{0x04c8, 0x50}, //0x31,
	{0x04c9, 0x10}, //0x13,
	{0x04ca, 0x00},
	{0x04cb, 0x04}, //0x46,
	{0x052c, 0xd3},
	{0x052d, 0x10},
	{0x052e, 0x0d},
	{0x0558, 0x06},
	{0x0559, 0xe1},
	{0x055a, 0x11},
	{0x0560, 0x1e},
	{0x0561, 0x61},
	{0x0562, 0x10},
	{0x05b0, 0x08},
	{0x05b1, 0x30},
	{0x05b2, 0x00},
	{0x05b3, 0x04},
	{0x0605, 0x20},
	{0x0606, 0x00},
	{0x0607, 0x03},
	{0x0614, 0x0a},
	{0x0618, 0x49},
	{0x0619, 0x04},
	{0x062c, 0x47},
	{0x062d, 0x00},
	{0x0630, 0x0f},
	{0x0631, 0x00},
	{0x0641, 0x0c},
	{0x0642, 0x0c},
	{0x0643, 0x03},
	{0x0644, 0x20}, //0x40,
	{0x0645, 0x25}, //0x4a,
	{0x0646, 0x47},
	{0x0647, 0x00},
	{0x0648, 0x1f},
	{0x0649, 0x00},
	{0x064a, 0x17},
	{0x064b, 0x00},
	{0x064c, 0x0f},
	{0x064d, 0x00},
	{0x064e, 0x17},
	{0x064f, 0x00},
	{0x0650, 0x47},
	{0x0651, 0x00},
	{0x0652, 0x0f},
	{0x0653, 0x00},
	{0x0654, 0x0f},
	{0x0655, 0x00},
	{0x0672, 0x9c},
	{0x0673, 0x07},
//0x0201, 0x1,//lock the sensor
	{IMX291_REG_DELAY, 0x14},	/* END MARKER */
	{IMX291_REG_END, 0x00},	/* END MARKER */
};

#endif


int imx291_write_array(int fd, regval_list_t* regListp)
{
	int ret;
	ioctl_args_t a;
	a.rdwrFlag = 1;//write
	
	while(regListp->reg_num != IMX291_REG_END)
	{
		if(regListp->reg_num == IMX291_REG_DELAY)
		{
			usleep(regListp->value * 1000);	//m sleep
		}else
		{
			a.regH = ((regListp->reg_num)>>8)&0xff;
			a.regL = (regListp->reg_num)&0xff;
			a.val = regListp->value;
			ret = ioctl(fd, 0xff, &a);
			if (ret < 0)
			{
				perror("ioctl imx291 failed.!");
				return ret;
			}
		}
		regListp++;
	}
	return 0;
}

int imx291_read_array(int fd, regval_list_t* regListp)
{
	int ret;
	ioctl_args_t a;
	a.rdwrFlag = 0;//read
	
	while(regListp->reg_num != IMX291_REG_END)
	{
		if(regListp->reg_num == IMX291_REG_DELAY)
		{
			usleep(regListp->value * 1000);	//m sleep
		}else
		{
			a.regH = ((regListp->reg_num)>>8)&0xff;
			a.regL = (regListp->reg_num)&0xff;
			//a.val = regListp->value;
			ret = ioctl(fd, 0xff, &a);
			if (ret < 0)
			{
				perror("ioctl imx291 failed.!");
				return ret;
			}
		}
		regListp++;
	}
	
	return 0;
}


static int imx291_detect(int fd)
{
	ioctl_args_t a;
	
	//IdH ->0xa0
	bzero(&a, sizeof(ioctl_args_t));
	a.rdwrFlag = 0;
	a.regH = 0x30;
	a.regL = 0x08;
	ioctl(fd, 0xff, &a);
	printf("read idH: %#x\n\n", a.val);
	//idL -> 0xb2
	bzero(&a, sizeof(ioctl_args_t));
	a.rdwrFlag = 0;
	a.regH = 0x30;
	a.regL = 0x1e;
	ioctl(fd, 0xff, &a);
	printf("read idL: %#x\n\n", a.val);
	
	return 0;
}


int main(int argc, char* argv[])
{
	int fd;
	ioctl_args_t a;

	fd = open("/dev/imx291", O_RDWR);
	if(fd < 0)
	{
		perror("open");
		exit(1);
	}else
	{
		imx291_detect(fd);
		imx291_write_array(fd, imx291_init_regs_1920_1080_25fps);
		//imx291_read_array(fd, imx291_init_regs_1920_1080_25fps);
		close(fd);
	}
	return 0;
}
