package utilClasses;

import java.io.BufferedReader;
import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.nio.file.Path;


public class ExecuteCppCompiler {
	
	private Path Filepath;
	private Path CompilerPath;
	
	
	public ExecuteCppCompiler(Path path, Path compilerPath){
		
		this.Filepath = path;
		this.CompilerPath  = compilerPath;
		
	}
	
	public  BufferedReader compile() throws IOException, InterruptedException{
		BufferedReader br = null;
		
		if (this.Filepath.toString().toLowerCase().endsWith(".rail")) {
		
			System.out.print("Compile " + this.Filepath.getFileName()+ "\n");
			String param = new String(CompilerPath.toString() + " -i "
					+ this.Filepath.toString() + " -o "
					+ this.Filepath.toString().replaceFirst("[.][^.]+$", "")
					+ ".class");

			
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
	

	public void serialize() throws IOException, InterruptedException{
		
		if (this.Filepath.toString().toLowerCase().endsWith(".rail")) {
			
			System.out.print("serialize " + this.Filepath.getFileName()+ "\n");
			String param = new String(CompilerPath.toString() + " -i "
					+ this.Filepath.toString() + " -s "
					+ this.Filepath.toString().replaceFirst("[.][^.]+$", "")
					+ ".csv");

			
			Process p = Runtime.getRuntime().exec(param);
			
			p.waitFor();
		}
		
	}
public void deserialize() throws IOException, InterruptedException{
		
	    File fl = new File(this.Filepath.toString().replaceFirst("[.][^.]+$", "")+".csv");
	    
		
		if (fl.exists()) {
			System.out.print("deserialize " + fl.getName()+ "\n");
			String param = new String(CompilerPath.toString() + " -d "
					+ this.Filepath.toString() + " -o "
					+ this.Filepath.toString().replaceFirst("[.][^.]+$", "")
					+ ".csv");

			
			Process p = Runtime.getRuntime().exec(param);
			
			p.waitFor();
		}
		
	}
	

	
}
