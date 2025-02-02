#include "iWatch.h"

extern unsigned char xdata main_cache[];
extern unsigned char xdata sub_cache[];

static PageExecuteRate_TypeDef Rate_50hz = {20,0};
static PageExecuteRate_TypeDef Rate_125hz = {8,0};

static unsigned char code Text[] = "LSM6DSM";
static unsigned char code Icon[] = {
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0xC0,0xC0,0x60,0x60,
0x30,0x30,0x10,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x30,0x30,0x30,
0x60,0x60,0xC0,0xC0,0x80,0x00,0x00,0x80,0xE0,0xF0,0xF0,0x60,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0xC0,0xF0,0x38,0x0E,0x07,0x03,0xF1,0xF8,0x18,0x18,0x18,
0x98,0xF0,0xF0,0x60,0xF0,0xF8,0x98,0x98,0x0C,0x0C,0x04,0x06,0x06,0x06,0x06,0x06,
0x0C,0x0C,0x18,0x39,0xF3,0xC3,0x0F,0x1F,0x79,0xE0,0xC0,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0xF8,0xFF,0x03,0x00,0x00,0x00,0x00,0x00,0xE1,0xC7,0x1E,0x38,0xF3,
0xC3,0x81,0x08,0x3C,0xF8,0xF8,0xB1,0x73,0xE7,0xCE,0x9C,0x38,0x70,0xE0,0xC0,0x80,
0x00,0x00,0x00,0x80,0xF1,0x7F,0x00,0x00,0x00,0x01,0x0F,0xFE,0xF0,0x00,0x00,0x00,
0x00,0x00,0x00,0x1F,0xFF,0xE0,0x00,0x00,0x00,0xF8,0xFF,0x03,0x00,0x00,0x00,0x00,
0x01,0x03,0x07,0x0E,0x1C,0x39,0x73,0xE7,0xC6,0x8F,0x1F,0x1E,0x1C,0x80,0x81,0xE3,
0x7F,0x3C,0xFE,0xE7,0x83,0x00,0x00,0x00,0x00,0x80,0xF0,0x7F,0x07,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x03,0x0F,0x9C,0xF8,0xF0,0xC3,0xCF,0x9C,0x18,0x30,0x30,
0x30,0x20,0x20,0x20,0x30,0x30,0x30,0x18,0x19,0x19,0x03,0x07,0x06,0x0C,0x0D,0x1C,
0x18,0x18,0x18,0x8F,0xC7,0xE0,0x70,0x38,0x0E,0x07,0x01,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x06,0x0F,0x0F,0x07,0x01,0x00,0x00,0x01,0x01,0x03,0x07,0x06,
0x06,0x0C,0x0C,0x0C,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x08,0x0C,0x0C,0x0C,0x06,
0x06,0x03,0x03,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
/* (48 X 48 )*/
};
const unsigned char code CIRCLE16X16[] = {
	0xC0,0x30,0x08,0x84,0x64,0x22,0x12,0x12,0x12,0x22,0x64,0x84,0x08,0x30,0xC0,0x00,
	0x07,0x18,0x20,0x43,0x4C,0x88,0x90,0x90,0x90,0x88,0x4C,0x43,0x20,0x18,0x07,0x00
};

struct lsm6dsm_data LSM6DSM;			//LSM6DSM数据结构体
extern iWatch_config config;
static int x, y;
static unsigned char str[16];
/**
  * @brief  页面初始化事件
  * @param  无
  * @retval 无
  */
static void Setup()
{
	ClearCache(sub_cache);
	sprintf(str, "x:%.1f  ", LSM6DSM.AngleX);
	ShowString(0, 0, str, sub_cache, FONT6X8, NO_INVERSED);
	sprintf(str, "y:%.1f  ", LSM6DSM.AngleY);
	ShowString(0, 1, str, sub_cache, FONT6X8, NO_INVERSED);
	sprintf(str, "z:%.1f  ", LSM6DSM.AngleZ);
	ShowString(0, 2, str, sub_cache, FONT6X8, NO_INVERSED);
	BMPToCache(56, 3, 16, 16, CIRCLE16X16, sub_cache, 0);
	PageOpenAnim(ANIM_RIGHT);
	LSM6DSMConfigAcc(ACC_ODR_208_HZ, ACC_SCALE_4_G);
	LSM6DSMConfigGyr(GYR_ODR_208_HZ, GYR_SCALE_500_DPS);
}
/**
  * @brief  页面退出事件
  * @param  无
  * @retval 无
  */
static void Exit()
{
	ScreenSetInverse(config.screen_inverse);
	LSM6DSMConfigAcc(ACC_ODR_416_HZ, ACC_SCALE_4_G);
	LSM6DSMConfigGyr(GYR_POWER_DOWN, GYR_SCALE_500_DPS);
	PageCloseAnim(ANIM_RIGHT);
}
/**
  * @brief  页面循环执行的内容
  * @param  无
  * @retval 无
  */
static void Loop()
{
	if(PageExecuteRate(&Rate_125hz) == 1)
	{
		LSM6DSMReadGYRAndACC(&LSM6DSM);
		IMUupdate(&LSM6DSM);
	}
	if(PageExecuteRate(&Rate_50hz) == 1)
	{
		ClearCache(main_cache);
		LSM6DSM.AngleX -= config.cal_anglex;
		LSM6DSM.AngleY -= config.cal_angley;
		sprintf(str, "x:%.1f  ", LSM6DSM.AngleX);
		ShowString(0, 0, str, main_cache, FONT6X8, NO_INVERSED);
		sprintf(str, "y:%.1f  ", LSM6DSM.AngleY);
		ShowString(0, 1, str, main_cache, FONT6X8, NO_INVERSED);
		sprintf(str, "z:%.1f  ", LSM6DSM.AngleZ);
		ShowString(0, 2, str, main_cache, FONT6X8, NO_INVERSED);
		BMPToCache(56, 3, 16, 16, CIRCLE16X16, main_cache, 0);
		x = 64 + LSM6DSM.AngleX;
		y = 32 + LSM6DSM.AngleY;
		if(x > 126)
			x = 126;
		else if(x < 1)
			x = 1;
		if(y > 62)
			y = 62;
		else if(y < 1)
			y = 1;
		if(x ==63 && y == 32)
			ScreenSetInverse(config.screen_inverse == NO_INVERSED?INVERSED:NO_INVERSED);
		else
		ScreenSetInverse(config.screen_inverse);
		DrawDot(x, y);
		DrawDot(x - 1, y);
		DrawDot(x + 1, y);
		DrawDot(x, y + 1);
		DrawDot(x, y - 1);
		ScreenRefreshAll(main_cache);
		printf("#%03d%03d", (int)LSM6DSM.AngleX, (int)LSM6DSM.AngleY);
	}
}
/**
  * @brief  页面事件
  * @param  btn:发出事件的按键
  * @param  event:事件编号
  * @retval 无
  */
static void Event(unsigned char event)
{
	if(event == KEY1)
		PageShift(page_menu);
}

void PageRegister_page_lsm6dsm(unsigned char pageID)
{
	PageRegister(pageID, Text, Icon, Setup, Loop, Exit, Event);
}