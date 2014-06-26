package tests;
import java.nio.file.FileSystems;
import java.nio.file.Path;
import java.util.Vector;
import utilClasses.DirectoryList;

public class SartTest {

	/**
	 * @param args
	 */
	public static void main(String[] args) {

		Path pathE = FileSystems.getDefault().getPath("src");
		Path StartPath = pathE.toAbsolutePath().getParent().getParent().resolve("projekt-compiler").resolve("test");
		DirectoryList lsf = new DirectoryList(StartPath);

		Vector<Path> FileList = lsf.getDirectoryList();
		for (int j = 0; j < FileList.size(); j++) {
			System.out.print(FileList.get(j).getFileName() + "\n");
		}
		
	}

}
