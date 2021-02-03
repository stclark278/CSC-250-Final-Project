#include <stdio.h>

//function to determine the Gravel Pack Size
//Median Gravel Pack Grain Size = (between 5 or 6) * Median Formation Grain Size
//standad_gravel_pack_sizes.txt is an editable text file for available gravel pack information
int findGravelPackSize(double smallestGrainSize)
{
	FILE *fp = NULL;
	int gravelPackSizeInteger;
	int i;
	int standardGravelPackSizes[6];
	double standardGravelPackMedianGrainSize[6];
	

	fp = fopen("standard_gravel_pack_sizes.txt", "r");

	if (fp == NULL)
	{
		printf("Unable to read file standard_gravel_pack_sizes.txt\n");
		return -1;
	}

	for (i = 0; i < 5; i++)
	{
		fscanf(fp, "%d", &standardGravelPackSizes[i]);
		fscanf(fp, "%lf", &standardGravelPackMedianGrainSize[i]);
	}

	fclose(fp);

	//used 5* instead of 6* because this is the worst case scenario and gravel packs should be designed for worst case scenarios
  i = 0;
	while(standardGravelPackMedianGrainSize[i] > 5 * smallestGrainSize && i < 5)
	{
		i++;
	}

	gravelPackSizeInteger = standardGravelPackSizes[i-1];

	return gravelPackSizeInteger;
}


//function to determin Slot Width based off of gravel pack Size
//standard_slot_widths.txt is a editable file to input available slot sizes
int findSlotWidth(int gravelPackSizeInteger)
{
	FILE *fp = NULL;
	int i;
	int standardGravelPackSizes[6];
	double standardGravelPackMedianGrainSize[6];
	double medianGravelPackSize;
	double standardSlotWidths[9];
	int slotWidth;

	fp = fopen("standard_gravel_pack_sizes.txt", "r");

	if (fp == NULL)
	{
		printf("Unable to read file standard_gravel_pack_sizes.txt\n");
		return -1;
	}

	for (i = 0; i < 5; i++)
	{
		fscanf(fp, "%d", &standardGravelPackSizes[i]);
		fscanf(fp, "%lf", &standardGravelPackMedianGrainSize[i]);
	}

	i = 0;
	while(standardGravelPackSizes[i] != gravelPackSizeInteger)
	{
		i++;
	}

	medianGravelPackSize = standardGravelPackMedianGrainSize[i];

	fp = NULL;
	fp = fopen("standard_slot_widths.txt", "r");

	if (fp == NULL)
	{
		printf("Unable to read file standard_slot_widths.txt\n");
		return -2;
	}

	for (i = 0; i < 8; i++)
	{
		fscanf(fp, "%lf", &standardSlotWidths[i]);
	}

	fclose(fp);

	//desired slot width is .5 * Median Gravel Pack Size rounded down to the next available slot size for worst case scenario.
	i = 0;
	while(standardSlotWidths[i] > .5 * medianGravelPackSize && i < 8)
	{
		i++;
	}

	//convert slot size inches to standard slot mesh size
	slotWidth = 1000 * standardSlotWidths[i];

	return slotWidth;
}