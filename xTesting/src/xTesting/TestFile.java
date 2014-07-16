package xTesting;

import java.nio.file.Path;
import java.sql.SQLException;
import java.util.LinkedList;

import xTesting.Env.Color;
import xTesting.Env.Mode;
import xTesting.exec.ExecResult;
import xTesting.exec.Execute;



public class TestFile implements Comparable<TestFile> {
	
	final CrossTest ct;
	private long dbId;
	
	final Path filename;
	final Path file;
	
	private boolean expectCompiling;
	private LinkedList<TestCase> testcases = new LinkedList<>();
	
	
	public TestFile(CrossTest ct, Path filename, Path file) throws SQLException {
		this.ct = ct;
		this.file = file;
		this.filename = filename;
		
		// Get Database ID
		this.dbId = ct.stats.getTestfileId(filename.toString());
		
	}
	
	
	
	public final void dispatch(int id) throws SQLException {
		
		
		// parse .io
		String fname = file.getFileName().toString();
		IoParser ioParser = new IoParser(file.resolveSibling(
				fname.substring(0, fname.length()-4) + "io"
		).toFile());
		ioParser.parse(testcases);
		expectCompiling = ioParser.expectCompiling();
		
		
		
		// Run tests
		//FIXME alle wieder rein
		if(Env.hasCpp()) {
			runTest(id, Mode.Cpp);
			runTest(id, Mode.Cpp_Cpp);
			if(Env.hasHaskell()) {
				runTest(id, Mode.Cpp_Haskell);
				runTest(id, Mode.Haskell_Cpp);
			}
		}
		if(Env.hasHaskell()) {
			runTest(id, Mode.Haskell);
			runTest(id, Mode.Haskell_Haskell);
		}
		if(Env.hasInterpreter()) {
			runTest(id, Mode.Interpreter);
		}
		
	}
	
	
	
