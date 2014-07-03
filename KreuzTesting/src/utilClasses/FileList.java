package utilClasses;

import java.io.IOException;
import java.io.Serializable;
import java.nio.file.DirectoryIteratorException;
import java.nio.file.DirectoryStream;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.Vector;

public class FileList implements Serializable {

	/**
	 * 
	 */

	private static final long serialVersionUID = 2L;

	public Vector<Path> FileListVector = new Vector<Path>();

	public FileList() {

	}

	public void getFileList(Path path) {

		try {
			DirectoryStream<Path> stream = Files.newDirectoryStream(path);
			for (Path entry : stream) {
				if (Files.isDirectory(entry)) {

					getFileList(entry);
				}
						
				this.FileListVector.add(entry.toAbsolutePath());
			}

		} catch (IOException | DirectoryIteratorException x) {

			System.err.println(x);

		}
	}
}
