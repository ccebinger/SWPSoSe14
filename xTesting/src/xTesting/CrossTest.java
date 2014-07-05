package xTesting;

import java.io.File;
import java.io.IOException;
import java.nio.file.FileSystems;
import java.nio.file.FileVisitResult;
import java.nio.file.FileVisitor;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.attribute.BasicFileAttributes;
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.TreeSet;

import xTesting.Env.Mode;
import xTesting.stats.Stats;




public class CrossTest {
	
	
	final Stats stats;
	final TreeSet<TestFile> testFiles = new TreeSet<>();
	
	
	public CrossTest() {
		// Stats
		stats = Stats.getFreshStats();
		
	}
	
	
	
	public void dispatch(ArrayList<String> testPaths) throws IOException {
		
		
		// Find all test files, parse .io files
		for(String s : testPaths) {
			dispatchPath(s);
		}
		
		
		
		long timeStart = System.currentTimeMillis();
		
		
		// Dispatch all tests
		int id = 0;
		for (TestFile tf : testFiles) {
			
			try {
				++id;
				
				System.out.print("[ ");
				int paddingTestNo = String.valueOf(testFiles.size()).length();
				System.out.print(String.format("%"+paddingTestNo+"s/%"+paddingTestNo+"s", id, testFiles.size()));
				System.out.println(" ] " + tf.filename);
				
				
				if(Env.hasCpp()) {
					tf.dispatch(id, Mode.Cpp);
					tf.dispatch(id, Mode.Cpp_Cpp);
					if(Env.hasHaskell()) {
						tf.dispatch(id, Mode.Cpp_Haskell);
						tf.dispatch(id, Mode.Haskell_Cpp);
					}
				}
				if(Env.hasHaskell()) {
					tf.dispatch(id, Mode.Haskell);
					tf.dispatch(id, Mode.Haskell_Haskell);
				}
				if(Env.hasInterpreter()) {
//					tf.dispatch(id, Mode.Interpreter);
				}
			
			} catch (SQLException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			
		}
		
		
		
		long timeDiff = System.currentTimeMillis() - timeStart;
		
		
		
		//TODO Auswertung
		
		
		System.out.println();
		System.out.println("-----------------------------------------------------------------------------------");
		System.out.println("done... (took " + (timeDiff / 1000.0) + " s)");
	}
	
	
	
	private void dispatchPath(String dir) throws IOException {
		
		Path path = FileSystems.getDefault().getPath(dir);
		
		if(!path.toFile().exists()) {
			throw new RuntimeException("Test path could not be resolved: " + path);
		}
		
		
		Files.walkFileTree(path, new FileVisitor<Path>() {
			
			private CrossTest crossTest;
			private Path basePath;
			
			public FileVisitor<? super Path> init(CrossTest crossTest, Path basePath) {
				this.crossTest = crossTest;
				this.basePath = basePath;
				return this;
			}
			
			public FileVisitResult postVisitDirectory(Path arg0, IOException arg1) throws IOException {
				return FileVisitResult.CONTINUE;
			}

			public FileVisitResult preVisitDirectory(Path arg0, BasicFileAttributes arg1) throws IOException {
				File dir = Env.getTempPath().resolve(basePath.relativize(arg0)).toFile();
				if(!dir.exists()) {
					dir.mkdirs();
				}
				return FileVisitResult.CONTINUE;
			}

			public FileVisitResult visitFile(Path arg0, BasicFileAttributes arg1) throws IOException {
				
				if(arg0.toString().endsWith(".rail")) {
					TestFile tf = new TestFile(
						crossTest,
						basePath.relativize(arg0),
						arg0
					);
					testFiles.add(tf);
				}
				
				return FileVisitResult.CONTINUE;
			}

			public FileVisitResult visitFileFailed(Path arg0, IOException arg1) throws IOException {
				return FileVisitResult.CONTINUE;
			}
			
		}.init(this, path));
		
	}
		
	
}
