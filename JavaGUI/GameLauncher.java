package JavaGUI;

import javax.swing.*;
import java.awt.*;
import java.awt.event.*;
import java.io.*;
import java.util.*;

public class GameLauncher extends JFrame {
    
    private static final String EXE_PATH;
    private static final String LEADERBOARD_FILE = "leaderboard.txt";

    static {
        if (new File("build\\Debug\\PlantsVsZombies.exe").exists()) {
            EXE_PATH = "build\\Debug\\PlantsVsZombies.exe";
        } else if (new File("..\\build\\Debug\\PlantsVsZombies.exe").exists()) {
            EXE_PATH = "..\\build\\Debug\\PlantsVsZombies.exe";
        } else {
            EXE_PATH = "PlantsVsZombies.exe"; // Fallback
        }
    }

    public GameLauncher() {
        setTitle("Plants vs Zombies - Launcher");
        setSize(400, 300);
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setLocationRelativeTo(null);
        setLayout(new GridLayout(4, 1, 10, 10));

        JLabel titleLabel = new JLabel("Plants vs Zombies", SwingConstants.CENTER);
        titleLabel.setFont(new Font("Arial", Font.BOLD, 24));
        add(titleLabel);

        JButton startButton = new JButton("Start New Game");
        startButton.setFont(new Font("Arial", Font.PLAIN, 18));
        startButton.addActionListener(e -> launchGame(""));
        add(startButton);

        JButton loadButton = new JButton("Load Saved Game");
        loadButton.setFont(new Font("Arial", Font.PLAIN, 18));
        loadButton.addActionListener(e -> launchGame("--load"));
        add(loadButton);

        JButton leaderboardButton = new JButton("View Leaderboard");
        leaderboardButton.setFont(new Font("Arial", Font.PLAIN, 18));
        leaderboardButton.addActionListener(e -> showLeaderboard());
        add(leaderboardButton);
    }

    private void launchGame(String args) {
        try {
            // Setup the command
            String command = EXE_PATH + (args.isEmpty() ? "" : " " + args);
            File workingDir = new File(EXE_PATH).getAbsoluteFile().getParentFile();
            Process p = Runtime.getRuntime().exec(command, null, workingDir);

            // Read the output from the C++ executable
            BufferedReader reader = new BufferedReader(new InputStreamReader(p.getInputStream()));
            String line;
            while ((line = reader.readLine()) != null) {
                System.out.println(line); // Print C++ output to console
                
                // If C++ sends the game over score signal
                if (line.startsWith("GAME_OVER:")) {
                    try {
                        int score = Integer.parseInt(line.substring(10).trim());
                        promptAndSaveScore(score);
                    } catch (NumberFormatException e) {
                        System.err.println("Failed to parse score: " + line);
                    }
                }
            }
            
            p.waitFor(); // Wait for game to close completely
            
        } catch (Exception ex) {
            JOptionPane.showMessageDialog(this, "Error launching game: " + ex.getMessage(), "Error", JOptionPane.ERROR_MESSAGE);
        }
    }

    private void promptAndSaveScore(int score) {
        String name = JOptionPane.showInputDialog(this, "Game Over! Your Score is: " + score + "\nEnter your name for the Leaderboard:");
        if (name != null && !name.trim().isEmpty()) {
            try (FileWriter fw = new FileWriter(LEADERBOARD_FILE, true);
                 BufferedWriter bw = new BufferedWriter(fw);
                 PrintWriter out = new PrintWriter(bw)) {
                out.println(name.trim() + "," + score);
                JOptionPane.showMessageDialog(this, "Score saved!");
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

            // Sort by score descending
            scores.sort((a, b) -> Integer.compare(Integer.parseInt(b[1]), Integer.parseInt(a[1])));

            StringBuilder sb = new StringBuilder("--- LEADERBOARD ---\n\n");
            for (int i = 0; i < scores.size(); i++) {
                sb.append(i + 1).append(". ").append(scores.get(i)[0]).append(" - ").append(scores.get(i)[1]).append(" pts\n");
            }

            JTextArea textArea = new JTextArea(sb.toString());
            textArea.setEditable(false);
            textArea.setFont(new Font("Monospaced", Font.PLAIN, 14));
            JOptionPane.showMessageDialog(this, new JScrollPane(textArea), "Leaderboard", JOptionPane.INFORMATION_MESSAGE);

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
