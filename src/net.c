#include "net.h"

#include <stdio.h>
#include <string.h>
#include <winsock.h>

bool net_download(const char* host, const char* path, const char* dest_path) {
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(1, 1), &wsa) != 0) {
        fprintf(stderr, "Error: Failed to initialize network stack\n");
        return false;
    }

    struct hostent* he = gethostbyname(host);
    if (he == NULL || he->h_addr_list[0] == NULL) {
        fprintf(stderr, "Error: Could not resolve host '%s'\n", host);
        WSACleanup();
        return false;
    }

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(80);
    memcpy(&addr.sin_addr, he->h_addr_list[0], he->h_length);

    SOCKET s = socket(AF_INET, SOCK_STREAM, 0);
    if (s == INVALID_SOCKET) {
        fprintf(stderr, "Error: Failed to create socket\n");
        WSACleanup();
        return false;
    }

    if (connect(s, (struct sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR) {
        fprintf(stderr, "Error: Failed to connect to '%s'\n", host);
        closesocket(s);
        WSACleanup();
        return false;
    }

    char req[256];
    sprintf(req, "GET %s HTTP/1.0\r\nHost: %s\r\n\r\n", path, host);
    send(s, req, strlen(req), 0);

    FILE* fp = fopen(dest_path, "wb");
    if (fp == NULL) {
        fprintf(stderr, "Error: Failed to open '%s' for writing\n", dest_path);
        closesocket(s);
        WSACleanup();
        return false;
    }

    char buf[4096];
    int n, header_done = 0;

    while ((n = recv(s, buf, sizeof(buf), 0)) > 0) {
        if (!header_done) {
            char* body = strstr(buf, "\r\n\r\n");

            if (body != NULL) {
                header_done = 1;
                body += 4;
                fwrite(body, 1, n - (body - buf), fp);
            }
        } else {
            fwrite(buf, 1, n, fp);
        }
    }

    fclose(fp);
    closesocket(s);
    WSACleanup();

    return true;
}
