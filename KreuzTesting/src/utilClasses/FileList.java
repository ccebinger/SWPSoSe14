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
	private Path path;

	public FileList(Path path) {

		this.path = path;

	}

	public void setPath(Path path) {

		this.path = path;
	}

	public Path getPath() {

		return (this.path);
	}

	public Vector<Path> getFileList() {
		Vector<Path> FileListVector = new Vector<Path>();

		try (DirectoryStream<Path> stream = Files.newDirectoryStream(path,".rail")) {
			for (Path entry : stream) {
				FileListVector.add(entry.getFileName());
			}

		} catch (IOException | DirectoryIteratorException x) {

			System.err.println(x);

		}
		return FileListVector;
		
}
}
