// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.4.0
// LVGL version: 8.3.11
// Project name: DiceCalculator

#ifndef _DICECALCULATOR_UI_H
#define _DICECALCULATOR_UI_H

#ifdef __cplusplus
extern "C" {
#endif

#if defined __has_include
#if __has_include("lvgl.h")
#include "lvgl.h"
#elif __has_include("lvgl/lvgl.h")
#include "lvgl/lvgl.h"
#else
#include "lvgl.h"
#endif
#else
#include "lvgl.h"
#endif

#include "ui_helpers.h"
#include "ui_events.h"

// SCREEN: ui_Screen1
void ui_Screen1_screen_init(void);
extern lv_obj_t * ui_Screen1;
extern lv_obj_t * ui_RollsContainer;
void ui_event_RolledDice(lv_event_t * e);
extern lv_obj_t * ui_RolledDice;
void ui_event_SumContainer(lv_event_t * e);
extern lv_obj_t * ui_SumContainer;
extern lv_obj_t * ui_SumText;
extern lv_obj_t * ui_SumEqualSign;
extern lv_obj_t * ui_SelectionsContainer;
void ui_event_Image3(lv_event_t * e);
extern lv_obj_t * ui_Image3;
void ui_event_Image4(lv_event_t * e);
extern lv_obj_t * ui_Image4;
void ui_event_Image5(lv_event_t * e);
extern lv_obj_t * ui_Image5;
void ui_event_Image6(lv_event_t * e);
extern lv_obj_t * ui_Image6;
void ui_event_Image7(lv_event_t * e);
extern lv_obj_t * ui_Image7;
void ui_event_Image8(lv_event_t * e);
extern lv_obj_t * ui_Image8;
extern lv_obj_t * ui____initial_actions0;


LV_IMG_DECLARE(ui_img_d4_80h_png);    // assets/d4_80h.png
LV_IMG_DECLARE(ui_img_d6_80h_png);    // assets/d6_80h.png
LV_IMG_DECLARE(ui_img_d8_80h_png);    // assets/d8_80h.png
LV_IMG_DECLARE(ui_img_d10_80h_png);    // assets/d10_80h.png
LV_IMG_DECLARE(ui_img_d12_80h_png);    // assets/d12_80h.png
LV_IMG_DECLARE(ui_img_d20_80h_png);    // assets/d20_80h.png



LV_FONT_DECLARE(ui_font_DiceFont48);



void ui_init(void);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif