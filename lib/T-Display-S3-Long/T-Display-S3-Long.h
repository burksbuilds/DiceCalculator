#pragma once

#include "Arduino.h"
#include "lvgl.h"
#include "AXS15231B.h"
#include "Wire.h"

#define TFT_WIDTH             180
#define TFT_HEIGHT            640
#define TFT_BL                1

#define LCD_USB_QSPI_DREVER   1

#define SPI_FREQUENCY           32000000
#define TFT_SPI_MODE          SPI_MODE0
#define TFT_SPI_HOST          SPI2_HOST






#define AXS_TOUCH_ONE_POINT_LEN             6
#define AXS_TOUCH_BUF_HEAD_LEN              2

#define AXS_TOUCH_GESTURE_POS               0
#define AXS_TOUCH_POINT_NUM                 1
#define AXS_TOUCH_EVENT_POS                 2
#define AXS_TOUCH_X_H_POS                   2
#define AXS_TOUCH_X_L_POS                   3
#define AXS_TOUCH_ID_POS                    4
#define AXS_TOUCH_Y_H_POS                   4
#define AXS_TOUCH_Y_L_POS                   5
#define AXS_TOUCH_WEIGHT_POS                6
#define AXS_TOUCH_AREA_POS                  7

#define AXS_GET_POINT_NUM(buf) buf[AXS_TOUCH_POINT_NUM]
#define AXS_GET_GESTURE_TYPE(buf)  buf[AXS_TOUCH_GESTURE_POS]
#define AXS_GET_POINT_X(buf,point_index) (((uint16_t)(buf[AXS_TOUCH_ONE_POINT_LEN*point_index+AXS_TOUCH_X_H_POS] & 0x0F) <<8) + (uint16_t)buf[AXS_TOUCH_ONE_POINT_LEN*point_index+AXS_TOUCH_X_L_POS])
#define AXS_GET_POINT_Y(buf,point_index) (((uint16_t)(buf[AXS_TOUCH_ONE_POINT_LEN*point_index+AXS_TOUCH_Y_H_POS] & 0x0F) <<8) + (uint16_t)buf[AXS_TOUCH_ONE_POINT_LEN*point_index+AXS_TOUCH_Y_L_POS])
#define AXS_GET_POINT_EVENT(buf,point_index) (buf[AXS_TOUCH_ONE_POINT_LEN*point_index+AXS_TOUCH_EVENT_POS] >> 6)

static lv_indev_drv_t indev_drv;
static lv_disp_drv_t disp_drv;
static lv_disp_draw_buf_t draw_buf;
static lv_color_t *screen_buf_A;
static lv_color_t *screen_buf_B;

void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area,
                   lv_color_t *color_p) {
    uint32_t w = (area->x2 - area->x1 + 1);
    uint32_t h = (area->y2 - area->y1 + 1);

    #ifdef LCD_SPI_DMA
        char i = 0;
		while (get_lcd_spi_dma_write())
		{
			i = i >> 1;
			lcd_PushColors(0, 0, 0, 0, NULL);
		}
    #endif
      lcd_PushColors(area->x1, area->y1, w, h, (uint16_t *)&color_p->full);

    #ifdef LCD_SPI_DMA

    #else
        lv_disp_flush_ready(disp);
    #endif
}

void display_init(lv_disp_rot_t rotation)
{
    pinMode(TFT_BL, OUTPUT);
    digitalWrite(TFT_BL, HIGH);

    axs15231_init();

    size_t buffer_size =
        sizeof(lv_color_t) * TFT_WIDTH * TFT_HEIGHT;
    screen_buf_A = (lv_color_t *)ps_malloc(buffer_size);
    if (screen_buf_A == NULL) {
      while (1) {
        Serial.println("bufA NULL");
        delay(500);
      }
    }

    screen_buf_B = (lv_color_t *)ps_malloc(buffer_size);
    if (screen_buf_B == NULL) {
      while (1) {
        Serial.println("bufB NULL");
        delay(500);
      }
    }

    lv_disp_draw_buf_init(&draw_buf, screen_buf_A, screen_buf_B, buffer_size);
    /*Initialize the display*/
  
    lv_disp_drv_init(&disp_drv);
    /*Change the following line to your display resolution*/
    disp_drv.hor_res = TFT_WIDTH;
    disp_drv.ver_res = TFT_HEIGHT;
    disp_drv.flush_cb = my_disp_flush;
    disp_drv.draw_buf = &draw_buf;
    disp_drv.full_refresh = 1;          //full_refresh must be 1

    //#ifdef DISPLAY_LANDSCAPE
    if(rotation == LV_DISP_ROT_90 || rotation == LV_DISP_ROT_270)
    {
        disp_drv.sw_rotate = 1;             //If you turn on software rotation, Do not update or replace LVGL
    }
    disp_drv.rotated = rotation;  
    //#endif

    lv_disp_drv_register(&disp_drv);
}

uint8_t read_touchpad_cmd[11] = {0xb5, 0xab, 0xa5, 0x5a, 0x0, 0x0, 0x0, 0x8};
void my_touchpad_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data) {
    uint8_t buff[20] = {0};

    Wire.beginTransmission(0x3B);
    Wire.write(read_touchpad_cmd, 8);
    Wire.endTransmission();
    Wire.requestFrom(0x3B, 8);
    while (!Wire.available());
    Wire.readBytes(buff, 8);

    uint16_t pointX;
    uint16_t pointY;
    uint16_t type = 0;

    type = AXS_GET_GESTURE_TYPE(buff);
    pointX = AXS_GET_POINT_X(buff,0);
    pointY = AXS_GET_POINT_Y(buff,0);

    if (!type && (pointX || pointY)) {
        pointX = (640-pointX);
        if(pointX > 640) pointX = 640;
        if(pointY > 180) pointY = 180;
        data->state = LV_INDEV_STATE_PR;
        data->point.x = pointY;
        data->point.y = pointX;

        char buf[20] = {0};
        sprintf(buf, "(%d, %d)", data->point.x, data->point.y);
        
    }
    else {
        data->state = LV_INDEV_STATE_REL;
    }
}

void touchpad_init()
{
    Wire.begin(TOUCH_IICSDA, TOUCH_IICSCL);
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = my_touchpad_read;
    lv_indev_drv_register(&indev_drv);
}

void device_reset()
{
    digitalWrite(TOUCH_RES, HIGH);delay(2);
    digitalWrite(TOUCH_RES, LOW);delay(10);
    digitalWrite(TOUCH_RES, HIGH);delay(2);
}

void device_setup(lv_disp_rot_t rotation)
{
    pinMode(TOUCH_RES, OUTPUT);
    device_reset();
    lv_init();
    display_init(rotation);
    touchpad_init();
}

extern uint32_t transfer_num;
extern size_t lcd_PushColors_len;
void device_loop()
{
    if (transfer_num <= 0 && lcd_PushColors_len <= 0)
        lv_timer_handler();

    if (transfer_num <= 1 && lcd_PushColors_len > 0) {
        lcd_PushColors(0, 0, 0, 0, NULL);
    }
}