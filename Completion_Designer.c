#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "GravelPackSlotSize.h"


struct seive_analysis_def
{
	int standardSeiveSize;
	double seiveSizeInches;
	double percentOfSamplePassing;
};
typedef struct seive_analysis_def seive;

struct seive_to_inches_def
{
	int standardSeiveSize;
	double seiveSizeInches;
};
typedef struct seive_to_inches_def StoI;

int readSeiveData(FILE *ifp, seive seiveData[]);
void seiveSizeToInches(seive seiveData[], int numberSeiveData);
double findMedianGrainSize(seive *seiveData, int numberSeiveData);

int main(int argc, char *argv[])
{
	//33 standard seive sizes
	FILE *ifp = NULL;
	FILE *ofp = NULL;
	int numberWells;
	int numberSeiveData[10];
	double medianGrainSize[10];
	double smallestGrainSize = 1;
	int i;
	int gravelPackSizeInteger;
	char gravelPackSizeString[50];
	char buffer[50];
	int slotWidth;


	//check to see if the input and output files were entered on the command line
	if (argc != 3)
	{
		printf("Syntax Error: Expected Input ./<exec> <inFile> <outFile>\n");
		exit(1);
	}

	//check to see if the input file can be read
	ifp = fopen(argv[1], "r");
	if (ifp == NULL)
	{
		printf("Unable to read file %s\n", argv[1]);
		exit(1);
	}

	//scan in the number of wells to analyze
	//declare the 2D arrays to hold the well data
	fscanf(ifp, "%d", &numberWells);
	seive seiveData[numberWells][34];
	char wellName[numberWells][50];
	char fieldName[numberWells][50];
	char APINumber[numberWells][20];

	//scan in well data from input file	
	for (i = 0; i < numberWells; i++)
	{
		fscanf(ifp, "%s", wellName[i]);
		fscanf(ifp, "%s", fieldName[i]);
		fscanf(ifp, "%s", APINumber[i]);
		numberSeiveData[i] = readSeiveData(ifp, seiveData[i]);
	}
	fclose(ifp);

	//convert standard seive size to inches to find the grain size distribution
	for (i = 0; i < numberWells; i++)
	{
		seiveSizeToInches(seiveData[i], numberSeiveData[i]);
	}

	//find the median grain size of the samples to use to determain gravel pack size
	for (i = 0; i < numberWells; i++)
	{
		medianGrainSize[i] = findMedianGrainSize(seiveData[i], numberSeiveData[i]);
	}

	//take the worst case scenario (the smallest grain size)
	for (i = 0; i < numberWells; i++)
	{
		if (medianGrainSize[i] < smallestGrainSize)
		{
			smallestGrainSize = medianGrainSize[i];
		}
	}


	//determine the gravel pack size using the header file attached
	gravelPackSizeInteger = findGravelPackSize(smallestGrainSize);

	if (gravelPackSizeInteger < 0)
	{
		exit(1);
	}

	sprintf(buffer, "%d", gravelPackSizeInteger);
	

	//formats the string to standard representation of Gravel Pack size
	if (strlen(buffer) < 4)
	{
		strncpy(gravelPackSizeString, buffer, 1);
		strcat(gravelPackSizeString, " X ");
		strcat(gravelPackSizeString, &buffer[1]);
	}

	else
	{
		strncpy(gravelPackSizeString, buffer, 2);
		strcat(gravelPackSizeString, " X ");
		strcat(gravelPackSizeString, &buffer[2]);
	}

	//determine slotted liner slot size using the header file attached
	slotWidth = findSlotWidth(gravelPackSizeInteger);

	if (gravelPackSizeInteger < 0)
	{
		exit(1);
	}

	//print out the report
	ofp = fopen(argv[2], "w");

	fprintf(ofp, "Based on the data provided form the following wells:\n");

	for (i = 0; i < 70; i++)
	{
		fprintf(ofp,"-");
	}
	fprintf(ofp, "\n");

	for (i = 0; i < numberWells; i++)
	{
		fprintf(ofp, "%s\n", wellName[i]);
		fprintf(ofp, "%s\n", fieldName[i]);
		fprintf(ofp, "%s\n", APINumber[i]);
		fprintf(ofp, "\n");
	}

	fprintf(ofp, "\nThe following worst case scenario was found:\n");

	for (i = 0; i < 70; i++)
	{
		fprintf(ofp,"-");
	}
	fprintf(ofp, "\n");

	fprintf(ofp, "The smallest formation median grain size was:  %.4lf\n\n\n", smallestGrainSize);

	fprintf(ofp, "Based on this I recommend using the following completion design:\n");

	for (i = 0; i < 70; i++)
	{
		fprintf(ofp,"-");
	}
	fprintf(ofp, "\n");

	fprintf(ofp, "Gravel Pack Size: %s\n", gravelPackSizeString);
	fprintf(ofp, "Slotted Liner Size: %d Mesh\n", slotWidth);

	fprintf(ofp, "\n\nThank you for using the Completion Designer Program!!\n");
	fprintf(ofp, "Have a wonderful day!!");
	
	return 0;
}


//function to read in the seive analysis data from the input file
int readSeiveData(FILE *ifp, seive seiveData[])
{
	int numberSeiveData = 0;
	int i;

	fscanf(ifp, "%d", &numberSeiveData);

	for (i = 0; i < numberSeiveData * 2; i++)
	{
		fscanf(ifp, "%d", &seiveData[i].standardSeiveSize);
		fscanf(ifp, "%lf", &seiveData[i].percentOfSamplePassing);
	}

	return numberSeiveData;
}


//function to convert the seive sizes read in to inches using the attached standard_seive_sizes.txt file
void seiveSizeToInches(seive seiveData[], int numberSeiveData)
{
	FILE *fp = NULL;
	StoI standardData[34];
	int i;
	int j;

	fp = fopen("standard_seive_sizes.txt", "r");

	if (fp == NULL)
	{
		printf("Unable to read file standard_seive_sizes.txt");
		exit(1);
	}

	for (i = 0; i < 33; i++)
	{
		fscanf(fp, "%d", &standardData[i].standardSeiveSize);
		fscanf(fp, "%lf", &standardData[i].seiveSizeInches);
	}

	for (i = 0; i < numberSeiveData; i++)
	{
		for(j = 0; j < 33; j++)
		{
			if (seiveData[i].standardSeiveSize == standardData[j].standardSeiveSize)
			{
				seiveData[i].seiveSizeInches = standardData[j].seiveSizeInches;
			}
		}
	}
	
	fclose(fp);

	return;
}

//find the median grain size of the sample analyses given
double findMedianGrainSize(seive *seiveData, int numberSeiveData)
{
	double medianGrainSize;
	int i = -1;
	double percentOfSamplePassing;

	do 
	{
		i++;
		percentOfSamplePassing = seiveData[i].percentOfSamplePassing;
	} while (percentOfSamplePassing >= 50);
	
	medianGrainSize = seiveData[i].seiveSizeInches;

	return medianGrainSize;
}