import oracle.jdbc.OracleConnection;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.SQLException;
import java.util.Properties;

public class OracleTest {
    Connection con = null;
//    OracleOCIConnectionPool con = null;
//    OracleConnection con = null;

    OracleTest() throws SQLException, ClassNotFoundException {
        Properties connectionProps = new Properties();
        ConProperties conInfo = new ConProperties();
        String conURL = conInfo.url18cOCI;

        Class.forName(conInfo.jdbcDriverName);
//        connectionProps.put("user", conInfo.sysdbaUser);
//        connectionProps.put("password", conInfo.password);
//        connectionProps.setProperty(OracleConnection.CONNECTION_PROPERTY_USER_NAME, conInfo.username);
//        connectionProps.setProperty(OracleConnection.CONNECTION_PROPERTY_PASSWORD, conInfo.password);
//        connectionProps.setProperty(OracleConnection.CONNECTION_PROPERTY_THIN_VSESSION_MACHINE, "xuPc");
//        connectionProps.setProperty(OracleConnection.CONNECTION_PROPERTY_THIN_VSESSION_OSUSER, "xuUser");
//        connectionProps.setProperty(OracleConnection.CONNECTION_PROPERTY_THIN_VSESSION_PROCESS, "xuProcess");
//        connectionProps.setProperty(OracleConnection.CONNECTION_PROPERTY_THIN_VSESSION_PROGRAM, "xuProgram");
//        connectionProps.setProperty(OracleConnection.CONNECTION_PROPERTY_THIN_VSESSION_TERMINAL, "xuTerminal");
        this.con = DriverManager.getConnection(conURL, conInfo.normalUser, conInfo.password);

    }


    public static void main(String[] args) throws Exception {
        OracleTest orclTest = new OracleTest();
        StatementImp stateImp = new StatementImp();
        PrepareStatementImp preStateImp = new PrepareStatementImp();
        CallableStatementImp callStateImp = new CallableStatementImp();
        OracleStatusmentImp oraStateImp = new OracleStatusmentImp();
        SqlDefine sqlDefine = new SqlDefine();

//        StatementImp.execute(orclTest.con, sqlDefine.selectSql);
//        StatementImp.execute(orclTest.con, sqlDefine.deleteSql);
        StatementImp.cursorTest(orclTest.con);
//        StatementImp.setFetchSize(orclTest.con, "SELECT * FROM TBL_MAIN_TEST", 5);
//        StatementImp.setFetchDirection(orclTest.con, "SELECT * FROM TBL_MAIN_TEST", ResultSet.FETCH_REVERSE);
//        preStateImp.cursorTest(orclTest.con);
//        preStateImp.cursorLoopTest(orclTest.con);
//        preStateImp.bindTest(orclTest.con);
//        callStateImp.execute(orclTest.con);
//        OracleStatusmentImp.executeTest((OracleConnection) orclTest.con);
//        OracleStatusmentImp.executeLongSQLTest((OracleConnection) orclTest.con);
//        oraStateImp.databaseMetaData((OracleConnection)orclTest.con);
//        oraStateImp.resultSet((OracleConnection)orclTest.con);

        orclTest.con.close();
    }
}

