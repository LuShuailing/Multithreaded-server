
//作者：卢帅领
/*muti-client2.cc*/
//版本：2.0

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<iostream>
#include<cstring>


using namespace std;


struct Toserver
{
char packet[800];

};




int main()
{

	Toserver pack;
	//创建套接字
	int sock = socket(AF_INET, SOCK_STREAM, 0);

	//服务器的ip为本地，端口号1234
	struct sockaddr_in serv_addr;
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	serv_addr.sin_port = htons(5000);
	
	//向服务器发送连接请求
	connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
	//发送并接收数据
	
	string buff;
	while(1){
        cout<<"输入信息:"<<endl;
        cin>>buff;
        int len=buff.length();
        char buffer[100];
        strcpy(buffer,buff.c_str());
        buffer[len]='\0';
        //char hello[200];
        //memset(&hello,0,sizeof(hello));
        //strcpy(hello,Packetcontent.c_str());
        
	send(sock, buffer, len,0);
	
	memset(&buffer,0,sizeof(buffer));
	
	recv(sock, buffer, sizeof(buffer),0);
        //printf("Serve send: %s\n", buffer);
       strcpy(pack.packet,buffer);
        //string Packetcontent=to_string(*(buffer));
        //for(int i=0;i<sizeof(buffer);i++)
        //{
 	cout<<pack.packet<<endl;
 	}
        
	//断开连接
	close(sock);

        return 0;
}

