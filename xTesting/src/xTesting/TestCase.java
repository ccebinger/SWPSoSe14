package xTesting;

import xTesting.exec.ExecResult;


public class TestCase {
	
	public final int id;
	public final String stdIn;
	public final String stdOut;
	public final String stdErr;
	
	public TestCase(int id, String stdIn, String stdOut, String stdError) {
		this.id = id;
		this.stdIn = stdIn.trim();
		this.stdOut = stdOut.trim();
		this.stdErr = stdError;
	}
	
	
	public final boolean agreeOnStdOut(ExecResult result) {
		return this.stdOut.equals(result.stdOut);
	}
	
	
	public final boolean agreeOnStdErr(ExecResult result) {
		return this.stdErr.equals(result.stdErr);
	}
	
	
	public final boolean agreeBoth(ExecResult result) {
		return agreeOnStdOut(result) && agreeOnStdErr(result);
	}
	
}
