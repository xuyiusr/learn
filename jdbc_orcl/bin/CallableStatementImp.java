import java.sql.CallableStatement;
import java.sql.Connection;
import java.sql.SQLException;

public class CallableStatementImp {
    public static void execute(Connection con) throws SQLException {
        SqlDefine sqlDefine = new SqlDefine();

        CallableStatement cstmt = con.prepareCall(sqlDefine.callSql);
        cstmt.registerOutParameter(1, java.sql.Types.TINYINT);
        cstmt.registerOutParameter(2, java.sql.Types.DECIMAL, 3);
        cstmt.executeQuery();
    }
}
