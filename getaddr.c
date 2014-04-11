/*---------------------
* Copyright(C)2014 All rights reserved
*author:guochengfeng
*last modify:2014--04--06
*email:guocf20@gmail.com
*=================================*/
#include<stdio.h>
#include<netdb.h>
#include<sys/socket.h>
#include<string.h>
#include<error.h>
#include<sys/time.h>
#include<unistd.h>
#include<ctype.h>
#define HEADER_MAX 4096
#define HOST_MAX   100
#define URL_MAX    1000
#define HOST_LEFT   92           /*100 - strlen("Host:") - strlen("\r\n")- 1*/
char buf[HEADER_MAX]={0};
char host[HOST_MAX]={0};

char url_start[] = "href=";
char url_start_len = sizeof(url_start)/sizeof(url_start[0]) - 1;

#define HTTP_STATUS_OK 200
#define HTTP_STATUS_FORBIDDEN 403
#define HTTP_STATUS_REMOVE  301

#define RESPONSE_1  "HTTP/1.1 200 OK"
#define RESPONSE_2  "HTTP/1.1 403"
#define RESPONSE_3  "HTTP/1.1 301"
char *find_str(char *str1, int len1, char *str2, int len2)
{
    if (len1 < len2)
    {   
        return NULL;
    }   
    int j=0;
    while (str1 != '\0' && j < len1 - len2)
    {   
        if (memcmp(str1,str2, len2) == 0)
        {
            return (str1);
        }
        str1++;
        j++;
    }   
    return NULL;
}


int config_recv_buf_len(int socket, int len)
{
    if (setsockopt(socket,SOL_SOCKET, SO_RCVBUF, (void *)&len, sizeof(len)) != 0)
    {
        return 1;
    }
    return 0;
}


int config_recv_timeout(int socket, int sec, int usec)
{
    struct timeval t;
    t.tv_sec = sec;
    t.tv_usec =usec ;
    if (setsockopt(socket,SOL_SOCKET,SO_RCVTIMEO, (void *)&t,sizeof(struct timeval)) != 0)
    {
        return 1;
    }
    return 0;
}


int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("usage: ./getaddr url\n");
        return 1;
    }
    if (strlen(argv[1]) > HOST_LEFT)
    {
        printf("error host is too long %s\n", argv[1]);
        return 1;
    }
    //init header with argv[1]
    strncat(host, "Host: ", HOST_MAX);
    strncat(host, argv[1], HOST_MAX);
    strncat(host,"\r\n", HOST_MAX);
    int header_len = sizeof(buf)/sizeof(buf[0]);
    strncat(buf, "GET / HTTP/1.1\r\n", header_len);
    strncat(buf, "Accept: */*\r\n", header_len);
    strncat(buf, "User-Agent: Wget/1.13.4 (linux-gnu)\r\n", header_len);
    strncat(buf, host, header_len);
    strncat(buf, "Connection: Keep-Alive\r\n\r\n",header_len);
    
    struct addrinfo *host=NULL;
    struct addrinfo *aip=NULL;
    struct addrinfo hint;
    memset(&hint,0,sizeof(hint));
    hint.ai_family=AF_INET;
    //instead of getaddrbyname
    int i = getaddrinfo(argv[1],"http", &hint,&host);
    if (i != 0)
    {
        printf("%s\n",gai_strerror(i));
        return 1;
    }
    int http_client = socket(AF_INET, SOCK_STREAM, 0);
    if (http_client == -1)
    {
        perror("socket");
    }

    int conn = -1;

    //get available {ip port} to connect
    for(aip=host;aip != NULL; aip=aip->ai_next)
    {
        conn =  connect(http_client, (struct sockaddr *)aip->ai_addr,sizeof(struct sockaddr));
        if (conn == 0)
        {
            break;
        }
        else
        {
            printf("connect failed\n");
        
        }
    }
    
    if (conn == -1)
    {
        printf("connect failed\n");
        return 1;
    }
    config_recv_timeout(http_client, 2, 0);
    //config_recv_buf_len(http_client, 1024);
    if (send(http_client, buf, strlen(buf),MSG_DONTWAIT)== -1)
    {
        perror("send");
    }
    char rec[1000000];
    char html[1000000];
    int j = 0;
    int k = 0;
    //recv data
    while ((k = recv(http_client,rec,100000,0)) > 0)
    {
        j++;
        //printf("down loading...%d\n", k);
        strncat(html, rec, k);
    }
    char *cao=html;

//judge status
    if (memcmp(html, RESPONSE_1,strlen(RESPONSE_1)) == 0)
    {
        printf("recv ok\n");
    }
    else if (memcmp(html, RESPONSE_2,strlen(RESPONSE_2)) == 0)
    {
        printf("forbidden \n");
    }
    else if (memcmp(html,RESPONSE_3,strlen(RESPONSE_3)) == 0)
    {
        printf("moved\n");
        char *temp = find_str(cao,strlen(html),"Location:", 5);
        printf("%s",temp);
    }

    char *c = NULL;
    char *q = html;
    char url_buf[1000]={0};

//find url links
    while ((c=find_str(q,strlen(html), url_start, url_start_len)) != NULL && q != NULL)
    {
        int i = 0;
        int j = 0;
        while (isspace(*(c + i + url_start_len)) && *(c + i + url_start_len) != '\0')
        {
            i++;
        }
        j = i;
        while (*(c + i + 1+ url_start_len) != '>' && *(c + i + 1 + url_start_len) != '\0')
        {
            i++;
        }
        snprintf(url_buf, (i-j+1), "%s", (c+url_start_len + j + 1));
        printf("%s\n",url_buf);
        q = c+url_start_len + i;
    }
    close(http_client);
    return 0; 
}