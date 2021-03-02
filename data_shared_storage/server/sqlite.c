#include "sqlite.h"

__thread sqlite3 *db_handle; //sqlite句柄

/* 初始化句柄 */
int init_sqlite()
{
    int ret = 0;

    ret = sqlite3_open("./data.db", &db_handle);
    if (ret){
        sqlite3_close(db_handle);
        return -1;
    }

    return 0;
}

/* 数据库操作 */
int sqlite_oper(char *sql)
{
    int ret = 0;
    
    if (!sql)
        return -1;

    ret = sqlite3_exec(db_handle, sql, NULL, NULL, NULL);
    if (ret != SQLITE_OK){
        printf("sqlite3_exec failed,ret %d\n", ret);
        return -1;
    }

    return 0;
}

/* 数据集查询 */
int sqlite_select(char *sql, callback func)
{
    int ret = 0;

    if (!sql || !func)
        return -1;

    ret = sqlite3_exec(db_handle, sql, func, NULL, NULL); 
    if (ret != SQLITE_OK)
      return -1;  

    return 0;
}

/* 关闭数据库 */
int uninit_sqlite()
{
    sqlite3_close(db_handle);
}
