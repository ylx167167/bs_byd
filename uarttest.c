
#include <stdio.h>
#include <wiringPi.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <wiringSerial.h>
#include <unistd.h>
// typedef struct sensordata
// {
//   char tem[6];
//   char hum[6];
//   char position;
// }CharData;

int main(void) {
  // CharData  bufstr ;

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
