package tests;
import java.io.IOException;
import java.nio.file.DirectoryIteratorException;
import java.nio.file.DirectoryStream;
import java.nio.file.FileSystems;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.Vector;

import  utilClasses.FileList;
import  utilClasses.DirectoryList;

public class SartTest {

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		
		
		
		Path path = FileSystems.getDefault().getPath("projekt-compiler");
		Path StartPath = path.toAbsolutePath();
		System.out.print(StartPath.toString());
		//DirectoryList lsf = new DirectoryList(StartPath);
		
		 try (DirectoryStream<Path> stream = Files.newDirectoryStream(StartPath)) {
			   for (Path entry: stream) {
			    	System.out.print(entry.getFileName());
			 }
		
		} catch (IOException | DirectoryIteratorException x) {
		    
		    System.err.println("aaaaa"+x);
		   
		    
		}
		
		 //Vector<Path> FileList = lsf.getDirectoryList();
		 //for (int j = 0; j < FileList.size() ; j++) {
			//		System.out.print(FileList.get(j)+"\n");
			//	}
			//lsf.setPath(StartPath);
			//lsf.getDirectoryList();
			//for (int j = 0; j < lsf.DirectoryList.size() ; j++) {
			//	System.out.print(lsf.DirectoryList.get(j)+"\n");
			//}
			
		
		
		// TODO Auto-generated method stub

	}

}
