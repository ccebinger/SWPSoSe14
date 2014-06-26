package utilClasses;

import java.io.IOException;
import  utilClasses.FileList;

public class ExecuteClass {

	public void launchFile(String pathFile) throws IOException {

		Runtime x = Runtime.getRuntime();
		x.exec(pathFile);

	}
	public void launchFileinOrdner(String pathOrdner) throws IOException {

		

	}
}
