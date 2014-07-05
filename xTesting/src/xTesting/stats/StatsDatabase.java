package xTesting.stats;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;

import xTesting.Env.Mode;

public class StatsDatabase extends Stats {

	private long runId;
	private static Connection conn;
	private static ResultSet result = null;
	private static Statement stmt = null;
	
	StatsDatabase() {
		try {
			if(conn == null) {
				Class.forName("com.mysql.jdbc.Driver");
				//FIXME wert in Env eintragen?
//				conn = DriverManager.getConnection("jdbc:mysql://85.116.219.166/xtStats?user=xtStats&password=CDAX3gayybZ8pseKHJSTwXe");
				conn = DriverManager.getConnection("jdbc:mysql://localhost/xtStats?user=xtStats&password=123");
				stmt = conn.createStatement();
			}
			
			
			
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
					+ "`durationMs` INT(11) UNSIGNED NOT NULL,"
					+ "PRIMARY KEY (`id`),"
					+ "INDEX `idRun_idTestfile` (`idRun`, `idTestfile`)"
				+ ") COLLATE='utf8_unicode_ci' ENGINE=InnoDB;"
			);
			
			// create TestRun
			
			stmt.executeUpdate("INSERT INTO run () VALUES ();", Statement.RETURN_GENERATED_KEYS);
			result = stmt.getGeneratedKeys();
			result.next();
			runId = result.getLong(1);
			
			
			
		} catch(Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
	
	
	
	
	
	public long getTestfileId(String filepath) throws SQLException {
		result = stmt.executeQuery("SELECT idTestfile FROM testfile WHERE path='"+filepath+"';");
		if(result.next()) {
			return result.getLong(1);
		}
		else {
			stmt.executeUpdate("INSERT INTO testfile (path) VALUES ('"+ filepath +"');", Statement.RETURN_GENERATED_KEYS);
			result = stmt.getGeneratedKeys();
			result.next();
			return result.getLong(1);
		}
	}
	
	
	public void writeTestResult(long testFileId, Mode mode, int blame, int testcaseId, String msg, String stdOut, String stdErr, long durationMs) {
		try {
			PreparedStatement s = conn.prepareStatement("INSERT INTO result (idRun, idTestfile, testcaseId, type, blame, msg, stdOut, stdErr, durationMs) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?);");
			s.setLong	(1, this.runId);
			s.setLong	(2, testFileId);
			s.setInt	(3, testcaseId);
			s.setInt	(4, mode.id);
			s.setInt	(5, blame);
			s.setString	(6, msg);
			s.setString	(7, stdOut);
			s.setString	(8, stdErr);
			s.setLong	(9, durationMs);
			s.executeUpdate();
		} catch (SQLException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
	
}
