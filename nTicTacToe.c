/*****************************************************************************************
Prologue

UCSC Extension: Advanced C Programming

Instructor: Rajainder A. Yeldandi

Author: Kevin Geiszler

Assignment Number: 6, Tic-Tac-Toe

Topics: Multidimensional Arrays

File Name: assignment6_KevinGeiszler.c

Date: 2/7/2017

Objective: This program creates an N-by-N tic-tac-toe board and prompts two users to play
		   the game.
		   
Comments:  A player wins by placing their symbol (X or O) N times in a row. This can be
		   done in any row, column, or major diagonal. The game board is printed to an
		   output file after each player makes a move. The game board is also shown in the
		   console, along with the prompt for each user.

*****************************************************************************************/

//Preprocessor directives
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DIM 4

/****************************************************************************************/

//Function prototypes
void displayWelcome(FILE *fptr);
void initializeBoard(char grid[][DIM]);
void showBoard(FILE *fptr, char grid[][DIM]);
int rowFull(char grid[][DIM], int row);
int anyRowFull(char grid[][DIM]);
int colFull(char grid[][DIM], int col);
int anyColFull(char grid[][DIM]);
int majorDiagonalFull(char grid[][DIM]);
int checkWinner(char grid[][DIM]);
int checkCell(char grid[][DIM], int *pRow, int *pCol);
int checkBoard(char grid[][DIM]);
void getUserInput(int *pSpot, char myString[]);
void promptPlayer(FILE *fptr, char grid[][DIM], int playerNum, int *pRow, int *pCol, char rString[], char cString[]);
void displayWinner(FILE *fptr, int winner);

/****************************************************************************************/

//The main function
int main(int argc, char *argv[])
{
	char grid[DIM][DIM]; //Used to hold the game board
	int row = 0;
	int col = 0;
	int winner = 0;
	int turnNumber = 0;
	char rString[4] = "row";
	char cString[7] = "column";
	FILE *pOut;
	
	//Check command line arguments
	if (argc != 2)
	{
		printf("Error! Need following file usage: %s outputFile\n", argv[0]);
		exit(1);
	}
	
	//Check if the output file has been successfully opened
	if ((pOut = fopen(argv[1], "w")) == NULL)
	{
		printf("Error opening %s.\n", argv[1]);
		exit(1);
	}
	
	initializeBoard(grid);
	displayWelcome(pOut);
	
	//Loop while there is NO winner and while the board is NOT full
	while(!(winner = checkWinner(grid)) && !(checkBoard(grid)))
	{			
		turnNumber++;
		showBoard(pOut, grid);
		printf("Turn number %d\n", turnNumber);	//Print to the console
		fprintf(pOut, "Turn number %d\n", turnNumber);	//Print to the output file
		promptPlayer(pOut, grid, 1, &row, &col, rString, cString); //Prompt Player 1
		grid[row][col] = 'X'; //Place an 'X' at the appropriate position on the board
		fprintf(pOut, "Player 1 placed an 'X' in (%d, %d):\n\n", row, col); //Print to the output file
		
		//If there is now a winner, or the board is now full, exit the loop
		if ((winner = checkWinner(grid)) || (checkBoard(grid)))
			break;
		
		showBoard(pOut, grid);
		promptPlayer(pOut, grid, 2, &row, &col, rString, cString); //Prompt Player 2
		grid[row][col] = 'O'; //Place an 'O' at the appropriate position on the board
		fprintf(pOut, "Player 2 placed an 'O' in (%d, %d):\n\n", row, col); //Print to the output file
	}
	
	showBoard(pOut, grid);
	displayWinner(pOut, winner);
		
	fclose(pOut);
	
	return 0;
}
/****************************************************************************************/

//Display a welcome message to the user

//Return value: None

