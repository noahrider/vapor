#include <stdio.h>

//sockets
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")

#define assert(exp, ...) if (!(exp)) { printf(__VA_ARGS__); exit(1); }

#define PORT "6969"

class VPRServer { //simple 'vapor' server :O
private:
    int result;
    WSAData wsaData;
public:
    bool init() {
        
    };
    void update() {

    };
    void cleanup() {

    };
};

int main() {
    VPRServer server;

    if (!server.init()) {
        printf("ERROR!! -> Failed to initialize VPRServer");
        return 1;
    }

    server.update();
    server.cleanup();

    return 0;
}