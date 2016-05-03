void readString(char*);
void print(char*);
int getSize(char*);
int div(int,int);
void checkCommand(char*);
void clear (char*, int);
main(){

while(1){
	char line[180];
	char array [6];
	array[0]='S';
	array[1]='h';
	array[2]='e';
	array[3]='l';
	array[4]='l';
	array[5]='>';
	

	
  	interrupt(0x21,0,"shell>\0",0,0);//Shell> //print string
	interrupt(0x21,1,line,0,0);//Shell> //read string

	//yakhod el input, print Bad command, b3den yprint shell>
		checkCommand(line);//ashuf b2a di anhi command w execute
		clear(line,180);


}

}
void checkCommand(char* buffer){
	char bufferTemp [6],shell[80],copyTempBuffer [13312], file2 [6],resultBuffer[13312],step[80];//max size 
	int messageCounter,i,commandSize,j,secNum,address;

	commandSize=getSize(buffer);

	//fe kol el a7wal ana had5ol elfor loop di mara wahda bas fa dyman i =0
	for(i=0;i<180;i++){

		//step 1: You should now modify your shell to recognize the command view filename	
		if(buffer[i]=='v' && buffer[i+1]=='i' && buffer[i+2]=='e' && buffer[i+3]=='w'){
			//view filename, hakhod el filename a2raholo. will call readFile

			//3la asAS eno haydakhal view messag--+5 3ashan el space
			for(messageCounter=i+5 ;messageCounter<commandSize;messageCounter++){
			//will load the whole file name into a buffer
				bufferTemp[i]= buffer[messageCounter];
				i++;

			}
			//b3den a view el filename dah
			interrupt(0x21,3, bufferTemp, resultBuffer, 0);	//read
			interrupt(0x21,0, resultBuffer, 0, 0);//print
			return;

		}

		//step 2:Now modify the shell to recognize the command execute filename
		if(buffer[i]=='e' && buffer[i+1]=='x' && buffer[i+2]=='e' && buffer[i+3]=='c' && buffer[i+4]=='u' && buffer[i+5]=='t' && buffer[i+6]=='e'){
			//i will call execute method 

			//3al asas en execute tstpr2
			for(messageCounter= i+8 ;messageCounter<commandSize; messageCounter++ ){
				//will load the whole file name into a buffer
				bufferTemp[i]= buffer[messageCounter];
				i++;


			}
			//b3den a execute el filename dah
			interrupt(0x21,4, bufferTemp,0x2000,1);//mfrod en el cx hwa el segment
			
			return;
		}

		//step 3:You should add a delete filename command to the shell
		if(buffer[i]=='d' && buffer[i+1]=='e' && buffer[i+2]=='l' && buffer[i+3]=='e' && buffer[i+4]=='t' && buffer[i+5]=='e'){
			// i will call deleteFile

			//3ala asas enaha delete messag
			for(messageCounter= i+7 ;messageCounter<commandSize; messageCounter++){
				//will load the whole file name into a buffer
				bufferTemp[i]= buffer[messageCounter];
				i++;

				
			}
			//b3den a delete el filename b2a
			interrupt(0x21, 7,bufferTemp, 0x2000, 0);//msh3rfa el 0x2000 di f kol el interupts wla eh??

			return;
		}

		//step 4: you will write a copy command for the shell
		if(buffer[i]=='c' && buffer[i+1]=='o' && buffer[i+2]=='p' && buffer[i+3]=='y'){
	
			//example : copy messag m2... +5 3ashan el space
			messageCounter=i+5;

			while(buffer[messageCounter]!= 0x20){
				bufferTemp[i]= buffer[messageCounter];
				i++;
				messageCounter++;


			}
			//b3den i will read this file1 into copyTempBuffer
			interrupt(0x21, 3, bufferTemp, copyTempBuffer, 0);

			
			j=0;
			for(messageCounter=messageCounter+1; messageCounter<getSize(buffer); messageCounter++){

				file2[j]= buffer[messageCounter];
				j++;
			}
			//file2 feeh esm el file el tany

			interrupt(0x21,8,file2 , copyTempBuffer, 1);
			return;
             }

	

	// 	//step 5 : you will write a dir command
		 //should print out the files in the directory.
		 //also print out the sizes of the files in sectors -- M3Ndish ay fekra EZAY??!
	if(buffer[i]=='d' && buffer[i+1]=='i' && buffer[i+2]=='r'){
			
			for(i=0;i<13312;i++){
				copyTempBuffer[i]= 0x0;
				resultBuffer[i]= 0x0;
				
			}
			for(i=0;i<6;i++){
				file2[i]=0x0;

			}

			interrupt(0x21,2,copyTempBuffer,2, 1);//readSector directory

		for(i=0; i<16; i++){
         	if(copyTempBuffer[32*i]!=0x0){ // not empty
		   
		      for(j=0; j<6; j++){
		
			    resultBuffer[j]=copyTempBuffer[j+(32*i)]; // yekteb el filename f file2
                      
                      }
                      resultBuffer[6]='\0';
                      //read el file dah
						//ka2eni b3ml view file2
//                    interrupt(0x21,3, file2, resultBuffer, 0);	//read
//					interrupt(0x21,0, resultBuffer, 0, 0);//print
                      // for(i=0;i<13312;i++){

                      // 	interrupt(0x21,0, resultBuffer[i], 0, 0);//print the file name only
                      // }
                    interrupt(0x21,0, resultBuffer, 0, 0);//print the file name only
			 
                    
		
		     }



	    }
         return;
         }


		
  

  //step 6 el akhiraaaaa
	if(buffer[i]=='c' && buffer[i+1]=='r' && buffer[i+2]=='e' && buffer[i+3]=='a' && buffer[i+4]=='t' && buffer[i+5]=='e'){

			for(messageCounter= i+7 ;messageCounter<commandSize; messageCounter++ ){
				//will load the whole file name into a buffer
				bufferTemp[i]= buffer[messageCounter];
				i++;


			}

			address=0;
			
			
			while(1){
				interrupt(0x21,1,step,0,0);//read content ml user
				if(step[0]== '\n' || step[0]== \0 || step[0]== 0x20){
			
					break;
				}

				else{
					for(i=0;i<getSize(step);i++){
						resultBuffer[address]=step[i];
						address++;
					}
				}
				
				
				for(j=0;j<80;j++){
					step[j]=0x0;
				}
			}
			j=getSize(resultBuffer) +1;
			secNum= div(512,j);
			//lama ye break ye write el file b2a
			interrupt(0x21,8, bufferTemp, resultBuffer, secNum);
			return;

			

	}
	else{
		//print command not found
		interrupt(0x21,0,"command not found\0",0,0);
		return;
	}  



}
}


void print(char* buff){
	

	interrupt(0x21,0,buff,0,0);

}


int getSize(char* array){
	int i=0;
	int size=0;
	while(array[i]!=0x00){
		size+=1;
		i+=1;
	}
	return size;
}
void clear(char* buff, int len){
	int i;
	for(i=0;i<len;i++){
		buff[i] = 0x0;
	}
}

int div (int num1,int num2){
	int x;
	x=(int) num1/num2;
	return x;
}