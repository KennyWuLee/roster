/*******************************/
/*
/*  Project Name: Trees Worse than Whomping Willows
/*  Description: store hogwarts student data
/*  File names: roster.c Student.h makefile
/*  Date: 04/11/2014
/*  Authors: Tim Miranda, Till Krischer
/*
/*******************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Student.h"

/*
   Description: Compares names of two students

   Parameters:  first1		- First name of the first student
   				last1		- Last name of the first student
				first1		- First name of the second student
   				last1		- Last name of the second student


   Returns:	    a negative number if student1's name comes before student2's name
   				0 if they are the same
   				a positive number if student1's name comes after student2's name
*/
int compareName(char* first1, char* last1, char* first2, char* last2)
{
	int diff = strcmp(last1, last2); // records the difference between the last names
	if(diff)			//if it is non zero, then they are not the same
		return diff;
	else
		return strcmp(first1, first2); // if last names are the same, it checks first names
}

/*
   Description: Inserts a student into the tree root

   Parameters:  root	- tree to insert in
   				node	- Student to insert
*/
void insert(Student** root, Student* node)
{
	// checks to see if tree is empty
	//and then determines correct location to insert
	if(*root == NULL)
		*root = node;
	else if(compareName(node->first, node->last, (*root)->first, (*root)->last) < 0)
		insert(&((*root)->left), node);
	else
		insert(&((*root)->right), node);
}

/*
   Description: prints Students in a tree in order

   Parameters:  root 	- root of the tree to print

*/
void printInOrder(Student* root)
{
	char name[26];// to contain both first and last name
	// basic recursive inorder traversal
	if(root)
	{
		printInOrder(root->left);
		sprintf(name, "%s %s", root->first, root->last);
		printf("%-25sPoints: %d\tYear: %d House: %s\n", name, root->points, root->year, HOUSE_NAMES[root->house]);
		printInOrder(root->right);
	}
}

/*
   Description: prints Students in a tree in preorder

   Parameters:  root 	- root of the tree to print

*/
void printPreOrder(Student* root)
{
	char name[26];
	// basic recursive preorder traversal
	if(root)
	{

		sprintf(name, "%s %s", root->first, root->last);
		printf("%-25sPoints: %d\tYear: %d House: %s\n", name, root->points, root->year, HOUSE_NAMES[root->house]);
		printPreOrder(root->left);
		printPreOrder(root->right);
	}
}

/*
   Description: prints Students in a tree in postorder

   Parameters:  root 	- root of the tree to print

*/
void printPostOrder(Student* root)
{
	char name[26];
	// basic recursive postorder traversal
	if(root)
	{
		printPostOrder(root->left);
		printPostOrder(root->right);
		sprintf(name, "%s %s", root->first, root->last);
		printf("%-25sPoints: %d\tYear: %d House: %s\n", name, root->points, root->year, HOUSE_NAMES[root->house]);

	}
}

/*
   Description: Searches a student inside of a binary tree

   Parameters:  root		- tree to search in
   				first 		- first name of student thats searched
   				last 		- last name of student thats searched

   Returns:	    pointer to student on success,
   				null pointer, if student is not found
*/
Student* search( Student* root, char* first, char* last )
{
	if(root!=NULL)
	{
		// checks to see if the name comes before or after current student
		if(strcmp(root->last,last) > 0)
			return search(root->left, first, last);
		else if(strcmp(root->last,last)<0)
			return search(root->right, first, last);
		else
		{
			// if the last names match, then check first names
			if(strcmp(root->first,first) > 0)
				return search(root->left, first, last);
			else if(strcmp(root->first,first) < 0)
				return search(root->right, first, last);
			else
				return root;
		}
	}
	return NULL;
}

