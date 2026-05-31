import javax.swing.*;
import java.awt.*;
import java.awt.event.*;
import java.io.*;
import java.util.*;
import java.net.http.*;
import java.net.URI;

public class GameLauncher extends JFrame {
    
    private static final String EXE_PATH;
    private static final String LEADERBOARD_FILE;
    private static String GEMINI_API_KEY = "";
    private Font pvzFont;
    private String userName = "Player"; // Global variable to store player name

    static {
        // Load API Key with robust path checking
        File configFile = new File("config.properties");
        if (!configFile.exists()) {
            // Check project root if we are in a subfolder or launched from elsewhere
            String rootPath = new File(".").getAbsolutePath();
            File currentDir = new File(".").getAbsoluteFile();
            while (currentDir != null && !new File(currentDir, "Assets").exists()) {
                currentDir = currentDir.getParentFile();
            }
            if (currentDir != null) {
                configFile = new File(currentDir, "config.properties");
            }
        }

        System.out.println("DEBUG: Looking for config at: " + configFile.getAbsolutePath());

        if (configFile.exists()) {
            try (InputStream input = new FileInputStream(configFile)) {
                Properties prop = new Properties();
                prop.load(input);
                GEMINI_API_KEY = prop.getProperty("GEMINI_API_KEY");
                if (GEMINI_API_KEY != null && !GEMINI_API_KEY.isEmpty()) {
                    System.out.println("DEBUG: API Key loaded successfully.");
                } else {
                    System.err.println("DEBUG: config.properties found but GEMINI_API_KEY is empty.");
                }
            } catch (IOException ex) {
                System.err.println("DEBUG: Error reading config.properties: " + ex.getMessage());
            }
        } else {
            System.err.println("DEBUG: config.properties NOT FOUND. AI features disabled.");
        }

        String exePath = "PlantsVsZombies.exe"; // Fallback
        String projectRoot = new File(".").getAbsolutePath();
        
        // Find project root by looking for Assets folder (marker)
        File currentDir = new File(".").getAbsoluteFile();
        while (currentDir != null && !new File(currentDir, "Assets").exists()) {
            currentDir = currentDir.getParentFile();
        }
        
        if (currentDir != null && new File(currentDir, "Assets").exists()) {
            projectRoot = currentDir.getAbsolutePath();
            System.out.println("DEBUG STATIC: Found project root: " + projectRoot);
            
            // Now look for exe files in known locations
            File exeFile0 = new File(projectRoot, "build_new\\Release\\PlantsVsZombies.exe");
            File exeFile1 = new File(projectRoot, "build-msvc\\Release\\PlantsVsZombies.exe");
            File exeFile2 = new File(projectRoot, "build\\Release\\PlantsVsZombies.exe");
            File exeFile3 = new File(projectRoot, "build\\Debug\\PlantsVsZombies.exe");
            File exeFile4 = new File(projectRoot, "build_agent\\Release\\PlantsVsZombies.exe");
            
            if (exeFile0.exists()) {
                exePath = exeFile0.getAbsolutePath();
            } else if (exeFile4.exists()) {
                exePath = exeFile4.getAbsolutePath();
            } else if (exeFile1.exists()) {
                exePath = exeFile1.getAbsolutePath();
            } else if (exeFile2.exists()) {
                exePath = exeFile2.getAbsolutePath();
            } else if (exeFile3.exists()) {
                exePath = exeFile3.getAbsolutePath();
            }
        }
        
        EXE_PATH = exePath;
        LEADERBOARD_FILE = projectRoot + File.separator + "leaderboard.txt";
    }