void displayWelcome(FILE *fptr)
{
	//Print to the console
	printf("\nWelcome to Kevin Geiszler's Tic-Tac-Toe game! This game will be played on a "
		   "%d-by-%d grid. Good luck to both players! Here is the empty board:\n", DIM, DIM);
	
	//Print to the output file.
	fprintf(fptr, "Welcome to Kevin Geiszler's Tic-Tac-Toe game! This game will be played"
		   " on a %d-by-%d grid. Good luck to both players! Here is the empty board:\n\n", DIM, DIM);
}

/****************************************************************************************/

//Set each element on the game board to an empty spot, which is designated by '-'

//Return value: None

void initializeBoard(char grid[][DIM])
{
	int row, col;
	
	for (row = 0; row < DIM; row++)
		for (col = 0; col < DIM; col++)
			grid[row][col] = '-';
}

/****************************************************************************************/

//Display the game board to the console, and print the same display to a file

//Return value: None

void showBoard(FILE *fptr, char grid[][DIM])
{
	int row, col;
	
	//This block of code displays the game board to the console for the users to look at
	printf("\n");
	for (row = 0; row <= DIM; row++)
	{	
		for (col = 0; col <= DIM; col++)
		{
			if (row == 0 && col ==0)
				printf("     ");
		
			if (row == 0 && col != 0)
				printf(" Col%d", col - 1);
				
			if (row != 0 && col == 0)
				printf("Row%d ", row - 1);
				
			if (row != 0 && col != 0)
				printf("  %c  ", grid[row - 1][col - 1]);
		}
		printf("\n");
	}
	printf("\n");
	
	//This block of code prints the output to a file for review
	for (row = 0; row <= DIM; row++)
	{	
		for (col = 0; col <= DIM; col++)
		{
			if (row == 0 && col ==0)
				fprintf(fptr, "     ");
		
			if (row == 0 && col != 0)
				fprintf(fptr, " Col%d", col - 1);
				
			if (row != 0 && col == 0)
				fprintf(fptr, "Row%d ", row - 1);
				
			if (row != 0 && col != 0)
				fprintf(fptr, "  %c  ", grid[row - 1][col - 1]);
		}
		fprintf(fptr, "\n");
	}
	fprintf(fptr, "\n");
	
}

/****************************************************************************************/

/*
Check is a given row has been filled with characters from the same player. This function
is used inside of anyRowFull()

Return value: 1 if Player 1 (X) has N characters in a row in the given row
  			  2 if Player 2 (O) has N characters in a row in the given row
			  0 otherwise
*/
int rowFull(char grid[][DIM], int row)
{
	if (grid[row][0] == 'X' && grid[row][1] == 'X' &&
			 				   grid[row][2] == 'X' && grid[row][3] == 'X')
		return 1;
		
	else if (grid[row][0] == 'O' && grid[row][1] == 'O' &&
			 				   grid[row][2] == 'O' && grid[row][3] == 'O')
		return 2;
		
	else
		return 0;
}

/****************************************************************************************/

/*
Loop through each row and use the rowFull() function to check is a player has placed
N items in a row within a given row.

Return value: 1 if Player 1 (X) has N characters in a row in the given row
  			  2 if Player 2 (O) has N characters in a row in the given row
			  0 otherwise
*/
int anyRowFull(char grid[][DIM])
{
	for (int row = 0; row < DIM; row++)
	{
		if (rowFull(grid, row) == 1)
			return 1;
		else if (rowFull(grid, row) == 2)
			return 2;
	}
	
	return 0;
}

/****************************************************************************************/

/*
Check is a given column has been filled with characters from the same player. This function
is used inside of anyColFull()

Return value: 1 if Player 1 (X) has N characters in a column in the given column
  			  2 if Player 2 (O) has N characters in a column in the given column
			  0 otherwise
*/
int colFull(char grid[][DIM], int col)
{
	if (grid[0][col] == 'X' && grid[1][col] == 'X' &&
			 				   grid[2][col] == 'X' && grid[3][col] == 'X')
		return 1;
		
	else if (grid[0][col] == 'O' && grid[1][col] == 'O' &&
	 						   grid[2][col] == 'O' && grid[3][col] == 'O')
		return 2;
		
	else
		return 0;
}

