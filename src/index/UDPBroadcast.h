#ifndef UDPBROADCAST_H
#define UDPBROADCAST_H

#include <WiFi.h>
#include <WiFiUdp.h>

class UDPBroadcast {
private:
    WiFiUDP udp;
    const char* ssid;
    const char* password;
    int udpPort;
    IPAddress broadcastIP;
    
    char rxBuffer[256];
    char txBuffer[256];
    int txIndex;
    
    bool calculateBroadcastIP();

public:
    UDPBroadcast();
    
    // Network management
    bool begin(const char* ssid, const char* password, int port = 5000);
    bool begin(int port = 5000); // Use if WiFi is already connected
    void disconnect();
    bool isConnected();
    IPAddress getLocalIP();
    IPAddress getBroadcastIP();
    
    // Main loop processing - call this in your loop()
    void update();
    
    // Send methods (like Serial.print)
    size_t print(const String &s);
    size_t print(const char str[]);
    size_t print(char c);
    size_t print(int n);
    size_t print(unsigned int n);
    size_t print(long n);
    size_t print(unsigned long n);
    size_t print(double n);
    
    size_t println(const String &s);
    size_t println(const char str[]);
    size_t println(char c);
    size_t println(int n);
    size_t println(unsigned int n);
    size_t println(long n);
    size_t println(unsigned long n);
    size_t println(double n);
    size_t println(); // just newline
    
    // Receive methods
    int available();
    String readString();
    String readStringUntil(char terminator);
    int read();
    void flush();
    
    // Advanced methods
    void sendRaw(const uint8_t* data, size_t length);
    void setEchoEnabled(bool enabled);
    void setAutoFlush(bool enabled); // Auto-send on newline
    
private:
    bool echoEnabled = true;
    bool autoFlush = true;
    void flushTX(); // Send buffered data
};

extern UDPBroadcast UDPBroadcaster;

#endif