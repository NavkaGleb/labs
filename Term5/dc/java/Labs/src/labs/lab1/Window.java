package labs.lab1;

import javax.swing.*;

public class Window extends JFrame {

    public Window(int width, int height, String title) {
        super(title);

        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setSize(width, height);
        setLocationRelativeTo(null);
    }
}