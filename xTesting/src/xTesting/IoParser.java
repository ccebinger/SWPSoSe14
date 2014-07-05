package xTesting;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.InputStreamReader;
import java.nio.charset.Charset;
import java.util.LinkedList;


public class IoParser {
	
	
	private File ioFile;
	private FileInputStream fis;
	private BufferedReader br;
	private boolean expectCompiling = true;
	
	
	
	public IoParser(File ioFile) {
		this.ioFile = ioFile;
	}
	
	
	
	public final void parse(LinkedList<TestCase> result) {
		
		if(ioFile.exists() == false) {
			System.err.println("Warning: .io file missing: "+ ioFile);
			return;
		}
		
		
		try {
			int actId = 0;
			
			String stdIn = "", stdOut = "", stdErr = "";
			fis = new FileInputStream(ioFile);
			br = new BufferedReader(new InputStreamReader(fis, Charset.forName("UTF-8")));
			
			String line = br.readLine();
			
			
			// Handle Utf-8 BOM
			if(line != null && line.startsWith("\uFEFF")) {
				line = line.substring(1);
			}
			
			
			
			if(line != null && line.startsWith("$")) {
				if(line.startsWith("$0")) {
					this.expectCompiling = false;
				}
				
				// eat line separator
				line = br.readLine();
				if(line.equals("---")) {
					line = br.readLine();
				}
				else {
					throw new RuntimeException(ioFile + " @ Test "+ actId +" is malformed, wrong separator found: '" + line + "', expected '---'");
				}
			}
			
			
			
			// empty .io file -> add empty test
			if(line == null) {
				actId++;
				result.add(new TestCase(actId, "", "", ""));
				return;
			}
			
			
			
			while(line != null) {
				
				stdIn = "";
				stdOut = "";
				stdErr = "";
				
				
				// fill stdIn
				while(line != null && line.matches("^(#|%|---)$") == false) {
					stdIn += line;
					line = br.readLine();
				}
				
				
				// fill stdOut
				if(line != null && line.equals("#")) {
					line = br.readLine();
					while(line != null && line.matches("^(%|---)$") == false) {
						stdOut += line;
						line = br.readLine();
					}
				}
				
				// fill stdErr
				if(line != null && line.equals("%")) {
					line = br.readLine();
					while(line != null && line.equals("---") == false) {
						stdErr += line;
						line = br.readLine();
					}
				}
				
				
				// eat LineSeparator
				if(line != null && line.startsWith("---")) {
					line = br.readLine();
				}
				
				stdIn = stdIn.trim();
				stdOut = stdOut.trim();
				stdErr = stdErr.trim();
				
				// ignore empty tests
//				if(!stdIn.isEmpty() || !stdOut.isEmpty() || !stdErr.isEmpty()) {
//					// generate Test
//					actId++;
//					result.add(new TestCase(actId, stdIn, stdOut, stdErr));
//				}
//				else {
//					System.err.println("Warning: refused empty Testcase in "+ioFile);
//				}
				actId++;
				result.add(new TestCase(actId, stdIn, stdOut, stdErr));
				
			}
			
			
//			System.out.println("parsed " + result.size() + " test cases");
//			for(TestCase tc : result) {
//				System.out.println(this.ioFile.getAbsolutePath());
//				System.out.println("\tstdIn : " + tc.stdIn);
//				System.out.println("\tstdOut: " + tc.stdOut);
//				System.out.println("\tstdErr: " + tc.stdErr);
//				System.out.println();
//			}
			
			
			br.close();
			fis.close();
		}
		catch(Exception e) {
			e.printStackTrace();
		}
	}
	
	
	
	
	
	
	
	
	public final boolean expectCompiling() {
		return this.expectCompiling;
	}
	
	
	
	
	
}
