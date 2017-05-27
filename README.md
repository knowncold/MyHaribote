# PomeloOS
> 看《30天自制操作系统》这本书的记录

## 环境配置

本来想尝试在Linux环境写的，结果发现很多问题暂时无法克服，就只能在Windows下完成了。

其中的`tolset`文件夹来自书的光盘。

### Windows

使用书附带光盘中的文件就能直接模拟运行了。

### Linux

	$ sudo apt-get install qemu

运行

	$ qemu qemu-system-i386 -hda *.img
