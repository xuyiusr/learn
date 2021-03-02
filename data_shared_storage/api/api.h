#include <stdio.h>

int make_create_table_req(char *table, char *column);
int make_add_column_req(char *table, char *column);
int make_insert_record_req(char *table, char *record);
int make_update_record_req(char *table, char *record, char *condition);
int make_del_record_req(char *table, char *condition);
int make_select_record_req(char *table, char *condition, char *result);
int make_reg_column_ciphertext_req(char *table, char *column);
int make_reg_column_ciphertext_req(char *table, char *column);
int make_unreg_column_ciphertext_req(char *table, char *column);
int make_reg_listen_table_notify_req(char *table);
int make_unreg_listen_table_notify_req(char *table);
int get_notify(int *is_change);
char *get_error_info();

