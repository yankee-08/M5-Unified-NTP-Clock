#include <M5Unified.h> // M5Unifiedライブラリをプログラムで使用可能にします。

#include <WiFi.h>
// #include <math.h>
// #include "time.h"
// Local
#include "main.h"
#include "config.h"

void drawNumberNormal(uint8_t x_start, uint8_t y_start, uint8_t number, uint8_t bar_width, uint8_t bar_length, uint8_t bar_gap, uint8_t corner_radius, uint16_t color_value)
{
  if (number > 10)
  {
    number = 10;
  }
  // top
  if (digits_normal[number] & 0b0000000000000001)
    M5.Display.fillRoundRect(x_start, y_start, bar_length, bar_width, corner_radius, color_value);
  // upper-left
  if (digits_normal[number] & 0b0000000000000010)
    M5.Display.fillRoundRect((x_start - bar_gap * 2), (y_start + bar_gap), bar_width, bar_length, corner_radius, color_value);
  // under-left
  if (digits_normal[number] & 0b0000000000000100)
    M5.Display.fillRoundRect((x_start - bar_gap * 2), (y_start + bar_gap + bar_length * 1), bar_width, bar_length, corner_radius, color_value);
  // bottom
  if (digits_normal[number] & 0b0000000000001000)
    M5.Display.fillRoundRect(x_start, (y_start + bar_length * 2), bar_length, bar_width, corner_radius, color_value);
  // under-right
  if (digits_normal[number] & 0b0000000000010000)
    M5.Display.fillRoundRect((x_start + bar_length), (y_start + bar_gap + bar_length * 1), bar_width, bar_length, corner_radius, color_value);
  // upper-right
  if (digits_normal[number] & 0b0000000000100000)
    M5.Display.fillRoundRect((x_start + bar_length), (y_start + bar_gap), bar_width, bar_length, corner_radius, color_value);
  // center
  if (digits_normal[number] & 0b0000000001000000)
    M5.Display.fillRoundRect(x_start, (y_start + bar_length * 1), bar_length, bar_width, corner_radius, color_value);
}

void BlinkCount::incrementCount(void)
{
  ++count;
}

void BlinkCount::resetCount(void)
{
  count = 0;
}

boolean BlinkCount::isHideDisplay(void)
{
  if ((count / LCD_DISP_BLINK_LOOP_CNT % 2) == 0)
  {
    return false;
  }
  else
  {
    return true;
  }
}

void SystemClock::backupCurrentTime(void)
{
  prev_year = year;
  prev_month = month;
  prev_day = day;
  prev_hour = hour;
  prev_minute = minute;
  prev_week_day = week_day;
  prev_second = second;
}

void SystemClock::updateBySoftTimer(uint32_t elasped_second)
{
  struct tm *local_time;
  time_t timer_add = timer + elasped_second;
  local_time = localtime(&timer_add);
  year = local_time->tm_year + 1900;
  month = local_time->tm_mon + 1;
  day = local_time->tm_mday;
  hour = local_time->tm_hour;
  minute = local_time->tm_min;
  week_day = local_time->tm_wday;
  second = local_time->tm_sec;
}

void SystemClock::updateByNtp(void)
{
  Serial.println("---NTP ACCESS---");
  if (!getLocalTime(&time_info))
  {
    year = 0;
    month = 0;
    day = 0;
    hour = 0;
    minute = 0;
    week_day = 0;
    second = 0;
    timer = 0;
  }
  else
  {
    year = time_info.tm_year + 1900;
    month = time_info.tm_mon + 1;
    day = time_info.tm_mday;
    hour = time_info.tm_hour;
    minute = time_info.tm_min;
    week_day = time_info.tm_wday;
    second = time_info.tm_sec;
    timer = mktime(&time_info);
  }
}

