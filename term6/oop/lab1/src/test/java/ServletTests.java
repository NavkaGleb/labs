import org.junit.Test;
import org.junit.jupiter.api.BeforeEach;
import org.mockito.InjectMocks;
import org.mockito.Mock;
import org.mockito.MockitoAnnotations;
import servlets.*;

import static org.junit.Assert.*;
import static org.junit.jupiter.api.Assertions.assertThrows;
import static org.mockito.Mockito.*;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.PrintWriter;
import java.util.HashMap;
import java.util.Map;

public class ServletTests {

    @Mock
    HttpServletRequest request = mock(HttpServletRequest.class);
    @Mock
    HttpServletResponse response = mock(HttpServletResponse.class);

    @InjectMocks
    LoginServlet loginServlet = new LoginServlet();
    @InjectMocks
    CartServlet cartServlet = new CartServlet();
    @InjectMocks
    LogoutServlet logoutServlet = new LogoutServlet();
    @InjectMocks
    ProductsServlet productsServlet = new ProductsServlet();
    @InjectMocks
    RegistrationServlet registrationServlet = new RegistrationServlet();
    @InjectMocks
    UsersServlet usersServlet = new UsersServlet();

    @BeforeEach
    void injectDependencies() {
        MockitoAnnotations.openMocks(this);
    }

    private void setRequestParametersForLogin(String name, String password) {
        Map<String, String[]> parameterMap = new HashMap<>();
        parameterMap.put("username", new String[]{name});
        parameterMap.put("password", new String[]{password});
        when(request.getParameterMap()).thenReturn(parameterMap);
    }

    private void setRequestParametersForRegistration(String name, String password, String role) {
        Map<String, String[]> parameterMap = new HashMap<>();
        parameterMap.put("username", new String[]{name});
        parameterMap.put("password", new String[]{password});
        parameterMap.put("role", new String[]{role});
        when(request.getParameterMap()).thenReturn(parameterMap);
    }

    /* LoginServletTests*/

    @Test
    public void testLoginIncorrectDetails() throws IOException {
        setRequestParametersForLogin("name", "password");
        final ByteArrayOutputStream outputStream = new ByteArrayOutputStream();
        when(response.getWriter()).thenReturn(new PrintWriter(outputStream));
        loginServlet.doPost(request, response);
        assertNotNull(response);
        assertTrue(outputStream.toString().isEmpty());
    }

    /* CartServletTests*/

    @Test
    public void testGetNullRequestCart() {
        Throwable exception = assertThrows(IllegalArgumentException.class, () -> cartServlet.doGet(null, response));
        assertEquals("Response/request must not be null.", exception.getMessage());
    }

    @Test
    public void testGetNullResponseCart() {
        Throwable exception = assertThrows(IllegalArgumentException.class, () -> cartServlet.doGet(request, null));
        assertEquals("Response/request must not be null.", exception.getMessage());
    }

    /* LogoutServletTests*/

    @Test
    public void testGetNullRequestLogout() {
        Throwable exception = assertThrows(IllegalArgumentException.class, () -> logoutServlet.doPost(null, response));
        assertEquals("Response/request must not be null.", exception.getMessage());
    }

    @Test
    public void testGetNullResponseLogout() {
        Throwable exception = assertThrows(IllegalArgumentException.class, () -> logoutServlet.doPost(request, null));
        assertEquals("Response/request must not be null.", exception.getMessage());
    }

    /* ProductsServletTests*/

    @Test
    public void testShowAllProducts() throws IOException {
        final ByteArrayOutputStream outputStream = new ByteArrayOutputStream();
        when(response.getWriter()).thenReturn(new PrintWriter(outputStream));
        productsServlet.doGet(request, response);
        assertNotNull(response);
        assertTrue(outputStream.toString().isEmpty());
    }


    @Test
    public void testGetNullRequestProducts() {
        Throwable exception = assertThrows(IllegalArgumentException.class, () -> productsServlet.doPost(null, response));
        assertEquals("Response/request must not be null.", exception.getMessage());
    }

    @Test
    public void testGetNullResponseProducts() {
        Throwable exception = assertThrows(IllegalArgumentException.class, () -> productsServlet.doPost(request, null));
        assertEquals("Response/request must not be null.", exception.getMessage());
    }

    /* RegistrationServletTests*/

    @Test
    public void testGetNullRequestRegistration() {
        Throwable exception = assertThrows(IllegalArgumentException.class, () -> registrationServlet.doPost(null, response));
        assertEquals("Response/request must not be null.", exception.getMessage());
    }

    @Test
    public void testGetNullResponseRegistration() {
        Throwable exception = assertThrows(IllegalArgumentException.class, () -> registrationServlet.doPost(request, null));
        assertEquals("Response/request must not be null.", exception.getMessage());
    }

    @Test
    public void testRegistrationSuccess() {
        setRequestParametersForRegistration("name1", "password1", "client");
        registrationServlet.doPost(request, response);
        verify(response).setStatus(HttpServletResponse.SC_OK);
    }

    /* UsersServletTests*/

    @Test
    public void testGetNullRequestUsers() {
        Throwable exception = assertThrows(IllegalArgumentException.class, () -> usersServlet.doGet(null, response));
        assertEquals("Response/request must not be null.", exception.getMessage());
    }

    @Test
    public void testGetNullResponseUsers() {
        Throwable exception = assertThrows(IllegalArgumentException.class, () -> usersServlet.doGet(request, null));
        assertEquals("Response/request must not be null.", exception.getMessage());
    }
}
