# Note

客户端环境：VS 2022

服务器环境：ubuntu 20.04 LTS

-----
基于udp打洞技术实现内网穿透
* **使用说明**
  * 代码在code文件夹内。
  * 服务器端编译`server.cpp`并运行；
  * 客户端导入头文件`udp_hole_punch.h`以及对应的cpp文件即可。
* **测试实例**
  * 在test文件夹里提供了使用函数的实例，分别是两个对端客户端的执行程序。

-----

功能实现：

* 逆向连接
* NAT环回
* P2P内网穿透

> 完整的开发日志可以参考我写的博客：[UDP hole punching | JySama](https://jysama.cn/2022/11/26/udp_hole_punching/)

