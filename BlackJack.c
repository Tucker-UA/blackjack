 
/***************************************
*File: BlackJack.c                     *
*Author: Tucker J. Ervin               *
*	                               *
*Just a simple blackjack game to play  *
*on my server.                         *
*                                      *
*Date: 1/31/16                         *
*				       *
***************************************/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>

#define  DECK_SIZE 52
#define  NUM_DECKS 1
#define  MAX_HAND  4
#define  MAX_SUITS 4
#define  MAX_VALUE 13
#define  CARD_INFO 2
#define	 MIN_BET   2.00

struct deck {
	short int numOfCardsLeft;
	char cards[MAX_SUITS][MAX_VALUE];
};

struct hand {
	short int numOfCards;
	char *name;
	char cards[MAX_HAND][CARD_INFO];
	short int total;
	float cash;
	float currentBet;
};

int firstTurn(struct deck *d, struct hand *pl, struct hand *dl);
int deal(struct deck *d, struct hand *h);
int setUp(struct deck *d);
int hit(struct deck *d, struct hand *h);
int bet(struct hand *h);

//enum suit {CLUBS, DIAMONDS, HEARTS, SPADES};
//enum value {TWO = 2, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN, JACK = 10, QUEEN = 10, KING = 10, ACE};

char suit[] = {'C', 'D', 'H', 'S'};
char value[] = {'2', '3', '4', '5', '6', '7', '8', '9', 'T', 'J', 'Q', 'K', 'A'};

int main(){
	srand(time(NULL));
	struct deck d;
	struct hand player = {.numOfCards = 0, .name = "Player", .total = 0, .cash=50.00};
	struct hand dealer = {.numOfCards = 0, .name = "Dealer", .total = 0, .cash=500.00};
	short int continuePlaying = 1;
	short int anotherRound = 1;
	char answer;
	setUp(&d);

	while(anotherRound){
		bet(&player);
		continuePlaying = firstTurn(&d, &player, &dealer);
		while(continuePlaying){
			printf("Hit? Y/N: ");
			scanf(" %c", &answer);
			if(tolower(answer) == 'y'){
				if((hit(&d, &player)) > 0){
					continuePlaying = 0;
				} else{
					continuePlaying = 1;
				}
			} else if(tolower(answer) != 'y'){
				while(dealer.total < 17 && dealer.total < player.total){
					if(hit(&d, &dealer) == 3){
						break;
					}
				}
				continuePlaying = 0;
			}
		}

		if((player.total > dealer.total && player.total < 21) || dealer.total > 21){
			printf("You won!\n");
			player.cash += 2.0*player.currentBet;
			dealer.cash -= player.currentBet;
		} else if((player.total < dealer.total && dealer.total < 21) || player.total > 21){
			printf("You lost.\n");
			dealer.cash += player.currentBet;
		} else if(player.total == dealer.total){
			printf("You both tied.\n");
			player.cash += player.currentBet;
		} else if(dealer.total == 21 && dealer.total != player.total){
			printf("The dealer got Blackjack. You lost.\n");
			dealer.cash += player.currentBet;
		} else if(player.total == 21 && dealer.total != player.total){
			printf("You won with Blackjack!\n");
			player.cash += 2.5*player.currentBet;
			dealer.cash -= 1.5*player.currentBet;
		}
		printf("Your current cash: %.2f\n", player.cash);
		printf("Play another round? Y/N: ");
		scanf(" %c", &answer);
		if(tolower(answer) == 'y'){
			anotherRound = 1;
			continuePlaying = 1;
		} else{
			anotherRound = 0;
		}
	}

	return 0;
}

int deal(struct deck *d, struct hand *h){
	int cardSuit = 0, cardValue = 0, present  = 0;

	if(d->numOfCardsLeft == 0){
		printf("Deck is empty. Shuffle cards back into deck.\n");

		return 1;
	}

	do{
		cardSuit = rand()%4;
		cardValue = rand()%13;
		if(d->cards[cardSuit][cardValue] == 0){
			cardSuit = rand()%4;
			cardValue = rand()%13;
			present = 0;
		} else{
			h->cards[h->numOfCards][0] = suit[cardSuit];
			h->cards[h->numOfCards][1] = d->cards[cardSuit][cardValue];
			d->cards[cardSuit][cardValue] = 0;
			h->numOfCards++;
			d->numOfCardsLeft--;
			present = 1;
		}
	} while(!present);

	switch(h->cards[h->numOfCards-1][1]){
		case '2':	h->total += 2; break;
		case '3':	h->total += 3; break;
		case '4':	h->total += 4; break;
		case '5':	h->total += 5; break;
		case '6':	h->total += 6; break;
		case '7':	h->total += 7; break;
		case '8':	h->total += 8; break;
		case '9':	h->total += 9; break;
		case 'T':	h->total += 10; break;
		case 'J':	h->total += 10; break;
		case 'Q':	h->total += 10; break;
		case 'K':	h->total += 10; break;
		case 'A':	h->total += 11; break;
		default:	h->total += 0; break;
	}

	printf("Card in %s's hand: %c%c\n", h->name, h->cards[h->numOfCards-1][0], h->cards[h->numOfCards-1][1]);
	return 0;
}

int setUp(struct deck *d){
	short int x, y;

	d->numOfCardsLeft = DECK_SIZE;

	for(x = 0; x < MAX_SUITS; x++){
		for(y = 0; y < MAX_VALUE; y++){
			d->cards[x][y] = value[y];
		}
	}

	return 0;
}

int firstTurn(struct deck *d, struct hand *pl, struct hand *dl){
	int x;

	pl->numOfCards = 0;
	dl->numOfCards = 0;
	pl->total = 0;
	dl->total = 0;

	for(x = 0; x < 2; x++){
		deal(d, pl);
	}

	deal(d, dl);

	printf("%s total is: %i\n", pl->name, pl->total);
	printf("%s total is: %i\n", dl->name, dl->total);

	if(pl->total == 21){
		printf("%s has Blackjack!\n", pl->name);
		return 0;
	} else{
		return 1;
	}
}

int hit(struct deck *d, struct hand *h){
	if(deal(d, h) == 1){
		return 3;
	}

	if(h->total == 21){
		printf("%s has Blackjack!\n", h->name);
		return 2;
	} else if(h->total > 21){
		printf("%s is over 21.\n", h->name);
		return 1;
	}
	printf("%s's total is: %i\n", h->name, h->total);
	return 0;
}

int bet(struct hand *h){
	float bet, minBet = MIN_BET;

	printf("Minimum bet is $%.2f\nYour bet: $", minBet);
	scanf(" %f", &bet);

	h->cash -= bet;
	h->currentBet = bet;

	return 0;
}