/*
   Description: Deletes a student from a binary tree

   Parameters:  root		- tree to search in
   				first 		- first name of student thats deleted
   				last 		- last name of student thats deleted

   Returns:	    pointer to deleted student on success,
   				null pointer, if student is not found
*/
Student* delete(Student** root, char* first, char* last)
{
	//return NULL if empty tree or Student not found
    if(*root == NULL)
        return NULL;
    //find student recursively;
    if(compareName(first, last, (*root)->first, (*root)->last) < 0)
        return delete(&((*root)->left), first, last);
    else if(compareName(first, last, (*root)->first, (*root)->last) > 0)
        return delete(&((*root)->right), first, last);
    // found right node
    else
    {
    	//safe student to return later
        Student* temp = (*root);
        if((*root)->left && (*root)->right) //two children
        {
        	//lookfor succesor to deleted node, starting at right child
            Student* parentleftmost = (*root)->right;
            if(parentleftmost->left)
            {
            	//find the parent of the leftmost node
            	while(parentleftmost->left->left)
                	parentleftmost = parentleftmost->left;
                //save a pointer to the leftmost node
                Student** leftmost = &(parentleftmost->left);
            	Student* temp2 = *leftmost;
            	//move the right child of leftmost node up.
           		(*leftmost) = (*leftmost)->right;
           		//change the childs of the leftmost node to the children of root
           		temp2->right = (*root)->right;
           		temp2->left = (*root)->left;
           		//put leftmostnode into root;
           		*root = temp2;
           	}
           	//if right child is succesor, it becomes the new node
            else
            {
            	parentleftmost->left = (*root)->left;
            	(*root) = parentleftmost;
            }
        }
        else if((*root)->left) //only left child
        	//move the left child up
            *root = (*root)->left;
        else if((*root)->right) //only right child
        	//move the right child up
            *root = (*root)->right;
        else  // leaf node
        	//delete the node
            *root = NULL;
        return temp;
    }
}

/*
   Description: Converts a string to the right House

   Parameters:  House 		-  String to convert

   Returns:	    House on success
   				-1 when the String is not a house
*/
House getHouse(char* house)
{
	int i = 0;
	// loops through the number of houses to see if input is a match
	for(; i < HOUSES; ++i)
		if (strcmp(HOUSE_NAMES[i], house) == 0)
			return i;
	return -1;
}

/*
   Description:  Takes information about a student, checks if it's valid, then creates
   					the student and adds him to the right house

   Parameters:  houses[HOUSES]		- Array containing the trees for the different houses
   				first 				- first name of the student
   				last 				- last name of the student
   				points 				- points of the student
   				year 				- year in school of the student
   				house 				- house of the student

   Returns:	    1 on success, 0 on fail
*/
int add(Student* houses[HOUSES], char* first, char* last, int points, int year, char* house)
{
	//checks to seee if year is within range
	if(year < 1 || year > 7)
	{
		printf("Add failed. Invalid year: %d\n", year);
		return 0;
	}
	House h = getHouse(house);
	// checks to see if it is a valid house
	if(h == -1)
	{
		printf("Add failed. Invalid house: %s\n", house);
		return 0;
	}

	if(search(houses[h], first, last))
	{
		printf("Add failed. Student named %s %s already present in roster.\n", first, last);
		return 0;
	}

	Student* temp = (Student*) malloc(sizeof(Student));
	temp->first = strdup(first);
	temp->last = strdup(last);
	temp->points = points;
	temp->year = year;
	temp->house = h;

	insert(&(houses[h]), temp);

	return 1;
}

/*
   Description: writes all students from a tree into a file in preorder

   Parameters:  file 		- FILE pointer to write to
   				root 		- tree containing the students
*/
void savePreOrder(FILE* file, Student* root)
{
	// basic preorder traversal, but printing to a file
	if(root)
	{
		fprintf(file, "%s %s %d %d %s\n", root->first, root->last, root->points, root->year, HOUSE_NAMES[root->house]);
		savePreOrder(file, root->left);
		savePreOrder(file, root->right);
	}
}

/*
   Description: This recursive function sums up all the scores in a given tree

   Parameters:  root - the root of the given tree
	    sum  - the running total of the scores

   Returns:	    the sum of the current sum plus the current node's score
*/
int score(Student* root, int sum)
{
    if(root!=NULL)
    {
    	// used an inorder traversal to visit each node
        sum = score(root->left, sum);
        sum+=root->points;
        sum = score(root->right, sum);
    }
    return sum;

}

