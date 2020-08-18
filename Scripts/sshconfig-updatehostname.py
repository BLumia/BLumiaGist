#!/bin/python3

import sys
import os

def gen_config_strarr(one_srv):
	return [
		"Host {}\n".format(one_srv[0]),
		"    HostName        {}\n".format(one_srv[1]),
		"    Port            22\n",
		"    User            root\n",
		"    ServerAliveInterval 30\n",
	]

def main():
	if len(sys.argv) != 3:
		print("用法：{} host hostname\n示例：{} aliyun-gd-1 123.123.123.123".format(
			sys.argv[0], sys.argv[0]
		))
		return

	if not os.path.isfile(os.path.expanduser("~/.ssh/config")):
		print("~/.ssh/config 不存在，将创建。")
	f = open(os.path.expanduser("~/.ssh/config"), 'r')
	lines = f.readlines()
	f.close()

	one_srv = (sys.argv[1], sys.argv[2])
	matched = False
	for i, line in enumerate(lines):
		if re.search(r'\s' + one_srv[0] + r'$', line.strip()) and "HostName" in lines[i + 1]:
			matched = True
			lines[i + 1] = "    HostName        {}\n".format(one_srv[1])
			break
	if not matched:
		lines = lines + gen_config_strarr(one_srv)

	with open(os.path.expanduser("~/.ssh/config"), 'w+') as handle:
		handle.writelines(lines)

	if matched:
		print("将 {} 的 HostName 更新为了 {}".format(one_srv[0], one_srv[1]))
	else:
		print("追加了 {} 的配置信息，HostName 为 {}".format(one_srv[0], one_srv[1]))

if __name__ == "__main__":
	main()
