package utilClasses;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.nio.file.Path;

public class ExecuteRailInterpreter {

	
	

	
	private Path CompilerPath;
	
	
	public ExecuteRailInterpreter(Path compilerPath){
		
		
		this.CompilerPath  = compilerPath;
		
	}
	
	public  StreamCollection compile(Path Filepath) throws IOException, InterruptedException{
		
		BufferedReader br = null;
		StreamCollection scr = new StreamCollection();
		
		if (Filepath.toString().toLowerCase().endsWith(".rail")) {
		
			System.out.print("Compile Rail Interpreter " + Filepath.getFileName()+ "\n");
			String param = new String(CompilerPath.toString() 
					+ " "
					+Filepath.toString()
					+ " --output="
					+ Filepath.toString().replaceFirst("[.][^.]+$", "")
					+ ".output");
				System.out.println("\n"+param+"\n");
			
			Process p = Runtime.getRuntime().exec(param);
			scr.setStreams(p.getInputStream(), p.getErrorStream(),
					p.getOutputStream());
			
			p.waitFor();
			
			
		}
		return scr;
	}
}