void displayDateTimeScreen()
{
  static boolean ntp_access_flag = true;
  static uint32_t base_milli_time;
  uint32_t elasped_second = 0;
  uint32_t elasped_milli_time = 0;
  uint32_t diff_milli_time = 0;

  // uint16_t bkground_color = M5.Display.color565(200, 200, 0);
  if (is_state_changed == true)
  {
    M5.Display.fillScreen(bkground_color);
    ntp_access_flag = true;
    is_state_changed = false;
  }

  if (ntp_access_flag == true)
  {
    base_milli_time = millis();
    Serial.print("base_milli_time:");
    Serial.println(base_milli_time);
    cl_system_clock.updateByNtp();
    ntp_access_flag = false;
  }
  else
  {
    diff_milli_time = millis() - base_milli_time;
    if (diff_milli_time > NTP_ACCESS_MS_INTERVAL)
    {
      ntp_access_flag = true;
    }
    // Serial.print("diff_milli_time:");
    // Serial.println(diff_milli_time);
    elasped_second = diff_milli_time / 1000;
    cl_system_clock.updateBySoftTimer(elasped_second);
    // base_milli_time = elasped_milli_time;
  }
  // Serial.print("elasped_second:");
  // Serial.println(elasped_second);

  M5.Display.setTextColor(TFT_BLACK);
  M5.Display.setTextSize(2);

  // Month
  if (cl_system_clock.month != cl_system_clock.prev_month)
  {
    drawNumberNormal(10, LCD_CLOCK_YMD_DISP_Y_POS, LCD_DIGITS_CLEAR_ELM_NO, LCD_SMALL_BAR_WIDTH, LCD_SMALL_BAR_LENGTH, LCD_SMALL_BAR_GAP, LCD_SMALL_BAR_CORNER_RADIUS, bkground_color);
    drawNumberNormal(35, LCD_CLOCK_YMD_DISP_Y_POS, LCD_DIGITS_CLEAR_ELM_NO, LCD_SMALL_BAR_WIDTH, LCD_SMALL_BAR_LENGTH, LCD_SMALL_BAR_GAP, LCD_SMALL_BAR_CORNER_RADIUS, bkground_color);
  }
  drawNumberNormal(10, LCD_CLOCK_YMD_DISP_Y_POS, (cl_system_clock.month / 10), LCD_SMALL_BAR_WIDTH, LCD_SMALL_BAR_LENGTH, LCD_SMALL_BAR_GAP, LCD_SMALL_BAR_CORNER_RADIUS, TFT_BLACK);
  drawNumberNormal(35, LCD_CLOCK_YMD_DISP_Y_POS, (cl_system_clock.month % 10), LCD_SMALL_BAR_WIDTH, LCD_SMALL_BAR_LENGTH, LCD_SMALL_BAR_GAP, LCD_SMALL_BAR_CORNER_RADIUS, TFT_BLACK);

  M5.Display.drawLine(60, LCD_SMALL_BAR_LENGTH * 2 + 10, 70, 10, TFT_BLACK);

  // Day
  if (cl_system_clock.day != cl_system_clock.prev_day)
  {
    drawNumberNormal(80, LCD_CLOCK_YMD_DISP_Y_POS, LCD_DIGITS_CLEAR_ELM_NO, LCD_SMALL_BAR_WIDTH, LCD_SMALL_BAR_LENGTH, LCD_SMALL_BAR_GAP, LCD_SMALL_BAR_CORNER_RADIUS, bkground_color);
    drawNumberNormal(105, LCD_CLOCK_YMD_DISP_Y_POS, LCD_DIGITS_CLEAR_ELM_NO, LCD_SMALL_BAR_WIDTH, LCD_SMALL_BAR_LENGTH, LCD_SMALL_BAR_GAP, LCD_SMALL_BAR_CORNER_RADIUS, bkground_color);
  }
  drawNumberNormal(80, LCD_CLOCK_YMD_DISP_Y_POS, (cl_system_clock.day / 10), LCD_SMALL_BAR_WIDTH, LCD_SMALL_BAR_LENGTH, LCD_SMALL_BAR_GAP, LCD_SMALL_BAR_CORNER_RADIUS, TFT_BLACK);
  drawNumberNormal(105, LCD_CLOCK_YMD_DISP_Y_POS, (cl_system_clock.day % 10), LCD_SMALL_BAR_WIDTH, LCD_SMALL_BAR_LENGTH, LCD_SMALL_BAR_GAP, LCD_SMALL_BAR_CORNER_RADIUS, TFT_BLACK);

  // Year
  if (cl_system_clock.year != cl_system_clock.prev_year)
  {
    drawNumberNormal(180, LCD_CLOCK_YMD_DISP_Y_POS, LCD_DIGITS_CLEAR_ELM_NO, LCD_SMALL_BAR_WIDTH, LCD_SMALL_BAR_LENGTH, LCD_SMALL_BAR_GAP, LCD_SMALL_BAR_CORNER_RADIUS, bkground_color);
    drawNumberNormal(205, LCD_CLOCK_YMD_DISP_Y_POS, LCD_DIGITS_CLEAR_ELM_NO, LCD_SMALL_BAR_WIDTH, LCD_SMALL_BAR_LENGTH, LCD_SMALL_BAR_GAP, LCD_SMALL_BAR_CORNER_RADIUS, bkground_color);
    drawNumberNormal(230, LCD_CLOCK_YMD_DISP_Y_POS, LCD_DIGITS_CLEAR_ELM_NO, LCD_SMALL_BAR_WIDTH, LCD_SMALL_BAR_LENGTH, LCD_SMALL_BAR_GAP, LCD_SMALL_BAR_CORNER_RADIUS, bkground_color);
    drawNumberNormal(255, LCD_CLOCK_YMD_DISP_Y_POS, LCD_DIGITS_CLEAR_ELM_NO, LCD_SMALL_BAR_WIDTH, LCD_SMALL_BAR_LENGTH, LCD_SMALL_BAR_GAP, LCD_SMALL_BAR_CORNER_RADIUS, bkground_color);
  }
  drawNumberNormal(180, LCD_CLOCK_YMD_DISP_Y_POS, (cl_system_clock.year / 1000), LCD_SMALL_BAR_WIDTH, LCD_SMALL_BAR_LENGTH, LCD_SMALL_BAR_GAP, LCD_SMALL_BAR_CORNER_RADIUS, TFT_BLACK);
  drawNumberNormal(205, LCD_CLOCK_YMD_DISP_Y_POS, ((cl_system_clock.year % 1000) / 100), LCD_SMALL_BAR_WIDTH, LCD_SMALL_BAR_LENGTH, LCD_SMALL_BAR_GAP, LCD_SMALL_BAR_CORNER_RADIUS, TFT_BLACK);
  drawNumberNormal(230, LCD_CLOCK_YMD_DISP_Y_POS, (((cl_system_clock.year % 1000) % 100) / 10), LCD_SMALL_BAR_WIDTH, LCD_SMALL_BAR_LENGTH, LCD_SMALL_BAR_GAP, LCD_SMALL_BAR_CORNER_RADIUS, TFT_BLACK);
  drawNumberNormal(255, LCD_CLOCK_YMD_DISP_Y_POS, (((cl_system_clock.year % 1000) % 100) % 10), LCD_SMALL_BAR_WIDTH, LCD_SMALL_BAR_LENGTH, LCD_SMALL_BAR_GAP, LCD_SMALL_BAR_CORNER_RADIUS, TFT_BLACK);

  // hour
  if (cl_system_clock.hour != cl_system_clock.prev_hour)
  {
    drawNumberNormal(30, LCD_CLOCK_HM_DISP_Y_POS, LCD_DIGITS_CLEAR_ELM_NO, LCD_LARGE_BAR_WIDTH, LCD_LARGE_BAR_LENGTH, LCD_LARGE_BAR_GAP, LCD_LARGE_BAR_CORNER_RADIUS, bkground_color);
    drawNumberNormal(95, LCD_CLOCK_HM_DISP_Y_POS, LCD_DIGITS_CLEAR_ELM_NO, LCD_LARGE_BAR_WIDTH, LCD_LARGE_BAR_LENGTH, LCD_LARGE_BAR_GAP, LCD_LARGE_BAR_CORNER_RADIUS, bkground_color);
    M5.Display.setTextColor(bkground_color);
    M5.Display.drawString("PM", 20, LCD_CLOCK_PM_STR_DISP_Y_POS);
    M5.Display.setTextColor(TFT_BLACK);
  }
  drawNumberNormal(30, LCD_CLOCK_HM_DISP_Y_POS, (cl_system_clock.hour / 10), LCD_LARGE_BAR_WIDTH, LCD_LARGE_BAR_LENGTH, LCD_LARGE_BAR_GAP, LCD_LARGE_BAR_CORNER_RADIUS, TFT_BLACK);
  drawNumberNormal(95, LCD_CLOCK_HM_DISP_Y_POS, (cl_system_clock.hour % 10), LCD_LARGE_BAR_WIDTH, LCD_LARGE_BAR_LENGTH, LCD_LARGE_BAR_GAP, LCD_LARGE_BAR_CORNER_RADIUS, TFT_BLACK);

  // Sec
  if (cl_system_clock.second != cl_system_clock.prev_second)
  {
    drawNumberNormal(180, LCD_CLOCK_YMD_DISP_Y_POS + 40, LCD_DIGITS_CLEAR_ELM_NO, LCD_SMALL_BAR_WIDTH, LCD_SMALL_BAR_LENGTH, LCD_SMALL_BAR_GAP, LCD_SMALL_BAR_CORNER_RADIUS, bkground_color);
    drawNumberNormal(205, LCD_CLOCK_YMD_DISP_Y_POS + 40, LCD_DIGITS_CLEAR_ELM_NO, LCD_SMALL_BAR_WIDTH, LCD_SMALL_BAR_LENGTH, LCD_SMALL_BAR_GAP, LCD_SMALL_BAR_CORNER_RADIUS, bkground_color);
  }
  drawNumberNormal(180, LCD_CLOCK_YMD_DISP_Y_POS + 40, (cl_system_clock.second / 10), LCD_SMALL_BAR_WIDTH, LCD_SMALL_BAR_LENGTH, LCD_SMALL_BAR_GAP, LCD_SMALL_BAR_CORNER_RADIUS, TFT_BLACK);
  drawNumberNormal(205, LCD_CLOCK_YMD_DISP_Y_POS + 40, (cl_system_clock.second % 10), LCD_SMALL_BAR_WIDTH, LCD_SMALL_BAR_LENGTH, LCD_SMALL_BAR_GAP, LCD_SMALL_BAR_CORNER_RADIUS, TFT_BLACK);

  if (cl_blink_count.isHideDisplay() == false)
  {
    M5.Display.fillEllipse(150, 120, 4, 4, TFT_BLACK);
    M5.Display.fillEllipse(150, 150, 4, 4, TFT_BLACK);
  }
  else
  {
    M5.Display.fillEllipse(150, 120, 4, 4, bkground_color);
    M5.Display.fillEllipse(150, 150, 4, 4, bkground_color);
  }

  // minute
  if (cl_system_clock.minute != cl_system_clock.prev_minute)
  {
    drawNumberNormal(185, LCD_CLOCK_HM_DISP_Y_POS, LCD_DIGITS_CLEAR_ELM_NO, LCD_LARGE_BAR_WIDTH, LCD_LARGE_BAR_LENGTH, LCD_LARGE_BAR_GAP, LCD_LARGE_BAR_CORNER_RADIUS, bkground_color);
    drawNumberNormal(250, LCD_CLOCK_HM_DISP_Y_POS, LCD_DIGITS_CLEAR_ELM_NO, LCD_LARGE_BAR_WIDTH, LCD_LARGE_BAR_LENGTH, LCD_LARGE_BAR_GAP, LCD_LARGE_BAR_CORNER_RADIUS, bkground_color);
  }
  drawNumberNormal(185, LCD_CLOCK_HM_DISP_Y_POS, (cl_system_clock.minute / 10), LCD_LARGE_BAR_WIDTH, LCD_LARGE_BAR_LENGTH, LCD_LARGE_BAR_GAP, LCD_LARGE_BAR_CORNER_RADIUS, TFT_BLACK);
  drawNumberNormal(250, LCD_CLOCK_HM_DISP_Y_POS, (cl_system_clock.minute % 10), LCD_LARGE_BAR_WIDTH, LCD_LARGE_BAR_LENGTH, LCD_LARGE_BAR_GAP, LCD_LARGE_BAR_CORNER_RADIUS, TFT_BLACK);

  // MONTH DATE
  const String s_month = "MONTH";
  const String s_date = "DATE";
  M5.Display.drawString(s_month, 10, LCD_CLOCK_MD_STR_DISP_Y_POS);
  M5.Display.drawString(s_date, 80, LCD_CLOCK_MD_STR_DISP_Y_POS);

  uint8_t week_count = 0;
  const char aweek[7][4] = {"SUN", "MON", "TUE", "WED", "THU", "FRI", "SAT"};
  uint8_t day_week_now = 0;
  day_week_now = cl_system_clock.week_day;
  for (week_count = 0; week_count < 7; ++week_count)
  {
    if ((week_count == day_week_now) && (cl_blink_count.isHideDisplay() == true))
    {
      M5.Display.setTextColor(bkground_color);
      M5.Display.drawString(aweek[week_count], week_count * 45 + 10, LCD_CLOCK_WEEK_STR_DISP_Y_POS);
      M5.Display.setTextColor(TFT_BLACK);
    }
    else
    {
      M5.Display.drawString(aweek[week_count], week_count * 45 + 10, LCD_CLOCK_WEEK_STR_DISP_Y_POS);
    }
  }

  cl_system_clock.backupCurrentTime();
}

