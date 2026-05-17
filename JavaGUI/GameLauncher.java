package JavaGUI;

import javax.swing.*;
import java.awt.*;
import java.awt.event.*;
import java.io.*;
import java.util.*;

public class GameLauncher extends JFrame {
    
    private static final String EXE_PATH;
    private static final String LEADERBOARD_FILE = "leaderboard.txt";
    private Font pvzFont;

    static {
        if (new File("build\\Debug\\PlantsVsZombies.exe").exists()) {
            EXE_PATH = "build\\Debug\\PlantsVsZombies.exe";
        } else if (new File("..\\build\\Debug\\PlantsVsZombies.exe").exists()) {
            EXE_PATH = "..\\build\\Debug\\PlantsVsZombies.exe";
        } else {
            EXE_PATH = "PlantsVsZombies.exe"; // Fallback
        }
    }

    class BackgroundPanel extends JPanel {
        private Image backgroundImage;

        public BackgroundPanel() {
            File bgFile = new File("Assets\\Background\\mm.png");
            if (!bgFile.exists()) {
                bgFile = new File("..\\Assets\\Background\\mm.png");
            }
            if (bgFile.exists()) {
                backgroundImage = new ImageIcon(bgFile.getAbsolutePath()).getImage();
            }
        }

        @Override
        protected void paintComponent(Graphics g) {
            super.paintComponent(g);
            if (backgroundImage != null) {
                g.drawImage(backgroundImage, 0, 0, getWidth(), getHeight(), this);
            } else {
                // Fallback elegant dark green background if image is missing
                g.setColor(new Color(20, 60, 20));
                g.fillRect(0, 0, getWidth(), getHeight());
            }
        }
    }

    public GameLauncher() {
        setTitle("Plants vs Zombies - Launcher");
        setSize(800, 600);
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setLocationRelativeTo(null);

        // Load custom Burbank font
        try {
            File fontFile = new File("Assets\\fonts\\burbank.otf");
            if (!fontFile.exists()) {
                fontFile = new File("..\\Assets\\fonts\\burbank.otf");
            }
            pvzFont = Font.createFont(Font.TRUETYPE_FONT, fontFile);
            GraphicsEnvironment ge = GraphicsEnvironment.getLocalGraphicsEnvironment();
            ge.registerFont(pvzFont);
        } catch (Exception e) {
            pvzFont = new Font("Arial", Font.BOLD, 18);
        }

        BackgroundPanel backgroundPanel = new BackgroundPanel();
        backgroundPanel.setLayout(new GridBagLayout());
        setContentPane(backgroundPanel);

        // Create main UI panel centered on screen
        JPanel mainPanel = new JPanel();
        mainPanel.setLayout(new BoxLayout(mainPanel, BoxLayout.Y_AXIS));
        mainPanel.setOpaque(false);

        // Sub-panel for translucent card effect
        JPanel cardPanel = new JPanel() {
            @Override
            protected void paintComponent(Graphics g) {
                Graphics2D g2 = (Graphics2D) g.create();
                g2.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
                g2.setColor(new Color(25, 20, 15, 200)); // Dark translucent wood-brown
                g2.fillRoundRect(0, 0, getWidth(), getHeight(), 25, 25);
                g2.setColor(new Color(218, 165, 32, 150)); // Golden border outline
                g2.setStroke(new BasicStroke(4));
                g2.drawRoundRect(2, 2, getWidth() - 5, getHeight() - 5, 25, 25);
                g2.dispose();
            }
        };
        cardPanel.setLayout(new GridLayout(4, 1, 15, 15));
        cardPanel.setBorder(BorderFactory.createEmptyBorder(30, 40, 30, 40));
        cardPanel.setOpaque(false);

        JLabel titleLabel = new JLabel("Plants vs Zombies", SwingConstants.CENTER);
        titleLabel.setFont(pvzFont.deriveFont(Font.BOLD, 36f));
        titleLabel.setForeground(new Color(255, 223, 0)); // Bright PvZ yellow
        titleLabel.setAlignmentX(Component.CENTER_ALIGNMENT);
        cardPanel.add(titleLabel);

        JButton startButton = createPvZButton("Start New Game");
        startButton.addActionListener(e -> launchGame(""));
        cardPanel.add(startButton);

        JButton loadButton = createPvZButton("Load Saved Game");
        loadButton.addActionListener(e -> launchGame("--load"));
        cardPanel.add(loadButton);

        JButton leaderboardButton = createPvZButton("View Leaderboard");
        leaderboardButton.addActionListener(e -> showLeaderboard());
        cardPanel.add(leaderboardButton);

        mainPanel.add(cardPanel);
        backgroundPanel.add(mainPanel);
    }

