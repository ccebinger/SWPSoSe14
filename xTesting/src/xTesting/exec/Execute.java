package xTesting.exec;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.io.OutputStreamWriter;

public class Execute {
	
	
	public static final ExecResult exec(String cmd, String input) {
		
		Runtime rt = Runtime.getRuntime();
		final Process proc;
		BufferedWriter stdIn;
		BufferedReader stdOut;
		BufferedReader stdErr;
		ExecTimeout execTimeout = null;
		
		
		StringBuilder sbOut = new StringBuilder();
		StringBuilder sbErr = new StringBuilder();
		
		boolean success = false;
		
		
		try {
			proc = rt.exec(cmd);
			
			stdIn = new BufferedWriter(new OutputStreamWriter(proc.getOutputStream()));
			stdOut = new BufferedReader(new InputStreamReader(proc.getInputStream()));
			stdErr = new BufferedReader(new InputStreamReader(proc.getErrorStream()));
			
			
			// set process timeout - kill infinite loops, compilations, etc
			execTimeout = new ExecTimeout(proc);
			Thread toThread = new Thread(execTimeout);
			toThread.setDaemon(true);
			toThread.start();
			
			
			// Write process input
			if(input != null && input.length() > 0) {
				stdIn.write(input);
				stdIn.flush(); // <- very important
			}
			
			
			// too lazy, at least one defective state possible
//			String out, err;
//			try {
//				while((out = stdOut.readLine()) != null) { sbOut.append(out); sbOut.append("\n"); }
//				while((err = stdErr.readLine()) != null) { sbErr.append(err); sbErr.append("\n"); }
//			}
//			catch(Exception e) {
//				// stream closed, etc - we don't care
//			}
			
			
			String out, err;
			boolean readOut = true;
			boolean readErr = true;
			while(readOut || readErr) {
				
				if(readOut) {
					try {
						out = stdOut.readLine();
						if(out != null) {
							sbOut.append(out);
							sbOut.append("\n");
						}
						else {
							readOut = false;
						}
					}
					catch(Exception e) { readOut = false; }
				}
				
				if(readErr) {
					try {
						err = stdErr.readLine();
						if(err != null) {
							sbErr.append(err);
							sbErr.append("\n");
						}
						else {
							readErr = false;
						}
					}
					catch(Exception e) { readErr = false; }
				}
				
				
			}
			
			
			
			
			
			
			
			// expect program to finish
			proc.waitFor();
			success = proc.exitValue() == 0;
			
		} catch (Exception e) {
			//FIXME error handling! Mark test as system failure
			sbErr.append(e.toString());
			e.printStackTrace();
		}
		return new ExecResult(
			success,
			sbOut.toString(),
			sbErr.toString(),
			execTimeout != null ? execTimeout.isTimeout() : false
		);
	}
	
}
