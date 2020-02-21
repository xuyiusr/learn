public class SqlDefine {
    String testBindSql = "select aaa from bbb where cc = :1";
    String testSql = "select \"ROWID\" from bbb";
    String selectSql = "SELECT * FROM TBL_MAIN_TEST";
    String updateSql = "UPDATE TBL_MAIN_TEST SET F_2 = 1234 WHERE F_1 = 1111";
    String deleteSql = "DELETE FROM TBL_MAIN_TEST WHERE F_1 = 1111";
    String insertSql = "INSERT INTO TBL_MAIN_TEST (F_1) VALUES (1111)";
    String mergeSql = "merge into tbl_main_test t1 using tbl_main_test_new t2 on (t1.f_1=t2.f_1) \n" +
            "when matched then\n" +
            "    update set f_2=2, f_6='66'\n" +
            "when not matched then\n" +
            "insert(f_2, f_6) values(2, '66')";
    String preExplainSql = "explain plan for select * from tbl_main_test";
    String explainSql = "select * from table(dbms_xplan.display)";
    String uninSql = "SELECT\n" +
            "    a.\"f_char\",\n" +
            "    a.\"f_blob\",\n" +
            "    b.\"f_float\"\n" +
            "FROM\n" +
            "    \"tbl_special_field_type\" a,\n" +
            "    \"tbl_normal_field_type\" b\n" +
            "WHERE\n" +
            "    a.id = b.id\n";
    String insertAllSql = "insert all \n" +
            "into test_insert(user_name,address) values('aaa','henan')\n" +
            "into test_insert(user_name,address) values('bbb','shanghai')\n" +
            "into test_insert(user_name,address) values('ccc','beijing')\n" +
            "select * from dual";
    String insertViewsql = "insert into view_special_field_type(f_char) values ('c')";
    String bindSelectSql = "SELECT * FROM TBL_MAIN_TEST WHERE F_1 = ? ";
    String bindInsertSql = "INSERT INTO TBL_MAIN_TEST(F_1) values( :dsa ) ";
    String callSql = "{call getTestData(?, ?)}";
    String createViewSql = "CREATE VIEW view_complex_field_exp_1 ( f_number, int_exp ) AS SELECT\n" +
            "\"f_number\",\n" +
            "CASE\n" +
            "    WHEN \"f_integer\" > 10 THEN\n" +
            "    9999 \n" +
            "    WHEN \"f_integer\" < 10 THEN\n" +
            "    0 ELSE 1 \n" +
            "END \n" +
            "FROM\n" +
            "    \"tbl_normal_field_type\"";
    String alterViewSql = "alter view view_complex_field_exp_1 compile";
    String createUserSql = "create user C##XUYI identified by aki2009";
}
