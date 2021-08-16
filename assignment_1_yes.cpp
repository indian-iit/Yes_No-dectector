// assignment_1_yes.cpp : Defines the entry point for the console application.
//Below programe will take an audio file which is decoded as text file as an input.
//The input file has one word either "yes" or "no".
//The  programe will predict the word present in the file.
//Adding the required header files
#include "stdafx.h"
#include "stdafx.h"
#include<stdio.h>
#include<conio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include<math.h>
#include<string.h>

// The below macrodefinition will help us to replace limit by 5000 in entire programe, which helps us in wave normalisation.
//5000 signifies the maximum amplitute a wave can posess.
#define limit 5000
//Below variables ad pointers are declared as global so that their scope is global and can be  accessible from any function.
//Below variables are initialised in their respective function where they have been used for first time.
FILE *input_file1,*input_normalisation,*input_file2,*input_energy_zcr;
long frames_count,begin,end;
double samples_Count,dc_Shift;
double energy_value[50000], zcr_value[50000];
double threshold_Energy,threshold_Zcr;

//Below function will take the dc.txt file as input and updates the dcshfit value.
//Dcshift value will also helps us to find whether the hardware is in good condition or not.
void calculate_dcshift(FILE *input_file)
{
	//"Rewind" function will make the file pointer to point the begining if file.
	rewind(input_file);
	//Intialisaing the required variables.
	char singlevalue[100];
	samples_Count=0;
	dc_Shift=0;
	int x=0;
	//feof indicates end of the file.
	//Until we reach the end of the file we keep on adding the values present in the file to dc_Shift.
	while(!feof(input_file))
	{
	   
	  fgets(singlevalue,100, input_file);
	     //Whenever we extract a charecter from the file we will check whether it is a digit or not with the help of isalpha function.
	     //This is done to aoid the compuation on header information present in the file.
	     if(!isalpha(singlevalue[0]))
	      {
			//atoi function will covert the charecter arry to integer.
			x = atoi(singlevalue);
			dc_Shift+=x;
			samples_Count++;
		  }
    }
	//Finally we compute the average dcshift by adding all values in file divided by no of samples present in the file.
	dc_Shift =dc_Shift/samples_Count;
}

//The below function reads data from the file pointed by input_file and stores the output("Normalised data") in file pointed by output_normalisation.
void apply_normalisation(FILE *input_file,FILE *output_normalisation)
{
	//Making the pointers of file pointing to the begining of the file.
	rewind(input_file);
	rewind(output_normalisation);
	//Initialising the required variables.
	double max=0;
	int count=0;
	char singlevalue[100];
	double normalisation_Factor = 0;
	while(!feof(input_file))
	{
        fgets(singlevalue, 100, input_file);
		double x = atoi(singlevalue);
		count++;
		if(abs(max) < abs(x))
		{
		   max = abs(x);
		}
	}
	//normalised_factor is calculates based on below formula where limit denotes 5000.
	//With the help of this normalised_factor we will increase the amplitute of the wave form if max value is less than 5000.
	//Similary we will reduce amplitude of wave form if max value is more than 5000.
	normalisation_Factor = limit/max;
	rewind(input_file);
	while(!feof(input_file))
	{
		fgets(singlevalue, 100, input_file);
		double x = atoi(singlevalue);
		//Before performing the multiplication operation, we will subtract the dcshift from the values present in the file to get more accurate results.
		int normalised_output = floor((x-dc_Shift) * normalisation_Factor);
		fprintf(output_normalisation, "%d\n", normalised_output);
	}
}

