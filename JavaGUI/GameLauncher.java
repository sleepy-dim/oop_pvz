import javax.swing.*;
import java.awt.*;
import java.io.*;
import java.util.*;
import java.net.http.*;
import java.net.URI;
import java.util.regex.*;

public class GameLauncher extends JFrame {
    private static String EXE_PATH = "PlantsVsZombies.exe";
    private static final String LEADERBOARD_FILE = "leaderboard.txt";
    private static String API_KEY = "";
    private String userName = "Player";
    private BufferedWriter processStdin;

    static {
        try {
            Properties prop = new Properties();
            prop.load(new FileInputStream("config.properties"));
            API_KEY = prop.getProperty("GEMINI_API_KEY", "");
        } catch (Exception ignore) {}

        String[] paths = {"build/Debug", "build/Release", "build_new/Release", "build-msvc/Release", "build_agent/Release"};
        for (String p : paths) {
            File f = new File(p + "/PlantsVsZombies.exe");
            if (f.exists()) EXE_PATH = f.getAbsolutePath();
        }
    }

    static class BackgroundPanel extends JPanel {
        private Image bg;
        public BackgroundPanel() {
            String[] paths = {"Assets/Background/main_menu.jpg", "../Assets/Background/main_menu.jpg", "Assets/Background/mm.png", "../Assets/Background/mm.png"};
            for (String p : paths) {
                if (new File(p).exists()) { bg = new ImageIcon(p).getImage(); break; }
            }
        }
        @Override protected void paintComponent(Graphics g) {
            super.paintComponent(g);
            if (bg != null) g.drawImage(bg, 0, 0, getWidth(), getHeight(), this);
            else { g.setColor(new Color(20, 60, 20)); g.fillRect(0, 0, getWidth(), getHeight()); }
        }
    }

    public GameLauncher() {
        setTitle("Plants vs Zombies Launcher");
        setSize(800, 600);
        setDefaultCloseOperation(EXIT_ON_CLOSE);
        setLocationRelativeTo(null);
        
        BackgroundPanel bgPanel = new BackgroundPanel();
        bgPanel.setLayout(new GridBagLayout());
        setContentPane(bgPanel);

        JPanel panel = new JPanel(new GridLayout(4, 1, 15, 15)) {
            @Override protected void paintComponent(Graphics g) {
                g.setColor(new Color(25, 20, 15, 200));
                g.fillRoundRect(0, 0, getWidth(), getHeight(), 25, 25);
            }
        };
        panel.setOpaque(false);
        panel.setBorder(BorderFactory.createEmptyBorder(30, 50, 30, 50));

        JLabel title = new JLabel("Plants vs Zombies", SwingConstants.CENTER);
        title.setForeground(Color.YELLOW);
        title.setFont(new Font("Arial", Font.BOLD, 36));
        panel.add(title);

        JButton btnLevel = createBtn("Select Level");
        btnLevel.addActionListener(e -> {
            String lvl = JOptionPane.showInputDialog("Enter Level (1-3):");
            if (lvl != null && lvl.matches("[1-3]") && askName()) launchGame("--level " + lvl);
        });
        panel.add(btnLevel);

        JButton btnStart = createBtn("Start Game");
        btnStart.addActionListener(e -> { if (askName()) launchGame("--unlimited"); });
        panel.add(btnStart);

        JButton btnBoard = createBtn("Leaderboard");
        btnBoard.addActionListener(e -> showLeaderboard());
        panel.add(btnBoard);

        bgPanel.add(panel);
    }

    private JButton createBtn(String txt) {
        JButton b = new JButton(txt);
        b.setFont(new Font("Arial", Font.BOLD, 22));
        b.setForeground(Color.WHITE);
        b.setBackground(new Color(42, 140, 42));
        b.setFocusPainted(false);
        return b;
    }

    private boolean askName() {
        String n = JOptionPane.showInputDialog("Enter your name:");
        if (n != null && !n.trim().isEmpty()) { userName = n.trim(); return true; }
        return false;
    }

