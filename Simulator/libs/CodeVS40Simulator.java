import java.util.*;
import java.io.BufferedReader;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.PrintWriter;


interface Visualizer {
	public abstract int runTest();
}

public class CodeVS40Simulator implements Visualizer {
	public static String execCommand = null;
	public static Process solution;

	public static void main(String[] args) {
		for (int i = 0; i < args.length; i++) {
			if (args[i].equals("-exec")) {
				execCommand = args[++i];
			} else {
				System.out.println("WARNING: unknown argument " + args[i] + ".");
			}
		}

		if (execCommand == null) {
			System.err.println("ERROR: You did not provide the command to exec your solution."
				+ " Please use -exec <command> for this.");
			System.exit(1);
		}

		CodeVS40Simulator simulator = new CodeVS40Simulator();
		try {
			int score = simulator.runTest();
			System.out.println("Score = " + score);
		} catch (RuntimeException e) {
			System.err.println("ERROR: Unexpected error while running your simulation.");
			e.printStackTrace();
			CodeVS40Simulator.stopSolution();
		}

		System.out.println("hogehoge");
	}

	public int runTest() {
		solution = null;

		try {
			solution = Runtime.getRuntime().exec(execCommand);
		} catch (Exception e) {
			System.err.println("ERROR: Unable to execute your solution using the provided command: "
				+ execCommand + ".");
			return -1;
		}

    BufferedReader reader = new BufferedReader(new InputStreamReader(solution.getInputStream()));
    PrintWriter writer = new PrintWriter(solution.getOutputStream());
    new ErrorStreamRedirector(solution.getErrorStream()).start();

    int boardSize = 10;
    int salary = 10;
    int snowFine = 10;
    writer.println(boardSize);
    writer.println(salary);
    writer.println(snowFine);
    writer.flush();

    for (int i = 0; i < 2000; i++) {
    	System.out.println(i);

    	writer.println(1);
    	writer.println(1);
    	writer.flush();

    	try {
	    	int cnt = Integer.parseInt(reader.readLine());
	    	System.out.println("cnt: " + cnt);
	    	for (int j = 0; j < cnt; j++) {
		    		int temp = Integer.parseInt(reader.readLine());
	    	}
    	} catch (Exception e) {
    		e.printStackTrace();
    	}
  }

    // Visualizing...

    stopSolution();
    System.out.println("Finish");


		return 0;
	}

	public static void stopSolution() {
		if (solution != null) {
			try {
				solution.destroy();
			} catch (Exception e) {
				// do nothing
			}
		}
	}

}