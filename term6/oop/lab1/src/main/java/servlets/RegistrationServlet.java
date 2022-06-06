package servlets;

import entity.User;
import entity.UserType;
import service.UserService;

import javax.servlet.RequestDispatcher;
import javax.servlet.ServletContext;
import javax.servlet.ServletException;
import javax.servlet.annotation.WebServlet;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.io.IOException;
import java.util.Map;
import java.util.logging.Logger;

@WebServlet("/servlets/registration")
public class RegistrationServlet extends HttpServlet {

    private static final Logger log = Logger.getLogger(RegistrationServlet.class.getName());

    public RegistrationServlet() {
        super();
    }

    @Override
    public void doPost(HttpServletRequest request, HttpServletResponse response) {
        if (response == null || request == null) {
            throw new IllegalArgumentException("Response/request must not be null.");
        }

        log.info("Received data from the registration.");

        Map<String, String[]> parameterMap = request.getParameterMap();
        String name = parameterMap.get("username")[0];
        String password = parameterMap.get("password")[0];
        UserType type = UserType.fromString(parameterMap.get("role")[0]);
        User user = new User(name, password, type, false);

        UserService.registerUser(user);
        response.setStatus(HttpServletResponse.SC_OK);
    }

    @Override
    public void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
        if (response == null || request == null) {
            throw new IllegalArgumentException("Response/request must not be null.");
        }

        ServletContext servletContext = getServletContext();
        RequestDispatcher requestDispatcher = servletContext.getRequestDispatcher("/registration");
        requestDispatcher.forward(request, response);
    }
}
