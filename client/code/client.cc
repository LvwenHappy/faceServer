#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define MAXLINE 4096*5

double* load_image(double* img, const char* path, int img_height = 28, int img_width = 28, int img_channels = 3);
void getSendData(char* sendData);

int main(int argc, char**argv)
{
    //double* img = load_image("./../data/test_0.txt");

    int sockfd = 0;
    char recvline[10];
    char* sendline = new char[MAXLINE];
    struct sockaddr_in servaddr;

    //创建socket
    if( (sockfd = socket(AF_INET,SOCK_STREAM,0)) == -1) {
        printf(" create socket error: %s (errno :%d)\n",strerror(errno),errno);
        return 0;
    }
    
    memset(&servaddr,0,sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(6666);
    //IP地址从“点分十进制”转换到“二进制整肃”
    if( inet_pton(AF_INET,"127.0.0.1", &servaddr.sin_addr) <=0 ) {
        printf("inet_pton error for %s\n",argv[1]);
        return 0;
    }

    //连接
    if( connect(sockfd,(struct sockaddr*)&servaddr,sizeof(servaddr)) <0) {
        printf(" connect socket error: %s(errno :%d)\n",strerror(errno),errno);
        return 0;
    }


    //while(1)
    {
        
        // memset(sendline, 0, MAXLINE);
        // uint32_t imgSize = 4;
        // strncpy(sendline, (const char*)&imgSize, 4);
        // if (send(sockfd,sendline,4,0) <0) {
        //     printf("send msg error: %s(errno :%d)\n",strerror(errno),errno);
        //     return 0;
        // }
        // printf("send head ok\n");
        
        // memset(sendline, 0, MAXLINE);
        //uint32_t imgSize = 3*28*28;
        //strncpy(sendline, (const char*)&imgSize, 4);
        // printf("send msg to server:\n");
        //fgets(sendline+4,4096,stdin);
        // sendline[strlen(sendline+4)+4-1] = '\0';
        // printf("strlen:\t%ld\n", strlen(sendline+4));

        //getchar();
        // char* sendline = new char[16];
        // double dTmp = 0.756863;
        // uint64_t imgSize = 8;//3*28*28*sizeof(double);
        // strncpy(sendline, (const char*)&imgSize, 8);
        // strncpy(sendline+8, (const char*)&dTmp, 8);
        getSendData(sendline);
        // uint64_t dataSize = *((uint64_t*)sendline);
        // printf("%ld\n", dataSize);
        // double* img = (double*)(sendline+8);
        // printf("%lf\n", *(img+28*28*3));
        //send发送
        printf("%lf\n", *(double*)(sendline+16));
        if (send(sockfd,sendline,18824,0) <0) {
            printf("send msg error: %s(errno :%d)\n",strerror(errno),errno);
            return 0;
        }

        int n = recv(sockfd,recvline,MAXLINE,0);
        recvline[n] = '\0';
        printf("recv msg from client:%f\n",*(float*)recvline);
    }

    close(sockfd);
    return 0;
}

void getSendData(char* sendData)
{
    memset(sendData, 0, MAXLINE);
    uint64_t imgSize = 3*28*28*sizeof(double);
    strncpy(sendData, (const char*)&imgSize, 8);

    load_image((double*)(sendData+8), "./../data/test_1.txt");
}


double* load_image(double* img, const char* path, int img_height, int img_width, int img_channels)
{
    //double* img = new double[3*28*28];
    int i = 0, j = 0, k = 0;



    // int i = 0, j = 0, k = 0;
    // double*** img = (double***)malloc(sizeof(double**)*img_channels);
    // for(i = 0; i < img_channels; i++)
    // {
    //     img[i] = (double**)malloc(sizeof(double*)*img_height);
    // }
    // for(i = 0; i < img_channels; i++)
    // {
    //     for(j = 0; j < img_height; j++)
    //     {
    //         img[i][j] = (double*)malloc(sizeof(double)*img_width);
    //     }
    // }

    FILE* fp_img = fopen(path, "r");

    float fTemp = 0;
    for (i = 0; i < img_channels; i++)
    {
        for (j = 0; j < img_height; j++)
        {
            for (k = 0; k < img_width; k++)
            {
                fscanf(fp_img, "%f", &fTemp);
                //printf("%d--%f\n", i*img_channels + j*img_height + k, fTemp);
                img[i*img_height*img_width + j*img_width + k] = fTemp;
            }
        }
    }

    return img;
}