// 曜日を算出
uint8_t subZeller(uint32_t year, uint32_t month, uint32_t day)
{
  if (month < 3)
  {
    --year;
    month += 12;
  }
  return (year + (year / 4) - (year / 100) + (year / 400) + ((13 * month + 8) / 5) + day) % 7;
}

void setup()
{
  auto cfg = M5.config();

  // 設定ファイル詳細：https://docs.m5stack.com/ja/api/m5unified/m5unified_appendix#m5config
  cfg.serial_baudrate = 115200;
  cfg.output_power = true;

  M5.begin(cfg); // 設定した値でデバイスを開始する。

  M5.Display.setBrightness(100);

  WiFi.mode(WIFI_MODE_STA);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED)
  {
    delay(100);
  }

  M5.Display.fillScreen(TFT_WHITE);
  cl_blink_count.resetCount();
  delay(1000);

  // init time setting
  configTime(gmt_offset_sec, daylight_offset_sec, ntp_server_1st, ntp_server_2nd);
  struct tm time_info;
  if (!getLocalTime(&time_info))
  {
    M5.Display.fillScreen(TFT_RED);
    delay(3000);
  }

  Serial.println("hello world");
}

void loop()
{
  M5.update();

  M5.Display.setRotation(1);
  displayDateTimeScreen();

  cl_blink_count.incrementCount();
  delay(500);
}
