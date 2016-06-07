/**
 * 本程序主要就是为了验证jsoncpp中Json::Value对象中的append、等号赋值是拷贝内容，还是赋值指针，
 * 从目前的情况来看应该是拷贝内容，所以就目前而言，下面这个删除数组数组中元素的方法还是比较好用的。
 */



#include <QCoreApplication>
#include "json/json.h"
#include <QString>
#include <QFile>
#include <QDebug>

#define ICON_PATH   "apps\/images\/"
#define SHELL_PATH  "\/usr\/share\/web\/lighttpd\/webpages\/shell\/"
#define JSON_FILE   ":/resource/json.txt"

bool jsonUpdateApp(Json::Value *programs, int index, QString icon, QString exec) {

    // 名字相同了，自然不需要再重新赋值了
    //(*programs)[index]["Name"]     = name.toStdString();
    (*programs)[index]["Icon"]     = QString(ICON_PATH).append(icon).toStdString();
    (*programs)[index]["Exec"]     = QString(SHELL_PATH) .append(exec).append(".sh").toStdString();

}

bool jsonAddApp(Json::Value *programs, QString name, QString icon, QString exec) {

    // 合成新的数据并加入json对象中
    Json::Value item;
    item["Name"]                = name.toStdString();
    item["Order"]               = 999;
    item["Icon"]                = QString(ICON_PATH).append(icon).toStdString();
    item["Type"]                = "application";
    item["Exec"]                = QString(SHELL_PATH).append(exec).append(".sh").toStdString();
    item["ProgramType"]         = "gui";
    item["Description_Link"]    = -1;
    item["Lock"]                = -1;

    programs->append(item);

}

void copyItem(Json::Value *programs, Json::Value *newPrograms, int index) {

    // 合成新的数据并加入json对象中
    Json::Value newItem;
    newItem["Name"] 	            = (*programs)[index]["Name"];
    newItem["Order"]	            = (*programs)[index]["Order"];
    newItem["Icon"] 	            = (*programs)[index]["Icon"];
    newItem["Type"]  	            = (*programs)[index]["Type"];
    newItem["Exec"]  	            = (*programs)[index]["Exec"];
    newItem["ProgramType"] 		    = (*programs)[index]["ProgramType"];
    newItem["Description_Link"] 	= (*programs)[index]["Description_Link"];
    newItem["Lock"] 				= (*programs)[index]["Lock"];

    newPrograms->append(newItem);

}

bool checkAndAddUpdateAppByName(Json::Value *oldRoot, Json::Value *newRoot, QString name,
        QString icon, QString exec) {

    bool ret = false;
    Json::Value programs = ((*oldRoot)["main_menu"])["apps"];
    Json::Value newPrograms;

    int i = 0;
    for ( i = 0; i < programs.size(); i++ ) {

        // 如果有name对应的app，那么就对app进行更新
        if ( QString::fromStdString(programs[i]["Name"].asString()).trimmed().compare(name.trimmed()) == 0 ) {
            jsonUpdateApp(&programs, i, icon, exec);
        }

        copyItem(&programs, &newPrograms, i);
    }

    // 如果name没有重复的，那么添加一个这个app
    if ( i >= programs.size() )
        jsonAddApp(&newPrograms, name, icon, exec);

    Json::Value newApps;
    newApps["apps"] = newPrograms;
    (*newRoot)["main_menu"] = newApps;

    return ret;
}

bool checkAndRemoveAppName(Json::Value *oldRoot, Json::Value *newRoot, QString name) {

    bool ret = false;
    Json::Value programs = ((*oldRoot)["main_menu"])["apps"];
    Json::Value newPrograms;

    int i = 0;
    for ( i = 0; i < programs.size(); i++ ) {

        if ( QString::fromStdString(programs[i]["Name"].asString()).trimmed().compare(name.trimmed()) == 0 ) {
            ret = true;
            continue;
        }

        /*
         * // 合成新的数据并加入json对象中
         * Json::Value newItem;
         * newItem["Name"] 	            = programs[i]["Name"];
         * newItem["Order"]	            = programs[i]["Order"];
         * newItem["Icon"] 	            = programs[i]["Icon"];
         * newItem["Type"]  	        = programs[i]["Type"];
         * newItem["Exec"]  	        = programs[i]["Exec"];
         * newItem["ProgramType"] 		= programs[i]["ProgramType"];
         * newItem["Description_Link"] 	= programs[i]["Description_Link"];
         * newItem["Lock"] 				= programs[i]["Lock"];
         * newPrograms.append(newItem);
         *
         * 下面这个函数是从上面的代码中重构出来的
         */
        copyItem(&programs, &newPrograms, i);
    }

    Json::Value newApps;
    newApps["apps"] = newPrograms;
    (*newRoot)["main_menu"] = newApps;

    return ret;
}