    private JButton createPvZButton(String text) {
        JButton button = new JButton(text) {
            @Override
            protected void paintComponent(Graphics g) {
                Graphics2D g2 = (Graphics2D) g.create();
                g2.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
                
                Color bgColor;
                if (getModel().isPressed()) {
                    bgColor = new Color(34, 112, 34, 240); // Darker green
                } else if (getModel().isRollover()) {
                    bgColor = new Color(50, 168, 50, 240); // Lighter vibrant green
                } else {
                    bgColor = new Color(42, 140, 42, 210); // Standard green
                }
                
                g2.setColor(bgColor);
                g2.fillRoundRect(0, 0, getWidth(), getHeight(), 15, 15);
                
                g2.setColor(new Color(218, 165, 32)); // Golden border
                g2.setStroke(new BasicStroke(3));
                g2.drawRoundRect(1, 1, getWidth() - 3, getHeight() - 3, 15, 15);
                
                g2.dispose();
                super.paintComponent(g);
            }
        };
        
        button.setFont(pvzFont.deriveFont(Font.BOLD, 22f));
        button.setForeground(Color.YELLOW);
        button.setFocusPainted(false);
        button.setContentAreaFilled(false);
        button.setOpaque(false);
        button.setBorder(BorderFactory.createEmptyBorder(10, 20, 10, 20));
        button.setCursor(new Cursor(Cursor.HAND_CURSOR));
        
        // Dynamic hover animation for text color scaling
        button.addMouseListener(new MouseAdapter() {
            public void mouseEntered(MouseEvent e) {
                button.setForeground(Color.WHITE);
            }
            public void mouseExited(MouseEvent e) {
                button.setForeground(Color.YELLOW);
            }
        });
        
        return button;
    }

    private void launchGame(String args) {
        // Keep launcher visible or temporarily hidden during execution
        this.setVisible(false);
        try {
            String command = EXE_PATH + (args.isEmpty() ? "" : " " + args);
            File workingDir = new File(EXE_PATH).getAbsoluteFile().getParentFile();
            Process p = Runtime.getRuntime().exec(command, null, workingDir);

            BufferedReader reader = new BufferedReader(new InputStreamReader(p.getInputStream()));
            String line;
            while ((line = reader.readLine()) != null) {
                System.out.println(line);
                if (line.startsWith("GAME_OVER:")) {
                    try {
                        int score = Integer.parseInt(line.substring(10).trim());
                        promptAndSaveScore(score);
                    } catch (NumberFormatException e) {
                        System.err.println("Failed to parse score: " + line);
                    }
                }
            }
            p.waitFor();
        } catch (Exception ex) {
            JOptionPane.showMessageDialog(this, "Error launching game: " + ex.getMessage(), "Error", JOptionPane.ERROR_MESSAGE);
        } finally {
            this.setVisible(true);
        }
    }

    private void promptAndSaveScore(int score) {
        String name = JOptionPane.showInputDialog(this, "Game Over! Your Score is: " + score + "\nEnter your name for the Leaderboard:");
        if (name != null && !name.trim().isEmpty()) {
            try (FileWriter fw = new FileWriter(LEADERBOARD_FILE, true);
                 BufferedWriter bw = new BufferedWriter(fw);
                 PrintWriter out = new PrintWriter(bw)) {
                out.println(name.trim() + "," + score);
                JOptionPane.showMessageDialog(this, "Score saved successfully!");
            } catch (IOException ex) {
                JOptionPane.showMessageDialog(this, "Error saving score.", "Error", JOptionPane.ERROR_MESSAGE);
            }
        }
    }

    private void showLeaderboard() {
        File file = new File(LEADERBOARD_FILE);
        if (!file.exists()) {
            JOptionPane.showMessageDialog(this, "Leaderboard is empty!");
            return;
        }

        try (BufferedReader br = new BufferedReader(new FileReader(file))) {
            ArrayList<String[]> scores = new ArrayList<>();
            String line;
            while ((line = br.readLine()) != null) {
                String[] parts = line.split(",");
                if (parts.length == 2) {
                    scores.add(parts);
                }
            }

            scores.sort((a, b) -> Integer.compare(Integer.parseInt(b[1]), Integer.parseInt(a[1])));

            StringBuilder sb = new StringBuilder("--- LEADERBOARD ---\n\n");
            for (int i = 0; i < scores.size() && i < 10; i++) {
                sb.append(String.format("%2d. %-15s - %d pts\n", i + 1, scores.get(i)[0], Integer.parseInt(scores.get(i)[1])));
            }

            JTextArea textArea = new JTextArea(sb.toString());
            textArea.setEditable(false);
            textArea.setFont(pvzFont.deriveFont(16f));
            textArea.setBackground(new Color(25, 20, 15));
            textArea.setForeground(Color.YELLOW);
            textArea.setBorder(BorderFactory.createEmptyBorder(10, 10, 10, 10));
            
            JScrollPane scrollPane = new JScrollPane(textArea);
            scrollPane.setBorder(BorderFactory.createLineBorder(new Color(218, 165, 32), 2));
            JOptionPane.showMessageDialog(this, scrollPane, "Leaderboard", JOptionPane.PLAIN_MESSAGE);

        } catch (Exception ex) {
            JOptionPane.showMessageDialog(this, "Error reading leaderboard.", "Error", JOptionPane.ERROR_MESSAGE);
        }
    }

    public static void main(String[] args) {
        SwingUtilities.invokeLater(() -> {
            new GameLauncher().setVisible(true);
        });
    }
}
