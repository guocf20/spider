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
#include<strings.h>
#include<error.h>
#include<sys/time.h>
#include<unistd.h>
#include<ctype.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include<stdbool.h>
#include <openssl/crypto.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/rand.h>


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

bool is_https = false;

char *get_src_path(char *url)
{
     char host[128] = {'\0'};
     char *p = NULL;
     char *q = NULL;
     p = strstr(url, "//");
     if (p == NULL)
     {
         q = strstr(url, "/");
         if (q == NULL)
         {
             return strdup("/");
         }
         else
         {
             return strdup(q);
         }
     }
     else
     {
         q = strstr(p + 2, "/");
         if(q == NULL)
         {
              return strdup("/");
         }
         else
         {
             return strdup(q);
         }
     }
}


char *get_host(char *url)
{
     char host[128] = {'\0'};
     char *p = NULL;
     char *q = NULL;
     p = strstr(url, "//");
     if (p == NULL)
     {
         q = strstr(url, "/");
         if (q == NULL)
         {
             return strdup(url);
         }
         else
         {
             memcpy(host, url, q - url);
             return strdup(host);
         }
     }
     else
     {
         q = strstr(p + 2, "/");
         if(q == NULL)
         {
              return strdup(p + 2);
         }
         else
         {
             memcpy(host, p + 2, q - p - 2);
             return strdup(host);
    
         }
     }
}


char init_header(char *url)
{
    char host[HOST_MAX] = {'\0'};

    int header_len = sizeof(buf)/sizeof(buf[0]);

    char *src = get_src_path(url);
    char *host_name = get_host(url);
    char cur_entry[128] = {'\0'};

    strncat(host, "Host:", header_len);
    strncat(host, host_name, header_len);
    strncat(host, "\r\n", sizeof("\r\n"));


    snprintf(cur_entry, 127, "GET %s HTTP/1.1\r\n", src);

    strncat(buf, cur_entry, header_len);
    strncat(buf, "Accept: */*\r\n", header_len);
    strncat(buf, host, header_len); 
    strncat(buf, "User-Agent: Wget/1.13.4 (linux-gnu)\r\n", header_len);
    strncat(buf, "Connection: Keep-Alive\r\n\r\n",header_len);
    free(src);
    free(host_name);
    return; 
}

int main(int argc, char *argv[])
{
    if (strlen(argv[1]) > HOST_LEFT)
    {
        printf("error host is too long %s\n", argv[1]);
        return 1;
    }
    init_header(argv[1]);
    printf("%s\n", buf);
    
    struct addrinfo *host=NULL;
    struct addrinfo *aip=NULL;
    struct addrinfo hint;
    memset(&hint,0,sizeof(hint));
    hint.ai_family=AF_INET;
    char *hostname = get_host(argv[1]);
    
    int i = 0;
    //instead of getaddrbynamea
    if(strstr(argv[1], "https://") != NULL)
    {
        getaddrinfo(hostname, "https", &hint, &host);
    }
    else
    { 
        i = getaddrinfo(hostname, "http", &hint,&host);
    }
    if (i != 0)
    {
        printf(" error %s\n",gai_strerror(i));
        return 1;
    }
    int http_client = socket(AF_INET, SOCK_STREAM, 0);
    if (http_client == -1)
    {
        perror("socket");
    }

    int conn = -1;

    SSL *ssl;
    SSL_CTX *ctx;

    //get available {ip port} to connect
    for(aip=host;aip != NULL; aip=aip->ai_next)
    {
        conn =  connect(http_client, (struct sockaddr *)aip->ai_addr,sizeof(struct sockaddr));
     

        char buf[100] = {'\0'};
        
        struct sockaddr_in *sa;

        sa = (struct sockaddr_in *)aip->ai_addr;
        

        printf("name: %s\nip:%s\n\n", aip->ai_canonname,   
                inet_ntop(AF_INET, &sa->sin_addr.s_addr, buf, sizeof (buf)));  



        if (conn ==  0)
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
    if(strstr(argv[2], "https") != NULL)
    {
        is_https = true;
    }

    if(is_https == true)  
    {
        SSL_library_init();
        SSL_load_error_strings();
        ctx = SSL_CTX_new(SSLv23_client_method());
    
        if(ctx == NULL)
        {
           exit(1);
        }
        ssl = SSL_new(ctx);
        int ret = SSL_set_fd(ssl, http_client);
        ret = SSL_connect(ssl);
    }

    config_recv_timeout(http_client, 2, 0);
    //config_recv_buf_len(http_client, 1024);
    if(is_https == true)
    {
         printf("write %s\n", buf);
         SSL_write(ssl, buf, strlen(buf));
    }
    else
    {
         printf("write %s\n", buf);
        if (send(http_client, buf, strlen(buf),MSG_DONTWAIT)== -1)
        {
            perror("send");
        }
    }
    char rec[1000000];
    char html[1000000];
    char buf[512];
    int j = 0;
    int k = 0;
    
    if(is_https == true)
    
    {
        int ret = SSL_read(ssl,rec, 256);
        while(ret > 0)
        {
            strcat(html, rec);
            memset(rec, '\0', sizeof(rec));
            ret =SSL_read(ssl, rec, 256);
        }
        printf("%s\n", html);
        return 0;
    }
    else
    {
        recvfrom(http_client, buf, 1024, MSG_PEEK, NULL, NULL);

        char *length = NULL;
        length = strstr(buf, "Content-Length:");

        int len = 0;
        bool use_content = false;
        if(length != NULL)
        {
            use_content = true;
            len = atoi(&length[strlen("Content-Length:")]);
            if(len < strlen(buf))
            {
               len = strlen(buf);
            } 
        }

        printf("content-length = %d %s \n", len, use_content == true?"true":"false");

        int left = len;
        while (left >  0 || use_content == false)
        {
            if(use_content == true)
            {
               k = recv(http_client,rec,len, 0);
            }
            else 
            {
               k = recv(http_client,rec,256,0);
            }
            if(k <= 0)
            {
                perror("recv");
                printf("end left %d %d\n", left, k);
                break;
            }
            printf("left %d %d\n", left, k);
            left -= k;
            j++;
         }
     }
    char *cao=rec;

    if (memcmp(rec, RESPONSE_1,strlen(RESPONSE_1)) == 0)
    {
        printf("recv ok\n");
    }
    else if (memcmp(rec, RESPONSE_2,strlen(RESPONSE_2)) == 0)
    {
        printf("forbidden \n");
    }
    else if (memcmp(rec,RESPONSE_3,strlen(RESPONSE_3)) == 0)
    {
        printf("moved******\n");
    }

    char *c = NULL;
    char *q = rec;
    char url_buf[1000]={0};
    
    while((c=strstr(q, url_start)) != NULL && q != NULL)
    {
        int i = 0;
        int j = 0;
        while (isspace(*(c + i + url_start_len)) && *(c + i + url_start_len) != '\0')
        {
            i++;
        }
        j = i;
        while (*(c + i + 1 + url_start_len) != '>' && *(c + i + 1 + url_start_len) != '\0' && *(c + i + 1 + url_start_len) != ' ')
        {
            i++;
        }
        snprintf(url_buf, (i-j + 2), "%s", (c+url_start_len + j));
        printf("%s\n",url_buf);
        q = c+url_start_len + i;
    }
    close(http_client);
    return 0; 
}
