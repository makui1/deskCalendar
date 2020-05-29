#ifndef STORAGE_H
#define STORAGE_H
#include "sqlitestorage.h"

//该类是为了避免重复创建数据库连接而创建的存储接口
class Storage
{
private:
    static SQLiteStorage *sqlInstance;

public:
    Storage();
    static SQLiteStorage* start();
    static void end();
};

#endif // STORAGE_H
