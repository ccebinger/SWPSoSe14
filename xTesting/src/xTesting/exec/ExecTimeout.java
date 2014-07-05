package xTesting.exec;

import java.util.concurrent.atomic.AtomicBoolean;

import xTesting.Env;

public class ExecTimeout implements Runnable {
	
	private final Process process;
	private final AtomicBoolean timedOut = new AtomicBoolean(false);
	
	
	public ExecTimeout(Process process) {
		this.process = process;
	}
	
	public void run() {
    	long now = System.currentTimeMillis();
    	while(System.currentTimeMillis() - now < Env.ExecTimeout) {
    		try {
    			Thread.sleep(1000);
    		}
    		catch(Exception e) {}
    	}
    	timedOut.set(true);
    	process.destroy();
    }
	
	
	public boolean isTimeout() {
		return timedOut.get();
	}
	
	
}
