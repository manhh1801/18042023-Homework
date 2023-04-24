#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdio.h>
#include <sys/unistd.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{

    /*<Setting up socket and connection>*/

    printf("\n");

    int client=socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    struct sockaddr_in cl_addr;
    cl_addr.sin_family=AF_INET;
    cl_addr.sin_addr.s_addr=inet_addr("127.0.0.1");
    cl_addr.sin_port=htons(9900);

    if(connect(client, (struct sockaddr*)&cl_addr, sizeof(cl_addr)))
    {
        perror("Connect failed.\n");
        return 1;
    }
    printf("Connected: %s:%d\n", inet_ntoa(cl_addr.sin_addr), ntohs(cl_addr.sin_port));

    /*<>*/


    /*<Communicating>*/

    //Initialize storage:

    char *buf=NULL; int buf_size=0;

    //Receive and process input data:

    char name[64]="";
    printf("Computer's name: ");
    fgets(name, 64, stdin);
    buf_size+=strlen(name);
    if(buf_size>0) {name[buf_size-1]='\0';}
    buf=realloc(buf, buf_size);
    memcpy(buf, name, buf_size);
    fflush(stdin);

    unsigned int disk_count;
    printf("Number of disk: ");
    scanf("%d", &disk_count);
    getchar();

    for(int index=0; index<disk_count; index++)
    {
        char disk_info[3];

        char disk_name;
        printf("Type name of disk %d: ", index+1);
        scanf("%c", &disk_name);
        disk_info[0]=disk_name;
        getchar();

        unsigned short disk_size;
        printf("Type size of disk %d: ", index+1);
        scanf("%hu", &disk_size);
        disk_info[1]=(disk_size>>8)&0xFF;
        disk_info[2]=(disk_size>>0)&0xFF;
        getchar();

        buf_size+=3;
        buf=realloc(buf, buf_size);
        memcpy(buf+buf_size-3, disk_info, 3);
    }

    //Send data to server:

    write(client, &buf_size, 4);
    write(client, buf, buf_size);
    printf("Sent to server.\n");

    free(buf);

    printf("\n");

    /*<>*/


    /*<Close connection>*/

    close(client);
    return 1;

    /*<>*/

}