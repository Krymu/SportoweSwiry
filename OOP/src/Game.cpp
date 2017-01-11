#include "GAME.H"
#include "TEXTUI.H"
#include <iostream>
#include <memory>
#include <vector>
#include <fstream>



Game::Game(int max_rounds, string player1, string player2, bool UI_text) {
	
	IUserInterface*  ptr = nullptr;
	if (UI_text)
	{
		ptr = new TextUI;
	}
	//else
	//GUI _UI;
	this->UI = ptr;
	
	this->RoundCount = 1;
	this->RoundMAX = max_rounds;
	Grid * grid1 = new Grid();
	Grid * grid2 = new Grid();
	grid_player1 = grid1;
	grid_player2 = grid2;
	AddPlayer(player1,grid1,grid2, UI_text);
	AddPlayer(player2,grid2,grid1, UI_text);
	
	for (auto i : Players)
	{
		i->Set_Player_Ships();
	}

	
	try {
		cout << "Player Grid:" << endl;
		PrintGrid(true);
		cout << "Computer grid" << endl;
		PrintGrid(false);
	}
	catch (Grid::bad_range br)
	{
		cout << br.what() << endl;
		cout << br.bi_val().first << " " << br.bi_val().second;
	}
	SaveToFile();
	this->StartGame();
}

Game::Game(bool UI_text)
{
	IUserInterface*  ptr = nullptr;
	if (UI_text)
	{
		ptr = new TextUI;
	}
	//else
	//GUI _UI;
	this->UI = ptr;
	ifstream GameStateFile("GameStateSave.txt");
	char data_char;
	int data_int;
	string line;
	int line_count;
	Grid * grid1 = new Grid();
	Grid * grid2 = new Grid();
	grid_player1 = grid1;
	grid_player2 = grid2;
	if (GameStateFile.is_open())
	{
		GameStateFile >> RoundCount;
		GameStateFile.get();
		GameStateFile >> RoundMAX;
		GameStateFile.get();
		getline(GameStateFile, line);
		if (line == "greedy")
			AddPlayer("greedy", grid1, grid2, UI_text);
		else if(line == "random")
			AddPlayer("random", grid1, grid2, UI_text);
		else if (line == "human")
			AddPlayer("human", grid1, grid2, UI_text);
		else
			throw Game::Load_From_File_Error();
		getline(GameStateFile, line);
		if (line == "greedy")
			AddPlayer("greedy", grid2, grid1, UI_text);
		else if (line == "random")
			AddPlayer("random", grid2, grid1, UI_text);
		else if (line == "human")
			AddPlayer("human", grid2, grid1, UI_text);
		else
			throw Game::Load_From_File_Error();

	}
	Players.at(0)->Set_Player_Ships_From_File("Player1Grid.txt");
	Players.at(1)->Set_Player_Ships_From_File("Player2Grid.txt");
	try {
		cout << "Player Grid:" << endl;
		PrintGrid(true);
		cout << "Computer grid" << endl;
		PrintGrid(false);
	}
	catch (Grid::bad_range br)
	{
		cout << br.what() << endl;
		cout << br.bi_val().first << " " << br.bi_val().second;
	}
	SaveToFile();
	this->StartGame();

}

int Game::getCurrentRound() {
	return RoundCount;
} 
int Game::getMaxRound() {
	return RoundMAX;
}


void Game::StartGame()
{
	bool if_continue = true;

	this->UI->PrintText("****************** \n Round no. : " + to_string(getCurrentRound()) + "\n");

	while (if_continue)
			PlayRound();
		for (auto i : Players)
		{
			i->Reset();
		}
		
		//case 1: last round played
		if (RoundCount >= RoundMAX)
			if_continue = false;

		//case 2: one of the players has lost its last ship
		if(!Players[0]->hasShips() || !Players[1]->hasShips())
			if_continue = false;

		//case 3: no more avaliable shots
		for (auto i : Players)
		{
			if (!i->CanMove())
				if_continue = false;
		}

		RoundCount++;
}


