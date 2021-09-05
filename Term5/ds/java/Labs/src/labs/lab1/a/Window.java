package labs.lab1.a;

import javax.swing.*;

class Window extends JFrame {

    Window(int width, int height, String title) {
        super(title);

        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setSize(width, height);
        setLocationRelativeTo(null);
    }
}