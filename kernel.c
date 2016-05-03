void printString(char*);
void readString(char*);
void handleInterrupt21(int,int,int,int);
void readSector(char*,int);
void writeSector(char*,int);
void readFile(char*,char*);
int findPosition(char* , char*);
void executeProgram(char*,int);
void deleteFile(char*);
void writeFile(char*, char*, int );
void terminate();
int div(int,int);
int mod(int,int);
void main (){
//char line[80];
//char buffer[512];

//step 1
// char buffer[13312]; /*this is the maximum size of a file*/
// makeInterrupt21();
// interrupt(0x21, 3, "messag\0", buffer, 0); /*read the file into buffer*/
// interrupt(0x21, 0, buffer, 0, 0); /*print out the file*/
// while(1); /*hang up*/

//step 2
 // makeInterrupt21();
 // interrupt(0x21, 4, "tstprg\0", 0x2000, 0);
 // while(1);

// step5
// char buffer [13312];
// makeInterrupt21();
// interrupt(0x21, 7, "messag\0", 0, 0); //delete messag
// interrupt(0x21, 3, "messag\0", buffer, 0); // try to read messag
// interrupt(0x21, 0, buffer, 0, 0); //print out the contents of buffer
// while(1);


//step6
// int i=0;
// char buffer1[13312];
// char buffer2[13312];
// buffer2[0]='h'; buffer2[1]='e'; buffer2[2]='l';buffer2[3]='l';
// buffer2[4]='o';
// for(i=5; i<13312; i++) buffer2[i]=0x0;
// makeInterrupt21();
// interrupt(0x21,8, "testW\0", buffer2, 1); //write file testW
// interrupt(0x21,3, "testW\0", buffer1, 0); //read file testW
// interrupt(0x21,0, buffer1, 0, 0); // print out contents of testW
// while(1);

// //TASK1 TEST
// printString("Hello World\0");

// //TASK2 TEST
// printString("Enter a line: \0");
// readString(line);
// printString(line);

// //TASK3 TEST
// readSector(buffer,30);
// printString(buffer);

// //TASK4 TEST
// makeInterrupt21();
// interrupt(0x21,0,0,0,0);

// //TASK5 TEST
// makeInterrupt21();
// interrupt(0x21,1,line,0,0);
// interrupt(0x21,0,line,0,0);

// while(1);
makeInterrupt21();
interrupt(0x21, 4, "shell\0", 0x2000, 0);
while(1);

}
void readString(char* str){
	//int len=sizeof(str);	
	int j=0;
	
	char ch =0x0;
	
		while(ch!=0xd){
				ch=interrupt(0x16,0,0,0,0);
				interrupt(0x10,0xE*256+ch,0,0,0);	
						str[j]=ch;
						j++;
					}
					interrupt(0x10,0xE*256+0xa,0,0,0);
			   
		
		
		//hageeb ASCI. PRINT using interupt
	

}
void printString(char* str){
	int i=0;
	while(str [i] !='\0'){
		interrupt(0x10,0xE*256+str[i],0,0,0);
		i++;
	}
}


//task3
int mod(int a, int b){
	int y;
	int ret;
		if(b<0)
			return mod(-a,-b);
			  y=div(a,b);
			  ret= a-(y*b);
		   //int ret = a % b;
		   if(ret<0)
		   	ret+=b;
		   return ret;
}

int div (int num1,int num2){
	int x;
	x=(int) num1/num2;
	return x;
}

void readSector(char* buffer, int sector){
 int relative_sector = mod(sector,18) +1;
 int head = (int) mod(div(sector,18),2);
 int track = (int) div(sector,36);
 interrupt(0x13,2*256+1,buffer,track*256+relative_sector,head*256+0);

}
 //task 4
// void handleInterrupt21(int ax,int bx, int cx,int dx){

//  	printString("your message");
//  }

// //task5
void handleInterrupt21(int ax,int bx, int cx,int dx){
	
		if(ax==0){
		printString(bx);
	}
	if(ax==1){
		readString(bx);
	}
	if(ax==2){
		readSector(bx,cx);
	}
	if(ax==3){
		readFile(bx,cx);
	}
	if(ax==4){
		executeProgram(bx,cx);
	}
	if(ax==5){
		terminate();
	}
	if(ax==6){
		writeSector(bx,cx);
	}
	if(ax==7){
		deleteFile(bx);
	}
	if(ax==8){
		writeFile(bx,cx,dx);
	}
}


int findPosition(char* buffer, char* fileName){
int i, j, flag;
for(i=0; i<16; i++){
	if(buffer[32*i]!=0x0){ // not empty
		for(j=0; j<6; j++){
		
			if(buffer[j+32*i]== fileName[j]){ // trying to match
                flag=1;
			}
			else {
				flag=0;
				break;
			}
		}
		if(flag==1){
		return i;
	}
	else {
		
	}

	}

}

if(flag==0){
	return -1;//not found
}

	
}

