import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.SQLException;
import java.sql.Statement;

import com.cloudera.hive.jdbc41.*;

public class MainTest {

    public static void main(String[] args) throws SQLException, ClassNotFoundException {
        ConnProprety connProprety = new ConnProprety();
        Connection conn = null;

        Class.forName(connProprety.driverName);
        conn = DriverManager.getConnection(connProprety.url,connProprety.clusterAdmin,connProprety.clusterPassword);

        Statement stat = conn.createStatement();
        stat.execute("show tables");

        conn.close();
    }
}
