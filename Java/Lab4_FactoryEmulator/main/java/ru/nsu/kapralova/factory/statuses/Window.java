package ru.nsu.kapralova.factory.statuses;

import javax.swing.*;
import javax.swing.event.ChangeEvent;
import javax.swing.event.ChangeListener;
import javax.swing.border.EmptyBorder;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.time.Duration;

public class Window extends JFrame implements ActionListener {
    private Duration BODY_TIME;
    private Duration ENGINE_TIME;
    private Duration ACCESSORIES_TIME;
    private Duration DEALER_TIME;
    private Duration WORKER_TIME;

    JLabel engine = new JLabel("engine");
    JLabel body = new JLabel("body");
    JLabel accessory = new JLabel("accessories");
    JLabel mainStorage = new JLabel("car storage");

    public void showCountBody(int size, int capacity) {
        String output = String.format("Count bodies in storage is %d of %d", size, capacity);
        body.setText(output);
    }
    public void showCountEngine(int size, int capacity) {
        String output = String.format("Count engines in storage is %d of %d", size, capacity);
        engine.setText(output);
    }
    public void showCountAccessories(int size, int capacity) {
        String output = String.format("Count accessories in storage is %d of %d", size, capacity);
        accessory.setText(output);
    }
    public void showCountCar(int size, int capacity) {
        String output = String.format("Count car in storage is  %d of %d", size, capacity);
        mainStorage.setText(output);
    }

    public Window(){
        super("Factory emulator");

        BODY_TIME = Duration.ofSeconds(5);
        ENGINE_TIME = Duration.ofSeconds(5);
        ACCESSORIES_TIME = Duration.ofSeconds(5);
        DEALER_TIME = Duration.ofSeconds(15);
        WORKER_TIME = Duration.ofSeconds(15);

        Font font = new Font("Verdana", Font.ITALIC, 16);
        engine.setFont(font);
        body.setFont(font);
        accessory.setFont(font);
        JLabel workers = new JLabel("Workers create car every so seconds:  ");
        workers.setFont(font);
        JLabel dealers = new JLabel("Dealers get car every so seconds:  ");
        dealers.setFont(font);
        JLabel speedE = new JLabel("Engines are created every so seconds:");
        speedE.setFont(font);
        JLabel speedB = new JLabel("Bodies are created every so seconds:");
        speedB.setFont(font);
        JLabel speedA = new JLabel("Accessories are created every so seconds:");
        speedA.setFont(font);

        JPanel frame = new JPanel();

        JPanel details = new JPanel(new GridLayout(1, 3));
        add(details, BorderLayout.PAGE_START);

        JPanel engines = new JPanel(new GridLayout(4, 1));
        engines.setBorder(new EmptyBorder(10, 10, 10, 10));
        details.add(engines);
        engines.add(engine);
        engines.add(speedE);

        JSlider slider = new JSlider(0, 10);
        slider.setPaintLabels(true);
        slider.setMajorTickSpacing(1);
        //slider.setBackground(Color.lightGray);
        engines.add(slider);
        slider.addChangeListener(new ChangeListener() {
            public void stateChanged(ChangeEvent e) {
                int value = ((JSlider)e.getSource()).getValue();
                ENGINE_TIME = Duration.ofSeconds(value);
            }
        });

        JPanel bodies = new JPanel(new GridLayout(4, 1));
        bodies.setBorder(new EmptyBorder(10, 10, 10, 10));
        details.add(bodies);
        bodies.add(body);
        bodies.add(speedB);

        JSlider slider2 = new JSlider(0, 10);
        slider2.setPaintLabels(true);
        slider2.setMajorTickSpacing(1);
        //slider2.setBackground(Color.lightGray);
        bodies.add(slider2);
        slider2.addChangeListener(new ChangeListener() {
            public void stateChanged(ChangeEvent e) {
                int value = ((JSlider)e.getSource()).getValue();
                BODY_TIME = Duration.ofSeconds(value);
            }
        });


        JPanel accessories = new JPanel(new GridLayout(4, 1));
        accessories.setBorder(new EmptyBorder(10, 10, 10, 10));
        details.add(accessories);
        accessories.add(accessory);
        accessories.add(speedA);

        JSlider slider3 = new JSlider(0, 10);
        slider3.setPaintLabels(true);
        slider3.setMajorTickSpacing(1);
        //slider3.setBackground(Color.lightGray);
        accessories.add(slider3);
        slider3.addChangeListener(new ChangeListener() {
            public void stateChanged(ChangeEvent e) {
                int value = ((JSlider)e.getSource()).getValue();
                ACCESSORIES_TIME = Duration.ofSeconds(value);
            }
        });

        frame.add(workers);
        JSlider slider4 = new JSlider(0, 30);
        slider4.setPaintLabels(true);
        slider4.setMajorTickSpacing(3);
        //slider4.setBackground(Color.lightGray);
        frame.add(slider4);
        slider4.addChangeListener(new ChangeListener() {
            public void stateChanged(ChangeEvent e) {
                int value = ((JSlider)e.getSource()).getValue();
                WORKER_TIME = Duration.ofSeconds(value);
            }
        });

        frame.add(dealers);
        JSlider slider5 = new JSlider(0, 30);
        slider5.setPaintLabels(true);
        slider5.setMajorTickSpacing(3);
        //slider5.setBackground(Color.lightGray);
        frame.add(slider5);
        slider5.addChangeListener(new ChangeListener() {
            public void stateChanged(ChangeEvent e) {
                int value = ((JSlider)e.getSource()).getValue();
                DEALER_TIME = Duration.ofSeconds(value);
            }
        });

        mainStorage.setFont(font);
        frame.add(mainStorage);

        add(frame);
        setSize(1100, 300);
        setVisible(true);
        setDefaultCloseOperation(EXIT_ON_CLOSE);
    }

    @Override
    public void actionPerformed(ActionEvent e) { }
    public Duration getBODY_TIME() {
        return BODY_TIME;
    }
    public Duration getENGINE_TIME() {
        return ENGINE_TIME;
    }
    public Duration getACCESSORIES_TIME() {
        return ACCESSORIES_TIME;
    }
    public Duration getDEALER_TIME() {
        return DEALER_TIME;
    }
    public Duration getWORKER_TIME() {
        return WORKER_TIME;
    }
}