/****************************************************************************************/

/*
Loop through each column and use the colFull() function to check is a player has placed
N items in a row within a given column.

Return value: 1 if Player 1 (X) has N characters in a column in the given column
  			  2 if Player 2 (O) has N characters in a column in the given column
			  0 otherwise
*/
int anyColFull(char grid[][DIM])
{
	for (int col = 0; col < DIM; col++)
	{
		if (colFull(grid, col) == 1)
			return 1;
		else if (colFull(grid, col) == 2)
			return 2;
	}
	
	return 0;
	
}

/****************************************************************************************/

/*
Loop through each major diagonal in the board to check for N elements in a row.

Return value: 1 if Player 1 (X) has N characters in a diagonal
  			  2 if Player 2 (O) has N characters in a diagonal
			  0 otherwise 
*/
int majorDiagonalFull(char grid[][DIM])
{
	int row, col;
	int numX = 0; //Counter for finding out how many X's there are in a row in the diagonal
	int numO = 0; //Counter for finding out how many O's there are in a row in the diagonal
	
	//Loop from top left of the board to bottom right of the board
	for (row = col = 0; row < DIM && col < DIM; row++, col++)
	{
		//If an element on the board contains 'X', add 1 to our "X counter"
		if (grid[row][col] == 'X')
			numX++;
		//If an element on the board contains 'O', add 1 to our "O counter"
		else if (grid[row][col] == 'O')
			numO++;
	}
	
	if (numX == DIM) //Return 1 if this major diagonal is filled with X's
		return 1;
	else if (numO == DIM) //Return 2 if this major diagonal is filled with X's
		return 2;
	
	//Reset the counters back to 0 so we can test the other major diagonal	
	numX = 0;
	numO = 0;
	
	//Loop from top right of the board to bottom left of the board
	for (row = 0, col = DIM - 1; row < DIM && col >= 0; row++, col--)
	{
		//If an element on the board contains 'X', add 1 to our "X counter"
		if (grid[row][col] == 'X')
			numX++;
		//If an element on the board contains 'O', add 1 to our "O counter"
		else if (grid[row][col] == 'O')
			numO++;
	}
		
	if (numX == DIM) //Return 1 if this major diagonal is filled with X's
		return 1;
	else if (numO == DIM) //Return 2 if this major diagonal is filled with X's
		return 2;
	else
		return 0; //Otherwise, neither major diagonal is filled with the same character
}

/****************************************************************************************/

/*
Check ifs a player has won by seeing if they have placed N elements in a row within a row,
column, or major diagonal.

Return value: 1 if Player 1 (X) has won
			  2 if Player 2 (O) has won
			  0 otherwise
*/
int checkWinner(char grid[][DIM])
{
	if (anyRowFull(grid) == 1 || anyColFull(grid) == 1 || majorDiagonalFull(grid) == 1)
		return 1;
	else if (anyRowFull(grid) == 2 || anyColFull(grid) == 2 || majorDiagonalFull(grid) == 2)
		return 2;
	else
		return 0;
}

/****************************************************************************************/

/*
Check if a cell is empty. A cell is considered empty if it contains the '-' character.
This function is used inside of promptPlayer().

Return value: 1 if the cell is empty
			  0 otherwise
*/
int checkCell(char grid[][DIM], int *pRow, int *pCol)
{
	if (grid[*pRow][*pCol] == '-')
		return 1;
	else
		return 0;
}

/****************************************************************************************/

/*
Check if the board is full. If there is even one cell that contains the '-' character,
then the board is not full.

Return value: 1 if the board if full
			  0 otherwise
*/

//Return 1 if the board is full, 0 otherwise
int checkBoard(char grid[][DIM])
{
	int row, col;
	
	for (row = 0; row < DIM; row++)
		for (col = 0; col < DIM; col++)
			if (grid[row][col] == '-')
				return 0;
	return 1;
}

/****************************************************************************************/

