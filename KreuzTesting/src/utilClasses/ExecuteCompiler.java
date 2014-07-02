package utilClasses;

import java.io.BufferedReader;
import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.nio.file.FileSystems;
import java.nio.file.Path;


public class ExecuteCompiler {
	
	private Path path;
	private static final Path pathE = FileSystems.getDefault().getPath("src").toAbsolutePath()
			.getParent().getParent();
	private static final Path CompilerPath = pathE.resolve("projekt-compiler/Debug/Compiler");
	
	
	public ExecuteCompiler(Path path){
		
		this.path = path;
		
	}
	
	public  BufferedReader compile() throws IOException, InterruptedException{
		BufferedReader br = null;
		
		if (this.path.toString().toLowerCase().endsWith(".rail")) {
		
			System.out.print("Compile " + this.path.getFileName()+ "\n");
			String param = new String(CompilerPath.toString() + " -d "
					+ this.path.toString() + " -o "
					+ this.path.toString().replaceFirst("[.][^.]+$", "")
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
		
		if (this.path.toString().toLowerCase().endsWith(".rail")) {
			
			System.out.print("serialize " + this.path.getFileName()+ "\n");
			String param = new String(CompilerPath.toString() + " -i "
					+ this.path.toString() + " -s "
					+ this.path.toString().replaceFirst("[.][^.]+$", "")
					+ ".csv");

			
			Process p = Runtime.getRuntime().exec(param);
			
			p.waitFor();
		}
		
	}
public void deserialize() throws IOException, InterruptedException{
		
	    File fl = new File(this.path.toString().replaceFirst("[.][^.]+$", "")+".csv");
	    
		
		if (fl.exists()) {
			System.out.print("deserialize " + fl.getName()+ "\n");
			String param = new String(CompilerPath.toString() + " -d "
					+ this.path.toString() + " -o "
					+ this.path.toString().replaceFirst("[.][^.]+$", "")
					+ ".csv");

			
			Process p = Runtime.getRuntime().exec(param);
			
			p.waitFor();
		}
		
	}
	

	
}
