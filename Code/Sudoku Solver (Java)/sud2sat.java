import java.io.*;
import java.util.*;
import java.util.Scanner;
import java.io.BufferedReader;
import java.io.FileReader;

public class sud2sat{
	public static void main(String args[])throws Exception{
		Scanner scan= new Scanner(new BufferedReader(new FileReader(args[0])));
		String temp = ""; 
		String row = "";
		while (scan.hasNextLine()) {
            		temp = scan.nextLine();
			row+= temp;
		}
		String [] stringArray= row.split("");
		for(int j=0;j<stringArray.length;j++){
			if(stringArray[j].equals("*")||stringArray[j].equals(".")||stringArray[j].equals("?"))
				stringArray[j]="0"; // this part deals with different wildcards ".,*,?" and changes it to 0
		}
	
		int[] num = new int[stringArray.length];
		for(int i = 0;i < stringArray.length;i++){
			num[i] = Integer.parseInt(stringArray[i]);
		}
		int [][] grid =gridGenerator(num);
		int clauses = 0;
		
		try{
			File file = new File("sud2sat.txt");
			FileOutputStream fos = new FileOutputStream(file);
			PrintStream ps = new PrintStream(fos);
			PrintStream console = System.out;
			System.setOut(ps);
			clauses+=CNFfill(grid,ps);
			clauses+=CNF(grid,ps);
			clauses+=CNFcolumn_Row(grid,ps);
			clauses+=CNF3x3(grid,ps);
			RandomAccessFile file2 = new RandomAccessFile("sud2sat.txt", "rws");
			byte[] text = new byte[(int) file2.length()];
			file2.readFully(text);
			file2.seek(0);
			file2.writeBytes("p cnf 729 "+clauses+"\n");
			file2.write(text);
			file2.close();
			System.setOut(console);
		}
		catch(IOException e1) {
			System.out.println("Error during reading/writing");
		}
}
	public static int[][] gridGenerator(int [] num){
		int[][] Grid =new int[9][9];
		for(int i =0;i<9;i++){
			for(int j =0;j<9;j++){
				Grid[i][j] = num[i*9+j];
			}
		}
		return Grid;
	}
		public static int CNFfill(int [][]grid,PrintStream ps){
			int count=0;
			System.setOut(ps);
			for(int i=0;i<9;i++){
				for(int j=0;j<9;j++){
					if (grid[i][j]!=0){
						System.out.println(ConvertToBase9(i+1,j+1,grid[i][j])+" 0");
						count++;
					}
				}
			}
		return count;
	}
	public static int CNF(int [][]grid,PrintStream ps){
		int count=0;
		System.setOut(ps);
			for(int i=1;i<10;i++){
				for(int j=1;j<10;j++){
					for(int k=1;k<10;k++){
						System.out.print(ConvertToBase9(i,j,k)+" ");
					}
					System.out.println(" 0");
					count++;
				}
			}
			
		return count;
	}
	public static int CNFcolumn_Row(int [][]grid,PrintStream ps){
		int count = 0;
		System.setOut(ps);
		for(int i=1;i<10;i++){
			for(int j=1;j<10;j++){
				for(int k=1;k<9;k++){
					for(int l =j+1;l<10;l++){
					System.out.println("-"+ConvertToBase9(i,j,k)+" -"+ConvertToBase9(i,l,k)+" 0");
					count++;
					}
				}
			}
		}
		for(int j =1;j<10;j++){
			for(int k=1;k<10;k++){
				for(int i=1;i<9;i++){
					for(int l=i+1;l<10;l++){
						System.out.println("-"+ConvertToBase9(i,j,k)+" -"+ConvertToBase9(l,j,k)+" 0");
						count++;
					}
				}
			}
		}
		return count;
	}
	public static int CNF3x3(int [][]grid,PrintStream ps){
		int count =0;
		System.setOut(ps);
		for(int i =1;i<10;i++){
			for(int xaxis=0;xaxis<3;xaxis++){
				for(int yaxis=0;yaxis<3;yaxis++){
					for(int k=1;k<4;k++){
						for(int l=1;l<4;l++){
							for(int m =l+1;l<4;l++){
								int a= xaxis * 3 + k;
								int b= yaxis * 3 + l;
								int c= yaxis * 3 + m;
								System.out.println("-"+ConvertToBase9(a,b,i)+" -"+ConvertToBase9(a,c,i)+" 0");
								count++;
							}
							for(int m =k+1;m<4;m++){
								for(int n =1;n<4;n++){
									int a= xaxis * 3 + k;
									int b= yaxis * 3 + l;
									int c= xaxis * 3 + m;
									int d= yaxis * 3 + n;
									System.out.println("-"+ConvertToBase9(a,b,z)+" -"+ConvertToBase9(c,d,i)+" 0");
									count++;
								}
							}
						}
					}
				}
			}
		}
	
		return count;
	}
	public static int ConvertToBase9(int i,int j,int k){
		return (i-1)*81 + (j-1)*9 + (k-1)+1;
	}
}
