#include "sqlite.h"
#include "base64.h"

typedef struct encode_table{
    char table_name[128];
    char field[128];
}encode_table;

typedef struct oper_param{
    char table_name[128];
    char condition[1024];
    char modify[1024];
}oper_param;

enum {
    ADD_TABLE = 0,
    INSERT_RECORD = 1,
    UPDATE_RECORD = 2,
    DELETE_RECORD = 3,
    SELECT_RECORD = 4,
    REG_ENCODE = 5,
    UNREG_RNCODE = 6,
    REG_NOTIFY = 7,
    UNREG_NOTIFY = 8,
    ALL_BUSINESS_CODE,
};

typedef int (*deal_msg)(char *, oper_param *);
typedef int (*encode)(oper_param *);
typedef int (*decode)(const char*,char **);
typedef int (*database_oper)(oper_param *);

typedef struct _fun_st{
    int business_code;
    deal_msg deal_msg_func;
    encode encode_func;
    decode decode_func;
    database_oper database_oper_func;
}_fun_st;

