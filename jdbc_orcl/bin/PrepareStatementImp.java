import java.sql.*;
import java.util.ArrayList;
import java.util.List;

public class PrepareStatementImp {
    public static void cursorTest(Connection con) throws Exception {
        List<PreparedStatement> lStat = new ArrayList<PreparedStatement>();
        PreparedStatement pStat = null;
        ResultSet rSet = null;
        SqlDefine sqlDefine = new SqlDefine();
        int cursorNum = 564;

        for (int i = 0; i < cursorNum; i++) {
            pStat = con.prepareStatement(sqlDefine.insertSql);
            rSet = pStat.executeQuery();
            lStat.add(pStat);
        }

        for (int i = 0; i < cursorNum; i++) {
            lStat.get(i).close();
        }

        pStat = con.prepareStatement(sqlDefine.insertSql);
        rSet = pStat.executeQuery();

        pStat.close();
    }

    public static void bindTest(Connection con) throws Exception {
        PreparedStatement pStat = null;
        ResultSet rSet = null;
        SqlDefine sqlDefine = new SqlDefine();
        int bindExecTimes = 1;
        int bindBum = 5;
        Clob clob = con.createClob();
        RowId rowId = null;
//        for(int i = 1 ; i < bindBum ; i++){
//            String tmp = String.format("and F_%d = ?",i + 1);
//            sqlBindString += tmp;
//        }

        pStat = con.prepareStatement(sqlDefine.bindInsertSql);
        for (int i = 0; i < bindExecTimes; i++) {
            pStat.setInt(1, 564);
            pStat.executeQuery();
        }
//        pStat.addBatch();
//        pStat.executeBatch();
    }

    public static void cursorLoopTest(Connection con) throws Exception {
        PreparedStatement pStat = null;
        ResultSet rSet = null;
        SqlDefine sqlDefine = new SqlDefine();
        int loop = 256;

        pStat = con.prepareStatement(sqlDefine.selectSql);

        for (int i = 0; i < loop; i++) {
            rSet = pStat.executeQuery();
        }

        pStat.close();
    }
}
