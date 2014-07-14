package tests;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.nio.file.FileSystems;
import java.nio.file.Path;
import java.util.Iterator;

import utilClasses.CompareCSV;
import utilClasses.ExecuteCppCompiler;
import utilClasses.ExecuteJavaClass;
import utilClasses.ExecuteRailInterpreter;
import utilClasses.FileList;
import utilClasses.StreamCollection;

public class Xtesting {

	private static final Path pathE = FileSystems.getDefault().getPath("src")
			.toAbsolutePath().getParent().getParent();
	private static final Path CppCompilerPath = pathE
			.resolve("projekt-compiler/Debug/Compiler");
	private static final Path RailInterpreterPath = pathE
			.resolve("rail-interpreter/rail");

	private static final Path TestDirectoryPath = pathE
			.resolve("projekt-compiler/test/builtInCommand/conditional");

	public void start() throws IOException, InterruptedException,
			NullPointerException {

		BufferedReader br = null;
		String br2 = null;

		FileList fileList = new FileList();

		fileList.getFileList(TestDirectoryPath);

		Iterator<Path> fileIterator = fileList.FileListVector.iterator();
		ExecuteCppCompiler ec = new ExecuteCppCompiler(CppCompilerPath);
		int i=0;
		while (fileIterator.hasNext()) {
			
			Path nextFile = fileIterator.next();
			System.out.print(nextFile.toString());
			// ExecuteRailInterpreter ri = new
			// ExecuteRailInterpreter(RailInterpreterPath);
			// ExecuteJavaClass ejc = new ExecuteJavaClass();

			String param = new String(CppCompilerPath.toString() + " -i "
					+ nextFile.toString() + " -o "
					+ nextFile.toString().replaceFirst("[.][^.]+$", "")
					+ ".class");

			Process p = Runtime.getRuntime().exec(param);

			InputStream stdin = p.getErrorStream();
			InputStreamReader isr = new InputStreamReader(stdin);
			br = new BufferedReader(isr);

			String line = null;

			while ((line = br.readLine()) != null) {
				//System.out.println(line);

			}
			p.waitFor();

		}

		/*
		 * StreamCollection str = ec.compile(nextFile);
		 * 
		 * System.out.print("input: "); while ((br =
		 * str.getInputBufferedReader().readLine()) != null) {
		 * System.out.print(br + "\n"); } System.out.print("error: "); while
		 * ((br2 = str.getErrBufferedReader().readLine()) != null) {
		 * System.out.print(br2 + "\n"); }
		 */
	}
}
