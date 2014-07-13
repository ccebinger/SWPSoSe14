package xTesting.stats;


import java.sql.SQLException;

import xTesting.Env;
import xTesting.Env.Mode;


public class Stats {
	
	public static final Stats factory() {
		if(Env.useDatabase()) {
			return new StatsDatabase();
		}
		else {
			return new Stats();
		}
	}
	
	// Stubs - don't store results
	Stats() {}
	public long getTestfileId(String filepath) throws SQLException { return 1; }
	public void writeTestResult(long testFileId, Mode mode, int blame, int testcaseId, String msg, String stdOut, String stdErr, long durationMs) {}
	
}
