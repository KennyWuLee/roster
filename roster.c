#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Student.h"

int compareStudents(Student s1, Student s2)
{
	int diff = strcmp(s1.last, s2.last);
	if(diff)
		return diff;
	else
		return strcmp(s1.first, s2.first);
}

void insert(Student** root, Student* node)
{
	if(*root == NULL)
		*root = node;
	else if(compareStudents(*node, **root) < 0)
		insert(&((*root)->left), node);
	else
		insert(&((*root)->right), node);
}

void printInOrder(Student* root)
{
	char name[26];
	if(root)
	{
		printInOrder(root->left);
		sprintf(name, "%s %s", root->first, root->last);
		printf("%-25sPoints: %d\tYear: %d House: %s\n", name, root->points, root->year, HOUSE_NAMES[root->house]);
		printInOrder(root->right);
	}
}

Student* search( Student* root, char* first, char* last );

Student* delete( Student** root, char* first, char* last );

House getHouse(char* house)
{
	int i = 0;
	for(; i < HOUSES; ++i)
		if (strcmp(HOUSE_NAMES[i], house) == 0)
			return i;
	return -1;
}

int add(Student* houses[HOUSES], char* first, char* last, int points, int year, char* house)
{
	if(year < 1 || year > 7)
	{
		printf("Add failed. Invalid year: %d\n", year);
		return 0;
	}
	House h = getHouse(house);
	if(h == -1)
	{
		printf("Add failed. Invalid house: %s\n", house);
		return 0;
	}

	//todo call search on student;

	Student* temp = (Student*) malloc(sizeof(Student));
	temp->first = strdup(first);
	temp->last = strdup(last);
	temp->points = points;
	temp->year = year;
	temp->house = h;

	insert(&(houses[h]), temp);

	return 1;
}


int main()
{
	//houses
	Student* houses[HOUSES];
	int i = 0;
	for(; i < HOUSES; ++i)
		houses[i] = NULL;

	Student* dead = NULL;

	while(1)
	{
		printf("Enter command: ");
		char command[100];
		scanf("%s", command);
		if(strcmp(command, "quit") == 0)
			return 0;
		else if(strcmp(command, "add") == 0)
		{
			char first[25], last[25], house[25];
			int points = 0, year = 0;

			scanf("%s", first);
			scanf("%s", last);
			scanf("%d", &points);
			scanf("%d", &year);
			scanf("%s", house);

			if (add(houses, first, last, points, year, house))
				printf("Added %s %s to roster.\n", first, last);
		}
		else if(strcmp(command, "inorder") == 0)
		{
			printf("Inorder print-out of roster:\n\n");
			for(i = 0; i < HOUSES; ++i)
			{
				printf("%s House\n\n", HOUSE_NAMES[i]);
				printInOrder(houses[i]);
				printf("\n");
			}
			printf("Deceased\n\n");
			printInOrder(dead);
			printf("\n\n");
		}
		else
		{
			printf("Unknown command: %s\n", command);
		}
		while(getchar() != '\n');
	}
}

