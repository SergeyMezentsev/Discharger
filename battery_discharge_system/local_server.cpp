// This file represents server's API

#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>

#include "local_server.h"
#include "html_page.h"
#include "voltages_structure.h"

void handleRoot();
void sendVoltages();
void getVoltages();
void manageVoltages();
void handleNotFound();

WebServer server(80);

void handleRoot() {
    Serial.println("handleroot();");
    server.send(200, "text/html", pageHTML);
}

void sendVoltages()
{
    Serial.println("senVoltages();");
    String voltages_str("");
    voltages_str += voltages->battery_mv;
    voltages_str += ";";
    voltages_str += voltages->relay_1_mv;
    voltages_str += ";";
    voltages_str += voltages->relay_2_mv;
    voltages_str += ";";
    voltages_str += voltages->relay_3_mv;
    voltages_str += ";";
    voltages_str += voltages->hysteresis_mv;
    
    server.send(200, "text/plain", voltages_str);    
}

void getVoltages()
{
    voltages->battery_mv = (int) (server.arg(String("Bat_V"))).toInt();
    voltages->relay_1_mv = (int) (server.arg(String("R_1_V"))).toInt();
    voltages->relay_2_mv = (int) (server.arg(String("R_2_V"))).toInt();
    voltages->relay_3_mv = (int) (server.arg(String("R_3_V"))).toInt();
    voltages->hysteresis_mv = (int) (server.arg(String("His_V"))).toInt();
    
    server.send(200, "text/plain", "");
}

void manageVoltages()
{
    if (server.method() == HTTP_GET)
    {
        sendVoltages();
    } else if (server.method() == HTTP_POST) {
        getVoltages();
    } else {
        handleNotFound();
    }
}

void handleNotFound() {
    Serial.println("handleNotFound();");
    server.send(404, "text/plain", String("Page is absent!"));
}

void init_server(const char* ssid)
{
    Serial.begin(115200);

    // Start the server
    if (!WiFi.softAP(ssid)) {
        log_e("Soft AP creation failed.");
        while(1);
    }

    // Code to show IP address
    // This device sets IP address to "192.168.4.1"
    //
    // IPAddress myIP = WiFi.softAPIP();
    // Serial.print("AP IP address: ");
    // Serial.println(myIP);

    // Set HTTP handlers
    server.on("/", handleRoot);
    server.on("/voltages", manageVoltages);
    server.onNotFound(handleNotFound);

    // Debuf info
    // server.begin();
    // Serial.println("HTTP server started");
}


void handle_server()
{
    server.handleClient();
}












