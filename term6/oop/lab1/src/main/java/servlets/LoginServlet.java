package servlets;

import com.google.gson.Gson;
import service.UserService;

import javax.servlet.annotation.WebServlet;
import javax.servlet.http.Cookie;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.io.IOException;
import java.util.Map;
import java.util.logging.Logger;

@WebServlet("/servlets/login")
public class LoginServlet extends HttpServlet {

    private static final Logger log = Logger.getLogger(LoginServlet.class.getName());

    @Override
    public void doPost(HttpServletRequest request, HttpServletResponse response) throws IOException {
        log.info("Received data from logging in.");

        if (response == null || request == null) {
            throw new IllegalArgumentException("Response/request must not be null.");
        }

        Map<String, String[]> parameterMap = request.getParameterMap();
        String name = parameterMap.get("username")[0];
        String password = parameterMap.get("password")[0];
        UserService.UserInfo userInfo = UserService.findUser(name, password);
        Gson gson = new Gson();

        if (userInfo != null) {
            if (!userInfo.blocked) {
                log.info("Received info about the user in the servlet.");

                Cookie loginCookie = new Cookie("user", Integer.toString(userInfo.id));
                Cookie typeCookie = new Cookie("type", userInfo.type.toString());

                loginCookie.setPath("/");
                typeCookie.setPath("/");
                response.addCookie(loginCookie);
                response.addCookie(typeCookie);

                log.info("Redirecting to " + userInfo.type.toString());
                response.getWriter().println(gson.toJson(userInfo.type));
            } else {
                log.info("User is blocked");
                response.getWriter().println(gson.toJson("blocked"));
            }
        } else {
            log.info("User doesn't exist");
            response.getWriter().println(gson.toJson("notexists"));
        }
    }
}
