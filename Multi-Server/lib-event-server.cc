//作者：卢帅领
//muti-server2.cc
//版本：2.0
#include<iostream>
#include<string>
#include<sys/socket.h>
#include<unistd.h>
#include<cerrno>
#include<arpa/inet.h>
#include<pthread.h>
#include<cstring>
#include<event.h>

#define thread_num 5

using namespace std;


struct Toclient //客户端通知结构体
{
char packet[100];
int sock_name;
};



void *creat_base(void* arg);
void receive(int fd, short int events, void* arg);//线程函数



int main()
{
	Toclient pack[200];
	struct sockaddr_in serv_addr;//结构体
	

	int domain=AF_INET;  //ipv4
	int type=SOCK_STREAM; //字节流式socket
	int protocol=0; //TCP传输协议

	int sock=socket(domain,type,protocol); //创建套接字

	serv_addr.sin_family=AF_INET;
	serv_addr.sin_port=htons(5000);
	serv_addr.sin_addr.s_addr=inet_addr("127.0.0.1");



	if(bind(sock,(struct sockaddr*)&serv_addr,sizeof(serv_addr))<0)//套接字描述符绑定地址
	{
	cout<<"服务器绑定失败..."<<endl;
	exit(1);
	}

	cout<<"服务器开启，等待连接..."<<endl;


	int ls=listen(sock,1);
	if(ls)
	{
	cout<<"侦听中..."<<endl;
	}


	


	//初始化线程属性并设置为分离状态
	pthread_attr_t attr;//线程属性
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);//线程设置为分离状态
	for(int i=0;i<sizeof(pack)/sizeof(Toclient);i++)
	{
	pack[i].sock_name=accept(sock,NULL,NULL);//主线程accept连接
	
	if(pack[i].sock_name){cout<<"客户端"<<pack[i].sock_name<<"连接成功"<<endl;}
	pthread_t tid; //创建子线程
	Toclient *sock_struct=&pack[i];
	int ret=pthread_create(&tid,NULL,creat_base,(void*)sock_struct);//返回0,表示成功；表示出错，返回-1,非0的转为布尔值1
	if(ret)
	{
	cout<<"创建失败"<<ret<<endl;
	exit(-1);
	}
	
	
	}
	

	int close(sock); //关闭套接字
	pthread_attr_destroy(&attr);
	pthread_exit(NULL);
}












	/*libevent编程，事件通知库（网络库）获取待监听的内容的fd；
创建一个event_base；
创建一个event,指定待监听的fd，待监听事件的类型，以及事件放生时的回调函数及传给回调函数的参数；
将event添加到event_base的事件管理器中；
开启event_base的事件处理循环；
（异步）当事件发生的时候，调用前面设置的回调函数。
*/
void *creat_base(void* arg)
{
	
	Toclient *pack=(Toclient*)arg;
	//int ssock=pack->sock_name;
	struct event_base *base=event_base_new();
	//assert(base !=NULL);
	struct event* listen_event;
	//Toclient *llll=&pack;
	listen_event=event_new(base,pack->sock_name,EV_READ|EV_PERSIST,receive,(void*)pack);
	event_add(listen_event,NULL);
	event_base_dispatch(base);
	return (void *)0;
}


//接受函数
void receive(int fd, short int events, void* arg){
	Toclient *pack=(Toclient*)arg;    
	char buffer[80];
       while(1){
       
 	memset(&buffer,0,sizeof(buffer));

 
 	int hh=recv(pack->sock_name,buffer,sizeof(buffer),0);
 	//cout<<hh<<endl;
 
      if(hh==-1)
      {
      perror("读失败");
      pthread_exit(NULL);
      
      }
      else if(hh==0)
      {
      cout<<"客户端"<<pack->sock_name<<"断开连接"<<endl;
      close(pack->sock_name);
      break;
      
      }
      else{
	strcpy(pack->packet,buffer);
	
 
	cout<<"客户端"<<pack->sock_name<<"发送"<<buffer<<endl;   //输出接受到的信息 
	char budder[]="receive success";
	strcat(pack->packet,budder);

	send(pack->sock_name,pack->packet,sizeof(pack->packet),0);
	}
	}

	pthread_exit(NULL);

}