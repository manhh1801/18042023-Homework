#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdio.h>
#include <sys/unistd.h>
#include <stdlib.h>

int main()
{

    printf("\n");

    /*<Setting up socket and connection>*/

    int receiver=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    struct sockaddr_in recv_addr;
    recv_addr.sin_family=AF_INET;
    recv_addr.sin_addr.s_addr=htonl(INADDR_ANY);
    recv_addr.sin_port=htons(9000);

    if(bind(receiver, (struct sockaddr*)&recv_addr, sizeof(recv_addr)))
    {
        perror("Bind address to socket failed.\n");
        return 1;
    }

    /*<>*/


    /*<Communicating>*/

    //Initialize storage:

    unsigned int buf_count=0;

    //Receive from sender:

    recvfrom(receiver, &buf_count, 4, 0, NULL, NULL);

    printf("Received data from client: \"");
    for(int index=0; index<buf_count; index++)
    {
        char *buf=malloc(64);
        recvfrom(receiver, buf, 64, 0, NULL, NULL);
        printf("%s", buf);

        free(buf);
    }
    printf("\"\n");

    printf("\n");

    /*<>*/


    /*<Close connection>*/

    close(receiver);
    return 1;

    /*<>*/

}