#include "../include/thread.h"
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
pthread_t thread[2];  //两个线程

void *send_rec_mssage_thread(void * p){
    printf("线程1判断\n");
    while (1)
    {
    if(m_tcp.send_c ==true){
        // printf("m_tcp.send_c  :%d \r\n",m_tcp.send_c );
    pthread_mutex_lock(&mut);
    m_tcp.sendlen = sendmssage(m_tcp.clientSocket,m_tcp.id,m_tcp.sendbuf);
    // sleep(2);
    // m_tcp.reclen =  recmssage(m_tcp.clientSocket,m_tcp.recvbuf);

    printf("m_tcp.sendlen:%d",m_tcp.sendlen);
    m_tcp.send_c=false;
    // m_tcp.id=0;      
     memset((char *)&m_tcp.sendbuf,0,sizeof(m_tcp.sendbuf)); 

    }
    sleep(1);
    m_tcp.reclen =  recmssage(m_tcp.clientSocket,m_tcp.recvbuf);
    printf("m_tcp.reclen:%d",m_tcp.reclen);
    pthread_mutex_unlock(&mut);
    
    if(thread_pool[0].close==true){
    pthread_exit(NULL);
    }
    
    }
    
}

void *uart_thread(){
  int fd;
  int snum = 0;
  int tnum = 0;
  int j =0;
  unsigned char buf[1024];
  unsigned char p[1024];
  memset(buf,'\0',1024);
  memset(p,'\0',1024);

  wiringPiSetup();  // 使用wiring编码去初始化GPIO序号
  if ((fd = serialOpen ("/dev/ttyS3", 115200)) < 0)
  {
    fprintf (stderr, "Unable to open serial device: %s\n", strerror (errno)) ;
    return 1 ;
  }
  printf("open uart\r\n");
  serialPrintf(fd, "open uart\r\n");  // 串口打印
  
   for (;;)
  { 
      /* 串口缓存区记录到数组，遇到\n结束*/
      while (1) {
        snum = serialDataAvail(fd);  // 获取串口接收缓存区的可用字节数
        if (snum > 0) {
          buf[tnum] = serialGetchar(fd);  // 从接收缓存区读取一个字节
          tnum++;
          if (buf[tnum - 1] == '\n') break;

        }
        // delay(2000);
        // printf("%d  ,  %d\n",snum,tnum);
      }

      /* 输出接收到的数据 */
        const char *bufpar = buf ;
        const char *bufdata = p;
        strcpy(p,buf);
      //  const unsigned char *tempar = bufstr.tem ;
      //  const unsigned char *humpar = bufstr.hum ;
      int i = 0;

      const char *chartem ;
      const char *charhum ;
      const char *charposition ;
      while (tnum--) {
        serialPutchar(fd, buf[i++]);  // 通过串口发送字节`        
      }
      chartem=strtok(p," ") ;
      charhum=strtok(NULL," ") ;
      charposition=strtok(NULL," ") ;
  
      printf("%s\r\n",chartem);
      printf("%s\r\n",charhum);
      printf("%s\r\n",charposition);

      serialPuts(fd, chartem);
      serialPuts(fd, charhum);
      serialPuts(fd, charposition);
      serialPuts(fd, bufpar);  // 通过串口发送字节`
      memset(buf,'\0',1024);
      tnum = 0;
  }
}

void thread_wait(){
    if (thread_pool[0].p!=0)
    {
        pthread_join(thread_pool[0].p,NULL);
        printf("线程1已退出\n");
    }
    if (thread_pool[1].p!=0)
    {
        pthread_join(thread_pool[1].p,NULL);
        printf("线程1已退出\n");
    }
}

void thread_create(){
    pthread_mutex_init(&mut,NULL);
    int temp;
    printf("create_thread\n");    
    if((temp = pthread_create(&thread_pool[0].p, NULL, send_rec_mssage_thread, NULL)) != 0)  //comment2     
        {
            printf("线程1创建失败!\n");
        }
    if((temp = pthread_create(&thread_pool[1].p, NULL, uart_thread, NULL)) != 0)  //comment2     
        {
            printf("线程2创建失败!\n");
        }
    else{
        strcpy(thread_pool[0].name,TCP_REC_SEND);
        thread_pool[0].close=false; 
        }
 
}


