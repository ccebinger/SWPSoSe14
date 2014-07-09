package xTesting;

import java.io.File;
import java.nio.file.FileSystems;
import java.nio.file.Path;

public class Env {
	
	public static final long ExecTimeout = 2000;
	
	
	
	public enum Mode {
		Interpreter			(1, "Int",	"",		"",		""),
		Cpp					(2, "C++",	"",		"C++",	"JBC"),
		Cpp_Cpp				(3, "C++",	"Ast",	"C++",	"JBC"),
		Cpp_Haskell			(4, "C++",	"Ast",	"Hsk",	"LLVM"),
		Haskell				(5, "Hsk",	"",		"Hsk",	"LLVM"),
		Haskell_Cpp			(6, "Hsk",	"Ast",	"C++",	"JBC"),
		Haskell_Haskell		(7, "Hsk",	"Ast",	"Hsk",	"LLVM"),
		;
		
		
		public final int id;
		public final String front, back, ast, target;
		
		private Mode(int id, String front, String ast, String back, String target) {
			this.id = id;
			this.front = front;
			this.ast = ast;
			this.back = back;
			this.target = target;
		}
	}
	
	
	static final int BLAME_NONE = 0;
	static final int BLAME_FRONT = 1;
	static final int BLAME_BACK = 2;
	static final int BLAME_TARGET = 4;
	
	
	
	
	
	
	public enum Color {
		
		// https://en.wikipedia.org/wiki/ANSI_escape_code
		Nop			(""),
		
		Reset		("\u001B[0m"),
		Red			("\u001B[31m"),
		Green		("\u001B[32m"),
		Black		("\u001B[30m"),
		Yellow		("\u001B[33m"),
		Blue		("\u001B[34m"),
		Purple		("\u001B[35m"),
		Cyan		("\u001B[36m"),
		White		("\u001B[37m"),
		;
		private final String colorCode;
		private Color(final String colorCode) {
			this.colorCode = colorCode;
		}
		public String toString() {
			return this.colorCode;
		}
	}
	
	
	
	
	
	//------------------------------------------------------------------------------------------------------------------
	// Compiler/Interpreter binaries
	//------------------------------------------------------------------------------------------------------------------
	private static Path binaryCpp = null;
	public static boolean hasCpp() { return binaryCpp != null; }
	public static Path getCpp() { return binaryCpp; }
	public static void setCpp(String filepath) {
		Path path = FileSystems.getDefault().getPath(filepath);
		File file = path.toFile();
		if(!file.exists()) {
			throw new RuntimeException("Binary not found: " + filepath);
		}
		binaryCpp = path;
	}
	
	
	private static Path binaryHaskell = null;
	public static boolean hasHaskell() { return binaryHaskell != null; }
	public static Path getHaskell() { return binaryHaskell; }
	public static void setHaskell(String filepath) {
		Path path = FileSystems.getDefault().getPath(filepath);
		File file = path.toFile();
		if(!file.exists()) {
			throw new RuntimeException("Binary not found: " + filepath);
		}
		binaryHaskell = path;
	}
	
	
	private static Path binaryInterpreter = null;
	public static boolean hasInterpreter() { return binaryInterpreter != null; }
	public static Path getInterpreter() { return binaryInterpreter; }
	public static void setInterpreter(String filepath) {
		Path path = FileSystems.getDefault().getPath(filepath);
		File file = path.toFile();
		if(!file.exists()) {
			throw new RuntimeException("Binary not found: " + filepath);
		}
		binaryInterpreter = path;
	}
	
	
	
	
	private static Path tempDir;
	public static Path getTempPath() { return tempDir; }
	public static void setTempPath(String dirpath) { tempDir = FileSystems.getDefault().getPath(dirpath); }
	
	
	private static boolean colors = true;
	public static final boolean useColors() { return colors; }
	public static final void setColors(boolean colors) { Env.colors = colors; }
	
	
	private static boolean clean = true;
	public static final boolean doClean() { return clean; }
	public static final void setClean(boolean clear) { Env.clean = clear; }
	
	
	private static boolean verbose = false;
	public static final boolean isVerbose() { return verbose; }
	public static final void setVerbose(boolean verbose) { Env.verbose = verbose; }
	
	
	private static boolean useDb = true;
	public static final boolean useDatabase() { return useDb; }
	public static final void setDatabase(boolean useDb) { Env.useDb = useDb; }
	
	
	private static boolean errorsOnly = false;
	public static final boolean isErrorsOnly() { return errorsOnly; }
	public static final void setErrorsOnly(boolean errorsOnly) { Env.errorsOnly = errorsOnly; }
	
	
	
	
	
	public static final Path getCsvPath(Mode mode, Path filename) {
		String fname = filename.toString();
		fname = fname.substring(0, fname.length()-4);
		switch(mode) {
			case Cpp:				fname += "cpp";		break;
			case Cpp_Cpp:			fname += "cppCpp";	break;
			case Cpp_Haskell:		fname += "cppHs";	break;
			case Haskell:			fname += "hs";		break;
			case Haskell_Cpp:		fname += "hsCpp";	break;
			case Haskell_Haskell:	fname += "hsHs";	break;
			case Interpreter:
			default:
				throw new RuntimeException("Env.getCsvName() Unknown");
				//break;
		}
		fname += ".csv";
		return tempDir.resolve(fname);
	}
	
	
	
