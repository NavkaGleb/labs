import entity.Admin;
import entity.Client;
import entity.UserType;
import connection.ConnectionPool;
import org.junit.Test;

import java.sql.Connection;
import java.sql.SQLException;

import static junit.framework.TestCase.*;

public class AppTest {
    @Test
    public void testAdministratorHasType() {
        Admin administrator = new Admin(1, "test", "password");
        assertNotNull(administrator.getType());
        assertEquals(administrator.getType(), UserType.ADMINISTRATOR);
    }

    @Test
    public void testClientHasType() {
        Client client = new Client(1, "test", "password");
        assertNotNull(client.getType());
        assertEquals(client.getType(), UserType.CLIENT);
    }

    @Test
    public void testDefaultAdministratorIsNotBlocked() {
        Admin administrator = new Admin(1, "test", "password");
        assertFalse(administrator.isBlocked());
    }

    @Test
    public void testDefaultClientIsNotBlocked() {
        Client client = new Client(1, "test", "password");
        assertFalse(client.isBlocked());
    }

    @Test
    public void testPostgresqlConnectionExists() throws SQLException, InterruptedException {
        ConnectionPool cp = ConnectionPool.getConnectionPool();
        Connection connection = cp.getConnection();
        assertNotNull(connection);
    }
}
