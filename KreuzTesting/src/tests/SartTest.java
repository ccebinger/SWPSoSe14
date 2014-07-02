package tests;


import java.io.IOException;
import java.nio.file.FileSystems;
import java.nio.file.Path;
import java.util.Iterator;
import utilClasses.ExecuteCompiler;
import utilClasses.FileList;

public class SartTest {

	/**
	 * @param args
	 * @throws IOException
	 * @throws InterruptedException
	 */
	public static void main(String[] args) throws IOException,
			InterruptedException {

		Path pathE = FileSystems.getDefault().getPath("src").toAbsolutePath()
				.getParent().getParent();

		Path TestDirectoryPath = pathE
				.resolve("projekt-compiler/test/builtInCommand/conditional");
		

		FileList fileList = new FileList();

		fileList.getFileList(TestDirectoryPath);

		Iterator<Path> fileIterator = fileList.FileListVector.iterator();
		while (fileIterator.hasNext()) {

			Path nextFile = fileIterator.next();
			ExecuteCompiler ec = new ExecuteCompiler(nextFile);
			ec.compile();
			
		}
	}
}
