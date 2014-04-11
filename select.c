/*---------------------
* Copyright(C)2014 All rights reserved
*author:guochengfeng
*last modify:2014--04--09
*email:guocf20@gmail.com
*=================================*/
#include<sys/select.h>
#include<stdio.h>
#include<sys/socket.h>
#include<unistd.h>
#include<string.h>
struct timeval server_out;
void main()
{
    server_out.tv_sec = 2;
    server_out.tv_usec = 0;
    int cli = socket(AF_INET, SOCK_STREAM, 0);
    fd_set readset;
    FD_ZERO(&readset);
    FD_SET(cli, &readset);
    FD_SET(STDIN_FILENO,&readset);
    char buf[20];
    memset(buf,0,20);
    //select(2,&readset,NULL,NULL,&server_out);
    while(1)
    {
        select(2,&readset,NULL,NULL,NULL);
        if (FD_ISSET(STDIN_FILENO,&readset))
        {
        read(STDIN_FILENO,buf,20);
        write(STDOUT_FILENO,buf,20);
        }
    }
}
