package utilClasses;

import java.io.BufferedReader;
import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.nio.file.Path;

public class ExecuteCppCompiler {

	private Path CompilerPath;

	public ExecuteCppCompiler(Path compilerPath) {

		this.CompilerPath = compilerPath;

	}

	public StreamCollection compile(Path Filepath) throws IOException,
			InterruptedException {
		BufferedReader br = null;
		StreamCollection scr = new StreamCollection();

		if (Filepath.toString().toLowerCase().endsWith(".rail")) {

			System.out.print("Compile " + Filepath.getFileName() + "\n");
			String param = new String(CompilerPath.toString() + " -i "
					+ Filepath.toString() + " -o "
					+ Filepath.toString().replaceFirst("[.][^.]+$", "")
					+ ".class");

			Process p = Runtime.getRuntime().exec(param);
			
			scr.setStreams(p.getInputStream(), p.getErrorStream(),
					p.getOutputStream());
			
			InputStream stdin = p.getInputStream();
			InputStreamReader isr = new InputStreamReader(stdin);
			br = new BufferedReader(isr);

			String line = null;

			while ((line = br.readLine()) != null) {
				System.out.println(line);

			}
			p.waitFor();

		}
		return scr;
	}

	public void serialize(Path Filepath) throws IOException,
			InterruptedException {

		if (Filepath.toString().toLowerCase().endsWith(".rail")) {

			System.out.print("serialize " + Filepath.getFileName() + "\n");
			String param = new String(CompilerPath.toString() + " -i "
					+ Filepath.toString() + " -s "
					+ Filepath.toString().replaceFirst("[.][^.]+$", "")
					+ ".csv");

			Process p = Runtime.getRuntime().exec(param);

			p.waitFor();
		}

	}

	public void deserialize(Path Filepath) throws IOException,
			InterruptedException {

		File fl = new File(Filepath.toString().replaceFirst("[.][^.]+$", "")
				+ ".csv");

		if (fl.exists()) {
			System.out.print("deserialize " + fl.getName() + "\n");
			String param = new String(CompilerPath.toString() + " -d "
					+ Filepath.toString() + " -o "
					+ Filepath.toString().replaceFirst("[.][^.]+$", "")
					+ ".csv");

			Process p = Runtime.getRuntime().exec(param);

			p.waitFor();
		}

	}

}
