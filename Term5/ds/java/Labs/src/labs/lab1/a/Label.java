package labs.lab1.a;

import javax.swing.*;
import java.awt.*;

public class Label extends JLabel {

    Label(String text) {
        super(text);

        super.setFont(new Font("Arial", Font.PLAIN, 18));
    }

    Label(String text, int horizontalAlignment) {
        super(text, horizontalAlignment);

        super.setFont(new Font("Arial", Font.PLAIN, 18));
    }

    Label(String text, int horizontalAlignment, int fontSize) {
        super(text, horizontalAlignment);

        super.setFont(new Font("Arial", Font.PLAIN, fontSize));
    }
}
