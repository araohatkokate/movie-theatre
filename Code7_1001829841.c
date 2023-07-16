#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "MovieTheaterLib.h"
#include "QueueLib.h"
#include "BSTLib.h"
#include "ListLib.h"
#include "StackLib.h"

void PrintReceipts(SNODE **StackTop)
{
	 /*if the StackTop is empty, the print appropriate message - see example output -
	   else print out the receipts.  A receipt is one node of the stack and each receipt
	   consists of a receipt number, a movie theater name and a TicketList (a linked list
	   of all seats sold to one customer).  Use ReturnAndFreeLinkedListNode() to traverse
	   the linked list stored in each stack node and display the seats and free the
	   linked list nodes. Each call to ReturnAndFreeLinkedListNode() brings back ONE ticket
	   from the linked list in the Ticket variable.  Call pop() to remove the stack node.
	*/
    char t[100];

    if (*StackTop == NULL)
    {
        printf ("No receipts\n");
    }
    else
    {


        while (*StackTop != NULL)
        {
            printf("\n\nToday's Receipts");
            printf("\n\nReceipt #%d\n", (*StackTop)->ReceiptNumber);

            while(((*StackTop)->TicketList) != NULL)
            {

                ReturnAndFreeLinkedListNode(&((*StackTop)->TicketList), t);

                printf("\t%s ", t);
            }

            pop(StackTop);
        }
    }
}

BNODE *PickAndDisplayTheater(BNODE *BSTRoot, char MovieTheaterMap[][MAXCOLS], int *MapRow, int *MapCol)
{
	BNODE *MyTheater = NULL;
	char zip[6] = {};
	char MyDims[6] = {};
    char *token = NULL;

	printf("\n\nPick a theater by entering the zipcode\n\n");

	// Traverse the BST in order and print out the theaters in zip code order - use InOrder()
    InOrder(BSTRoot);

	// Prompt for a zipcode
    printf ("\n\nEnter zip ");
    scanf("%s", zip);

    MyTheater = SearchForBNODE(BSTRoot, zip);

	// If theater is not found, then print message
    if (MyTheater == NULL) // - ???????????
    {
        printf ("Theater does not exist\n");
    }
	// If theater is found, then tokenize the theater's dimensions and pass those dimensions and the MovieTheaterMap and the theater's filename to ReadMovieTheaterFile()
    else
    {
        strcpy (MyDims, MyTheater->Dimensions);

        token = strtok (MyDims, "x");
        *MapRow = atoi(token);

        token = strtok (NULL, "x");
        *MapCol = atoi(token);

        //ReadMovieTheaterFile (MovieTheaterMap, MyTheater->FileName, *MapRow, *MapCol);


        if (!ReadMovieTheaterFile (MovieTheaterMap, MyTheater->FileName, *MapRow, *MapCol))
        {
            printf ("Unable to print that seat map at this time.  Check back later.\n");
        }
        else
        {
            PrintSeatMap (MovieTheaterMap, *MapRow, *MapCol);
        }
    }

	// If ReadMovieTheaterFile() returns FALSE, then print "Unable to print that seat map at this time.  Check back later."

    //else call PrintSeatMap()

	// return the found theater
    return MyTheater;
}

void ReadFileIntoQueue(FILE *QueueFile, QNODE **QH, QNODE **QT)
{
	//	read the passed in file and calls enQueue for each name in the file
    char Audience[100];

    while (fgets(Audience, sizeof(Audience)-1, QueueFile))
    {
        if (Audience[strlen(Audience)-1] == '\n')
        {
            Audience[strlen(Audience)-1] = '\0';
        }

        enQueue(Audience, QH, QT);
    }
}

void ReadFileIntoBST(FILE *BSTFile, BNODE **BSTnode)
{
	//	read the passed in file and tokenize each line and pass the information to AddBSTNode

    char Theatre_name[50] = {};
    char Zip_code[10] = {};
    char Func[30] = {};
    char Dimension[30] = {};
    char *token = NULL;
    char *delimeter = "|";
    char buff[200];

    while (fgets(buff, sizeof(buff)-1, BSTFile))
    {
        token = strtok(buff, delimeter);
        strcpy(Theatre_name, token);

        token = strtok(NULL, delimeter);
        strcpy(Zip_code, token);

        token = strtok(NULL, delimeter);
        strcpy(Func, token);

        token = strtok(NULL, delimeter);
        strcpy(Dimension, token);

        AddBSTNode(BSTnode, Theatre_name, Zip_code, Func, Dimension);
    }

    //printf("\n%s|%s|%s|%s\n", MTN, ZC, FN, DIM);

    //AddBSTNode(BSTnode, MTN, ZC, FN, DIM);
}

void get_command_line_parameter(char *argv[], char ParamName[], char ParamValue[])
{
	int i = 0;

	while (argv[++i] != NULL)
	{
		if (!strncmp(argv[i], ParamName, strlen(ParamName)))
		{
			strcpy(ParamValue, strchr(argv[i], '=') + 1);
		}
	}
}

