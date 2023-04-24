#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdio.h>
#include <sys/unistd.h>
#include <stdlib.h>
#include <string.h>

int main()
{

    printf("\n");

    /*<Setting up socket>*/

    int sender=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    struct sockaddr_in send_addr;
    send_addr.sin_family=AF_INET;
    send_addr.sin_addr.s_addr=inet_addr("127.0.0.1");
    send_addr.sin_port=htons(9000);

    /*<>*/


    /*<Communicating>*/

    //Initialize storage:

    unsigned int buf_count=0;
    char **buf_storage=NULL;

    //Read file:

    char file_name[256];
    printf("Type file name: ");
    fgets(file_name, 256, stdin);
    file_name[strlen(file_name)-1]='\0';
    fflush(stdin);
    FILE *file=fopen(file_name, "rb");

    while(1)
    {
        char *buf=malloc(64);
        int byte_read=fread(buf, 1, 64, file);

        if(byte_read!=0)
        {
            buf_count++;
            buf_storage=(char**)realloc(buf_storage, 8*buf_count);
            buf_storage[buf_count-1]=buf;
            if(byte_read<64)
            {
                for(int index=byte_read; index<64; index++)
                {
                    buf[index]='\0';
                }
                break;
            }
        }
        else {break;}
    }

    fclose(file);

    //Send data to server:

    sendto(sender, &buf_count, 4, 0, (struct sockaddr*)&send_addr, sizeof(send_addr));

    printf("Sent data to receiver: \"");
    for(int index=0; index<buf_count; index++)
    {
        sendto(sender, buf_storage[index], 64, 0, (struct sockaddr*)&send_addr, sizeof(send_addr));
        printf("%s", buf_storage[index]);
        free(buf_storage[index]);
    }
    printf("\"\n");

    free(buf_storage);

    printf("\n");

    /*<>*/


    /*<Close connection>*/

    close(sender);
    return 1;

    /*<>*/

}