#include <stdio.h>
#include <vector>

//sockets
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")

#define assert(exp, ...) if (!(exp)) { printf(__VA_ARGS__); exit(1); }

#define PORT "6969"

enum S2C_PktID {
    Ping = 0x0000,
};

enum C2S_PktID {
    Pong = 0x0000,
};

class VPRClient { //simple vapor client
private:
    int accountID;
    int socket;
    char screenName[32];
    char message[512];
};

class VPRServer { //simple 'vapor' server :O
private:
    int result;
    WSAData wsaData;
    struct addrinfo *addrinf = NULL, hints;
    SOCKET sockListen = INVALID_SOCKET;
    SOCKET sockClient = INVALID_SOCKET;
    
    bool running = false;
    std::vector<VPRClient*> clients;
public:
    bool init() {
        //TODO: server initalization

        //initalize WS2_32.dll
        if ((result = WSAStartup(MAKEWORD(2, 2), &wsaData)) != 0) {
            return false;
        }

        //address info
        ZeroMemory(&hints, sizeof(hints));
        hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_protocol = IPPROTO_TCP;
        hints.ai_flags = AI_PASSIVE;

        if ((result = getaddrinfo(NULL, PORT, &hints, &addrinf)) != 0) {
            WSACleanup();
            return false;
        }

        sockListen = socket(addrinf->ai_family, addrinf->ai_socktype, addrinf->ai_protocol);
        if (sockListen == INVALID_SOCKET) {
            freeaddrinfo(addrinf);
            WSACleanup();
            return false;
        }

        if ((result = bind(sockListen, addrinf->ai_addr, (int)addrinf->ai_addrlen)) != 0) {
            freeaddrinfo(addrinf);
            closesocket(sockListen);
            WSACleanup();
            return false;
        }

        freeaddrinfo(addrinf); //we dont need this anymore

        printf("INFO!! -> server initialization successful!\n");
        running = true;
        return true;
    };
    void update() {
        printf("INFO!! -> listening on port %s\n", PORT);
        for (;;) {
            if (!running) {
                printf("INFO!! -> server shutdown\n");
                break;
            }

            if (listen(sockListen, SOMAXCONN) == SOCKET_ERROR) {
                printf("ERROR!! -> listen failure\n");
                closesocket(sockListen);
                WSACleanup();
                return;
            }

            sockClient = accept(sockListen, NULL, NULL);
            if (sockClient == INVALID_SOCKET) {
                printf("ERROR!! -> accept failure\n");
                closesocket(sockListen);
                WSACleanup();
                return;
            }

            // for (VPRClient *client : clients) {

            // }
        }
    };
    void cleanup() {

    };
};

int main() {
    VPRServer server;

    if (!server.init()) {
        printf("ERROR!! -> failed to initialize VPRServer\n");
        return 1;
    }

    server.update();
    server.cleanup();

    return 0;
}