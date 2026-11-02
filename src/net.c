#include "net.h"

#include <stdio.h>
#include <stdlib.h>
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

    char req[2048];
    snprintf(req, sizeof(req), "GET %s HTTP/1.0\r\nHost: %s\r\n\r\n", path, host);
    send(s, req, strlen(req), 0);

    FILE* fp = NULL;
    char buf[4096];
    int n, header_done = 0;

    while ((n = recv(s, buf, sizeof(buf) - 1, 0)) > 0) {
        buf[n] = '\0';

        if (!header_done) {
            int status = 0;
            char* space = strchr(buf, ' ');

            if (n >= 12 && strncmp(buf, "HTTP/", 5) == 0 && space != NULL) {
                status = atoi(space + 1);
            }

            if (status != 200) {
                if (status == 404) {
                    fprintf(stderr, "Error: HTTP 404 Not Found\n");
                } else if (status == 301 || status == 302) {
                    fprintf(stderr, "Error: Resource moved (HTTP %d redirect)\n", status);
                } else {
                    fprintf(stderr, "Error: Server returned HTTP %d\n", status);
                }

                closesocket(s);
                WSACleanup();
                return false;
            }

            char* body = strstr(buf, "\r\n\r\n");

            if (body != NULL) {
                header_done = 1;
                body += 4;

                fp = fopen(dest_path, "wb");
                if (fp == NULL) {
                    fprintf(stderr, "Error: Failed to open '%s' for writing\n", dest_path);
                    closesocket(s);
                    WSACleanup();
                    return false;
                }

                int body_len = n - (body - buf);
                if (body_len > 0) {
                    fwrite(body, 1, body_len, fp);
                }
            }
        } else {
            fwrite(buf, 1, n, fp);
        }
    }

    if (fp != NULL) {
        fclose(fp);
    }

    closesocket(s);
    WSACleanup();

    return header_done != 0;
}
