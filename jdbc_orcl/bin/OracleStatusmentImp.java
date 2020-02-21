import oracle.jdbc.OraclePreparedStatement;
import oracle.jdbc.OracleStatement;
import oracle.jdbc.driver.OracleConnection;

import java.sql.Clob;
import java.sql.DatabaseMetaData;
import java.sql.ResultSet;

public class OracleStatusmentImp {

    public static void executeTest(OracleConnection con) throws Exception {
        SqlDefine sql = new SqlDefine();
        OracleStatement stat = (OracleStatement) con.createStatement();
        OraclePreparedStatement preStat = (OraclePreparedStatement) con.prepareStatement(sql.testBindSql);

//        stat.setFetchSize(564);
//        con.setDefaultExecuteBatch(564);

        stat.executeQuery(sql.testSql);
        preStat.setInt(1, 12340);
        preStat.executeUpdate();
//
//        for (int i = 0; i < 10; i++) {
//            stat.addBatch(sql.insertSql);
//        }
//        stat.executeBatch();
//        con.commit();
//        stat.closeOnCompletion();
    }

    public static void executeLongSQLTest(OracleConnection con) throws Exception {
        String testLongSQL = new String();

        testLongSQL += "select * from ccc where aaa = bbb";
        for(int i = 0 ;  i < 3000 ; i ++ ){
            testLongSQL += " and aaa = bbb ";
        }
        OracleStatement stat = (OracleStatement) con.createStatement();
        stat.executeQuery(testLongSQL);
    }

    public static void clobTest(OracleConnection con) throws Exception {
        SqlDefine sql = new SqlDefine();
        Clob clob = con.createClob();
        clob.setString(1, "strFldValue");
        OraclePreparedStatement stat = (OraclePreparedStatement) con.prepareStatement(sql.bindSelectSql);
        stat.setClob(1, clob);
        stat.executeQuery();
    }

    public static void databaseMetaData(OracleConnection con) throws Exception {
        DatabaseMetaData dbmd = con.getMetaData();
        System.out.println("数据库已知的用户: " + dbmd.getUserName());
        System.out.println("数据库的系统函数的逗号分隔列表: " + dbmd.getSystemFunctions());
        System.out.println("数据库的时间和日期函数的逗号分隔列表: " + dbmd.getTimeDateFunctions());
        System.out.println("数据库的字符串函数的逗号分隔列表: " + dbmd.getStringFunctions());
        System.out.println("数据库供应商用于 'schema' 的首选术语: " + dbmd.getSchemaTerm());
        System.out.println("数据库URL: " + dbmd.getURL());
        System.out.println("是否允许只读:" + dbmd.isReadOnly());
        System.out.println("数据库的产品名称:" + dbmd.getDatabaseProductName());
        System.out.println("数据库的版本:" + dbmd.getDatabaseProductVersion());
        System.out.println("驱动程序的名称:" + dbmd.getDriverName());
        System.out.println("驱动程序的版本:" + dbmd.getDriverVersion());

        System.out.println("数据库中使用的表类型");
        ResultSet rs = dbmd.getTableTypes();
        while (rs.next()) {
            System.out.println(rs.getString("TABLE_TYPE"));
        }
        rs = dbmd.getSchemas();
        while (rs.next()) {
            String tableSchem = rs.getString("TABLE_SCHEM");
            System.out.println(tableSchem);
        }
    }

    public static void resultSet(OracleConnection con) throws Exception {
        OracleStatement stat = (OracleStatement) con.createStatement();
        SqlDefine sql = new SqlDefine();

        ResultSet rs = stat.executeQuery(sql.selectSql);
//        rs.absolute(5); // moves the cursor to the fifth row of rs
        rs.updateString("F_3", "AINSWORTH"); // updates the
        // NAME column of row 5 to be AINSWORTH
        rs.updateRow(); // updates the row in the data source
    }
}

