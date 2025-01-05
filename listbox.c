/*====================================================================*/
/* +ListBox with double linked list and selection menu in C with
 * horizontal scroll.
   +Scroll function added. 
   Last modified : 24/07/2023 - ESC KEY ADDED
   Coded by Velorek. Raw output
   Target OS: Linux.                                                  */
/*====================================================================*/

/*====================================================================*/
/* COMPILER DIRECTIVES AND INCLUDES */
/*====================================================================*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rterm.h"
#include "listbox.h"
#include "keyb.h"
#include "tm.h"
#include "global.h"

/*====================================================================*/
/* CODE */
/*====================================================================*/
int double_escape = 0;
NTIMER timer2;
int globaltic2=0;
int newrows = 0, newcolumns = 0;
int listrows = 0, listcolumns = 0;
int tick2(void){
     char unstr[28] = "[+] MENEAME PARA TERMINALES";
     resetAnsi(0);
     outputcolor(F_BLACK, B_YELLOW);
     for (int i=0;i<strlen(unstr); i++){
        gotoxy(54+i,1);
       if (54+i<=newcolumns) printf("%c", unstr[i]);
     }
     outputcolor(F_WHITE,B_YELLOW);
     gotoxy(54+globaltic2,1);
     printf("%c\n",unstr[globaltic2]);
     globaltic2++;
     if (globaltic2 ==27) globaltic2=0;
     resetAnsi(0);
    if (newrows != listrows || newcolumns != listcolumns)
		return -1;
	else
		return 0;

}

/* --------------------- */
/* Dynamic List routines */
/* --------------------- */

// create new list element of type LISTCHOICE from the supplied text string
LISTCHOICE *newitem(char *text)
{
	LISTCHOICE *newp;
	newp = (LISTCHOICE *) malloc(sizeof(LISTCHOICE));
	newp->item = (char *)malloc(strlen(text) + 1);
	strcpy(newp->item, text);
	newp->next = NULL;
	newp->back = NULL;
	return newp;
}

// deleleteList: remove list from memory
void removeList(LISTCHOICE **head)
{
	LISTCHOICE *aux = *head;
	LISTCHOICE *next = NULL;
	while (aux != NULL) {
		next = aux->next;
		free(aux->item);
		free(aux);	//remove item
		aux = next;
	}
	*head = NULL;
}

/* addatend: add new LISTCHOICE to the end of a list  */
/* usage example: listBox1 = (addatend(listBox1, newitem("Item")); */
LISTCHOICE *addatend(LISTCHOICE *head, LISTCHOICE *newp)
{
	LISTCHOICE *p2;
	if (head == NULL) {
		newp->index = 0;
		newp->back = NULL;
		return newp;
	}
// now find the end of list
	for (p2 = head; p2->next != NULL; p2 = p2->next) ;
	p2->next = newp;
	newp->back = p2;
	newp->index = newp->back->index + 1;
	return head;
}

/* ---------------- */
/* Listbox routines */
/* ---------------- */

void gotoIndex(LISTCHOICE **aux, SCROLLDATA *scrollData, unsigned indexAt)
//Go to a specific location on the list.
{
	LISTCHOICE *aux2;
	unsigned counter = 0;
	*aux = listBox1;
	aux2 = *aux;
	while (counter != indexAt) {
		aux2 = aux2->next;
		counter++;
	}
	//Highlight current item

	displayItem(aux2, scrollData, SELECT_ITEM);

	//Update pointer
	*aux = aux2;
}

void printlist(LISTCHOICE *head, SCROLLDATA *scrollData, unsigned displayLimit)
{
/*
Displays the items contained in the list with the properties specified
in scrollData.
*/

	LISTCHOICE *aux;
	unsigned wherey, counter = 0;

	aux = head;
	gotoIndex(&aux, scrollData, 0);
	/* Save values */
	//wherex = scrollData->wherex;
	wherey = scrollData->wherey;
	do {
		displayItem(aux, scrollData, UNSELECT_ITEM);
		aux = aux->next;
		counter++;
		scrollData->selector++;	// wherey++
	} while (counter != displayLimit);
	scrollData->selector = wherey;	//restore value
}

void loadlist(LISTCHOICE *head, SCROLLDATA *scrollData, unsigned indexAt)
{
/*
Displays the items contained in the list with the properties specified
in scrollData.
*/

	LISTCHOICE *aux;
	unsigned wherey, counter = 0;

	aux = head;
	gotoIndex(&aux, scrollData, indexAt);
	/* Save values */
	//wherex = scrollData->wherex;
	wherey = scrollData->wherey;
	do {
		displayItem(aux, scrollData, UNSELECT_ITEM);
		aux = aux->next;
		counter++;
		scrollData->selector++;	// wherey++
	} while (counter != scrollData->displayLimit);
	scrollData->selector = wherey;	//restore value
}

