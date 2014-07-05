package xTesting.stats;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;

import xTesting.Env.Mode;


public class Stats {
	
	private static Connection conn;
	private static ResultSet result = null;
	private static Statement stmt = null;
	
	
	public static final Stats getFreshStats() {
		try {
			if(conn == null) {
				Class.forName("org.sqlite.JDBC");
				//FIXME wert in Env eintragen?
				conn = DriverManager.getConnection("jdbc:sqlite:testresults.sqlite");
				stmt = conn.createStatement();
			}
		} catch(Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		return new Stats();
	}
	
	
	
	private long runId;
	
	
	
	private Stats() {
		try {
			// create tables if neeeded
			stmt.executeUpdate("CREATE TABLE IF NOT EXISTS run ("
					+ "id INTEGER PRIMARY KEY AUTOINCREMENT,"
					+ "date DATETIME"
				+ ")"
			);
			stmt.executeUpdate("CREATE TABLE IF NOT EXISTS testfile ("
					+ "id INTEGER PRIMARY KEY AUTOINCREMENT,"
					+ "path TEXT UNIQUE ON CONFLICT IGNORE"
				+ ")"
			);
			stmt.executeUpdate("CREATE TABLE IF NOT EXISTS result (" +
					"id INTEGER PRIMARY KEY AUTOINCREMENT," +
					"idRun INTEGER," +
					"idTestfile INTEGER," +
					"idTest INTEGER," +
					"type INTEGER," +							// (cpp_cpp, ...)
					"blame INTEGER," +						// BitSet: like blame: 0 = success, ...
					"msg TEXT," +
					"stdOut TEXT," + 
					"stdErr TEXT," +
					"durationMs LONG" +
				")"
			);
			
			// create TestRun
			
			stmt.executeUpdate("INSERT INTO run (date) VALUES(datetime('now', 'localtime'));");
			result = stmt.getGeneratedKeys();
			runId = result.getLong(1);
			
		} catch (SQLException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
	}
	
	
	
	public long getTestfileId(String filepath) throws SQLException {
		stmt.executeUpdate("INSERT INTO testfile (path) VALUES('"+ filepath +"');");
		result = stmt.executeQuery("SELECT id FROM testfile WHERE path='"+filepath+"';");
		return result.getLong(1);
	}
	
	
	
	
	public void writeTestResult(long testFileId, Mode mode, int blame, int testId, String msg, String stdOut, String stdErr, long durationMs) {
		int type = 0;
		switch (mode) {
			case Cpp:				type = 1;	break;
			case Cpp_Cpp:			type = 2;	break;
			case Cpp_Haskell:		type = 3;	break;
			case Haskell:			type = 4;	break;
			case Haskell_Cpp:		type = 5;	break;
			case Haskell_Haskell:	type = 6;	break;
			case Interpreter:		type = 7;	break;
			default: return;
		}
		
		
		try {
			PreparedStatement s = conn.prepareStatement("INSERT INTO result (idRun, idTestfile, idTest, type, blame, msg, stdOut, stdErr, durationMs) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?);");
			s.setLong	(1, this.runId);
			s.setLong	(2, testFileId);
			s.setInt	(3, type);
			s.setInt	(4, blame);
			s.setInt	(5, testId);
			s.setString	(6, msg);
			s.setString	(7, stdOut);
			s.setString	(8, stdErr);
			s.setLong	(9, durationMs);
			s.executeUpdate();
			
			//FIXME stdIn raus, dafür noch test-id rein. Compile = 0
			//stmt.executeUpdate("INSERT INTO result (idRun, idTestfile, type, blame, testId, msg, stdOut, stdErr) VALUES ("+ this.runId +", "+testFileId+", "+type+", "+blame+", "+testId+", '"+msg+"','"+stdOut+"', '"+stdErr+"');");
			
			
		} catch (SQLException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
	
	
	
}
