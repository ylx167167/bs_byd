
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include "../include/global.h"
#include "../include/mssage.h"
#include <stdio.h>
#include <string.h>
#include  "../include/thread.h"
#include "../include/socket.h"
#include "../include/cJsonFile.h"
#include "../include/cJSON.h"
#include "../include/bs.h"

int Inittcp(){
//客户端只需要一个套接字文件描述符，用于和服务器通信
//描述服务器的socket
struct sockaddr_in serverAddr;
if((m_tcp.clientSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
{
perror("socket");
return 1;
}
serverAddr.sin_family = AF_INET;
serverAddr.sin_port = htons(SERVER_PORT);
//指定服务器端的ip，本地测试：127.0.0.1
//inet_addr()函数，将点分十进制IP转换成网络字节序IP
serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
if(connect(m_tcp.clientSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
{
perror("connect");
return 1;
}
printf("连接到主机...\n");
memset((char *)&thread_pool,0,sizeof(thread_pool));
return 0;
}


int main()
{
 char j_buf[1024]={0};
 char *buf;
cJSON *root=NULL;
//清空全局tcp变量
memset((char *)&m_tcp,0,sizeof(m_tcp));
Inittcp();
FILE *fp;
thread_create();
char *filename ="/home/lucy/sambaFile/byd/test.json";
bsstruct bs;
memset(&bs,'0',sizeof(bsstruct));


buf=Bs_GetCharBufFromJsonFIle(filename);
printf("%s\r\n",buf);
// //发送内容
m_tcp.sendbuf=buf;
m_tcp.id=1;
m_tcp.send_c=true;
sleep(3);
while(1){

}
return 0;
 
}


