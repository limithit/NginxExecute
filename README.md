# NginxExecute

## Introduction

The NginxExecute module executes the shell command through GET and POST to display the result.

Configuration example：

```nginx
location ... {
            ......
            command on;
        }
```
```nginx
    worker_processes  2;
    events {
        worker_connections  1024;
    }
    http {
        include       mime.types;
        default_type  application/octet-stream;
        sendfile        on;
        keepalive_timeout  65;
        server {
            listen       80;
            server_name  localhost;
            location / {
                root   html;
                index  index.html index.htm;
                command on;
            }
            error_page   500 502 503 504  /50x.html;
            location = /50x.html {
                root   html;
            }
        }
    }
```
Usage:  ```view-source:http://192.168.18.22/?system.run[command]```  or  ```curl -g "http://192.168.18.22/?system.run[command]"``` 
The ```command``` can be any system command. The command you will want to use depends on the permissions that nginx runs with.

    view-source:http://192.168.18.22/?system.run[ifconfig]

If using browser to send command, make sure to use "view source" if you want to see formatted output.
Alternatively, you can also use some tools such as Postman, Fiddler.

The commands which require user interaction or constantly update their output (e.g. ```top```) will not run properly, so do not file a bug for this.

## Demo
![Pump Demo Video](https://github.com/limithit/NginxExecute/blob/master/demo.png)

## Installation

### CentOS 7 and official Nginx stable

Simply install the official stable Nginx and extras YUM repository and you don't need to worry about compiling anything.

    yum install http://nginx.org/packages/centos/7/noarch/RPMS/nginx-release-centos-7-0.el7.ngx.noarch.rpm
    yum install https://extras.getpagespeed.com/redhat/7/noarch/RPMS/getpagespeed-extras-7-0.el7.gps.noarch.rpm
    yum install nginx nginx-module-execute

Add to your *nginx.conf* :

    load_module modules/ngx_http_execute_module.so;

### Other platforms. Option #1: Compile Nginx with module bundled

    cd nginx-**version**
    ./configure --add-module=/path/to/this/NginxExecute
    make
    make install


### Other platforms. Option #2: Compile dynamic module for Nginx

Starting from NGINX 1.9.11, you can also compile this module as a dynamic module, by using the ```--add-dynamic-module=PATH``` option instead of ```--add-module=PATH``` on the ```./configure``` command line above. And then you can explicitly load the module in your ```nginx.conf``` via the [load_module](http://nginx.org/en/docs/ngx_core_module.html#load_module) directive, for example,

```nginx
    load_module /path/to/modules/ngx_http_execute_module.so;
```
This module is compatible with following nginx releases:




Author
Gandalf zhibu1991@gmail.com

