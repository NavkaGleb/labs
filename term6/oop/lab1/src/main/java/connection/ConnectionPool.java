package connection;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.SQLException;
import java.util.concurrent.BlockingQueue;
import java.util.concurrent.LinkedBlockingQueue;
import java.util.logging.Logger;


public class ConnectionPool {
    private static final ConnectionPool cp = new ConnectionPool();
    private static final Logger log = Logger.getLogger(ConnectionPool.class.getName());

    private final String url = "jdbc:postgresql://localhost:5432/internetshop";
    private final String user = "postgres";
    private final String password = "admin";
    private final int MAX_CONNECTIONS = 8;

    private BlockingQueue<Connection> connections;

    private ConnectionPool() {
        try {
            Class.forName("org.postgresql.Driver");
        } catch (ClassNotFoundException e) {
            log.warning("JDBC Driver not found");
        }

        connections = new LinkedBlockingQueue<>(MAX_CONNECTIONS);

        try {
            for (int i = 0; i < MAX_CONNECTIONS; ++i) {
                connections.put(DriverManager.getConnection(url, user, password));
            }
        } catch (SQLException e) {
            log.warning("Troubles with database");
        } catch (InterruptedException e) {
            log.warning("Connection was interrupted");
        }
    }

    public static ConnectionPool getConnectionPool() {
        return cp;
    }

    public Connection getConnection() throws InterruptedException, SQLException {
        Connection c = connections.take();

        if (c.isClosed()) {
            c = DriverManager.getConnection(url, user, password);
        }

        return c;
    }

    public void releaseConnection(Connection c) throws InterruptedException {
        connections.put(c);
    }
}

