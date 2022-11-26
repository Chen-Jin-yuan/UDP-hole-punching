#include <string>
#include <sys/socket.h>
#include <netinet/in.h>//sockaddr_in
#include <arpa/inet.h>//in_addr
#include <cstring>
#include <unistd.h>//close
#include <vector>

using namespace std;

void init_udp_Socket(int& listenfd, const int port)
{
    listenfd = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);//UDP
    if(listenfd < 0) 
    {
        printf("create listen socket error, port-%d\n",port);
        exit(1);
    }
    struct sockaddr_in socketaddr;
    socketaddr.sin_family = AF_INET;//ipv4
    socketaddr.sin_port = htons(port);
    socketaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    
    //Port reused
    int optval = 1;
    int ret = setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, (const void*)&optval, sizeof(int));
    if(ret == -1) 
    {
        close(listenfd);
        exit(1);
    }
    
    //bind
    if(bind(listenfd,(struct sockaddr *)&socketaddr,sizeof(socketaddr))==-1)
    {
        close(listenfd);
        exit(1);
    }
}

string udp_hole_punching(int listenfd)
{
    struct sockaddr_in gateway;
    socklen_t addr_len = sizeof(gateway);
    memset(&gateway, 0, sizeof(gateway));
    char recvbuf[128];
    memset(&recvbuf, 0, sizeof(recvbuf));

    int res = recvfrom(listenfd, recvbuf, 128, 0, (struct sockaddr *)&gateway, &addr_len);
    if(res < 0)
    {
        printf("udp hole punching receive error!\n");
        return "";
    }
    else
    {
        string ip = string(inet_ntoa(gateway.sin_addr));
        string port = to_string(ntohs(gateway.sin_port));
        
        string host = string(recvbuf);
        printf("udp hole punching  NAT ip: %s, port: %s; host:%s\n",ip.c_str(),port.c_str(),host.c_str());
        
        return ip+" "+port+" "+host;
    }
    return "";
}

vector<string> parse(string str)
{
    str = str + " ";//add a space
    vector<string> res;
    size_t pos = 0;
    size_t pos1;
    while ((pos1 = str.find(' ', pos)) != string::npos)
    {
        res.push_back(str.substr(pos, pos1 - pos));
        while (str[pos1] == ' ')
            pos1++;
        pos = pos1;
    }
    return res;//move rvalue
}

int main()
{
    int listenudp;
    const int port = 10000;
    init_udp_Socket(listenudp , port);
    string ip_port1 = udp_hole_punching(listenudp);
    string ip_port2 = udp_hole_punching(listenudp);
    if(ip_port1 == "" || ip_port2 == "")
        exit(1);

    vector<string> host1 = parse(ip_port1);
    vector<string> host2 = parse(ip_port2);

    //The server sends back using a NAT ip
    string ip1 = host1[0];
    string port1 = host1[1];
    string ip2 = host2[0];
    string port2 = host2[1];

    struct sockaddr_in socketaddr1;
    socketaddr1.sin_family = AF_INET;//ipv4
    socketaddr1.sin_port = htons(stoi(port1));
    inet_pton(AF_INET, ip1.c_str(), &socketaddr1.sin_addr);

    struct sockaddr_in socketaddr2;
    socketaddr2.sin_family = AF_INET;//ipv4
    socketaddr2.sin_port = htons(stoi(port2));
    inet_pton(AF_INET, ip2.c_str(), &socketaddr2.sin_addr);

    if(ip1 != ip2) //Different intranets
    {
        ip_port1 = ip1 + " " + port1;
        ip_port2 = ip2 + " " + port2;
    }
    else //In the same NAT
    {
        ip_port1 = host1[2] + " " + host1[3];
        ip_port2 = host2[2] + " " + host2[3];
    }

    int res = sendto(listenudp, ip_port1.c_str(), 
                  ip_port1.size(), 0, (struct sockaddr*)&socketaddr2, sizeof(socketaddr2));
    if(res < 0)
    {
        printf("udp sendto error!\n");
        exit(1);
    }

    res = sendto(listenudp, ip_port2.c_str(), 
                     ip_port2.size(), 0, (struct sockaddr*)&socketaddr1, sizeof(socketaddr1));
    if(res < 0)
    {
        printf("udp sendto error!\n");
        exit(1);
    }

    return 0;
}