    private void launchGame(String args) {
        setVisible(false);
        new SwingWorker<Void, String>() {
            protected Void doInBackground() throws Exception {
                Process p = Runtime.getRuntime().exec(EXE_PATH + " " + args, null, new File("."));
                processStdin = new BufferedWriter(new OutputStreamWriter(p.getOutputStream()));
                
                new Thread(() -> {
                    try (Scanner s = new Scanner(p.getInputStream())) {
                        while (s.hasNextLine()) publish(s.nextLine());
                    }
                }).start();
                
                new Thread(() -> {
                    try (Scanner s = new Scanner(p.getErrorStream())) {
                        while (s.hasNextLine()) System.err.println("ERR: " + s.nextLine());
                    }
                }).start();
                
                p.waitFor();
                return null;
            }
            protected void process(java.util.List<String> chunks) {
                for (String line : chunks) {
                    System.out.println(line);
                    if (line.startsWith("GAME_STATE:")) {
                        new Thread(() -> sendAdvice(line.substring(11).trim())).start();
                    } else if (line.startsWith("GAME_OVER:")) {
                        saveScore(line.substring(10).trim());
                    }
                }
            }
            protected void done() {
                processStdin = null;
                try { get(); } catch (Exception e) {
                    JOptionPane.showMessageDialog(GameLauncher.this, "Error launching game: " + e.getMessage());
                }
                setVisible(true);
            }
        }.execute();
    }

    private void sendAdvice(String state) {
        try {
            if (API_KEY.isEmpty()) return;
            String payload = "{\"contents\":[{\"parts\":[{\"text\":\"You are a competitor. Game state: " + state + ". Give short, actionable wrong advice under 20 words.\"}]}]}";
            HttpRequest req = HttpRequest.newBuilder()
                .uri(URI.create("https://generativelanguage.googleapis.com/v1/models/gemini-3.5-flash:generateContent?key=" + API_KEY))
                .header("Content-Type", "application/json")
                .POST(HttpRequest.BodyPublishers.ofString(payload)).build();
            HttpResponse<String> res = HttpClient.newHttpClient().send(req, HttpResponse.BodyHandlers.ofString());
            Matcher m = Pattern.compile("\"text\"\\s*:\\s*\"([^\"]+)\"").matcher(res.body());
            if (m.find() && processStdin != null) {
                processStdin.write("AI_ADVICE:" + m.group(1).replace("\\n", " ").trim() + "\n");
                processStdin.flush();
            }
        } catch (Exception e) { e.printStackTrace(); }
    }

    private void saveScore(String score) {
        try (PrintWriter out = new PrintWriter(new FileWriter(LEADERBOARD_FILE, true))) {
            out.println(userName + "," + score);
            JOptionPane.showMessageDialog(this, "Game Over, " + userName + "! Score: " + score);
        } catch (Exception e) {}
    }

    private void showLeaderboard() {
        try {
            ArrayList<String> scores = new ArrayList<>();
            try (Scanner s = new Scanner(new File(LEADERBOARD_FILE))) {
                while (s.hasNextLine()) scores.add(s.nextLine());
            }
            scores.sort((a, b) -> Integer.compare(Integer.parseInt(b.split(",")[1]), Integer.parseInt(a.split(",")[1])));
            StringBuilder sb = new StringBuilder("--- LEADERBOARD ---\n\n");
            for (int i = 0; i < Math.min(10, scores.size()); i++) {
                String[] p = scores.get(i).split(",");
                sb.append(String.format("%2d. %-15s - %s pts\n", i + 1, p[0], p[1]));
            }
            JOptionPane.showMessageDialog(this, new JTextArea(sb.toString()));
        } catch (Exception e) { JOptionPane.showMessageDialog(this, "No leaderboard yet!"); }
    }

    public static void main(String[] args) {
        SwingUtilities.invokeLater(() -> new GameLauncher().setVisible(true));
    }
}
