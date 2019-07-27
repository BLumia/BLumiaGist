## [LVM](https://wiki.archlinux.org/index.php/LVM#Create_partitions) [on LUKS](https://wiki.archlinux.org/index.php/Dm-crypt/Encrypting_an_entire_system#LVM_on_LUKS)

目的并非全盘加密，就只是创建加密分区供测试。首先需要准备一个分区用来放 LUKS 容器，这里的待使用分区是 `/dev/sda6`。

``` plain
# cryptsetup luksFormat /dev/sda1
```

注意这里确认的时候让输入的是全大写的 `YES` ，人傻了没仔细看输了好多次小写的 `yes` 没反应过来怎么就操作中断了。。

接下来需要打开这个容器：

``` plain
# cryptsetup open /dev/sda6 cryptlvm # cryptlvm 是名称
```

此时容器就可以使用了，位于 `/dev/mapper/cryptlvm` ，然后要做的是在其之上创建一个物理卷（Physical Volume）

``` plain
# pvcreate /dev/mapper/cryptlvm
```

然后创建一个卷组（Volume Group）用来放卷

```plain
# vgcreate MyVolGroup /dev/mapper/cryptlvm
```

在卷组中分配卷(下面只有第一行是我实际做的，后面只是示例可以用的写法)：

``` plain
# lvcreate -L 10G MyVolGroup -n lvolhome # -n 后是名称
# lvcreate -l 100%FREE MyVolGroup -n home # 可以用 100%FREE 这种写法表示占用卷组上的所有剩余空间
# lvdisplay # 查看实际分配情况
```

最后格式化分区就是了

``` plain
# mkfs.ext4 /dev/MyVolGroup/lvolhome
```

若要让 gio 能够显示加密的 LUKS 容器，需要先把打开的容器关掉：

``` plain
# cryptsetup close cryptlvm # 写名称就行
```

此时再试图使用 gio 挂载就会询问密码进行挂载了，询问时就是 `ask_password` 的信号。确认完毕后 gio 会打开 LUKS 容器，效果就会和手动 `cryptsetup open` 之后的一样了，gio 打开时所用的名称应该就是 `luks-<uuid>`。