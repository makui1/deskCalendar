#include "storage.h"

SQLiteStorage *Storage::sqlInstance = nullptr;

Storage::Storage()
{

}

//创建数据库连接
SQLiteStorage *Storage::start()
{
    if(!sqlInstance)
        sqlInstance = new SQLiteStorage;
    return sqlInstance;
}

//结束数据库连接
void Storage::end()
{
    if(sqlInstance){
        delete sqlInstance;
        sqlInstance = nullptr;
    }
}
