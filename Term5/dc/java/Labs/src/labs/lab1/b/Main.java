package labs.lab1.b;

import javax.swing.*;

import labs.lab1.Button;
import labs.lab1.Window;
import labs.lab1.Label;
import labs.lab1.Slider;

import java.awt.*;

class ThreadComponent {

    public final Button startButton;
    public final Button stopButton;

    public Thread       thread;

    public ThreadComponent(String startButtonText, String stopButtonText) {
        startButton = new Button(startButtonText);
        stopButton = new Button(stopButtonText);
    }
}

public class Main {

    private int             semaphore;
    private final Window    window;

    private final ThreadComponent threadComponent1;
    private final ThreadComponent threadComponent2;

    private final Slider    slider;
    private final Label     status;

    private Main() {
        semaphore = 0;

        window = new Window(600, 400, "MainWindow");

        threadComponent1 = new ThreadComponent("Start1", "Stop1");
        threadComponent2 = new ThreadComponent("Start2", "Stop2");

        slider = new Slider();
        status = new Label("Label", SwingConstants.CENTER);

        threadComponent1.startButton.addActionListener(event -> {
            if (semaphore == 1) {
                return;
            }

            threadComponent1.thread = new Thread(() -> {
                semaphore = 1;

                slider.setValue(10);
                status.setText("Busy!");
                threadComponent2.stopButton.setEnabled(false);

                while (!Thread.interrupted());
            });

            threadComponent1.thread.setPriority(Thread.MIN_PRIORITY);
            threadComponent1.thread.start();
        });

        threadComponent2.startButton.addActionListener(event -> {
            if (semaphore == 1) {
                return;
            }

            threadComponent2.thread = new Thread(() -> {
                semaphore = 1;

                slider.setValue(90);
                status.setText("Busy!");
                threadComponent1.stopButton.setEnabled(false);

                while (!Thread.interrupted());
            });

            threadComponent2.thread.setPriority(Thread.MIN_PRIORITY);
            threadComponent2.thread.start();
        });

        threadComponent1.stopButton.addActionListener(e -> {
            if (semaphore == 0) {
                return;
            }

            threadComponent1.thread.interrupt();
            threadComponent2.stopButton.setEnabled(true);
            status.setText("Free");

            semaphore = 0;
        });

        threadComponent2.stopButton.addActionListener(e -> {
            if (semaphore == 0) {
                return;
            }

            threadComponent2.thread.interrupt();
            threadComponent1.stopButton.setEnabled(true);
            status.setText("Free");

            semaphore = 0;
        });

        setComponents();
    }

    private void setComponents() {
        JPanel mainPanel = new JPanel();
        JPanel startButtonPanel = new JPanel();
        JPanel stopButtonPanel = new JPanel();

        startButtonPanel.setLayout(new GridLayout(1, 2, 4, 4));
        startButtonPanel.add(threadComponent1.startButton);
        startButtonPanel.add(threadComponent2.startButton);

        stopButtonPanel.setLayout(new GridLayout(1, 2, 4, 4));
        stopButtonPanel.add(threadComponent1.stopButton);
        stopButtonPanel.add(threadComponent2.stopButton);

        mainPanel.setLayout(new GridLayout(4, 1, 4, 4));
        mainPanel.add(startButtonPanel);
        mainPanel.add(stopButtonPanel);
        mainPanel.add(slider);
        mainPanel.add(status);

        window.setContentPane(mainPanel);
        window.setVisible(true);
    }

    public static void main(String[] args) {
        SwingUtilities.invokeLater(Main::new);
    }
}
