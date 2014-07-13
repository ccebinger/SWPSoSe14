package xTesting.exec;

public class ExecResult {
	
	public final boolean success;
	public final String stdOut;
	public final String stdErr;
	public final boolean isTimeout;
	
	public ExecResult(boolean success, String stdOutput, String stdError, boolean isTimeout) {
		this.success = success;
		this.stdOut = stdOutput.trim();
		this.stdErr = stdError.trim();
		this.isTimeout = isTimeout;
	}
	
}