/*
   Description: clears all students from a tree.

   Parameters:  root 		- tree containing the students
*/
void clear(Student* root)
{
	//only clear if there is a root
	if(root)
	{
		//clear the children first
		clear(root->left);
		clear(root->right);
		//free the strings of the students
		free(root->first);
		free(root->last);
		//free the student
		free(root);
	}
}

int main()
{
	//Array holding the 4 trees for the houses
	Student* houses[HOUSES];
	//initialize roots with NULL
	int i = 0;
	for(; i < HOUSES; ++i)
		houses[i] = NULL;

	//tree for dead students, initialized with NULL
	Student* dead = NULL;

	//main loop reading the input and doing the appropriate action
	while(1)
	{
		printf("Enter command: ");
		//read the userinput into command
		char command[100];
		scanf("%s", command);
		//check for the different commands
		if(strcmp(command, "quit") == 0)
		{
			//loop through houses and clear them all.
			for(i = 0; i < HOUSES; ++i)
				clear(houses[i]);
			//clear the dead tree
			clear(dead);
			printf("All data cleared.\n");
			return 0;
		}
		else if(strcmp(command, "help") == 0)
		{
			printf("help\nPrints this display\n\n");
			printf("load <filename>\nAdds the contents of a file to the current roster\n\n");
			printf("save <filename>\nSaves the current roster to a file\n\n");
			printf("clear\nClear the current roster\n\n");
			printf("inorder\nPrint out an inorder traversal of the roster for each house\n\n");
			printf("preorder\nPrint out a preorder traversal of the roster for each house\n\n");
			printf("postorder\nPrint out a postorder traversal of the roster for each house\n\n");
			printf("add <firstname> <lastname> <points> <year> <house>\nAdds a student to the roster\n\n");
			printf("load <filename>\nAdds the contents of a file to the current roster\n\n");
			printf("kill <firstname> <lastname> <house>\nMoves a student to the deceased list\n\n");
			printf("find <firstname> <lastname> <house>\nSearches for a student in a house\n\n");
			printf("points <firstname> <lastname> <house> <points>\nChanges the points a student has earned by the specified amount\n\n");
			printf("score\nLists the point totals for all four houses\n\n");
			printf("quit\nQuits the program\n\n");
		}
		else if(strcmp(command, "load") == 0)
		{
			//read filename from user
			char filename[100];
			scanf("%s", filename);

			//open file for read
			FILE* file = fopen(filename, "r");
			//if open was successfull
			if (file)
			{
				char first[25], last[25], house[25];
				int points = 0, year = 0;
				//read student info into variables until end of file
				while(fscanf(file, "%s %s %d %d %s", first, last, &points, &year, house) != EOF)
					//add the student
					add(houses, first, last, points, year, house);
				fclose(file);
				printf("Successfully loaded data from file %s.\n", filename);
			}
			else
				printf("Load failed. Invalid file: %s\n", filename);

		}
		else if(strcmp(command, "save") == 0)
		{
			//read filename from user
			char filename[100];
			scanf("%s", filename);

			//open file for write
			FILE* file = fopen(filename, "w");
			//if open was successfull
			if (file)
			{
				//loop through houses and save every student to the file
				for(i = 0; i < HOUSES; ++i)
					savePreOrder(file, houses[i]);
				fclose(file);
				printf("Successfully saved data to file %s.\n", filename);
			}
			else
				printf("Save failed. Invalid file: %s\n", filename);

		}
		else if(strcmp(command, "clear") == 0)
		{
			//loop trough houses and clear the trees
			for(i = 0; i < HOUSES; ++i)
			{
				clear(houses[i]);
				//set root to NULL
				houses[i] = NULL;
			}
			//claer dead tree and set it's root to NULL
			clear(dead);
			dead = NULL;
			printf("All data cleared.\n");

		}
		else if(strcmp(command, "inorder") == 0)
		{
			printf("Inorder print-out of roster:\n\n");
			//loop through houses and print them inorder
			for(i = 0; i < HOUSES; ++i)
			{
				printf("%s House\n\n", HOUSE_NAMES[i]);
				printInOrder(houses[i]);
				printf("\n");
			}
			//print dead students
			printf("Deceased\n\n");
			printInOrder(dead);
			printf("\n\n");
		}
		else if(strcmp(command, "preorder") == 0)
		{
			printf("Preorder print-out of roster:\n\n");
			//loop through houses and print them preorder
			for(i = 0; i < HOUSES; ++i)
			{
				printf("%s House\n\n", HOUSE_NAMES[i]);
				printPreOrder(houses[i]);
				printf("\n");
			}
			//print dead students
			printf("Deceased\n\n");
			printPreOrder(dead);
			printf("\n\n");
		}
		else if(strcmp(command, "postorder") == 0)
		{
			printf("Postorder print-out of roster:\n\n");
			//loop through houses and print them postorder
			for(i = 0; i < HOUSES; ++i)
			{
				printf("%s House\n\n", HOUSE_NAMES[i]);
				printPostOrder(houses[i]);
				printf("\n");
			}
			//print dead students
			printf("Deceased\n\n");
			printPostOrder(dead);
			printf("\n\n");
		}
		else if(strcmp(command, "add") == 0)
		{
			char first[25], last[25], house[25];
			int points = 0, year = 0;

			//read userinput int variables
			scanf("%s", first);
			scanf("%s", last);
			scanf("%d", &points);
			scanf("%d", &year);
			scanf("%s", house);

			//add the student, if the input data was fine print success message
			if (add(houses, first, last, points, year, house))
				printf("Added %s %s to roster.\n", first, last);
		}
		else if(strcmp(command, "kill") == 0)
		{
			//read userinput into variables
			char first[25], last[25], house[25];
			scanf("%s %s %s", first, last, house);
			//check if the entered house is correct
			House h = getHouse(house);
			if(h == -1)
				printf("Kill failed. Invalid house: %s\n", house);
			else
			{
				//delete student from the right tree, save deleted student in temp
				Student* temp = NULL;
				temp = delete(&houses[h], first, last);
				//if delete was succesfule add temp student to dead-tree
				if(temp)
				{
					//set temp-student's children to NULL before adding
					temp->left = NULL;
					temp->right = NULL;
					insert(&dead, temp);
				}
				else
					printf("Kill failed. %s %s was not found in House %s\n", first, last, house);
			}
		}
		else if(strcmp(command, "find")==0)
		{
			//read userinput into variables
			char first[25], last[25], house[25];
			char name[26];
			scanf("%s", first);
			scanf("%s", last);
			scanf("%s", house);
			House h = getHouse(house);
			Student* temp = NULL;
			//check if the entered house is correct
			if(h == -1)
				printf("Find failed. Invalid house: %s\n", house);
			else
			{
				//do the search, store result in temp
				temp = search(houses[h], first, last);
				if(temp == NULL)
					printf("Find Failed. %s %s was not found in %s House\n", first, last, house);
				else
				{
					//if a student was found print the info
					sprintf(name, "%s %s", temp->first, temp->last);
					printf("%-25sPoints: %d\tYear: %d House: %s\n", name, temp->points, temp->year, HOUSE_NAMES[temp->house]);
				}
			}

		}
		else if(strcmp(command, "points")==0)
		{
			//read userinput into variables
			char first[25], last[25], house[25];
			int number = 0;
			scanf("%s", first);
			scanf("%s", last);
			scanf("%s", house);
			scanf("%d", &number);
			House h = getHouse(house);
			Student* temp = NULL;
			//check if the entered house is correct
			if(h == -1)
				printf("Point change failed. Invalid house: %s\n", house);
			else
				//search for the student with the name
				temp = search(houses[h], first, last);
			if(temp == NULL)
				printf("Point change Failed. %s %s was not found in %s House\n", first, last, house);
			else
				//if the student was found add the points to his score
				temp->points += number;
		}
		else if(strcmp(command, "score")==0)
		{
			//calculate score for each house and print out
			printf("Point totals:\n\n");
            printf("Gryyfindor Score: \t%d\n", score(houses[0],0));
            printf("Ravenclaw Score: \t%d\n", score(houses[1],0));
            printf("Hufflepuff Score: \t%d\n", score(houses[2],0));
            printf("Slytherin Score: \t%d\n", score(houses[3],0));
		}
		else
		{
			//print unknown command message
			printf("Unknown command: %s\n", command);
		}
		//read until the rest of the line before reading in the next command
		while(getchar() != '\n');
	}
}

