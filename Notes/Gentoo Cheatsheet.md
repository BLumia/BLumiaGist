### [����Ϊĳ�������û�������](https://wiki.gentoo.org/wiki/Knowledge_Base:Overriding_environment_variables_per_package)

���� `/etc/portage/package.env` �ļ��� `/etc/portage/env/` Ŀ¼�� `/etc/portage/env/` �з�����Ҫ�Ĺ����﷨�� `make.conf` һ�¡�`/etc/portage/package.env` �ļ�����Ϊһ����Ӧ��ϵ����ʾĳ������Ӧʹ�� `/etc/portage/env/` Ŀ¼�е�ĳ���ļ���

���磺

``` shell
$ ls /etc/portage/env/
j1.conf    noccache.conf   gcc7.conf
$ cat /etc/portage/env/j1.conf
MAKEOPTS="-j1"
$ cat /etc/portage/package.env
qt-dev/qtwebkit    j1.conf
```