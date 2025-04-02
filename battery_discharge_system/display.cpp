// This file is responsible for display managing

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "display.h"

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino resetpin)

typedef struct {
    uint8_t x_name;
    uint8_t y_name;
    uint8_t x_voltage;
    uint8_t y_voltage;
    uint8_t name_is_highlighted;
    uint8_t voltage_is_chosen;
    char name[5];
    char voltage[7];
} line_t;

static Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
static line_t line_array[5];

static void update_line(uint8_t line);

void init_display()
{
    // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        Serial.begin(115200);
        Serial.println(F("SSD1306 allocation failed"));
        for (;;); // Don't proceed, loop forever
    }

    display.clearDisplay();     // Clear the buffer
    display.setTextSize(1);     // Display Text
    display.setTextColor(WHITE);

    line_array[BAT_LINE] = {
        10, 20, 40, 20, 0U, 0U,
        .name = {'B', 'a', 't', ':', '\0'},
        .voltage = {'-', '.', '-', '-', ' ', 'V', '\0'}
    };
    line_array[R_1_LINE] = {
        10, 30, 40, 30, 0U, 0U,
        .name = {'R', '_', '1', ':', '\0'},
        .voltage = {'2', '.', '8', '0', ' ', 'V', '\0'}
    };
    line_array[R_2_LINE] = {
        10, 40, 40, 40, 0U, 0U,
        .name = {'R', '_', '2', ':', '\0'},
        .voltage = {'2', '.', '1', '0', ' ', 'V', '\0'}
    };
    line_array[R_3_LINE] = {
        10, 50, 40, 50, 0U, 0U,
        .name = {'R', '_', '3', ':', '\0'},
        .voltage = {'1', '.', '3', '0', ' ', 'V', '\0'}
    };
    line_array[G_LINE] = {
        85, 30, 85, 40, 0U, 0U,
        .name = {' ', 'G', ':', ' ', '\0'},
        .voltage = {'0', '.', '3', '0', ' ', 'V', '\0'}
    };

    display.drawFastVLine(80, 20, 40, WHITE);

    update_line(BAT_LINE);
    update_line(R_1_LINE);
    update_line(R_2_LINE);
    update_line(R_3_LINE);
    update_line(G_LINE);
}


static void update_line(uint8_t line)
{
    display.setCursor(line_array[line].x_name, line_array[line].y_name);
    display.println(line_array[line].name);
    display.setCursor(line_array[line].x_voltage, line_array[line].y_voltage);
    display.println(line_array[line].voltage);
    display.display();
}


void set_line_voltage(uint8_t line, float voltage)
{
    sprintf(line_array[line].voltage, "%.2f V", voltage);
    display.fillRect(line_array[line].x_voltage, line_array[line].y_voltage, 36, 7, BLACK);
    display.setCursor(line_array[line].x_voltage, line_array[line].y_voltage);
    display.println(line_array[line].voltage);
    display.display();
}


void set_bat_voltage(float voltage)
{
    set_line_voltage(BAT_LINE, voltage);
}


void highlight_line(uint8_t line)
{
    line_array[line].name_is_highlighted = 1U;
    if (line == G_LINE)
        display.setCursor(line_array[line].x_name - 2, line_array[line].y_name);
    else
        display.setCursor(line_array[line].x_name - 7, line_array[line].y_name);
    display.println(">");
    display.display();
}


void dehighlight_line(uint8_t line)
{
    line_array[line].name_is_highlighted = 0U;
    if (line == G_LINE)
        display.fillRect(line_array[line].x_name - 2, line_array[line].y_name, 7, 7, BLACK);
    else
        display.fillRect(line_array[line].x_name - 7, line_array[line].y_name, 7, 7, BLACK);
    display.display();
}


void choose_line(uint8_t line)
{
    line_array[line].voltage_is_chosen = 1U;
    if (line == G_LINE)
        display.drawFastHLine(line_array[line].x_voltage, line_array[line].y_voltage + 8, 36, WHITE);
    else
    {
        display.setCursor(line_array[line].x_voltage - 7, line_array[line].y_voltage);
        display.println(">");
    }
    display.display();
}


void unchoose_line(uint8_t line)
{
    line_array[line].voltage_is_chosen = 0U;
    if (line == G_LINE)
        display.drawFastHLine(line_array[line].x_voltage, line_array[line].y_voltage + 8, 36, BLACK);
    else
        display.fillRect(line_array[line].x_voltage - 7, line_array[line].y_voltage, 7, 7, BLACK);
    display.display();
}

