package servlets;

import com.google.gson.Gson;
import entity.User;
import service.UserService;

import javax.servlet.annotation.WebServlet;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.io.IOException;
import java.util.List;
import java.util.logging.Logger;

@WebServlet("/servlets/users/*")
public class UsersServlet extends HttpServlet {

    private static final Logger log = Logger.getLogger(UsersServlet.class.getName());

    public UsersServlet() {
        super();
    }

    @Override
    public void doGet(HttpServletRequest request, HttpServletResponse response) throws IOException {
        log.info("Received get request");

        if (response == null || request == null) {
            throw new IllegalArgumentException("Response/request must not be null.");
        }

        Gson gson = new Gson();

        if (request.getPathInfo() == null || request.getPathInfo().equals("/")) {
            log.info("Received request to get all users");

            List<User> clients = UserService.getClientUsers();

            response.getWriter().println(gson.toJson(clients.toArray(new User[]{})));

            return;
        }

        String[] urls = request.getPathInfo().split("/");

        if (urls.length == 2) {
            int id = Integer.parseInt(request.getParameter("id"));

            switch (urls[1]) {
                case "block" -> UserService.editBlocked(id, true);
                case "unblock" -> UserService.editBlocked(id, false);
            }
        }
    }
}