/*
Get the input from the user. The user is prompted to enter an integer, which corresponds
to the row or column pertaining to where they would like to make their move. This function
uses fgets() along with strtol() to get the input from the user and check for valid input.
This function is used inside of promptPlayer().

Return value: None
*/
void getUserInput(int *pSpot, char myString[])
{
	char input[20]; //This string is used in strtol()
	long lvalue = 0; //Declare a variable of type long since strtol() returns this type
	int ivalue = 0; //Declare a variable of type int so we can truncate our value
	char *end; //This pointer is used in strtol(). It points to the garbage values in the string

	while (1)
	{
		printf("Type the %s number (%d to %d) where you would like to place your move: ", myString, 0, DIM - 1);
	
		//Get the input from the user. Exit the program and display a message if there is
		//an error with fgets()
		if (fgets(input, sizeof(input), stdin) == NULL)
		{
			printf("Error with fgets() in getUserInput() function.\n");
			exit(1);
		}
		
		//The user is not allowed to enter a blank line
		if (input[0] == '\n')
		{
			printf("\nPlease don't enter a blank line.\n\n");
			continue;
		}
	
		//Remove '\n' that fgets() inserts	
		input[strlen(input) - 1] = '\0';
		
		//Set the extracted number equal to lvalue. 
		lvalue = strtol(input, &end, 10);
		
		//If the "strlen()th" element in input[] is not equal to end (usually '\O), then
		//the user entered invalid input.
		if (end != (input + strlen(input)))
		{
			printf("\nError! You must enter an integer.\n\n");
			continue;
		}

		//Truncate lvalue to type int. No reasonable tic-tac-toe board has a number of 
		//dimensions that require a variable of type long
		ivalue = (int)lvalue;

		//If the inputted value is out of range, prompt the user to re-enter.
		if ((ivalue < 0) || (ivalue > DIM - 1))
		{
			printf("\nError! This input is not between %d and %d.\n\n", 0, DIM - 1);
			continue;
		}
		else //If the input is valid, the user's input is placed in the respective address
		{
			*pSpot = ivalue;
			break;
		}
	}
}

/****************************************************************************************/

/*
Prompt the user to enter numbers so they can place their character onto the game board by
using getUserInput(). Use checkCell() to check if the cell is empty. If the cell if not
empty, then the user cannot place their character there.

Return value: None
*/
void promptPlayer(FILE *fptr, char grid[][DIM], int playerNum, int *pRow, int *pCol, char rString[], char cString[])
{	
	printf("It's Player %d's turn", playerNum); //Print to console
	fprintf(fptr, "It's Player %d's turn", playerNum); //Print to file
	
	if (playerNum == 1) //If we're prompting Player 1
	{
		printf(" (X)\n\n"); //Print to console
		fprintf(fptr, " (X)\n\n"); //Print to file
	}
	else if (playerNum == 2) //If we're prompting Player 2
	{
		printf(" (O)\n\n"); //Print to console
		fprintf(fptr, " (O)\n\n"); //Print to file
	}
	
	while(1)
	{
		getUserInput(pRow, rString); //Get row number input
		getUserInput(pCol, cString); //Get column number input
		
		//If the cell is not empty, display an error message	
		if (!checkCell(grid, pRow, pCol))
			printf("\nThat spot has been taken. Please try again.\n\n");
		else
			break;
	}
}

/****************************************************************************************/

/*
Display the winner to the console and write the same message to a file.

Return value: None
*/
void displayWinner(FILE *fptr, int winner)
{
	if (winner == 1) //If Player 1 wins
	{
		printf("Player 1 (X) wins!\n"); //Print to console
		fprintf(fptr, "Player 1 (X) wins!\n"); //Print to file
	}
	else if (winner == 2) //If Player 2 wins
	{
		printf("Player 2 (O) wins!\n"); //Print to console
		fprintf(fptr, "Player 2 (O) wins!\n"); //Print to file
	}
	else //If it's a draw
	{
		printf("It's a draw!\n"); //Print to console
		fprintf(fptr, "It's a draw!\n"); //Print to file
	}
}

/****************************************************************************************/






