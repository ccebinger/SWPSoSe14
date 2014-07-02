package tests;

import java.io.IOException;
import java.nio.file.FileSystems;
import java.nio.file.Path;
import java.util.Iterator;

import utilClasses.ExecuteCppCompiler;
import utilClasses.ExecuteRailInterpreter;
import utilClasses.FileList;

public class Xtesting {
	
	private static final Path pathE = FileSystems.getDefault().getPath("src").toAbsolutePath()
			.getParent().getParent();
	private static final Path CppCompilerPath = pathE.resolve("projekt-compiler/Debug/Compiler");
	private static final Path RailInterpreterPath = pathE.resolve("rail-interpreter/rail");
	
	private static final Path TestDirectoryPath = pathE
			.resolve("projekt-compiler/test/builtInCommand/conditional");
	
	
  public void start() throws IOException, InterruptedException{
	FileList fileList = new FileList();

	fileList.getFileList(TestDirectoryPath);

	Iterator<Path> fileIterator = fileList.FileListVector.iterator();
	while (fileIterator.hasNext()) {

		Path nextFile = fileIterator.next();
		ExecuteCppCompiler ec = new ExecuteCppCompiler(nextFile, CppCompilerPath);
		ExecuteRailInterpreter ri = new ExecuteRailInterpreter(nextFile, RailInterpreterPath);
		ri.compile();
	
	}
	}

}
