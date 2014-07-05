import java.io.IOException;
import java.util.ArrayList;

import xTesting.CrossTest;
import xTesting.Env;


public class Main {

	public static void main(String[] args) {
		ArrayList<String> testPaths = new ArrayList<>();
		for(int i=0; i<args.length; ++i) {
			if(args[i].equals("--jail") && i+1 < args.length) { Env.setCpp(args[++i]); }
			if(args[i].equals("--haskell") && i+1 < args.length) { Env.setHaskell(args[++i]); }
			if(args[i].equals("--interpreter") && i+1 < args.length) { Env.setInterpreter(args[++i]); }
			if(args[i].equals("--tempDir") && i+1 < args.length) { Env.setTempPath(args[++i]); }
			if(args[i].equals("--no-colors")) { Env.setColors(false); }
			if(args[i].equals("--no-clean")) { Env.setClean(false); }
			if(args[i].equals("--no-db")) { Env.setDatabase(false); }
			if(args[i].equals("--verbose")) { Env.setVerbose(true); }
			if(args[i].equals("-d") && i+1 < args.length) { ++i; testPaths.add(args[i]); }
		}
		
		System.out.println();
		System.out.println("Parameters");
		System.out.println("  --jail          " + Env.getCpp());
		System.out.println("  --haskell       " + Env.getHaskell());
		System.out.println("  --interpreter   " + Env.getInterpreter());
		System.out.println("  --tempDir       " + Env.getTempPath());
		System.out.println("  --no-colors     " + !Env.useColors());
		System.out.println("  --no-clean      " + !Env.doClean());
		System.out.println("  --no-db         " + !Env.useDatabase());
		System.out.println("  --verbose       " + Env.isVerbose());
		System.out.println("  -d");
		for(String s : testPaths) {
			System.out.println("                  " + s);
		}
		System.out.println();
		
		
		Env.cleanTestPath();
		
		
		CrossTest ct = new CrossTest();
		try {
			ct.dispatch(testPaths);
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}

}