int query_length(LISTCHOICE **head)
{
//Return no. items in a list.
	{
		LISTCHOICE *aux;

		unsigned itemCount = 0;
		aux = *head;
		while (aux->next != NULL) {
			aux = aux->next;
			itemCount++;
		}
		return itemCount;
	}

}

void displayItem(LISTCHOICE *aux, SCROLLDATA *scrollData, int select)
//Select or unselect item animation
{
	size_t i = 0;
	wchar_t newchar = 0;
        resetAnsi(0);
	switch (select) {

	case SELECT_ITEM:
		gotoxy(scrollData->wherex, scrollData->selector);
		outputcolor(scrollData->foreColor1, scrollData->backColor1);

		//printf("%s\n", aux->item);

		for (i = 0; i < scrollData->selectorLimit; i++) {
			if (i < strlen(aux->item)) {
				newchar = aux->item[i];
				printf("%lc", newchar);
			} else
				printf("%c", 0x20);
		}
	if (scrollData->displayMetrics==1){
        gotoxy(4,8);
        outputcolor(F_YELLOW, B_BLACK);
        printf("Votes: %s |", noticias[aux->index].votes);
        gotoxy(16,8);
        outputcolor(F_CYAN, B_BLACK);
        printf("Clicks: %s |", noticias[aux->index].clicks);
        gotoxy(30,8);
        outputcolor(F_RED, B_BLACK);
        printf("(-): %s |", noticias[aux->index].negatives);
        gotoxy(40,8);
        outputcolor(F_MAGENTA, B_BLACK);
        printf("Karma: %s", noticias[aux->index].karma); 
      }
		printf("\n");
		break;

	case UNSELECT_ITEM:
		gotoxy(scrollData->wherex, scrollData->selector);
		outputcolor(scrollData->foreColor0, scrollData->backColor0);
		//printf("%s\n", aux->item);

		for (i = 0; i < scrollData->selectorLimit; i++) {
			if (i < strlen(aux->item)) {
				newchar = aux->item[i];
				printf("%lc", newchar);
			} else
				printf("%c", 0x20);

		}
		printf("\n");
	gotoxy(4,8);
        outputcolor(F_BLACK, B_BLACK);
        printf("                                          ");	
		break;
	}
}

int move_selector(LISTCHOICE **selector, SCROLLDATA *scrollData)
{
/* 
Creates animation by moving a selector highlighting next item and
unselecting previous item
*/

	LISTCHOICE *aux;
	unsigned scrollControl = 0, continueScroll = 0, circular =
	    CIRCULAR_INACTIVE;

	//Auxiliary pointer points to selector.
	aux = *selector;

	//Circular list animation when not scrolling.
	if (aux->index == scrollData->listLength - 1
	    && scrollData->scrollActive == SCROLL_INACTIVE
	    && scrollData->scrollDirection == DOWN_SCROLL) {
		//After last item go back to the top.
		displayItem(aux, scrollData, UNSELECT_ITEM);
		scrollData->selector = scrollData->wherey;
		gotoIndex(&aux, scrollData, 0);
		*selector = aux;
		circular = CIRCULAR_ACTIVE;
	}

	if (aux->index == 0 && scrollData->scrollActive == SCROLL_INACTIVE
	    && scrollData->scrollDirection == UP_SCROLL) {
		//Before first item go back to the bottom.
		displayItem(aux, scrollData, UNSELECT_ITEM);
		scrollData->selector =
		    scrollData->wherey + scrollData->listLength - 1;
		gotoIndex(&aux, scrollData, scrollData->listLength - 1);
		*selector = aux;
		circular = CIRCULAR_ACTIVE;
	}
	//Check if we do the circular list animation.
	//If active, we skip the code one time.

	if (circular == CIRCULAR_INACTIVE) {

		//Check if we are within boundaries.
		if ((aux->next != NULL
		     && scrollData->scrollDirection == DOWN_SCROLL)
		    || (aux->back != NULL
			&& scrollData->scrollDirection == UP_SCROLL)) {

			//Unselect previous item
			displayItem(aux, scrollData, UNSELECT_ITEM);

			//Check whether we move UP or Down
			switch (scrollData->scrollDirection) {

			case UP_SCROLL:
				//Calculate new top index if scroll is active 
				//otherwise it defaults to 0 (top)
				if (scrollData->scrollActive == SCROLL_ACTIVE)
					scrollControl =
					    scrollData->currentListIndex;
				else
					scrollControl = 0;

				//Move selector
				if (aux->back->index >= scrollControl) {
					scrollData->selector--;	//whereY--
					aux = aux->back;	//Go to previous item
				} else {
					if (scrollData->scrollActive ==
					    SCROLL_ACTIVE)
						continueScroll = 1;
					else
						continueScroll = 0;
				}
				break;

			case DOWN_SCROLL:
				//Calculate bottom index limit if scroll is ACTIVE
				//Otherwise it defaults to scrollData->ListLength-1

				if (scrollData->scrollActive == SCROLL_ACTIVE)
					scrollControl =
					    scrollData->currentListIndex +
					    (scrollData->displayLimit - 1);
				else
					scrollControl =
					    scrollData->listLength - 1;

				//Move selector
				if (aux->next->index <= scrollControl) {
					aux = aux->next;	//Go to next item
					scrollData->selector++;	//whereY++;
				} else {
					if (scrollData->scrollActive ==
					    SCROLL_ACTIVE)
						continueScroll = 1;
					else
						continueScroll = 0;
				}
				break;
			}

			/*  //Metrics
			   gotoxy(6, 5);
			   printf("Length:%d|Index:%d|Memory addr:%p",
			   scrollData->listLength, aux->index, aux);
			   gotoxy(6, 6);
			   printf("Scroll Limit: %d|IsScActive?:%d|ContinueScroll: %d",
			   scrollControl, scrollData->scrollActive, continueScroll);
			 */
			//Highlight new item
			displayItem(aux, scrollData, SELECT_ITEM);
			//    update_screen(screen1);
			//Update selector pointer
			*selector = aux;
		}
	}
	//update_screen(screen1);
	circular = CIRCULAR_INACTIVE;
	return continueScroll;
}