int PrintMenu()
{
	int choice = 0;

	printf("\n\n1.	Sell tickets to next customer\n\n");
	printf("2.	See who's in line\n\n");
	printf("3.	See the seat map for a given theater\n\n");
	printf("4.	Print today's receipts\n\n");
	printf("Choice : ");
	scanf("%d", &choice);

	while (choice < 1 || choice > 4)
	{
		printf("Invalid choice.  Please reenter. ");
		scanf("%d", &choice);
	}

	return choice;
}

int main(int argc, char *argv[])
{
	int i, j, k;
	FILE *queueFile = NULL;
	FILE *zipFile = NULL;
	char arg_value[20];
	char queuefilename[20];
	char zipfilename[20];
	int ReceiptNumber = 0;
	int choice = 0;
	int SeatNumber;
	char Row;
	char Ticket[5];
	int ArrayRow, ArrayCol;
	int MapRow, MapCol;
	char MovieTheaterMap[MAXROWS][MAXCOLS] = {};
	LNODE *TicketLinkedListHead = NULL;
	QNODE *QueueHead = NULL;
	QNODE *QueueTail = NULL;
	BNODE *BSTRoot = NULL;
	BNODE *MyTheater = NULL;
	SNODE *StackTop = NULL;
	int NumberOfTickets = 0;

	if (argc != 4)
	{
		printf("%s QUEUE=xxxxxx ZIPFILE=xxxxx RECEIPTNUMBER=xxxxx\n", argv[0]);
		exit(0);
	}

	get_command_line_parameter(argv, "QUEUE=", queuefilename);
	get_command_line_parameter(argv, "ZIPFILE=", zipfilename);
	get_command_line_parameter(argv, "RECEIPTNUMBER=", arg_value);
	ReceiptNumber = atoi(arg_value);

    queueFile = fopen (queuefilename, "r+");
    if (queueFile == NULL)
    {
        printf ("\nFile \"%s\" did not open\n\n", queuefilename);
    }

    zipFile = fopen (zipfilename, "r+");
    if (zipFile == NULL)
    {
        printf ("\nFile \"%s\" did not open\n\n", zipfilename);
    }


	ReadFileIntoQueue(queueFile, &QueueHead, &QueueTail);
	ReadFileIntoBST(zipFile, &BSTRoot);

	while (QueueHead != NULL)
	{
		choice = PrintMenu();

		switch (choice)
		{
			case 1 :
				printf("\n\nHello %s\n", QueueHead->name);
				MyTheater = PickAndDisplayTheater(BSTRoot, MovieTheaterMap, &MapRow, &MapCol);
				if (MyTheater != NULL)
				{
					printf("\n\nHow many movie tickets do you want to buy? ");
					scanf("%d", &NumberOfTickets);
					for (i = 0; i < NumberOfTickets; i++)
					{
						do
						{
							printf("\nPick a seat (Row Seat) ");
							scanf(" %c%d", &Row, &SeatNumber);
							Row = toupper(Row);
							ArrayRow = (int)Row - 65;
							ArrayCol = SeatNumber - 1;

							if ((ArrayRow < 0 || ArrayRow >= MapRow) ||
								(ArrayCol < 0 || ArrayCol >= MapCol))
							{
								printf("\nThat is not a valid seat.  Please choose again\n\n");
							}
						}
						while ((ArrayRow < 0 || ArrayRow >= MapRow) ||
							   (ArrayCol < 0 || ArrayCol >= MapCol));

						if (MovieTheaterMap[ArrayRow][ArrayCol] == 'O')
						{
							MovieTheaterMap[ArrayRow][ArrayCol] = 'X';
							sprintf(Ticket, "%c%d", Row, SeatNumber);
							InsertNode(&TicketLinkedListHead, Ticket);
						}
						else
						{
							printf("\nSeat %c%d is not available.\n\n", Row, SeatNumber);
							i--;
						}
						PrintSeatMap(MovieTheaterMap, MapRow, MapCol);
					}

					WriteSeatMap(MyTheater, MovieTheaterMap, MapRow, MapCol);
					push(&StackTop, TicketLinkedListHead, ReceiptNumber, MyTheater->MovieTheaterName);
					TicketLinkedListHead = NULL;
					ReceiptNumber++;
					printf("\nThank you %s - enjoy your movie!\n", QueueHead->name);
					deQueue(&QueueHead);
				}
				break;
			case 2 :
				printf("\n\nCustomer Queue\n\n");
				DisplayQueue(QueueHead);
				printf("\n\n");
				break;
			case 3 :
				PickAndDisplayTheater(BSTRoot, MovieTheaterMap, &MapRow, &MapCol);
				break;
			case 4 :
				PrintReceipts(&StackTop);
				break;
			default :
				printf("Bad menu choice");
		}
	}

	printf("Good job - you sold tickets to all of the customers in line.\n");
	PrintReceipts(&StackTop);

	return 0;
}
