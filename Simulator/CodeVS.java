import javax.swing.*;
import java.awt.*;
import java.awt.event.KeyAdapter;
import java.awt.event.KeyEvent;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.io.BufferedReader;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.security.SecureRandom;
import java.util.*;
import java.util.List;

class Drawer extends JFrame {
    public static final int EXTRA_WIDTH = 300;
    public static final int EXTRA_HEIGHT = 100;

    public Field field;
    public Game game;
    public DrawerPanel panel;

    public int cellSize, boardSize;
    public int width, height;

    public boolean pauseMode = false;

    class DrawerKeyListener extends KeyAdapter {
        public void keyPressed(KeyEvent e) {
            synchronized (keyMutex) {
                if (e.getKeyChar() == ' ') { // space key
                    pauseMode = !pauseMode;
                }
                keyPressed = true;
                keyMutex.notifyAll();
            }
        }
    }


    class DrawerPanel extends JPanel {
        private static final int MARGIN = 15;
        public void paint(Graphics g) {
        	// field empty cells
            g.setColor(Color.DARK_GRAY);
            g.fillRect(MARGIN, MARGIN, cellSize * boardSize + 1, cellSize * boardSize + 1);
            g.setColor(Color.RED);
            for (int i = 0; i <= boardSize; i++) {
                g.drawLine(MARGIN + i * cellSize, MARGIN, MARGIN + i * cellSize, MARGIN + cellSize * boardSize);
                g.drawLine(MARGIN, MARGIN + i * cellSize, MARGIN + cellSize * boardSize, MARGIN + i * cellSize);
            }

            // snow
            g.setColor(Color.WHITE);
            for (int i=0; i < boardSize; i++) {
                for (int j=0; j < boardSize; j++) {
                    if (field.haveSnow[i][j]) {
                        g.fillRect(15 + j * cellSize + 1, 15 + i * cellSize + 1, cellSize - 2, cellSize - 2);
                    }
                }
            }

            for (Unit unit : field.units) {
                switch (unit.type) {
                    case Worker: g.setColor(Color.BLUE); break;
                    case Knight: g.setColor(Color.BLUE); break;
                    case Fighter: g.setColor(Color.RED); break;
                    case Assassin: g.setColor(Color.YELLOW); break;
                    case Castle: g.setColor(Color.GREEN); break;
                    case Village: g.setColor(Color.ORANGE); break;
                    case Base: g.setColor(Color.PINK); break;
                    default: g.setColor(Color.WHITE);
                }
                g.fillRect(MARGIN + unit.x * cellSize + 1, MARGIN + unit.y * cellSize + 1, cellSize - 2, cellSize - 2);
            }

            // texts
            g.setColor(Color.BLACK);
            g.setFont(new Font("Arial", Font.BOLD, 12));
            Graphics2D g2 = (Graphics2D)g;

            int horPos = 40 + boardSize * cellSize;

//            g2.drawString("Board size = " + field.haveSnow.length, horPos, 30);
            g2.drawString("Turn: " + game.getTurn(), horPos, 30);
/*            synchronized (field.workersLock) {
                g2.drawString("Workers = " + field.workers.size(), horPos, 145);
            }*/
        }
    }

    class DrawerWindowListener extends WindowAdapter {
        public void windowClosing(WindowEvent event) {
//            Sample.stopSolution();
            System.exit(0);
        }
    }

    final Object keyMutex = new Object();
    boolean keyPressed;

    public void processPause() {
        synchronized (keyMutex) {
            if (!pauseMode) {
                return;
            }
            keyPressed = false;
            while (!keyPressed) {
                try {
                    keyMutex.wait();
                } catch (InterruptedException e) {
                    // do nothing
                }
            }
        }
    }

    public Drawer(Field field, Game game, int cellSize) {
        super();

        panel = new DrawerPanel();
        getContentPane().add(panel);

        addWindowListener(new DrawerWindowListener());

        this.field = field;
        this.game = game;

        boardSize = field.haveSnow.length;
        this.cellSize = cellSize;
        width = cellSize * boardSize + EXTRA_WIDTH;
        height = cellSize * boardSize + EXTRA_HEIGHT;

        addKeyListener(new DrawerKeyListener());

        setSize(width, height);
        setTitle("Visualizer tool for problem SnowCleaning");

        setResizable(false);
        setVisible(true);
    }
}

class Field {
//    final Object workersLock = new Object();

//    int snowCnt;
    boolean[][] haveSnow;
    List<Unit> units = new ArrayList<Unit>();

//    List<Cell> workers = new ArrayList<Cell>();
    Set<Integer> usedWorkers = new HashSet<Integer>();

//    int salary, fine;
//    int totSalary, totFine;
//    int curDay = -1;

    public Field(int boardSize) {
        haveSnow = new boolean[boardSize][boardSize];

        setUnits();

    }

    public void setUnits() {
        //Randomクラスのインスタンス化
        units.clear();
        Random rnd = new Random();
        for (int i = 0; i < 200; i++) {
            UnitType unitType = UnitType.fromId(rnd.nextInt(UnitType.values().length));
            units.add(new Unit(rnd.nextInt(100), rnd.nextInt(100), unitType));
        }
    }

}

enum UnitType {
    Worker(0),
    Knight(1),
    Fighter(2),
    Assassin(3),
    Castle(4),
    Village(5),
    Base(6);

    private int id;
    UnitType(int typeId) {
        this.id = typeId;
    }
    public static UnitType fromId(int typeId) {
        UnitType[] unitTypes = UnitType.values();
        for (UnitType unitType : unitTypes) {
            if (typeId == unitType.id) {
                return unitType;
            }
        }
        return null;
    }
}

class Unit {
    public int x, y;
    public UnitType type;
    public Unit(int x, int y, UnitType type) {
        this.x = x;
        this.y = y;
        this.type = type;
    }
/*    public int hashCode() {
        return 100 * r + c;
    }*/
}

class Game {
    public Game() {

    }
    private static int turn = 0;
    public int getTurn() {
        return this.turn;
    }
    public void nextTurn() {
        turn++;
    }
}


public class CodeVS {
    private static int turn = 0;

    public static int delay = 100;
	public static void main(String args[]) {

		Field field = new Field(100);
        Game game = new Game();
		Drawer drawer = new Drawer(field, game, 7);

        for (int i = 0; i < 100; i++) {
            game.nextTurn();
            field.setUnits();
            drawer.processPause();
            drawer.repaint();
            try {
                Thread.sleep(delay);
            } catch (Exception e) {
                // do nothing
            }
        }

	}
}