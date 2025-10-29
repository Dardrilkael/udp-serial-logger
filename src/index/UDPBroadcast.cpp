#include "UDPBroadcast.h"

UDPBroadcast::UDPBroadcast() {
    txIndex = 0;
    ssid = nullptr;
    password = nullptr;
    udpPort = 5000;
}

bool UDPBroadcast::begin(const char* ssid, const char* password, int port) {
    this->ssid = ssid;
    this->password = password;
    this->udpPort = port;
    
    Serial.print("[UDP] Connecting to Wi-Fi...");
    WiFi.begin(ssid, password);
    
    unsigned long startTime = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - startTime < 10000) {
        Serial.print(".");
        delay(500);
    }
    
    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("\n[UDP] Failed to connect to WiFi!");
        return false;
    }
    
    Serial.println("\n[UDP] WiFi Connected!");
    return begin(port);
}

bool UDPBroadcast::begin(int port) {
    this->udpPort = port;
    
    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("[UDP] Error: WiFi not connected!");
        return false;
    }
    
    if (!calculateBroadcastIP()) {
        Serial.println("[UDP] Error: Could not calculate broadcast IP!");
        return false;
    }
    
    udp.begin(udpPort);
    
    Serial.print("[UDP] Local IP: ");
    Serial.println(WiFi.localIP());
    Serial.print("[UDP] Broadcast IP: ");
    Serial.println(broadcastIP);
    Serial.print("[UDP] Port: ");
    Serial.println(udpPort);
    Serial.println("[UDP] Ready for broadcast communication!");
    
    return true;
}

bool UDPBroadcast::calculateBroadcastIP() {
    IPAddress ip = WiFi.localIP();
    IPAddress mask = WiFi.subnetMask();
    
    if (ip == INADDR_NONE || mask == INADDR_NONE) {
        return false;
    }
    
    broadcastIP = ~mask | ip;
    return true;
}

void UDPBroadcast::update() {
    // Check for incoming UDP packets
    int packetSize = udp.parsePacket();
    if (packetSize) {
        int len = udp.read(rxBuffer, sizeof(rxBuffer) - 1);
        if (len > 0) {
            rxBuffer[len] = '\0';
            
            if (echoEnabled) {
                Serial2.print("[UDP RX] ");
                Serial2.println(rxBuffer);
            }
            
            // Echo back to sender (optional)
            udp.beginPacket(udp.remoteIP(), udp.remotePort());
            udp.write((uint8_t*)"ESP32: ", 7);
            udp.write((uint8_t*)rxBuffer, len);
            udp.endPacket();
        }
    }
}

// Print methods
size_t UDPBroadcast::print(const String &s) {
    return print(s.c_str());
}

size_t UDPBroadcast::print(const char str[]) {
    if (str == nullptr) return 0;
    
    size_t n = 0;
    const char *p = str;
    while (*p) {
        if (txIndex < sizeof(txBuffer) - 1) {
            txBuffer[txIndex++] = *p;
            n++;
        }
        p++;
        
        // Auto-flush on buffer full or newline with autoFlush enabled
        if (txIndex >= sizeof(txBuffer) - 1 || (autoFlush && *(p-1) == '\n')) {
            flushTX();
        }
    }
    return n;
}

size_t UDPBroadcast::print(char c) {
    if (txIndex < sizeof(txBuffer) - 1) {
        txBuffer[txIndex++] = c;
        
        if (txIndex >= sizeof(txBuffer) - 1 || (autoFlush && c == '\n')) {
            flushTX();
        }
        return 1;
    }
    return 0;
}

size_t UDPBroadcast::print(int n) {
    return print(String(n));
}

size_t UDPBroadcast::print(unsigned int n) {
    return print(String(n));
}

size_t UDPBroadcast::print(long n) {
    return print(String(n));
}

size_t UDPBroadcast::print(unsigned long n) {
    return print(String(n));
}

size_t UDPBroadcast::print(double n) {
    return print(String(n));
}

// Println methods
size_t UDPBroadcast::println(const String &s) {
    size_t n = print(s);
    n += println();
    return n;
}

size_t UDPBroadcast::println(const char str[]) {
    size_t n = print(str);
    n += println();
    return n;
}

size_t UDPBroadcast::println(char c) {
    size_t n = print(c);
    n += println();
    return n;
}

size_t UDPBroadcast::println(int n) {
    size_t count = print(n);
    count += println();
    return count;
}

size_t UDPBroadcast::println(unsigned int n) {
    size_t count = print(n);
    count += println();
    return count;
}

size_t UDPBroadcast::println(long n) {
    size_t count = print(n);
    count += println();
    return count;
}

size_t UDPBroadcast::println(unsigned long n) {
    size_t count = print(n);
    count += println();
    return count;
}

size_t UDPBroadcast::println(double n) {
    size_t count = print(n);
    count += println();
    return count;
}

size_t UDPBroadcast::println() {
    return print('\n');
}

void UDPBroadcast::flushTX() {
    if (txIndex > 0) {
        udp.beginPacket(broadcastIP, udpPort);
        udp.write((uint8_t*)txBuffer, txIndex);
        udp.endPacket();
        
        if (echoEnabled) {
            Serial2.print("[UDP TX] ");
            Serial2.write((uint8_t*)txBuffer, txIndex);
        }
        
        txIndex = 0;
    }
}

// Receive methods
int UDPBroadcast::available() {
    return udp.parsePacket();
}

String UDPBroadcast::readString() {
    if (udp.parsePacket()) {
        int len = udp.read(rxBuffer, sizeof(rxBuffer) - 1);
        if (len > 0) {
            rxBuffer[len] = '\0';
            return String(rxBuffer);
        }
    }
    return String();
}

String UDPBroadcast::readStringUntil(char terminator) {
    String result = readString();
    int index = result.indexOf(terminator);
    if (index >= 0) {
        result = result.substring(0, index);
    }
    return result;
}

int UDPBroadcast::read() {
    if (udp.parsePacket()) {
        if (udp.read() >= 0) {
            // For single character reads, we'd need to buffer the packet
            // This is simplified - consider buffering the entire packet
            return 1;
        }
    }
    return -1;
}

void UDPBroadcast::flush() {
    flushTX();
}

// Advanced methods
void UDPBroadcast::sendRaw(const uint8_t* data, size_t length) {
    udp.beginPacket(broadcastIP, udpPort);
    udp.write(data, length);
    udp.endPacket();
}

void UDPBroadcast::setEchoEnabled(bool enabled) {
    echoEnabled = enabled;
}

void UDPBroadcast::setAutoFlush(bool enabled) {
    autoFlush = enabled;
}

// Utility methods
void UDPBroadcast::disconnect() {
    udp.stop();
    WiFi.disconnect();
}

bool UDPBroadcast::isConnected() {
    return WiFi.status() == WL_CONNECTED;
}

IPAddress UDPBroadcast::getLocalIP() {
    return WiFi.localIP();
}

IPAddress UDPBroadcast::getBroadcastIP() {
    return broadcastIP;
}

// Global instance
UDPBroadcast UDPBroadcaster;