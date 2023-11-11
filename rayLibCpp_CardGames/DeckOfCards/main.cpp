#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>

enum class CardStatus
{
	InPlay,
	InDeck,
	DiscardPile
};

std::ostream& operator<< (std::ostream& stream, CardStatus cardStatus)
{
	switch (cardStatus)
	{
	case CardStatus::InPlay: 
		return stream << "In Play";
	case CardStatus::InDeck: 
		return stream << "In Deck";
	case CardStatus::DiscardPile: 
		return stream << "Discard Pile";
	default: 
		return stream << "<unknown>";
	}
}

struct Card
{
public:
	std::string face;	
	std::string suit;
	int pointValue{ 0 };
	CardStatus status{ CardStatus::InDeck };	
};

std::ostream& operator<< (std::ostream& stream, Card card)
{
	return stream << card.face << " of " << card.suit;
}

void printDeck(const std::vector<Card> deck)
{
	std::cout << std::endl << "-------------------------------------------------------" << std::endl << std::endl;

	for (auto card : deck)
	{
		std::cout << card.face << " of " << card.suit;
		std::cout << " (" << card.status << ", " << card.pointValue << ")";
		std::cout << std::endl;
	}
}

void printDeckSimple(const std::string label, const std::vector<Card> deck, const int numCardsToHide = 0)
{
	std::cout << std::endl << "------- " << label << " -------" << std::endl;

	int i{ 0 };
	for (const auto& card : deck)
	{
		if (i++ < numCardsToHide)
			std::cout << "xxxxxxxxxxxxxxxxx" << std::endl;
		else
			std::cout << card << std::endl;		
	}
}

void initDeck
(
	std::vector<Card>& deck, 
	const std::vector<std::string>& suits, 
	const std::vector<std::string>& faces,
	const std::vector<int>& pointValues
)
{	
	for (auto suit = 0; suit < suits.size(); ++suit)
	{
		for (auto face = 0; face < faces.size(); ++face)
		{
			Card card;
			card.face = faces[face];			
			card.suit = suits[suit];
			card.pointValue = pointValues[face];
			card.status = CardStatus::InDeck;

			deck.push_back(card);
		}
	}
}

int processHand(const std::vector<Card>& hand)
{
	auto points = 0;
	bool hasAce = false;

	for (const auto& card : hand)
	{		
		if (card.face == "Ace") hasAce = true;
		
		points += card.pointValue;
	}
	
	if (hasAce)
	{
		// if there's an ace...see if the ace provides a better score at full value
		auto points2 = points + 10; // add extra for ace;
		if (points2 <= 21) return points2;
	}

	return points;
}

int main()
{
	std::cout << ".: Deck of Cards :." << std::endl;
		
	auto cardsInDeck{ 52 };
	auto suits = std::vector<std::string>{ "Spades", "Hearts", "Clubs", "Diamonds" };
	auto faces = std::vector<std::string>{ "Ace", "Two", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine", "Ten", "Jack", "Queen", "King" };
	auto pointValuesBlackJack = std::vector<int>{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10, 10 }; 
		
	//
	// MAIN LOOP
	//	
	while (true)
	{
		auto gameActive{ true };

		//
		// menu
		//
		char choice;
		std::cout << std::endl << "-------------------------------------------------------" << std::endl << std::endl;
		std::cout << "Please select an option:" << std::endl;
		std::cout << "(P)lay Blackjack" << std::endl;
		std::cout << "E(x)it" << std::endl;
		std::cin >> choice;

		switch (choice)
		{
		case 'P':
		case 'p':
			gameActive = true;
			break;
		case 'X':
		case 'x':
			gameActive = false;
			break;
		default:
			continue;
		}
		
		//
		// start game
		//
		if (gameActive)
		{
			std::cout << std::endl << "----------------------------------------" << std::endl << std::endl;

			// initialize and shuffle deck
			std::vector<Card> deck;
			initDeck(deck, suits, faces, pointValuesBlackJack);
			unsigned seed = std::chrono::steady_clock::now().time_since_epoch().count();
			auto rng = std::default_random_engine{seed};
			std::shuffle(std::begin(deck), std::end(deck), rng);

			// init player and dealer
			std::vector<Card> player;
			std::vector<Card> dealer;

			// deal 2 cards to player and dealer
			for (auto i = 0; i < 2; ++i)
			{
				player.push_back(deck.back());
				deck.pop_back();

				dealer.push_back(deck.back());
				deck.pop_back();
			}

			printDeckSimple("player", player);
			printDeckSimple("dealer", dealer, 1);

			bool isPlayersTurn{ true };
			auto dealerScore{ 0 };
			auto playerScore{ 0 };
			//
			// GAME LOOP
			//
			while (gameActive)
			{			
				if (isPlayersTurn)
				{
					//
					// Player Menu
					//
					std::cout << std::endl << "----------------------------------------" << std::endl << std::endl;
					std::cout << "(H)it" << std::endl;
					std::cout << "(S)tand" << std::endl;
					std::cin >> choice;
					
					// process player action
					switch (choice)
					{
					case 'H':
					case 'h':
						// deal another card
						player.push_back(deck.back());
						deck.pop_back();

						break;
					case 'S':
					case 's':
						isPlayersTurn = false;
						break;
					default:						
						continue;
					}

					printDeckSimple("player", player);										
					playerScore = processHand(player);
					std::cout << std::endl << "Player Score: " << playerScore << std::endl;
					if (playerScore > 21)
					{
						std::cout << "***** BUST *****" << std::endl;
						isPlayersTurn = false;
						gameActive = false;
					}
					else if (playerScore == 21)
					{
						std::cout << "***** BLACKJACK *****" << std::endl;
						isPlayersTurn = false;
						gameActive = false;
					}

					printDeckSimple("dealer", dealer, 1);
					//dealerScore = processHand(dealer);
					//std::cout << std::endl << "Dealer Score: " << dealerScore << std::endl;
				}
				else
				{
					//
					// dealers turn
					//
					std::cout << std::endl << std::endl << "dealer turn" << std::endl;

					dealerScore = processHand(dealer);
					if (dealerScore <= 16)
					{
						// deal another card
						dealer.push_back(deck.back());
						deck.pop_back();
					}
					else if (dealerScore >= 17)
					{
						gameActive = false;
					}

					printDeckSimple("player", player);
					playerScore = processHand(player);
					std::cout << std::endl << "Player Score: " << playerScore << std::endl;
					
					printDeckSimple("dealer", dealer);
					dealerScore = processHand(dealer);
					std::cout << std::endl << "Dealer Score: " << dealerScore << std::endl;
					if (dealerScore > 21)
					{
						std::cout << "***** BUST *****" << std::endl;						
						gameActive = false;
					}
					else if (playerScore == 21)
					{
						std::cout << "***** BLACKJACK *****" << std::endl;						
						gameActive = false;
					}
				} // end if player/dealer
				
			} // end while gameActive

			//
			// show winner
			//
			if (playerScore <= 21 && playerScore > dealerScore)
			{
				std::cout << std::endl << std::endl << "~*~*~*~*~*~*~*~ PLAYER WINS ~*~*~*~*~*~*~*~" << std::endl;
			}
			else
			{
				std::cout << std::endl << std::endl << "~*~*~*~*~*~*~*~ DEALER WINS ~*~*~*~*~*~*~*~" << std::endl;
			}
		}
		else 
		{
			// leave game
			break;
		}
	}


	return EXIT_SUCCESS;
}