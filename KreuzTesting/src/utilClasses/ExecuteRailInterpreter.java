package utilClasses;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.nio.file.Path;

public class ExecuteRailInterpreter {

	
	

	private Path Filepath;
	private Path CompilerPath;
	
	
	public ExecuteRailInterpreter(Path path, Path compilerPath){
		
		this.Filepath = path;
		this.CompilerPath  = compilerPath;
		
	}
	
	public  BufferedReader compile() throws IOException, InterruptedException{
		BufferedReader br = null;
		
		if (this.Filepath.toString().toLowerCase().endsWith(".rail")) {
		
			System.out.print("Compile Rail Interpreter " + this.Filepath.getFileName()+ "\n");
			String param = new String(CompilerPath.toString() 
					
					+ " --output="
					+ this.Filepath.toString().replaceFirst("[.][^.]+$", "")
					+ ".io2");
				System.out.println("\n"+param+"\n");
			
			Process p = Runtime.getRuntime().exec(param);
			InputStream stdin = p.getInputStream();
			InputStreamReader isr = new InputStreamReader(stdin);
			 br = new BufferedReader(isr);

			String line = null;

			while ((line = br.readLine()) != null) {
				System.out.println(line);

			}
			p.waitFor();
			
			
		}
		return br;
	}
}
