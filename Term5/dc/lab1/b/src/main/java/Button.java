package labs.lab1;

import javax.swing.*;
import java.awt.*;

public class Button extends JButton {

    private Color hoverBackgroundColor = new Color(194, 194, 194);
    private Color pressedBackgroundColor = new Color(166, 166, 166);

    public Button(String title) {
        super(title);

        super.setBackground(new Color(210, 210, 210));
        super.setBorder(null);
        super.setContentAreaFilled(false);
        super.setFocusPainted(false);
        super.setFont(new Font("Arial", Font.PLAIN, 18));
    }

    public Button(String title, int fontSize) {
        super(title);

        super.setBackground(new Color(210, 210, 210));
        super.setBorder(null);
        super.setContentAreaFilled(false);
        super.setFocusPainted(false);
        super.setFont(new Font("Arial", Font.PLAIN, fontSize));
    }

    @Override
    protected void paintComponent(Graphics graphics) {
        ButtonModel model = super.getModel();

        if (model.isPressed())
            graphics.setColor(pressedBackgroundColor);
        else if (model.isRollover())
            graphics.setColor(hoverBackgroundColor);
        else
            graphics.setColor(super.getBackground());

        graphics.fillRect(0, 0, getWidth(), getHeight());
        super.paintComponent(graphics);
    }

    public Color getHoverBackgroundColor() {
        return hoverBackgroundColor;
    }

    public void setHoverBackgroundColor(Color hoverBackgroundColor) {
        this.hoverBackgroundColor = hoverBackgroundColor;
    }

    public Color getPressedBackgroundColor() {
        return pressedBackgroundColor;
    }

    public void setPressedBackgroundColor(Color pressedBackgroundColor) {
        this.pressedBackgroundColor = pressedBackgroundColor;
    }
}