#include <stdio.h>
#include <vector>
#include <cstdint>

//sockets
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")

#define assert(exp, ...) if (!(exp)) { printf(__VA_ARGS__); exit(1); }

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

//packet ids (server -> client)
enum class S2C_PktID : u16 {
    Ping = 0x0000,
};

//packet ids (client -> server)
enum class C2S_PktID : u16 {
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
    const char *port;
    
    bool running = false;
    std::vector<VPRClient*> clients;
public:
    bool init(const char *svport) {
        //TODO: server initalization
        port = svport;

        //initalize WS2_32.dll
        if ((result = WSAStartup(MAKEWORD(2, 2), &wsaData)) != 0) {
            printf("ERROR!! -> failure in WSAStartup (%i)\n", result);
            return false;
        }

        //address info
        ZeroMemory(&hints, sizeof(hints));
        hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_protocol = IPPROTO_TCP;
        hints.ai_flags = AI_PASSIVE;

        if ((result = getaddrinfo(NULL, port, &hints, &addrinf)) != 0) {
            printf("ERROR!! -> failure in getaddrinfo (%i)\n", result);
            WSACleanup();
            return false;
        }

        sockListen = socket(addrinf->ai_family, addrinf->ai_socktype, addrinf->ai_protocol);
        if (sockListen == INVALID_SOCKET) {
            printf("ERROR!! -> failure in socket (%i)\n", WSAGetLastError());
            freeaddrinfo(addrinf);
            WSACleanup();
            return false;
        }

        if ((result = bind(sockListen, addrinf->ai_addr, (int)addrinf->ai_addrlen)) != 0) {
            printf("ERROR!! -> failure in bind (%i)\n", result);
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
        printf("INFO!! -> listening on port %s\n", port);
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

            closesocket(sockListen);

            for (VPRClient *client : clients) {

            }
        }
    };
    void cleanup() {

    };
};

int main(int argc, char *argv[]) {
    const char *port = "6000";

    for (int i = 0; i < argc; i++) {
        if (strcmp(argv[i], "-port") == 0) { //open server on custom port
            port = argv[i + 1];
        }
    }

    VPRServer server;
    
    if (!server.init(port)) {
        printf("ERROR!! -> failed to initialize VPRServer\n");
        return 1;
    }

    server.update();
    server.cleanup();

    return 0;
}