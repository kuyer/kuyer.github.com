# jar文件运行方式

## 前台运行demo.jar

```sh
java -jar demo.jar
```

缺点是，当用户退出或终端关闭，程序也会终止运行

## 后台运行demo.jar

```sh
nohup java -jar demo.jar &
```

nohup的意思是不挂断运行命令，当用户退出或终端关闭，程序仍然运行。
当使用nohup命令运行时，缺省情况下会把运行的日志输出到nohup.out的文件中

```sh
nohup java -jar demo.jar > demo.log &
```

这种方式是将运行的日志输入到demo.log文件中

## jobs、bg和fg命令

* jobs会列出所有后台运行的程序，每个程序都会有个编号，如：jobs -l 结果包含1 2 3等运行的程序

* fg会将运行的程序由后台调回到前台运行，如：fg 2 或 fg %2

* bg会将运行的程序由前台调回到后台运行，如：使用Ctrl+z是程序暂停，bg 2或bg %2，将程序放到后台运行

* kill会直接终止后台运行的程序，如：kill 2或kill %2
