import java.sql.*;
import java.util.ArrayList;
import java.util.List;

public class StatementImp {
    /**
     * execute返回值为boolean类型。一般放入的是DDL语言
     *
     * @param con
     * @param sqlString
     * @return
     */
    public static boolean execute(Connection con, String sqlString) {
        try {
            Statement stat = con.createStatement();
            boolean res = stat.execute(sqlString);
            System.out.println("execute result : " + res);
            return true;
        } catch (SQLException e) {
            e.printStackTrace();
            return false;
        }
    }

    /**
     * 更新几行int类型。放入的是DML语言
     *
     * @param con
     * @param sqlString
     * @return
     */
    public static boolean executeUpdate(Connection con, String sqlString) {
        try {
            Statement stat = con.createStatement();
            int res = stat.executeUpdate(sqlString);
            System.out.println("execute result : " + res);
            return true;
        } catch (SQLException e) {
            e.printStackTrace();
            return false;
        }
    }

    /**
     * 返回值为结果集ResultSet。放入的是DQL语言
     *
     * @param con
     * @param sqlString
     * @return
     */
    public static boolean executeQuery(Connection con, String sqlString) {
        try {
            Statement stat = con.createStatement();
            ResultSet res = stat.executeQuery(sqlString);
            while (res.next()) {
            }
            return true;
        } catch (SQLException e) {
            e.printStackTrace();
            return false;
        }
    }

    /**
     * 为 JDBC 驱动程序提供一个提示，它提示此 Statement 生成的 ResultSet 对象需要更多行时应该从数据库获取的行数
     *
     * @param con
     * @param sqlString
     * @param rows
     * @return
     */
    public static boolean setFetchSize(Connection con, String sqlString, int rows) {
        try {
            Statement stat = con.createStatement();
            stat.setFetchSize(rows);
            ResultSet res = stat.executeQuery(sqlString);
            while (res.next()) {
            }
            return true;
        } catch (SQLException e) {
            e.printStackTrace();
            return false;
        }
    }

    /**
     * 向驱动程序提供关于方向的提示，在使用此 Statement 对象创建的 ResultSet 对象中将按该方向处理行。
     * 可以设置的参数是ResultSet.FETCH_FORWARD 、ResultSet.FETCH_REVERSE 和 ResultSet.FETCH_UNKNOWN
     *
     * @param con
     * @param sqlString
     * @param direction
     * @return
     */
    public static boolean setFetchDirection(Connection con, String sqlString, int direction) {
        try {
            Statement stat = con.createStatement();
            stat.setFetchDirection(direction);
            ResultSet res = stat.executeQuery(sqlString);
            while (res.next()) {
            }
            return true;
        } catch (SQLException e) {
            e.printStackTrace();
            return false;
        }
    }
    public static void cursorTest(Connection con) throws Exception {
        List<Statement> lStat = new ArrayList<Statement>();
        Statement pStat = null;
        SqlDefine sqlDefine = new SqlDefine();
        int cursorNum = 564;

        for (int i = 0; i < cursorNum; i++) {
            pStat = con.createStatement();
            pStat.execute(sqlDefine.insertSql);
            pStat.execute(sqlDefine.insertSql);
            lStat.add(pStat);
        }

        for (int i = 0; i < cursorNum; i++) {
            lStat.get(i).close();
        }

        pStat = con.createStatement();
        pStat.execute(sqlDefine.insertSql);
        pStat.execute(sqlDefine.insertSql);

        pStat.close();
    }
}
