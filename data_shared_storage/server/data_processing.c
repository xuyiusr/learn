#include "data_processing.h"

#define MAX_ENCODE_NUM 30
static int encode_num = 0;
static encode_table s_table[32]; 

/* 获取指定字符前的值 */
int get_string_before_char(char *src, char *dst, char *split)
{
    char *p = NULL;

    p = strstr(src, split);
    if (!p)
        return -1;

    memcpy(dst, src, p - src);
    dst[p-src] = 0;

    return p - src;
} 

/* 打印信息 */
static int dump_oper_param(oper_param * param)
{
    printf("=======dump=======\n"
           "table name : %s\n"
           "modify : %s\n"
           "condition : %s\n"
           "==================\n",
           param->table_name, param->modify, param->condition);
} 

/* 处理消息
/* 消息格式
 * 表名;(更新内容)字段:值,字段:值;(更新条件);
 */
static int format_msg(char *msg, oper_param *param)
{
    int ret = 0;
    char *p = NULL;

    assert(msg && param);

    p = msg;
    printf("[%s] get a msg : %s\n", __func__, p);

    ret = get_string_before_char(p, param->table_name, ";");
    if (ret == -1){
        printf("[%s] get table name failed\n", __func__);
        return -1;
    }

    p += (ret + 1);

    ret = get_string_before_char(p, param->modify, ";");
    if (ret == -1){
        printf("[%s] get modify failed\n", __func__);
        return -1;
    }

    p += (ret + 1);

    ret = get_string_before_char(p, param->condition, ";");
    if (ret == -1){
        printf("[%s] get condition failed\n", __func__);
        return -1;
    }

    dump_oper_param(param);
    return 0;
}

/* 添加数据到sqlite */
static int sqlite_add_table(oper_param *param)
{
    char sql[4096];

    if (init_sqlite())
        return -1;

    snprintf(sql, sizeof(sql), "CREATE TABLE %s (%s);"
            ,param->table_name, param->modify);

    if (sqlite_oper(sql)){
        printf("sqlite oper failed. sql: %s\n", sql);
    }

    uninit_sqlite();

    return 0;
}

static int format_modify_query(char *modify, char *sql)
{
    int ret = 0;
    char tmp[128] = {0};
    char field[1024] = {0};
    char value[1024] = {0};
    char *p = NULL;

    p = modify;

    while(1){
        ret = get_string_before_char(p, tmp, ":");
        if (ret == -1)
            break;
        
        strcat(field, tmp);
        strcat(field, ",");
        p += (ret + 1);

        ret = get_string_before_char(p, tmp, ",");
        if (ret == -1)
            break;

        strcat(value, tmp);
        strcat(value, ",");
        p += (ret + 1);
    }

    strcat(value, p);
    field[strlen(field) - 1] = 0;
    
    sprintf(sql+strlen(sql), "(%s) VALUES (%s)", field, value);

    return 0;
}

/* 添加数据到sqlite */
static int sqlite_query(oper_param *param)
{
    int ret = 0;
    char sql[4096] = {0};

    if (init_sqlite())
        return -1;

    snprintf(sql, sizeof(sql), "INSERT INTO %s ", param->table_name);

    if (format_modify_query(param->modify, sql)){
        printf("format_modify failed\n");
        ret = -1;
        goto ERROR;
    }

    if (sqlite_oper(sql)){
        printf("sqlite oper failed. sql: %s\n", sql);
        ret = -1;
        goto ERROR;
    }

ERROR:
    uninit_sqlite();
    return ret;
}

static int sqlite_update(oper_param *param)
{
    int ret = 0;
    char sql[4096] = {0};

    if (init_sqlite())
        return -1;

    snprintf(sql, sizeof(sql), "UPDATE %s SET %s WHERE %s", 
            param->table_name, param->modify, param->condition);

    if (sqlite_oper(sql)){
        printf("sqlite oper failed. sql: %s\n", sql);
        ret = -1;
        goto ERROR;
    }

ERROR:
    uninit_sqlite();
    return ret;
}

static int put_encode_table(char *table, char * field)
{
    if (encode_num >= MAX_ENCODE_NUM)
        return -1;

    strcpy(s_table[encode_num].table_name, table);
    strcpy(s_table[encode_num].field, field);

    encode_num++;

    return 0;
}

static int get_encode_table(char *table, char *field)
{
    int i= 0;

    for (; i < encode_num; i++){
        if (strcmp(s_table[i].table_name, table) == 0){
            strcpy(field, s_table[i].field);
            return 0;
        }
    }

    return -1;
}

static int parse_encode_msg(char *msg, oper_param *param)
{
    int ret = -1;
    char table[128] = {0};
    char field[128] = {0};
    char *p = NULL;

    p = msg;
    ret = get_string_before_char(p, table, ";");
    if (ret == -1){
        printf("[%s] get table name failed\n", __func__);
        return -1;
    }

    p += (ret + 1);

    ret = get_string_before_char(p, field, ";");
    if (ret == -1){
        printf("[%s] get modify failed\n", __func__);
        return -1;
    }

    put_encode_table(table, field);

    return 0;
}

static int encode_field(oper_param *param)
{
    int ret = 0;
    char field[128] = {0};
    char value[128] = {0};
    char value_encode[256] = {0};
    char *p = NULL;

    ret = get_encode_table(param->table_name, field);
    if (ret == -1)
        return 0;

    if (strstr(param->modify, field)){
        get_string_before_char(param->modify, value, ",");
        base64_encode(value, value_encode);
    }

    return 0;
}

/* 处理流程 */
/* 1. 消息处理函数 */
/* 2. 加密方式 */
/* 3. 解密方式 */
/* 4. 数据库操作 */
_fun_st data_processing[] = {
    {ADD_TABLE, format_msg, NULL, NULL, sqlite_add_table},
    {INSERT_RECORD, format_msg, encode_field, NULL, sqlite_query},
    {UPDATE_RECORD, format_msg, encode_field, NULL, sqlite_update},
    {REG_ENCODE, parse_encode_msg, NULL, NULL, NULL},
}; 