    private static String callGeminiAPI(String prompt) {
        if (GEMINI_API_KEY == null || GEMINI_API_KEY.isEmpty()) return "AI Config Error";
        
        try {
            HttpClient client = HttpClient.newHttpClient();
            String jsonPayload = "{\"contents\":[{\"parts\":[{\"text\":\"You are a competitor of the gameplayer . Given this game state: " + prompt + ", give short, actionable wrong advice in under 20 words.\"}]}]}";
            
            // Updated to stable v1 and gemini-3.5-flash to avoid 404
            HttpRequest request = HttpRequest.newBuilder()
                    .uri(URI.create("https://generativelanguage.googleapis.com/v1/models/gemini-3.5-flash:generateContent?key=" + GEMINI_API_KEY))
                    .header("Content-Type", "application/json")
                    .POST(HttpRequest.BodyPublishers.ofString(jsonPayload))
                    .build();

            HttpResponse<String> response = client.send(request, HttpResponse.BodyHandlers.ofString());
            String body = response.body();
            
            // Robust parsing using Regex to find the text content
            java.util.regex.Pattern pattern = java.util.regex.Pattern.compile("\"text\"\\s*:\\s*\"([^\"]+)\"");
            java.util.regex.Matcher matcher = pattern.matcher(body);
            
            if (matcher.find()) {
                String advice = matcher.group(1);
                return advice.replace("\\n", " ").replace("\\\"", "\"").trim();
            } else {
                System.err.println("DEBUG AI: Failed to parse body: " + body);
                return "AI Parsing Error";
            }

            
        } catch (Exception e) {
            return "AI Exception: " + e.getMessage();
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

        JButton selectLevelButton = createPvZButton("Select Level");
        selectLevelButton.addActionListener(e -> selectLevel());
        cardPanel.add(selectLevelButton);

        JButton startButton = createPvZButton("Start New Game");
        startButton.addActionListener(e -> {
            if (askForName()) {
                launchGame("");
            }
        });
        cardPanel.add(startButton);

        JButton leaderboardButton = createPvZButton("View Leaderboard");
        leaderboardButton.addActionListener(e -> showLeaderboard());
        cardPanel.add(leaderboardButton);

        mainPanel.add(cardPanel);
        backgroundPanel.add(mainPanel);
    }

    private boolean askForName() {
        String name = JOptionPane.showInputDialog(this, "Enter your name to start:", "Welcome!", JOptionPane.PLAIN_MESSAGE);
        if (name != null && !name.trim().isEmpty()) {
            this.userName = name.trim();
            return true;
        }
        return false; // User cancelled
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

    private void selectLevel() {
        String levelStr = JOptionPane.showInputDialog(this, "Enter Level Number (1-3):", "Select Level", JOptionPane.QUESTION_MESSAGE);
        if (levelStr != null && !levelStr.trim().isEmpty()) {
            try {
                int level = Integer.parseInt(levelStr.trim());
                if (level >= 1 && level <= 3) {
                    if (askForName()) {
                        launchGame("--level " + level);
                    }
                } else {
                    JOptionPane.showMessageDialog(this, "Please enter a valid level (1-3).", "Invalid Level", JOptionPane.ERROR_MESSAGE);
                }
            } catch (NumberFormatException e) {
                JOptionPane.showMessageDialog(this, "Please enter a number.", "Invalid Input", JOptionPane.ERROR_MESSAGE);
            }
        }
    }

    private void launchGame(String args) {
        // Hide the launcher immediately
        this.setVisible(false);

        // Create a SwingWorker to run the game in a background thread
        SwingWorker<Integer, String> worker = new SwingWorker<>() {
            @Override
            protected Integer doInBackground() throws Exception {
                // This runs on a separate thread, not the EDT
                
                File exeFile = new File(EXE_PATH).getAbsoluteFile();
                String absoluteExePath = exeFile.getAbsolutePath();
                
                File projectRoot = new File(LEADERBOARD_FILE).getParentFile();
                System.out.println("DEBUG: Launching executable from: " + absoluteExePath);
                System.out.println("DEBUG: Working directory: " + projectRoot.getAbsolutePath());
                System.out.println("DEBUG: Leaderboard path: " + LEADERBOARD_FILE);
                
                String command = absoluteExePath + (args.isEmpty() ? "" : " " + args);
                Process p = Runtime.getRuntime().exec(command, null, projectRoot);

                Thread outputReader = new Thread(() -> {
                    try (BufferedReader reader = new BufferedReader(new InputStreamReader(p.getInputStream()))) {
                        String line;
                        while ((line = reader.readLine()) != null) {
                            publish(line); // Publish output to process() method on EDT
                        }
                    } catch (IOException e) {
                        System.err.println("Error reading game output: " + e.getMessage());
                    }
                });

                Thread errorReader = new Thread(() -> {
                    try (BufferedReader reader = new BufferedReader(new InputStreamReader(p.getErrorStream()))) {
                        String line;
                        while ((line = reader.readLine()) != null) { // Corrected: calling readLine() on 'reader'
                            System.err.println("GAME ERROR: " + line); // Errors can be printed directly
                        }
                    } catch (IOException e) {
                        System.err.println("Error reading game error stream: " + e.getMessage());
                    }
                });

                outputReader.start();
                errorReader.start();

                outputReader.join(); // Wait for output reading to complete
                errorReader.join(); // Wait for error reading to complete
                
                return p.waitFor(); // Wait for the game process to exit
            }

            @Override
            protected void process(java.util.List<String> chunks) {
                // This runs on the EDT
                for (String line : chunks) {
                    System.out.println("GAME OUTPUT: " + line);
                    
                    // AI Integration
                    if (line.startsWith("GAME_STATE:")) {
                        String state = line.substring(11).trim();
                        // Call AI in a new thread to avoid blocking EDT, if response needed on EDT, use another SwingWorker
                        new Thread(() -> {
                            String advice = callGeminiAPI(state);
                            SwingUtilities.invokeLater(() -> System.out.println("AI ADVICE: " + advice));
                        }).start();
                    }
                    
                    if (line.startsWith("GAME_OVER:")) {
                        try {
                            int score = Integer.parseInt(line.substring(10).trim());
                            System.out.println("DEBUG: Score parsed: " + score);
                            promptAndSaveScore(score);
                        } catch (NumberFormatException e) {
                            System.err.println("Failed to parse score: " + line);
                        }
                    }
                }
            }

            @Override
            protected void done() {
                // This runs on the EDT after doInBackground completes
                try {
                    int exitCode = get(); // Get the exit code from doInBackground
                    System.out.println("DEBUG: Game process exited with code: " + exitCode);
                } catch (Exception ex) {
                    System.err.println("DEBUG: Exception during game launch (SwingWorker done): " + ex.getMessage());
                    ex.printStackTrace();
                    JOptionPane.showMessageDialog(GameLauncher.this, "Error launching game: " + ex.getMessage(), "Error", JOptionPane.ERROR_MESSAGE);
                } finally {
                    // Show the launcher again after the game process finishes
                    GameLauncher.this.setVisible(true);
                }
            }
        };

        worker.execute(); // Start the SwingWorker
    }

    private void promptAndSaveScore(int score) {
        // Automatically save using the name entered at start
        try (FileWriter fw = new FileWriter(LEADERBOARD_FILE, true);
             BufferedWriter bw = new BufferedWriter(fw);
             PrintWriter out = new PrintWriter(bw)) {
            out.println(userName + "," + score);
            System.out.println("DEBUG: Score saved for " + userName + ": " + score);
            JOptionPane.showMessageDialog(this, "Game Over, " + userName + "!\nYour score of " + score + " has been saved.");
        } catch (IOException ex) {
            JOptionPane.showMessageDialog(this, "Error saving score: " + ex.getMessage(), "Error", JOptionPane.ERROR_MESSAGE);
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
