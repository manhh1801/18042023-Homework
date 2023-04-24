#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdio.h>
#include <sys/unistd.h>
#include <stdlib.h>

int main()
{

    /*<Setting up socket and connection>*/

    printf("\n");

    int client=socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    struct sockaddr_in cl_addr;
    cl_addr.sin_family=AF_INET;
    cl_addr.sin_addr.s_addr=inet_addr("127.0.0.1");
    cl_addr.sin_port=htons(9000);

    if(connect(client, (struct sockaddr*)&cl_addr, sizeof(cl_addr)))
    {
        perror("Connect failed.\n");
        return 1;
    }
    printf("Connected: %s:%d\n", inet_ntoa(cl_addr.sin_addr), ntohs(cl_addr.sin_port));

    /*<>*/


    /*<Communicating>*/

    //Initialize storage:

    unsigned int buf_count=0;
    char **buf_storage=NULL;

    //Read file:

    FILE *file=fopen("log.txt", "rb");

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

    write(client, &buf_count, 4);

    printf("Sent data to server: \"");
    for(int index=0; index<buf_count; index++)
    {
        write(client, buf_storage[index], 64);
        printf("%s", buf_storage[index]);
        free(buf_storage[index]);
    }
    printf("\"\n");

    free(buf_storage);

    printf("\n");

    /*<>*/


    /*<Close connection>*/

    close(client);
    return 1;

    /*<>*/

}