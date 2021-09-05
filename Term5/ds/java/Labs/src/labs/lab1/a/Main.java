package labs.lab1.a;

import javax.swing.*;
import java.awt.*;

class ThreadComponent {

    public Thread thread;

    public Button decreasePriorityButton;
    public Button increasePriorityButton;
    public Label priorityLabel;
    
    private int threadPriority;

    public ThreadComponent() {
        this.threadPriority = 5;

        this.decreasePriorityButton = new Button("-");
        this.increasePriorityButton = new Button("+");

        this.priorityLabel = new Label(String.valueOf(this.threadPriority), SwingConstants.CENTER, 17);
        this.priorityLabel.setFont(new Font("Arial", Font.PLAIN, 20));

        this.decreasePriorityButton.addActionListener(event -> {
            if (this.threadPriority > 1) {
                --threadPriority;

                this.thread.setPriority(this.threadPriority);
                this.priorityLabel.setText(String.valueOf(this.threadPriority));
            }
        });

        this.increasePriorityButton.addActionListener(event -> {
            if (this.threadPriority < 10) {
                ++threadPriority;

                this.thread.setPriority(this.threadPriority);
                this.priorityLabel.setText(String.valueOf(this.threadPriority));
            }
        });
    }

    public void startThread(Runnable runnable) {
        this.thread = new Thread(runnable);

        this.thread.setPriority(this.threadPriority);
        this.thread.start();
    }

    public void stopThread() {
        this.thread.interrupt();
        this.thread = null;
    }
}

public class Main {

    private final Window window;

    private final Button startButton;
    private final Button stopButton;
    private final Label valueLabel;

    private final Slider slider;
    private final ThreadComponent leftThreadComponent;
    private final ThreadComponent rightThreadComponent;

    private Main() {
        this.window = new Window(800, 500, "MainWindow");

        this.slider = new Slider();

        this.startButton = new Button("Start");
        this.stopButton = new Button("Stop");
        this.valueLabel = new Label(String.valueOf(this.slider.getValue()), SwingConstants.CENTER);

        this.leftThreadComponent = new ThreadComponent();
        this.rightThreadComponent = new ThreadComponent();

        setComponents();

        this.startButton.addActionListener(event -> {
            this.leftThreadComponent.startThread(() -> this.runThread(-1));
            this.rightThreadComponent.startThread(() -> this.runThread(1));
        });

        this.stopButton.addActionListener(event -> {
            this.leftThreadComponent.stopThread();
            this.rightThreadComponent.stopThread();
        });
    }

    private void runThread(int step) {
        while (!Thread.interrupted()) {
            synchronized (this) {
                this.slider.increment(step);
                this.valueLabel.setText(String.valueOf(this.slider.getValue()));
            }

            try {
                Thread.sleep(10);
            } catch (InterruptedException e) {
//                e.printStackTrace();
                Thread.currentThread().interrupt();
            }
        }
    }

    private void setComponents() {
        JPanel mainPanel = new JPanel();
        JPanel threadTitlePanel = new JPanel();
        JPanel threadControlPanel = new JPanel();
        JPanel sliderValuePanel = new JPanel();

        mainPanel.setLayout(new GridLayout(6, 1, 4, 4));

        threadTitlePanel.setLayout(new GridLayout(1, 2, 4, 4));
        threadTitlePanel.add(new Label("LeftThread", SwingConstants.CENTER));
        threadTitlePanel.add(new Label("RightThread", SwingConstants.CENTER));

        threadControlPanel.setLayout(new GridLayout(1, 6, 4, 4));
        threadControlPanel.add(this.leftThreadComponent.decreasePriorityButton);
        threadControlPanel.add(this.leftThreadComponent.priorityLabel);
        threadControlPanel.add(this.leftThreadComponent.increasePriorityButton);
        threadControlPanel.add(this.rightThreadComponent.decreasePriorityButton);
        threadControlPanel.add(this.rightThreadComponent.priorityLabel);
        threadControlPanel.add(this.rightThreadComponent.increasePriorityButton);

        sliderValuePanel.setLayout(new GridLayout(1, 3, 4, 4));
        sliderValuePanel.add(new Label("0", SwingConstants.CENTER));
        sliderValuePanel.add(this.valueLabel);
        sliderValuePanel.add(new Label("100", SwingConstants.CENTER));

        mainPanel.add(this.startButton);
        mainPanel.add(threadTitlePanel);
        mainPanel.add(threadControlPanel);
        mainPanel.add(this.slider);
        mainPanel.add(sliderValuePanel);
        mainPanel.add(this.stopButton);

        window.setContentPane(mainPanel);
        window.setVisible(true);
    }

    public static void main(String[] args) {
        SwingUtilities.invokeLater(Main::new);
    }
}