//as parameter give: human | random | greedy
void Game::AddPlayer(string type, Grid* grid_player, Grid* grid_oponent, bool text_mode)
{
	if (type == "human" && this->Players.size() <= 2)
	{
		IUserInterface* UI = (text_mode) ? new TextUI : new TextUI; // just 4 debiling
		HumanPlayer* player = new HumanPlayer(grid_player, grid_oponent, UI);
		
		Players.push_back(player);

	}
	else if (type == "greedy" && this->Players.size() <= 2)
	{
		Greedy_strategy *strategy = new  Greedy_strategy;
		ComputerPlayer* player = new ComputerPlayer(strategy, grid_player, grid_oponent);
		Players.push_back(player);
	}
	else if (type == "random" && this->Players.size() <= 2)
	{
		Random_strategy* strategy = new Random_strategy;
		ComputerPlayer* player = new ComputerPlayer(strategy, grid_player, grid_oponent);
		Players.push_back(player);
	}
	else
	{
		std::cout << "AddPlayer error: wrong parameter" << std::endl;
	}

	
	
}

void Game::PlayRound()
{
 
	while (Players.at(0)->CanMove() || Players.at(1)->CanMove())
	{
		cout << "Player grid \n ----------------" << endl;
		PrintGrid(true);
		cout << "Computer grid \n ----------------" << endl;
		PrintGrid(false);


		if (Players.at(0)->CanMove())
			Players.at(0)->Move();
		if (Players.at(1)->CanMove())
			Players.at(1)->Move();

		cout << "DEBUG:    Can Move: P1 " << Players.at(0)->CanMove() << "P2: " << Players.at(0)->CanMove() << endl;
	}
}

void Game::PrintGrid(bool first_grid)
{
	first_grid ? UI->printGrid(grid_player1) : UI->printGrid(grid_player2);
}


void Game::SaveToFile()
{
	ofstream GameState("GameStateSave.txt");
	ofstream Player1Grid("Player1Grid.txt");
	ofstream Player2Grid("Player2Grid.txt");
	GameState << RoundCount << endl;
	GameState << RoundMAX << endl;
	HumanPlayer * hpl;
	ComputerPlayer * cpl;
	Strategy * str;
	for (int i = 0; i < Players.size(); i++)
	{
		hpl = dynamic_cast<HumanPlayer*>(Players.at(i));
		if (hpl != nullptr)
			GameState << "human" << endl;
		else
		{
			cpl = dynamic_cast<ComputerPlayer*>(Players.at(i));
			str = dynamic_cast<Greedy_strategy*>(cpl->getStrategy());
			if (str != nullptr)
				GameState << "greedy" << endl;
			else
				GameState << "random" << endl;
		}
	}
	/*

	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			if (grid_player1->isAvaliable(coords(j, i)))
				Player1Grid << '0';
			else
				Player1Grid << grid_player1->getShip(coords(j, i))->getType();
			Player1Grid << " ";
		}
		Player1Grid << endl;
	}

	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			if (grid_player1->wasShot(coords(j, i)))
				Player1Grid << '*';
			else
				Player1Grid << '#';
			Player1Grid << " ";
		}
		Player1Grid << endl;
	}
	
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			if (grid_player2->isAvaliable(coords(j, i)))
				Player2Grid << '0';
			else
				Player2Grid << grid_player2->getShip(coords(j, i))->getType();
			Player2Grid << " ";
		}
		Player2Grid << endl;
	}
	

	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			if (grid_player2->wasShot(coords(j, i)))
				Player2Grid << '*';
			else
				Player2Grid << '#';
			Player2Grid << " ";
		}
		Player2Grid << endl;
	}

	*/
	Players.at(0)->Save_Info_To_File(0);
	Players.at(1)->Save_Info_To_File(1);
}