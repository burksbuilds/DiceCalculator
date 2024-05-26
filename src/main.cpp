#include <Arduino.h>
#include <lvgl.h>
#include "T-Display-S3-Long.h"
#include "ui/ui.h"
#include <vector>
#include <algorithm>

struct RolledDie {
  uint8_t rank;
  uint8_t value;
}; 

std::vector<RolledDie> rolledDies;
const std::string charPrefix = "\xEF\x84";
const std::string d4Chars[] = {"\xAD","\xAE","\xAF","\xB0"};
const std::string d6Chars[] = {"\xB1","\xB2","\xB3","\xB4","\xB5","\xB6"};
const std::string d8Chars[] = {"\xB7","\xB8","\xB9","\xBA","\xBB","\xBC","\xBD","\xBE"};
const std::string d10Chars[] = {"\x81","\x83","\x84","\x85","\x86","\x87","\x88","\x89","\x8A","\x82"};
const std::string d12Chars[] = {"\x8B","\x8F","\x90","\x91","\x92","\x93","\x94","\x95","\x96","\x8C","\x8D","\x8E"};
const std::string d20Chars[] = {"\x99","\xA4","\xA6","\xA7","\xA8","\xA9","\xAA","\xAB","\xAC","\x9A","\x9B","\x9C","\x9D","\x9E","\x9F","\xA0","\xA1","\xA2","\xA3","\xA5"};

const std::string* diceChars[] = {d4Chars,d20Chars,d20Chars,d20Chars,d20Chars,d20Chars};

uint8_t get_rolled_total()
{
  uint8_t total = 0;
  for(size_t i =0; i < rolledDies.size(); ++i)
  {
    total += rolledDies[i].value;
  }
  return total;
}

std::string get_dice_char(RolledDie die)
{
  switch (die.rank)
  {
  case 4: return d4Chars[std::min(std::max(die.value-1,0),3)];
  case 6: return d6Chars[std::min(std::max(die.value-1,0),5)];
  case 8: return d8Chars[std::min(std::max(die.value-1,0),7)];
  case 10: return d10Chars[std::min(std::max(die.value-1,0),9)];
  case 12: return d12Chars[std::min(std::max(die.value-1,0),11)];
  case 20: return d20Chars[std::min(std::max(die.value-1,0),19)];
  default:
    return std::string();
  }
}

void update_rolled_section()
{

  uint8_t rolledTotal = get_rolled_total();
  std::string rolledTotalText = std::to_string(rolledTotal);
  lv_label_set_text(ui_SumText,rolledTotalText.c_str());

  std::string rolledDiceText;
  for(size_t i = 0; i < rolledDies.size(); ++i)
  {
    std::string diceChar = get_dice_char(rolledDies[i]);
    if(!diceChar.empty())
    {
      rolledDiceText.append(charPrefix);
      rolledDiceText.append(diceChar);
    }
  }
  lv_label_set_text(ui_RolledDice, rolledDiceText.c_str());
}

void roll_new_die(uint8_t rank)
{
  uint8_t value = random(1,rank+1);
  RolledDie newRolledDie;
  newRolledDie.rank = rank;
  newRolledDie.value = value;
  rolledDies.push_back(newRolledDie);
}

void remove_rolled_die()
{
  if(rolledDies.size() > 0)
  {
    rolledDies.pop_back();
  }
}

void clear_all_dies()
{
  rolledDies.clear();
  update_rolled_section();
}

void sum_clicked(lv_event_t * e)
{
  clear_all_dies();
}

void rolled_clicked(lv_event_t * e)
{
  remove_rolled_die();
}

void d4_clicked(lv_event_t * e)
{
  roll_new_die(4);
  update_rolled_section();
}

void d6_clicked(lv_event_t * e)
{
  roll_new_die(6);
  update_rolled_section();
}

void d8_clicked(lv_event_t * e)
{
  roll_new_die(8);
  update_rolled_section();
}

void d10_clicked(lv_event_t * e)
{
  roll_new_die(10);
  update_rolled_section();
}

void d12_clicked(lv_event_t * e)
{
  roll_new_die(12);
  update_rolled_section();
}

void d20_clicked(lv_event_t * e)
{
  roll_new_die(20);
  update_rolled_section();
}

#if LV_USE_LOG != 0
void print_error( const char * buf )
{
    Serial.println(buf);
    Serial.flush();
}
#endif

void setup() {
    Serial.begin(115200);
    delay(2000);
    Serial.println("Started");
    
    device_setup(LV_DISP_ROT_90);
    #if LV_USE_LOG != 0
      lv_log_register_print_cb( print_error );
    #endif
    ui_init();
    update_rolled_section();
    Serial.println("Initialized");
    Serial.flush();
}


void loop() {
    delay(1);
    device_loop();
}

