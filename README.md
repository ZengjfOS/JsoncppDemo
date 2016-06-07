# JsoncppDemo
尝试解析TI的matrix-gui-v2中json.txt文件而编写的Demo，Json解析器采用jsoncpp。

## 程序运行效果：
* 如果输入如下json.txt：

```json
    {
       "main_menu" : {
          "apps" : [
             {
                "Description_Link" : -1,
                "Exec" : "/usr/share/web/lighttpd/webpages/shell/browser.sh http://processors.wiki.ti.com",
                "Icon" : "apps/images/web-icon.png",
                "Lock" : -1,
                "Name" : "Browser",
                "Order" : 999,
                "ProgramType" : "gui",
                "Type" : "application"
             },
             {
                "Description_Link" : -1,
                "Exec" : "/usr/share/web/lighttpd/webpages/shell/network.sh",
                "Icon" : "apps/images/network-icon.png",
                "Lock" : -1,
                "Name" : "Network",
                "Order" : 999,
                "ProgramType" : "gui",
                "Type" : "application"
             }
        ]
    }
```

* 那么输出为如下：

```json
    {
       "main_menu" : {
          "apps" : [
             {
                "Description_Link" : -1,
                "Exec" : "/usr/share/web/lighttpd/webpages/shell/browser.sh http://processors.wiki.ti.com",
                "Icon" : "apps/images/web-icon.png",
                "Lock" : -1,
                "Name" : "Browser",
                "Order" : 999,
                "ProgramType" : "gui",
                "Type" : "application"
             },
             {
                "Description_Link" : -1,
                "Exec" : "/usr/share/web/lighttpd/webpages/shell/zengjf.sh",
                "Icon" : "apps/images/zengjf",
                "Lock" : -1,
                "Name" : "zengjf",
                "Order" : 999,
                "ProgramType" : "gui",
                "Type" : "application"
             }
        ]
    }
```

## Author

[曾剑锋](http://www.cnblogs.com/zengjfgit/)