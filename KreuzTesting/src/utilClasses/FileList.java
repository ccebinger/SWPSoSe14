package utilClasses;


import java.io.File;
import java.io.Serializable;
import java.util.Vector;

public class FileList implements Serializable {

	/**
	 * 
	 */

	private static final long serialVersionUID = 1L;
	private String Path;
	public Vector<String> FileListVector = new Vector<String>();

	public FileList() {

	}

	public void setPath(String path) {

		this.Path = path;
	}

	public String getPath() {

		return (this.Path);
	}

	public Vector<String> getTxtFileList() {
		File folder = new File(this.Path);
		File[] listOfFiles = folder.listFiles();
		for (int i = 0; i < listOfFiles.length; i++) {

			if (listOfFiles[i].getName().endsWith(".txt")
					|| listOfFiles[i].getName().endsWith(".TXT")) {
				this.FileListVector.add(listOfFiles[i].getName());
			}

		}

		return this.FileListVector;
	}
	public Vector<String> getCSVFileList() {
		File folder = new File(this.Path);
		File[] listOfFiles = folder.listFiles();
		for (int i = 0; i < listOfFiles.length; i++) {

			if (listOfFiles[i].getName().endsWith(".csv")
					|| listOfFiles[i].getName().endsWith(".CSV")) {
				this.FileListVector.add(listOfFiles[i].getName());
			}

		}

		return this.FileListVector;
	}
}