	private final void runTest(int id, Mode mode) {
		// Measure time
		long timeStart;
		
		
		
		//--------------------------------------------------------------------------------
		// Compile
		//--------------------------------------------------------------------------------
		
		// Measure Compiler
		timeStart = System.currentTimeMillis();
		
		// skip Interpreter
		if(mode != Mode.Interpreter) {
			
			// Compile
			int blameCompile = 0;
			ExecResult resFront = Execute.exec(Env.getFrontendCmd(mode, filename, file), null);
			ExecResult resBack = null;
			switch(mode) {
				case Cpp:
				case Haskell:
					// no (de-)serialisation
					if(resFront.success != expectCompiling) {
						// cannot distinguish here
						blameCompile |= Env.BLAME_FRONT | Env.BLAME_BACK;
					}
					resBack = resFront;
					break;
				case Cpp_Cpp:
				case Cpp_Haskell:
				case Haskell_Cpp:
				case Haskell_Haskell:
					// use (de-)serialisation
					if(resFront.success != expectCompiling) { blameCompile |= Env.BLAME_FRONT; }
					if(resFront.success) {
						resBack = Execute.exec(Env.getBackendCmd(mode, filename), null);
						if(resBack.success != expectCompiling) { blameCompile |= Env.BLAME_BACK; }
					}
					break;
				default:
					throw new RuntimeException("compile - mode failed...");
					//break;
			}
			
			
			
			
			// Evaluate compilation
			if(blameCompile == 0) {
				if(expectCompiling) {
					log(id, Env.BLAME_NONE, mode, 0, resFront, "Compiling", System.currentTimeMillis() - timeStart);
				}
				else {
					log(id, Env.BLAME_NONE, mode, 0, resFront, "Compiling failed (as expected)", System.currentTimeMillis() - timeStart);
				}
			}
			else {
				ExecResult err = null;
				String msg = "Expect " + (expectCompiling ? "successful" : "errorous") + " compilation: ";
				
				if(resFront.isTimeout) {
					msg += "Frontend Timeout ("+ (Env.ExecTimeout / 1000) +" s) => ";
					err = resFront;
				}
				else if(resBack != null && resBack.isTimeout) {
					msg += "Backend Timeout ("+ (Env.ExecTimeout / 1000) +" s) => ";
					err = resBack;
				}
				else if(blameCompile == (Env.BLAME_FRONT | Env.BLAME_BACK)) {
					// both failed (both compiled successfully but unexpected)
					msg += "Frontend and Backend";
					err = new ExecResult(
						resBack.success,
						(
							"--- front.out -----------------------\n" + resFront.stdOut +
							"\n--- back.out ------------------------\n" + resBack.stdOut
						),
						(
							"--- front.err -----------------------\n" + resFront.stdErr +
							"\n--- back.err ------------------------\n" + resBack.stdErr
						),
						false
					);
				}
				else if(blameCompile == Env.BLAME_FRONT) {
					msg += "Frontend";
					err = resFront;
				}
				else if(blameCompile == Env.BLAME_BACK) {
					msg += "Backend";
					err = resBack;
				}
				else {
					msg += "???";
				}
				msg += " failed.";
				
				log(id, Env.BLAME_BACK, mode, 0, err, msg, System.currentTimeMillis() - timeStart);
				printExecResult(err);
			}
			
			
			// skip all tests on errorous compilation
			if(!expectCompiling || blameCompile != 0) {
				return;
			}
		}
		
		
		
		//--------------------------------------------------------------------------------
		// Run Tests
		//--------------------------------------------------------------------------------
		String cmdTarget;
		if(mode != Mode.Interpreter) {
			cmdTarget = Env.getTargetCmd(mode, filename);
		}
		else {
			cmdTarget = Env.getTargetCmd(mode, file.toAbsolutePath());
		}
		
		
		
		
		for(TestCase tc : testcases) {
			
			// Measure Testcase
			timeStart = System.currentTimeMillis();
			
			
			// prepend input
			// echo 4 | ./../fuSwp14ub/rail-interpreter/rail /home/asd/Desktop/emptyFuncName.rail
			//FIXME properly escape double quotes within tc.stdIn
			
			//cmdTarget = "echo \""+  +"\"" + cmdTarget;
			
			
			ExecResult erTarget = Execute.exec(cmdTarget, tc.stdIn);
			if(erTarget.success) {
				
				// success - compare output
				if(tc.agreeOnStdOut(erTarget)) {
					log(id, Env.BLAME_NONE, mode, tc.id, erTarget, "Output == expectation", System.currentTimeMillis()-timeStart);
				}
				else {
					// Output != expected output
					log(id, Env.BLAME_TARGET, mode, tc.id, erTarget, "Output != expectation", System.currentTimeMillis()-timeStart);
					printComparsion(cmdTarget, erTarget, tc);
				}
			}
			else {
				if(!tc.stdErr.isEmpty()) {
					// success - possibly compare stdError with actual error message
					
					//FIXME eventually compare error messages
					log(id, Env.BLAME_NONE, mode, tc.id, erTarget, "Program failed (as expected).", System.currentTimeMillis()-timeStart);
					printComparsion(cmdTarget, erTarget, tc);
				}
				else {
					log(id, Env.BLAME_TARGET, mode, tc.id, erTarget, "Program failed (unexpectedly).", System.currentTimeMillis()-timeStart);
					printComparsion(cmdTarget, erTarget, tc);
				}
			}
			
		}
	}
	
	
	
	
	//FIXME status ersetzen mit blame == 0 test
	private final void log(int id, int blame, Mode mode, int ioTestId, ExecResult execResult, String msg, long durationMs) {
		
		
		
		
		
		// Errors only
		if(Env.isErrorsOnly() && blame == 0) {
			return;
		}
		
		
		
		// reduce size if needed
		String eResOut = "";
		String eResErr = "";
		if(execResult != null) {
			eResOut = execResult.stdOut.length() > 1024 ? execResult.stdOut.substring(0, 1024) : execResult.stdOut;
			eResErr = execResult.stdErr.length() > 1024 ? execResult.stdErr.substring(0, 1024) : execResult.stdErr;
		}
		
		
		
		
		
		// Write to database
		ct.stats.writeTestResult(
			this.dbId,
			mode,
			blame,
			ioTestId,
			msg,
			eResOut,
			eResErr,
			durationMs
		);
		
		
		
		// Console output
		System.out.print("\t");
		if(blame == Env.BLAME_NONE) {
			System.out.print(colorize(Color.Green, " ok "));
		}
		else {
			System.out.print(colorize(Color.Red,   "fail"));
		}
		System.out.print(" ");
		
		
		System.out.print("[ ");
		int paddingTestNo = String.valueOf(testcases.size()).length();
		System.out.print(String.format("%"+paddingTestNo+"s/%"+paddingTestNo+"s", ioTestId, testcases.size()));
		System.out.print(" ]");
		
		
		
		System.out.print(
			String.format(
				"[ %3s  %3s  %3s  %4s ]",
				colorize((((blame & Env.BLAME_FRONT) != 0) ? Color.Red : Color.Green), mode.front),
				mode.ast,
				colorize((((blame & Env.BLAME_BACK) != 0) ? Color.Red : Color.Green), mode.back),
				colorize((((blame & Env.BLAME_TARGET) != 0) ? Color.Red : Color.Green), mode.target)
			)
		);
		
		
		
//		System.out.print("[ ");
//		System.out.print(this.filename);
//		System.out.print(" ]");
		
		
		
		
		if(msg != null) {
			System.out.print(" ");
			System.out.print(msg);
		}
		
		
		System.out.println();
	}
	
	
	
	
	
	
	
	private final void printExecResult(ExecResult result) {
		if(Env.isVerbose()) {
			if(result.isTimeout) {
				printIndented(2, "Process timeout (" + Env.ExecTimeout/1000f + "s)");
			}
			printIndented(2, "--- result.out ---");
			printIndented(2, result.stdOut);
			printIndented(2, "--- result.error ---");
			printIndented(2, result.stdErr);
		}
	}
	
	
	private final void printTestCase(TestCase expected) {
		if(Env.isVerbose()) {
			printIndented(2, "--- expected.out ---");
			printIndented(2, expected.stdOut);
			printIndented(2, "--- expected.error ---");
			printIndented(2, expected.stdErr);
		}
	}
	
	
	
	private final void printComparsion(String cmd, ExecResult result, TestCase expected) {
		if(Env.isVerbose()) {
			printIndented(2, "Command: " + cmd);
			printExecResult(result);
			printTestCase(expected);
		}
	}
	
	
	
	

	
	
	
	private final void printIndented(int times, String toPrint) {
		//FIXME enable colors ;D
		String indent = "";
		for(int i=0; i<times; ++i) {
			indent += "\t";
		}
		System.out.println(indent + toPrint.replaceAll("\n", "\n" + indent));
	}
	
	
	
	private final String colorize(Color color, String msg) {
		if(Env.useColors()) {
			return color.toString() + msg + Color.Reset.toString();
		}
		else {
			return msg;
		}
	}
	
	



	@Override
	public int compareTo(TestFile o) {
		return this.filename.compareTo(o.filename);
	}
	
	
}