char selectorMenu(LISTCHOICE *aux, SCROLLDATA *scrollData)
{
	char ch = 0;
	int keypressed = 0;
	int control = 0;
	int continueScroll = 0;
	unsigned counter = 0;
	char chartrail[5];

	//Go to and select expected item at the beginning

	gotoIndex(&aux, scrollData, scrollData->currentListIndex);

	if (scrollData->scrollDirection == DOWN_SCROLL
	    && scrollData->currentListIndex != 0) {
		//If we are going down we'll select the last item 
		//to create a better scrolling transition (animation)
		for (counter = 0; counter < scrollData->displayLimit; counter++) {
			scrollData->scrollDirection = DOWN_SCROLL;
			move_selector(&aux, scrollData);
		}
	} else {
		//Do nothing if we are going up. Selector is always at the top item.
	}

	//It break the loop everytime the boundaries are reached.
	//to reload a new list to show the scroll animation.
	while (control != CONTINUE_SCROLL) {

      		if (timerC(&timer2) == TRUE){
	      		//Animation
              		get_terminal_dimensions(&newrows,&newcolumns);
              		if (tick2() == -1){
				double_escape = 1;
			       scrollData->itemIndex = -1;
			        break;
				//animation that checks screen changes

			}
     		}
    		if (newrows != listrows || newcolumns != listcolumns){
     			break;
    		}

		keypressed = kbhit(1);
		if (keypressed == 1)
			ch = readch();
		else
			ch = 0;
		//if enter key pressed - break loop
		if (ch == K_ENTER)
			control = CONTINUE_SCROLL;	//Break the loop

		if (ch == 'x' || ch == K_BACKSPACE || ch == K_CTRL_C) {
			double_escape = 1;
			scrollData->itemIndex = -1;
			break;
		}
		//Check arrow keys
		if (ch == K_ESCAPE)	// escape key
		{
			strcpy(chartrail, "\0");
			read_keytrail(chartrail);

			if (chartrail[0] == K_ESCAPE && chartrail[1] == 0) {
				double_escape = 1;
				scrollData->itemIndex = -1;
				break;
			}
			if (strcmp(chartrail, K_ALT_X) == 0) {
				break;
			}
			if (strcmp(chartrail, K_UP_TRAIL) == 0) {
				// escape key + A => arrow key up
				//Move selector up
				scrollData->scrollDirection = UP_SCROLL;
				continueScroll =
				    move_selector(&aux, scrollData);
				//Break the loop if we are scrolling
				if (scrollData->scrollActive == SCROLL_ACTIVE
				    && continueScroll == 1) {
					control = CONTINUE_SCROLL;
					//Update data
					scrollData->currentListIndex =
					    scrollData->currentListIndex - 1;
					scrollData->selector =
					    scrollData->wherey;
					scrollData->item = aux->item;
					scrollData->itemIndex = aux->index;
					//Return value
					ch = control;
				}
			}
			// escape key + B => arrow key down
			if (strcmp(chartrail, K_DOWN_TRAIL) == 0) {
				//Move selector down
				scrollData->scrollDirection = DOWN_SCROLL;
				continueScroll =
				    move_selector(&aux, scrollData);
				//Break the loop if we are scrolling
				if (scrollData->scrollActive == SCROLL_ACTIVE
				    && continueScroll == 1) {
					control = CONTINUE_SCROLL;
					//Update data  
					scrollData->currentListIndex =
					    scrollData->currentListIndex + 1;
					scrollData->selector =
					    scrollData->wherey;
					scrollData->item = aux->item;
					scrollData->itemIndex = aux->index;
					scrollData->scrollDirection =
					    DOWN_SCROLL;
				}
				//Return value  
				ch = control;
			}

		}
	}

	if (ch == K_ENTER || ch == K_ENTER2)	// enter key
	{
		//Pass data of last item selected
		scrollData->item = aux->item;
		scrollData->itemIndex = aux->index;
	}
	return ch;
}

