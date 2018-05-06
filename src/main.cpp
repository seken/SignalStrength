#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>
#include "ESP8266WiFi.h"

#if (SSD1306_LCDHEIGHT != 32)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

#define OLED_RESET 16
Adafruit_SSD1306 display(OLED_RESET);

static const unsigned char PROGMEM wifi_high[] = {
	0x00,0x38,0x40,0x98,0xa0,0xa8,0x00	
};
static const unsigned char PROGMEM wifi_med[] = {
0x00
,0x00
,0x00
,0x18
,0x20
,0x28
,0x00
};
static const unsigned char PROGMEM wifi_low[] = {
		0x00
,0x00
,0x00
,0x00
,0x00
,0x08
,0x00
};
void setup() {
	Wire.pins(4, 5);

	Wire.begin();
	display.begin(SSD1306_SWITCHCAPVCC, 0x3C);

	display.clearDisplay();
	display.display();

	WiFi.mode(WIFI_STA);
	WiFi.disconnect();
}

void loop() {

	int n = WiFi.scanNetworks();
	if (n == 0) {
		display.setTextSize(1);
		display.setTextColor(WHITE);
		display.setCursor(0,0);
		display.println("no networks found");
		display.display();
		delay(200);
		display.clearDisplay();
	} else {

		auto pages = (n+2) / 3;

		for (auto page = 0; page < pages; ++page) {
			display.setTextSize(1);
			display.setTextColor(WHITE);
			display.setCursor(0,0);

			display.print(n);
			display.print(" networks ");
			if (pages > 1) {
				display.print("page ");
				display.print(page+1);
				display.print("/");
				display.println(pages);
			}

			for (int i = 0; i < 3 && (i + page*3) < n; ++i) {
				auto j = i + page*3;
				display.setCursor(7,8*(i+1));
				display.print(WiFi.SSID(j).substring(0,16));
				display.print((WiFi.encryptionType(j) == ENC_TYPE_NONE) ? " " : "*");
				display.println(WiFi.RSSI(j));
				if (WiFi.RSSI(j) > -50) {
					display.drawBitmap(0, 8*(i+1), wifi_high, 5, 7, 1);
				} else if (WiFi.RSSI(j) > -70) {
					display.drawBitmap(0, 8*(i+1), wifi_med, 5, 7, 1);
				} else {
					display.drawBitmap(0, 8*(i+1), wifi_low, 5, 7, 1);
				}
			}
			display.display();
			delay(2000);
			display.clearDisplay();
		}
	}

}
