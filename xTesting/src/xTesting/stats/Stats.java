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
				Class.forName("com.mysql.jdbc.Driver");
				//FIXME wert in Env eintragen?
				conn = DriverManager.getConnection("jdbc:mysql://85.116.219.166/xtStats?user=xtStats&password=CDAX3gayybZ8pseKHJSTwXe");
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
			stmt.executeUpdate("CREATE TABLE IF NOT EXISTS `run` ("
					+ "`idRun` INT(11) UNSIGNED NOT NULL AUTO_INCREMENT,"
					+ "`date` TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,"
					+ "PRIMARY KEY (`idRun`)"
				+ ") COLLATE='utf8_unicode_ci' ENGINE=InnoDB;"
			);
			
			
			stmt.executeUpdate("CREATE TABLE IF NOT EXISTS `testfile` ("
					+ "`idTestfile` INT(11) UNSIGNED NOT NULL AUTO_INCREMENT,"
					+ "`path` VARCHAR(128) NOT NULL COLLATE 'utf8_unicode_ci',"
					+ "PRIMARY KEY (`idTestfile`),"
					+ "UNIQUE INDEX `path` (`path`)"
				+ ") COLLATE='utf8_unicode_ci' ENGINE=InnoDB;"
			);
			
			
			stmt.executeUpdate("CREATE TABLE IF NOT EXISTS `result` ("
					+ "`id` INT(11) UNSIGNED NOT NULL AUTO_INCREMENT,"
					+ "`idRun` INT(11) UNSIGNED NOT NULL,"
					+ "`idTestfile` INT(11) UNSIGNED NOT NULL,"
					+ "`testcaseId` INT(11) UNSIGNED NOT NULL,"
					+ "`type` INT(11) UNSIGNED NOT NULL,"
					+ "`blame` INT(11) UNSIGNED NOT NULL,"
					+ "`msg` TEXT NOT NULL COLLATE 'utf8_unicode_ci',"
					+ "`stdOut` TEXT NOT NULL COLLATE 'utf8_unicode_ci',"
					+ "`stdErr` TEXT NOT NULL COLLATE 'utf8_unicode_ci',"
					+ "`durationMs` TEXT NOT NULL COLLATE 'utf8_unicode_ci',"
					+ "PRIMARY KEY (`id`),"
					+ "INDEX `idRun_idTestfile` (`idRun`, `idTestfile`)"
				+ ") COLLATE='utf8_unicode_ci' ENGINE=InnoDB;"
			);
			
			// create TestRun
			
			stmt.executeUpdate("INSERT INTO run () VALUES ();", stmt.RETURN_GENERATED_KEYS);
			result = stmt.getGeneratedKeys();
			result.next();
			runId = result.getLong(1);
			
		} catch (SQLException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
	}
	
	
	
	public long getTestfileId(String filepath) throws SQLException {
		stmt.executeUpdate("INSERT IGNORE INTO testfile (path) VALUES('"+ filepath +"');");
		result = stmt.executeQuery("SELECT idTestfile FROM testfile WHERE path='"+filepath+"';");
		result.next();
		return result.getLong(1);
	}
	
	
	
	
	public void writeTestResult(long testFileId, Mode mode, int blame, int testcaseId, String msg, String stdOut, String stdErr, long durationMs) {
		try {
			PreparedStatement s = conn.prepareStatement("INSERT INTO result (idRun, idTestfile, testcaseId, type, blame, msg, stdOut, stdErr, durationMs) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?);");
			s.setLong	(1, this.runId);
			s.setLong	(2, testFileId);
			s.setInt	(3, mode.id);
			s.setInt	(4, blame);
			s.setInt	(5, testcaseId);
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
