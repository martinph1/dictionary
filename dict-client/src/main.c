#include <stdio.h>        //printf
#include <string.h>       //strlen
#include <sys/socket.h>   //socket
#include <arpa/inet.h>    //inet_addr
#include <stdlib.h>

#include "message.pb-c.h"

int main(int argc, char **argv) {
    int sock;
        struct sockaddr_in server;
        void *out_buffer = NULL;
        int command_id = 0;
        size_t len = 0;
        ServerCommand cmd = SERVER_COMMAND__INIT;

        //Create socket
        sock = socket(AF_INET , SOCK_STREAM , 0);
        if (sock == -1)
        {
            printf("Could not create socket");
        }
        puts("Socket created");

        server.sin_addr.s_addr = inet_addr("127.0.0.1");
        server.sin_family = AF_INET;
        server.sin_port = htons( 5006 );

        //Connect to remote server
        if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
        {
            perror("connect failed. Error");
            return 1;
        }

        puts("Connected\n");

        //keep communicating with server
        while(1)
        {
            printf("Enter command id: ");
            scanf("%d", &command_id);
            cmd.command_id = command_id;

            len = server_command__get_packed_size(&cmd);

            out_buffer = malloc(len);

            if (out_buffer) {
                len = server_command__pack (&cmd, out_buffer);
            }

            if (len > 0) {
                send(sock, out_buffer, len, 0);
            }

            free(out_buffer);
            out_buffer = NULL;

            if (command_id == 20) break;
        }

        close(sock);
        return 0;
}
