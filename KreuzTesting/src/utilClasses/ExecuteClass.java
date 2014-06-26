package utilClasses;

import java.io.IOException;
import  utilClasses.FileList;

public class ExecuteClass {

	public void launchFile(String pathFile) throws IOException {

		Runtime x = Runtime.getRuntime();
		x.exec(pathFile);

	}
	public void launchFileinOrdner(String pathOrdner) throws IOException {

		FileList lsf = new FileList();
		lsf.setPath("../projekt-compiler/Tests/test-cases");
		lsf.getTxtFileList();
		for (int j = 0; j < lsf.FileListVector.size() ; j++) {
			Runtime x = Runtime.getRuntime();
			x.exec(pathOrdner);
		}
		

	}
}
