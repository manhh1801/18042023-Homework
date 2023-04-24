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
    sv_addr.sin_port=htons(9000);

    if(bind(server, (struct sockaddr*)&sv_addr, sizeof(sv_addr)))
    {
        perror("Bind address to socket failed.\n");
        return 1;
    }

    /*<>*/


    /*<Waiting and accepting connection>*/

    if(listen(server, 5))
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

    unsigned int buf_count=0;

    //Receive and process data from client:

    int numstr_count=0;
    char last_char='\0';

    read(client, &buf_count, 4);

    printf("Received data from client: \"");
    for(int index=0; index<buf_count; index++)
    {
        char *buf=malloc(64);
        read(client, buf, 64);
        printf("%s", buf);

        for(int index=0; index<64; index++)
        {
            if(buf[index]=='0') {last_char='0';}
            else if(buf[index]=='1'&&last_char=='0') {last_char='1';}
            else if(buf[index]=='2'&&last_char=='1') {last_char='2';}
            else if(buf[index]=='3'&&last_char=='2') {last_char='3';}
            else if(buf[index]=='4'&&last_char=='3') {last_char='4';}
            else if(buf[index]=='5'&&last_char=='4') {last_char='5';}
            else if(buf[index]=='6'&&last_char=='5') {last_char='6';}
            else if(buf[index]=='7'&&last_char=='6') {last_char='7';}
            else if(buf[index]=='8'&&last_char=='7') {last_char='8';}
            else if(buf[index]=='9'&&last_char=='8') {last_char='9';}
            else {last_char='\n';}

            if(last_char=='9') {numstr_count++;}
        }

        free(buf);
    }
    printf("\"\n");
    printf("Number of \"0123456789\" found: %d", numstr_count);
    printf("\n");

    printf("\n");

    /*<>*/


    /*<Close connection>*/

    close(client);
    close(server);
    return 1;

    /*<>*/

}