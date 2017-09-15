## TUN Device Starter Note

要想使用 ~~臀~~ tun 设备，首先需要启用这个内核模块，在我的系统（ Arch linux ）上，只需 `insmod` 一下就行了。

``` shell
find /lib/modules/ -iname 'tun.ko.gz' # 找到在哪儿
sudo insmod /lib/modules/4.12.12-1-ARCH/kernel/drivers/net/tun.ko.gz # 插入该模块
modprobe tun # 加载该内核模块，当然你也可以选择 the Windows way: 重启一下
modinfo tun # 可以检查一下了
lsmod | grep tun # 也可以这样检查
```

为了方便，我选择重启... 

PS. 据说在 ubuntu 下只需 `/sbin/modprobe tun` 就可以正常使用 tun 设备了，不过我手头没有办法测试，供参考。

### 简易测试

建立一个 tun 设备，然后设置路由表把数据包路由到 tun 设备里。

``` shell
sudo ip tuntap add dev dummytun mode tun # 添加一个叫 dummytun 的 tun 设备
sudo ip link set dummytun up # 把设备 dummytun 开起来
sudo route add 123.123.123.123 dummytun # 把 123.123.123.123 路由到 dummytun
ip route get 123.123.123.123 # 试试看是否搞成了
```

如上并没有给这个 tun 设备 IP （但 `ioctl()` 打开这个 tun 设备后就会有一个 IPv6 地址了），当然也可以给它一个 IP ：

``` shell
sudo ip addr add 172.0.0.0/24 dev dummytun
```

由于 tun 设备需要我们编写用户态的程序来操作数据包，所以需要写个东西来处理包数据的 IO 。tun 是三层设备，故能拿到的都是三层（ IP 层）的包了。下面是一个非常简单的代码片段，仅仅简单的把东西从 `dummytun` 中读出来而已。

``` c
// 此处省略了 tun_open() 的原型。
// 作用仅仅是 `open()` 该设备，`ioctl()` 连接到该设备最终返回文件描述符
int fd = tun_open("dummytun");
// 我们假设是按照刚刚的步骤，在运行程序之前就已经创建并 `up` 了设备 dummytun
// 如果你希望通过代码创建 tun 设备，别忘了把创建的设备 `up` 起来
printf("Device dummytun opened\n");

while(1) {
	int nbytes = read(fd, buf, sizeof(buf));  // FYI: char buf[1600];
	printf("Read %d bytes from dummytun\n", nbytes);
}
```

另外额外需要注意的事是，在写过路由表规则以及给设备绑 IP 之后，最好还是刷一下缓存比较好，以免出现测了半天才发现压根没经过自己的 tun 设备的情况。

``` shell
sudo ip route flush cache
```

当开始处理包时，我们就可以在 wireshark 或其他类似软件中看到流经该 tun 的网络包了。值得一提的是，即便事先没有给 tun 设备分配任何 IP 地址的情况下，在使用 `ioctl()` 打开 tun 设备后， tun 设备也将自动分得一个 IPv6 地址，所以在 wireshark 中是可以看到 ICMPv6 包存在的。