//The below function will compute the energy and zcr by considering the normalised data present in file pointed by input_file.
//Will store the output in file pointed by the output_file.
void energy_zcr(FILE *input_file,FILE *output_file)
{
	char single[100];
	//initialising the rquired variables.
	//previous_e denotes the energy of previous frame
	//e denotes the energy of the current frame.
	//count variable will help to keep track no of samples in a frame.
	double energy,zcr,count,previous_e,e,average_energy;
	count=energy=zcr=previous_e=e=average_energy=0.0;
	frames_count=0;
	//Making the file pointers to pint to the begining of the file.
	rewind(input_file);
	rewind(output_file);
	while(!feof(input_file))
	{
		fgets(single, 100, input_file);
		if(!isalpha(single[0]))
		{
	         e = atoi(single);
			 //We are considering 100 samples in a single frame.
			 //For every 100 values we compute average energy and zcr and store it in output file.
			 //After count reaching 100 we will initailize the zcr and energy.
			 if(count == 100)
			 {
				average_energy=energy/100.0;
				fprintf(output_file, "%lf %lf\n",average_energy, zcr);
				zcr = 0;
				energy = 0;
				count = 0;
			 }
			 //average energy is calculated by squaring the values and then taking the average of them.
		     energy =energy+(e*e);
			 //zcr refers to zero cross rating which means when two consecutive values of opposite sign we increment the value of zcr.
			 if((previous_e*e)<0)
			 {
				  zcr++;
			 }
			 count++;
			 //Current energy will become previous energy for next iteration.
			 previous_e=e;
		}
	}
	//Making the pointer of output_file to point to the begining of output_file.
	rewind(output_file);
	char *index;
	//satrt varaible refers to indices of enery_value and zcr_value arrays.
	//energy_value arry will store the average energy of frames,where as zcr_value will store the zcr count in that particular frame.
	long start=0;
	//Here we are storing the zcr and energy values in arrays by copying from output_file.
	while(!feof(output_file))
	{
		fgets(single, 100, output_file);
		energy_value[start] = strtod(single, &index);
		zcr_value[start] = atof(index + 1);
		start++;
	}
	//frames_count will store the count of the frames which will help in further computation.
	frames_count=start;
}
//The below function will mark the begining and ending of the word present in the file.
void mark_start_end()
{
     //Here we are considering  initial 10 frames of the file as noise frames.
	 // It is assumed in the initial 10 frames consists of only ambient noise.
	 //With the help of this ambient noise we will compute threshold energy and zcr.
	 double noise_frames=10;
	 //Initialising the required variables.
     threshold_Zcr= threshold_Energy=0.0;
	 for(int i=0; i<noise_frames; ++i)
	 {
		 threshold_Energy += energy_value[i];
		 threshold_Zcr += zcr_value[i];
	 }
	 //Computiong average energy and zcr count of initial 10 frames.
	 threshold_Energy =threshold_Energy/ noise_frames;
	 threshold_Zcr = threshold_Zcr/ noise_frames;
	 //if value of flag is made 1,  begining of word is marked.
	 //if value of flag is made 0,  ending of word is marked.
	 int flag=0;
	 //initial few frames are not considered beacause those may contain disturbences from microphone.
	 //if consecutively 3 frames have energy_value greater than threshold_energy*7 we consider it has begining of word.
	 //Similarly if 3 frames consecutively have ennergy_value less that threshold*3 we consider it has ending of word.
	 for(int k=7; k<frames_count-7; k++)
	 {
		 if(!flag && energy_value[k+1] > (threshold_Energy *7) && energy_value[k+2] > (threshold_Energy *7) && energy_value[k+3] > (threshold_Energy *7))
		 {
					begin = k;
					flag = 1;
		 }
		 else if(flag && energy_value[k-1] <= threshold_Energy * 3 && energy_value[k-2] <= threshold_Energy * 3 && energy_value[k-3] <= threshold_Energy * 3)
		 {
				end = k;
				flag = 0;
				break;
		 }
	 }
	 //If ending of the word is not identified we consider last frame as ending of word.
	 if(flag == 1) 
	 {
		end = frames_count - 1;
	 }
}

void  identify_word(FILE *input_file, long begin, long end)
{
	//Making input_file pointer to point the begining of file.
	rewind(input_file);
	//Initialise required variables.
	double count_frames = 0,energy =0 , zcr = 0,fricatives = 0;
	int i=begin;
	//If a particular frame has more than 10 zcr_value we consider it has an fricative.
	while(i<=end)
	{
		if(zcr_value[i]>10)
		{
			fricatives++;
		}
		i++;
	}
	count_frames=end-begin+1;
	//If more than 30 percent of frames are identified as fricatives we consider it as "yes" else "no".
	if(fricatives >= count_frames * 0.3)
		printf("Predicted Word in input audio file: Yes");
	else
		printf("Predicted Word in input audio file : NO");
}

int main()
{
	
	int choice=0;
	printf("Enter the choice of input audio file\n");
	printf("Enter 1 for Sample_1(No) file\n");
	printf("Enter 2 for Sample_2(Yes) file\n");
	printf("Enter 3 for Sample_3(No) file\n");
	printf("Enter 4 for Sample_4(Yes) file\n");
	printf("Enter 5 for Sample_5(No) file\n");
	printf("*********************************************************************************************************************************************************\n");
	scanf("%d",&choice);

	switch(choice)
	{
	     case 1: input_file2=fopen("Sample_1(No).txt","r");
		   break;
	     case 2: input_file2=fopen("Sample_2(Yes).txt","r");
		   break;
         case 3: input_file2=fopen("Sample_3(No).txt","r");
		   break;
		 case 4: input_file2=fopen("Sample_4(Yes).txt","r");
		   break;
		 case 5: input_file2=fopen("Sample_5(No).txt","r");
		   break;
		 default : printf("Please enter valid choice");
				break;
	}
	
	//File dc.txt will help us to find dc shift and correct it.
	//dc shift value depends on the hardware, dc.txt audio file is recorded by disabling the microphone of the system.
	input_file1=fopen("dc.txt","r");
	//normalised_input.txt and energy_zcr.txt files have been opened in "w+" mode beacuse it enables the file to be rewritten whenever we rerun the programe.
	input_normalisation=fopen( "normalised_input.txt", "w+");
	input_energy_zcr=fopen("energy_zcr.txt","w+");
	//The below function will compute dcshift.
	calculate_dcshift(input_file1);
	//The below programe will perform noramlisation on the file selected by the user.
	apply_normalisation(input_file2,input_normalisation);
	//Below function will compute energy and zcr values based on the normalised data.
	energy_zcr(input_normalisation, input_energy_zcr);
	//Below function will mark the starting and ending of the word present in the file.
	mark_start_end();
	//Below function will print the output.
	identify_word(input_energy_zcr,begin,end);
	//Clsoing all the files whicheverare opened by this programe.
	fclose(input_energy_zcr);
	fclose(input_file1);
	fclose(input_file2);
	fclose(input_normalisation);
	getch();
}