char listBox(LISTCHOICE *head,
	     unsigned whereX, unsigned whereY,
	     SCROLLDATA *scrollData, unsigned bColor0,
	     unsigned fColor0, unsigned bColor1, unsigned fColor1,
	     unsigned displayLimit, unsigned locked)
{

	unsigned list_length = 0;
	//unsigned currentIndex = 0;
	int scrollLimit = 0;
	unsigned currentListIndex = 0;
	char ch = 0;
        init_timer(&timer2,TIMER_SPEED);
	
	LISTCHOICE *aux = NULL;
	//Init values
	double_escape = 0;
	scrollData->scrollActive = 0;	//To know whether scroll is active or not.
	scrollData->scrollLimit = 0;	//Last index for scroll.
	scrollData->listLength = 0;	//Total no. of items in the list
	scrollData->currentListIndex = 0;	//Pointer to new sublist of items when scrolling.
	scrollData->displayLimit = 0;	//No. of elements to be displayed.
	scrollData->scrollDirection = 0;	//To keep track of scrolling Direction.
	scrollData->selector = 0;	//Y++
	scrollData->wherex = 0;
	scrollData->wherey = 0;
	scrollData->backColor0 = 0;	//0 unselected; 1 selected
	scrollData->foreColor0 = 0;
	scrollData->backColor1 = 0;
	scrollData->foreColor1 = 0;
	scrollData->itemIndex = 0;

	// Query size of the list
	list_length = query_length(&head) + 1;

	//Save calculations for SCROLL and store DATA
	scrollData->displayLimit = displayLimit;
	scrollLimit = list_length - scrollData->displayLimit;	//Careful with negative integers

	if (scrollLimit < 0 || displayLimit > list_length)
		scrollData->displayLimit = list_length;	//Failsafe for overboard values

	scrollData->scrollLimit = scrollLimit;
	scrollData->listLength = list_length;
	scrollData->wherex = whereX;
	scrollData->wherey = whereY;
	scrollData->selector = whereY;
	scrollData->backColor0 = bColor0;
	scrollData->backColor1 = bColor1;
	scrollData->foreColor0 = fColor0;
	scrollData->foreColor1 = fColor1;

	get_terminal_dimensions(&listrows, &listcolumns);
	//Check whether we have to activate scroll or not 
	//and if we are within bounds. [1,list_length)
	ch = 0;
	aux = head;
	printlist(aux, scrollData, scrollData->displayLimit);
	if (locked == LOCKED) {
		if (list_length > scrollData->displayLimit && scrollLimit > 0
		    && displayLimit > 0) {
			//Scroll is possible  

			scrollData->scrollActive = SCROLL_ACTIVE;

			currentListIndex = 0;	//We listBox1 the scroll at the top index.
			scrollData->currentListIndex = currentListIndex;

			//Scroll loop animation. Finish with ENTER.
			loadlist(aux, scrollData, currentListIndex);
			do {
				currentListIndex = scrollData->currentListIndex;
				loadlist(aux, scrollData, currentListIndex);
				gotoIndex(&aux, scrollData, currentListIndex);
				/*  gotoxy(6, 4);
				   printf("Current List Index: %d:%d\n", scrollData->currentListIndex,
				   aux->index); */
				ch = selectorMenu(aux, scrollData);
				if (double_escape == 1) {
					scrollData->itemIndex = -1;
					locked = FALSE;
					break;
				}
			//	if (_animation() == -1) {
			//		scrollData->itemIndex = -1;
			//		double_escape = 1;
			//		break;
			//	} 
			} while (ch != K_ENTER);

		} else {
			//Scroll is not possible.
			//Display all the elements and create selector
			//
			scrollData->scrollActive = SCROLL_INACTIVE;
			scrollData->currentListIndex = 0;
			scrollData->displayLimit = list_length;	//Default to list_length
			loadlist(head, scrollData, 0);
			ch = selectorMenu(head, scrollData);
		}
	}
	return ch;
}
