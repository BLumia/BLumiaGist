### [单独为某个包设置环境变量](https://wiki.gentoo.org/wiki/Knowledge_Base:Overriding_environment_variables_per_package)

创建 `/etc/portage/package.env` 文件和 `/etc/portage/env/` 目录。 `/etc/portage/env/` 中放置需要的规则，语法和 `make.conf` 一致。`/etc/portage/package.env` 文件内容为一个对应关系表，表示某个包对应使用 `/etc/portage/env/` 目录中的某个文件。

例如：

``` shell
$ ls /etc/portage/env/
j1.conf    noccache.conf   gcc7.conf
$ cat /etc/portage/env/j1.conf
MAKEOPTS="-j1"
$ cat /etc/portage/package.env
qt-dev/qtwebkit    j1.conf
```