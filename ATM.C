#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>

/*
  node structure for implementing a queue to store transaction history
*/
typedef struct node {
    char transactionStatement[50];
    struct node* join;
} node;

/*
  ATM function prototypes
*/
void autoGeneratePIN(void);
int verfiyingPIN(void);
void balanceVerification(int *);
void moneyDeposition(node **, int *);
void moneyWithdrawl(node **, int *);
void historyBackup(node **, char *);
void clearHistory(node **);
void displayHistory(node **);

int main(void) {
    int option1, option2;
    int validPIN = 0, balance = 0;

    node *head = NULL;

    while (1) {
        printf("\n\nATM System\n======================\n");
        printf("1. Generate PIN\n2. Utilize ATM\n3. Leave ATM\n");
        printf("\nYour choice: ");
        scanf("%d", &option1);

        switch (option1) {
            case 1: autoGeneratePIN();
                    exit(EXIT_SUCCESS);

            case 2: validPIN = verfiyingPIN();

                    if (validPIN) {
                        printf("\nValid PIN\n");
                    } else {
                        printf("\nInvalid PIN. Please generate a PIN if you don't have one.\n");
                        exit(EXIT_FAILURE);
                    }

                    /*
					  On valid PIN entry by user, the ATM Menu is presented to the user
					*/
                    while(validPIN) {
                        printf("\nATM System Menu\n===============\n\n");
                        printf("1. Check Balance\n2. Deposit\n3. Withdraw\n4. View transaction history\n5. Quit\n\n");

                        printf("Enter choice: ");
                        scanf("%d", &option2);

                        switch(option2) {
                            case 1: balanceVerification(&balance);
                                    break;
                            case 2: moneyDeposition(&head, &balance);
                                    break;
                            case 3: moneyWithdrawl(&head, &balance);
                                    break;
                            case 4: displayHistory(&head);
                                    break;
                            case 5: printf("\nThank you for using the ATM\n");
                                    exit(EXIT_SUCCESS);
                            default: printf("\nInvalid option entered!\n");
                                     break;
                        }
                    }

            case 3: exit(EXIT_SUCCESS);

            default: printf("\nInvalid choice...Try again...\n");
                     break;
        }
    }
    return 0;
}

/*
  This function will search for the PIN entered by the user
  in the file where the PIN numbers are stored
  If PIN is found return 1
  Otherwise, return 0
*/
int verfiyingPIN(void) {
    FILE *fp;
    // buffer to read PIN and store from file
    char pin[8];
    // buffer to read PIN and store from user
    char keyPin[8];
    int validPIN = 0;

    printf("\n\nEnter the PIN: \n");
    scanf("%s", keyPin);

    fp = fopen("pin.txt", "r");
    if (NULL == fp) {
        printf("\nFile cannot be opened\n");
        exit(EXIT_FAILURE);
    }

    /*
      Search for the PIN entered by user in file pin.txt
    */
    while (fgets(pin, sizeof(pin), fp) != NULL) {
        if (strstr(pin, keyPin)) {
            validPIN = 1;
        }
    }
    fclose(fp);

    return validPIN;
}

/*
  This function will generate a 4-digit random number that
  is considered as PIN
*/
void autoGeneratePIN(void) {
    FILE *fp;

    /*
	  Generate a random 4 digit number
	*/
    srand(time(NULL));
    int generatedPin = 1000+rand()%9000;

    printf("\nPIN is Auto generated successfully\n");
    printf("\nYour Auto generated PIN: %d\n", generatedPin);
    printf("\nRe-run the program and use ATM with this autocreated PIN\n\n");

    fp = fopen("pin.txt", "a");
    if (NULL == fp) {
        printf("\nCannot open file!");
        exit(EXIT_FAILURE);
    }

    /*
      Write PIN to the file
    */
    fprintf(fp, "%d\n", generatedPin);
    fclose(fp);
}

/*
  This function will display the current balance amount
*/
void balanceVerification(int *balance) {
    printf("\nYour current balance is Rs.%d\n", *balance);
}

/*
  This function will add the money deposited to the balance
*/
void moneyDeposition(node **head, int *balance) {
    int depositAmount;
    /*
      buffer to store
    */
    char depositStmt[50];

    printf("\nEnter amount to deposit: ");
    scanf("%d", &depositAmount);

    if (depositAmount > 0) {
        *balance += depositAmount;
        printf("\nRs.%d deposited\n", depositAmount);

		/*
		  saving formatted string in depositStmt character array
		*/
        snprintf(depositStmt, sizeof(depositStmt), "Rs.%d deposited\n", depositAmount);
        historyBackup(head, depositStmt);
    } else {
        printf("\nInvalid amount entered\n.");
    }
}

/*
  This function will deduct the money withdrawn from the balance
*/
void moneyWithdrawl(node **head, int *balance) {
    int withdrawAmount;
    char withdrawStmt[50];

    printf("\nEnter amount to withdraw: ");
    scanf("%d", &withdrawAmount);

    if (withdrawAmount > 0) {
        if (withdrawAmount > *balance) {
            printf("\nCannot withdraw. Balance Rs.%d\n", *balance);
        } else {
            *balance -= withdrawAmount;
            printf("\nRs.%d withdrawn\n", withdrawAmount);

            /*
			  saving formatted string in withdrawStmt character array
			*/
            snprintf(withdrawStmt, sizeof(withdrawStmt), "Rs.%d withdrawn\n", withdrawAmount);
            historyBackup(head, withdrawStmt);
        }
    } else {
        printf("\nInvalid amount entered\n.");
    }
}

/*
  This function will save a transaction statement
*/
void historyBackup(node **head, char *str) {
    static int count = 0;
    node *temp;
    temp = (node *)malloc(sizeof(node));

    strcpy(temp->transactionStatement, str);
    temp->join = NULL;

    if (NULL == *head) {
        *head = temp;
        count++;
    } else {
        if (10 == count) {
            clearHistory(head);
            count--;
        }
        node *p;
        p = *head;
        while (NULL != p->join) {
            p = p->join;
        }
        p->join = temp;
        count++;
    }
}

/*
  This function is used to remove the oldest transaction when
  10 transactions are made
*/
void clearHistory(node **head) {
    node *temp;
    temp = *head;
    *head = (*head)->join;
    temp->join = NULL;
    free(temp);
}

/*
  This function will display the transaction history
*/
void displayHistory(node **head) {
    node *temp;
    temp = *head;

    if (NULL == temp) {
        printf("\nNo transaction history...\n");
    } else {
        printf("\nTransaction History\n-------------------\n\n");
        while (NULL != temp) {
            printf("%s\n", temp->transactionStatement);
            temp = temp->join;
        }
    }
}