int main(int argc, char *argv[])
{

    QCoreApplication a(argc, argv);

    // 检查文件是否存在
    QFile jsonFileObject(JSON_FILE);
    if ( !jsonFileObject.exists() ) {
        qDebug() << "Please check json.txt file already exist." << endl;
        return -1;
    }

    // 打开并读取文件
    QString jsonString;
    if ( jsonFileObject.open(QIODevice::ReadOnly | QIODevice::Text) ) {
        jsonString.append(jsonFileObject.readAll());
        jsonFileObject.close();
    } else {
        qDebug() << "Open json file failt." << endl;
        return -1;
    }

    // 解析json数据
    Json::Value root;
    Json::Reader reader;
    bool parsingSuccessful = reader.parse(jsonString.toStdString(), root);
    if ( !parsingSuccessful ) {
        qDebug() << "Failed to parse configure.\n ";
        return -1;
    }

    // 测试获取json数据
    Json::Value programs = root["main_menu"]["apps"];
    qDebug() << "programs size: " << programs.size();
    // 这里的获取数据数据索引后面一定要加一个小写的u，目前还不知道具体的原因
    std::cout << "Exec: " << programs[0u]["Exec"].asString() << std::endl;

    Json::Value outRootWithRemove;
    checkAndRemoveAppName(&root, &outRootWithRemove, "Network");
    checkAndRemoveAppName(&root, &outRootWithRemove, "Browser");
    Json::Value outRootWithRemoveAndAdd;
    checkAndAddUpdateAppByName(&outRootWithRemove, &outRootWithRemoveAndAdd, "zengjf", "zengjf", "zengjf");

    // 生成写入文件的字符串
    Json::StyledWriter writer;
    std::string outputJsonstring = writer.write(outRootWithRemoveAndAdd);

    // 将输出字符串输出到console上查看效果
    std::cout << outputJsonstring << std::endl;

    // 目前仅仅是测试，不需要写入文件，省的每次还要去换json.txt文件
    QFile jsonOutFile("output.txt");
    if ( jsonOutFile.open(QIODevice::WriteOnly| QIODevice::Text) ) {
        jsonOutFile.write(outputJsonstring.data());
        jsonOutFile.close();
    } else {
        qDebug() << "Open json file failt." << endl;
        return -1;
    }

    return a.exec();
}


/**
  * json.txt
  * {
  *    "main_menu" : {
  *       "apps" : [
  *          {
  *             "Description_Link" : -1,
  *             "Exec" : "/usr/share/web/lighttpd/webpages/shell/browser.sh http://processors.wiki.ti.com",
  *             "Icon" : "apps/images/web-icon.png",
  *             "Lock" : -1,
  *             "Name" : "Browser",
  *             "Order" : 999,
  *             "ProgramType" : "gui",
  *             "Type" : "application"
  *          },
  *          {
  *             "Description_Link" : -1,
  *             "Exec" : "/usr/share/web/lighttpd/webpages/shell/network.sh",
  *             "Icon" : "apps/images/network-icon.png",
  *             "Lock" : -1,
  *             "Name" : "Network",
  *             "Order" : 999,
  *             "ProgramType" : "gui",
  *             "Type" : "application"
  *          }
  *     ]
  * }
  */


/**
  * 输出结果：
  * json.txt
  * {
  *    "main_menu" : {
  *       "apps" : [
  *          {
  *             "Description_Link" : -1,
  *             "Exec" : "/usr/share/web/lighttpd/webpages/shell/browser.sh http://processors.wiki.ti.com",
  *             "Icon" : "apps/images/web-icon.png",
  *             "Lock" : -1,
  *             "Name" : "Browser",
  *             "Order" : 999,
  *             "ProgramType" : "gui",
  *             "Type" : "application"
  *          },
  *          {
  *             "Description_Link" : -1,
  *             "Exec" : "/usr/share/web/lighttpd/webpages/shell/zengjf.sh",
  *             "Icon" : "apps/images/zengjf",
  *             "Lock" : -1,
  *             "Name" : "zengjf",
  *             "Order" : 999,
  *             "ProgramType" : "gui",
  *             "Type" : "application"
  *          }
  *     ]
  * }
  */
