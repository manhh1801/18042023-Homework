#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdio.h>
#include <sys/unistd.h>
#include <stdlib.h>

int main()
{

    /*<Setting up socket>*/

    printf("\n");

    int server=socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    struct sockaddr_in sv_addr;
    sv_addr.sin_family=AF_INET;
    sv_addr.sin_addr.s_addr=htonl(INADDR_ANY);
    sv_addr.sin_port=htons(9900);

    if(bind(server, (struct sockaddr*)&sv_addr, sizeof(sv_addr)))
    {
        perror("Bind address to socket failed.\n");
        return 1;
    }

    /*<>*/


    /*<Waiting and accepting connection>*/

    if(listen(server, 1))
    {
        perror("Connect failed.\n");
        return 1;
    }

    struct sockaddr_in cl_addr;
    int cl_addr_length=sizeof(cl_addr);

    int client=accept(server, (struct sockaddr*)&cl_addr, &cl_addr_length);
    printf("Accepted connection: %s:%d\n", inet_ntoa(cl_addr.sin_addr), ntohs(cl_addr.sin_port));

    /*<>*/


    /*<Communicating>*/

    //Initialize storage:
    char *buf=NULL; int buf_size=0;

    //Receive and process data from client:
    read(client, &buf_size, 4);
    buf=malloc(buf_size);
    read(client, buf, buf_size);

    int index=0;

    char name[64];
    while(1)
    {
        name[index]=*(buf+index);
        if(name[index++]==0) {break;}
    }
    printf("Computer's name: %s\n", name);

    printf("Disks:\n");
    while(index<buf_size-2)
    {
        char disk_name=buf[index];
        unsigned short disk_size=(buf[index+1]<<8)+buf[index];
        printf("%c - %hu\n", disk_name, disk_size);
        index+=3;
    }
    printf("\n");

    /*<>*/


    /*<Close connection>*/

    close(client);
    close(server);
    return 1;

    /*<>*/

}