void readFile(char* fileName, char* buffer){
int positionFound, i, sector, j, address,d;
int sectors[27]; //array containing the values of the sectors
char directory[512];
readSector(directory,2);
positionFound= findPosition(directory, fileName);
if(positionFound==-1){

	buffer[0]='T';
	buffer[1]='h';
	buffer[2]='i';
	buffer[3]='s';
	buffer[4]=' ';
	buffer[5]='F';
	buffer[6]='i';
	buffer[7]='l';
	buffer[8]='e';
	buffer[9]=' ';
	buffer[10]='d';
	buffer[11]='o';
	buffer[12]='e';
	buffer[13]='s';
	buffer[14]=' ';
	buffer[15]='n';
	buffer[16]='o';
	buffer[17]='t';
	buffer[18]=' ';
	buffer[19]='e';
	buffer[20]='x';
	buffer[21]='i';
	buffer[22]='s';
	buffer[23]='t';

 for(d=24;d<getSize(buffer);d++){
 	buffer[d]=' ';

 }
	printString("This file does not exist");
	return;
}
else {
	sector=(positionFound*32)+6;  //first sector index
	for(i=0; i<26; i++){ // looping on sectors array to fill it with values of sectors
     sectors[i]=directory[sector+i];
	}
   j=0;
   address=0;
   while(sectors[j]!=0x0){
   	readSector(buffer+address, sectors[j]); //loading sector by sector into the buffer
   	address+=512;
    j++;
   }


	
}


}
void executeProgram(char* name,int segment){
	char tempbuffer[13312];
	int i; 
	readFile(name,tempbuffer);
	
	for(i=0;i<13312;i++){
		// el i betebda2 b 0 w kol shewaya bet-increment ma3 el address
		putInMemory(segment,i,tempbuffer[i]);
	}
	launchProgram(segment);

}

void terminate(){
	//while(1);
	//3ashan ba3d ma3ml el execute tspr2 akhalih ye terminate ezay? 
	//By re executing el shell kolaha 3ala ba3daha ml awl	
	char shell[6];
	shell[0] = 's';
	shell[1] = 'h';
	shell[2] = 'e';
	shell[3] = 'l';
	shell[4] = 'l';
	shell[5] = 0x0;
	interrupt(0x21,4,shell,0x2000,0);
}

void writeSector(char* buffer, int sector){
 int relative_sector = mod(sector,18) +1;
 int head = (int) mod(div(sector,18),2);
 int track = (int) div(sector,36);
 interrupt(0x13,3*256+1,buffer,track*256+relative_sector,head*256+0);

}

void deleteFile(char* name){


	int found;
	char directory[512];
	char map[512];
	int i,sectorPosition;
	int value;

	//load the directory
	readSector(directory,2);
  	readSector(map,1);//map

  	//search through the directory and try to find the file name
  	found = findPosition(directory,name);//bet2oli la2eto f anhi row
   //found betraga3 makano
  	if(found!=0){

        
        	directory[found*32+i]= 0x00; //el first byte bas wala el file name kolo? y3ni loop of 6?
        
  		//write them back to their coresp sectors
  		writeSector(directory,2);
  		writeSector(map,1);

  		//sectors starts at found*32 +6
  		sectorPosition=(found*32)+6;

  		//step 4
  		for(i=sectorPosition;i<26;i++){

  			value=directory[i];
  			map[value+1]=0x00;
  		}

  		//write them back to their coresp sectors
  		writeSector(directory,2);
  		writeSector(map,1);
  	}
  	else{
  		printString("msh la2y el filename");
  	}





}

void writeFile(char* fileName,char* buffer,int secNum){
	int i,j,size,diffName,diffSector,k,m,n,l,s,address,d;
	char directory[512];
 	char map[512];
    char newFileName[6];
 	readSector(directory,2);
  	readSector(map,1);
    size=getSize(fileName);
    diffName=6-size;
    // copying file name into newFileName and add zeros
    if(size<6){
    	for(m=0;m<size;m++){
    		newFileName[m]=fileName[m];
    	}
      for(k=0;k<diffName;k++){
      	newFileName[size]=0x00;
      }


    }
    if(size==6){
    	for(n=0;n<6;n++){
    		newFileName[n]=fileName[n];
    	}
    }

    // copy the newFileName into the free directory
  	for(i=0;i<16;i++){
  		if(directory[i*32]==0x00){ 
      	   for(j=0;j<6;j++){
      	   	directory[(i*32)+j]=newFileName[j];
      	   }
      	   break;
  		}
  	}
    address=0;
    for(l=0;l<secNum;l++){
    	for(s=0;s<512;s++){
    		if(map[s]==0x00){
    			map[s]=0xFF;
    			directory[(i*32)+j]=s;
    			 writeSector(buffer+address, s); //loading sector by sector into the buffer
   	             address+=512;
   	             j++;
   	             break;
   	              
                 
    		}
    		
    	}
    }
   diffSector=26-secNum;
   for(d=0;d<diffSector;d++){
   	directory[(i*32)+j+1]=0x00;
   	j++;
   }
 writeSector(directory,2);
 writeSector(map,1);

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





 	
