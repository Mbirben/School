import java.io.*;
import java.util.*;
import java.util.Scanner;
import java.io.BufferedReader;
import java.io.FileReader;

public class sat2sud{
	public static void main(String args[])throws Exception{
		Scanner scan= new Scanner(new BufferedReader(new FileReader(args[0])));
		String temp = ""; 
		String row = "";
		temp = scan.nextLine();
		if(temp.equals("SAT")){
			System.out.println("satisfiable\nSolution");
		while (scan.hasNextLine()) {
            temp = scan.nextLine();
			row+= temp;
		}
		String [] stringArray= line.split(" ");
		int[] num = new int[stringArray.length];
		for(int i = 0;i < stringArray.length;i++){
			num[i] = Integer.parseInt(stringArray[i]);
		}
		for(int j=0;j<9;j++){
			String line ="";
			for(int k =0;k<9;k++){
				for(int l= 0;l<9;l++){
					if(num[(j*81)+(k*9)+l]>=0){
						String digits = Integer.toString(l+1);
						line = line+digits+" ";
						break;
					}
				}
			}
			System.out.println(line);
		}
		}
		else{
			System.out.println("This problem is unsatisfiable");
			System.exit(0);
		}
	}
}