	public static final Path getTargetPath(Mode mode, Path filename) {
		String fname = filename.toString();
		fname = fname.substring(0, fname.length()-5) + "_";
		switch(mode) {
			case Cpp:				fname += "cpp.class";		break;
			case Cpp_Cpp:			fname += "cppCpp.class";	break;
			case Cpp_Haskell:		fname += "cppHs";			break;
			case Haskell:			fname += "hs";				break;
			case Haskell_Cpp:		fname += "hsCpp";			break;
			case Haskell_Haskell:	fname += "hsHs";			break;
			case Interpreter:
				throw new RuntimeException("Env.getTargetName() Interpreter has no target file");
			default:
				throw new RuntimeException("Env.getTargetName() Unknown mode");
		}
		return tempDir.resolve(fname);
	}
	
	
	
	public static final String getFrontendCmd(Mode mode, Path filename, Path absFilePath) {
		switch (mode) {
			case Cpp: // skip AST
				return binaryCpp + " -q" + " -i "+ absFilePath.toAbsolutePath() + " -o " + getTargetPath(mode, filename);
			case Cpp_Cpp:
			case Cpp_Haskell:
				return binaryCpp + " -q" + " -i "+ absFilePath.toAbsolutePath() + " -s " + getCsvPath(mode, filename);
			case Haskell:
				return binaryHaskell.resolve("run-compiler.sh") + " " + absFilePath.toAbsolutePath() + " " + getTargetPath(mode, filename);
			case Haskell_Cpp:
			case Haskell_Haskell:
				return binaryHaskell.resolve("run-frontend.sh") + " " + absFilePath.toAbsolutePath() + " " + getCsvPath(mode, filename);
			case Interpreter:
				throw new RuntimeException("Env.getFrontendCmd() Interpreter != Compiler. Don't call me!");
			default:
				throw new RuntimeException("Env.getFrontendCmd() Unknown");
				//break;
		}
	}
	
	
	
	public static final String getBackendCmd(Mode mode, Path filename) {
		switch (mode) {
			case Cpp:
				throw new RuntimeException("Env.getBackendCmd() Cpp Backend not present for non-AST mode");
			case Cpp_Cpp:
			case Haskell_Cpp:
				return binaryCpp + " -q" + " -d "+ getCsvPath(mode, filename) + " -o " + getTargetPath(mode, filename);
			case Haskell:
				throw new RuntimeException("Env.getBackendCmd() Haskell Backend not present for non-AST mode");
			case Cpp_Haskell:
			case Haskell_Haskell:
				return binaryHaskell.resolve("run-backend.sh") + " " + getCsvPath(mode, filename) + " " + getTargetPath(mode, filename);
			case Interpreter:
				throw new RuntimeException("Env.getBackendCmd() Interpreter != Compiler. Don't call me!");
			default:
				throw new RuntimeException("Env.getBackendCmd() Unknown");
				//break;
		}
	}
	
	
	
	public static final String getTargetCmd(Mode mode, Path filename) {
		switch(mode) {
			case Cpp:
			case Cpp_Cpp:
			case Haskell_Cpp:
				String targetName = getTargetPath(mode, filename).toString();
				String classPath = targetName.substring(0, targetName.lastIndexOf("/")+1);
				targetName = targetName.substring(classPath.length(), targetName.length()-6);
				return "java -XX:-UseSplitVerifier -cp " + classPath + " " + targetName;
			case Haskell:
			case Cpp_Haskell:
			case Haskell_Haskell:
				return getTargetPath(mode, filename).toString();
			case Interpreter:
				return binaryInterpreter + " " + filename;
				//throw new RuntimeException("Env.getTargetCmd() Interpreter nyi");
				//break;
			default:
				throw new RuntimeException("Env.getTargetCmd() Unknown mode");
				//break;
		}
	}
	
	
	
	
	
	
	
	public static void cleanTestPath() {
		// Ensure IO-stuff
		File f = Env.getTempPath().toFile();
		if(Env.doClean()) {
			if(f.getAbsolutePath().indexOf("xTesting") >=0) {
				clearDir(f);
				System.out.println("cleaning tmp folder " + f.getAbsolutePath() + "...");
			}
			else {
				System.out.println("cannot clean up tmp folder " + f.getAbsolutePath() + "...");
			}
		}
		if(!f.exists()) {
			f.mkdirs();
		}
	}
	
	
	private static void clearDir(File directory) {
		if(directory.exists()) {
			File[] files = directory.listFiles();
			if(null != files) {
				for(int i=0; i<files.length; i++) {
					if(files[i].isDirectory()) {
						clearDir(files[i]);
					}
					files[i].delete();
				}
			}
		}
	}
	
}
