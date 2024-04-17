#include <algorithm>
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <cstring>
#include <cmath>
#include <iomanip>
#include <chrono>
#include <thread>
#include <random>




void lineclear(int len){
	std::cout << std::string(len,'\b');
	std::cout << std::string(len,' ');
	std::cout << std::string(len,'\b');
}

void lineback(int len){
	std::cout << std::string(len,'\b');
}

void screenreset(){
	//std::cout<<"XXX";
	/*
	std::string start = "\x1b[";
	std::string middle = std::to_string(2);
	std::string end = "j";
	std::cout<< start+middle+end;
	*/
	std::cout<< "\033[2J\033[1;1H";
}


void printSGR(int val){
	std::string start = "\x1b[";
	std::string middle = std::to_string(val);
	std::string end = "m";
	std::cout<< start+middle+end;
}

int selectmenu(std::vector<int> inps){
	std::string inpt = "";
	int inp = 0;
	
	while(inp==0){
		try{
			printSGR(1);
			std::cout<<"-> ";
			printSGR(0);
			std::getline(std::cin,inpt);
			inp = std::stoi(inpt);
			for (int x: inps){
				if(inp == x){
					return inp;
				}
			}
			std::cout<<"\033[F"<<"\x1b[2K";
			inp = 0;
			
			
		}catch (...){
			std::cout<<"\033[F"<<"\x1b[2K";
			inp = 0;
		}
		
	}
}





int menu1(){
	
	std::string title = "-ROUGELETTE-";
	//std::cout<<"-ROUGELETTE-";
	int siz = title.size();
	for (int i = 0; i<siz; i++){
		std::cout<<title[i];
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
	for (int i = 0; i<siz+1; i++){
		lineback(i);
		printSGR(1);
		printSGR(6);
		printSGR(7);
		std::cout<<title.substr(12-i);
		printSGR(0);
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
	
	lineback(12);	
	std::string var = "";
	std::getline(std::cin,var);
	int varsiz = var.length()+1;
	std::cout<<"\033[F";
	siz = 13;
	std::this_thread::sleep_for(std::chrono::milliseconds(200));
	if (varsiz>13){
		siz = varsiz;
	}
	for (int i = 0; i<siz; i++){
		lineback(i);
		std::cout << std::string(i,' ');
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
	lineback(siz);
	
	title = "- R O U G E L E T T E -\n";
	std::string undertitle = "-----------------------";
	siz = title.size();
	for (int i = 0; i<siz; i++){
		std::cout<<title[i];
		std::this_thread::sleep_for(std::chrono::milliseconds(20));
	}
	for (int i = 0; i<siz; i++){
		std::cout<<undertitle[i];
		std::this_thread::sleep_for(std::chrono::milliseconds(20));
	}
	printSGR(1);
	std::cout<<"\n\nA new round enters the chamber\n\n";
	printSGR(0);
	
	
	std::cout<<"\nGame rules.\n"
	<<"1. when the enemy has no health you win the round\n"
	<<"2. when you have no health you lose the round\n"
	<<"3. players must flip a coin to see who shoots\n"
	<<"4. the coin dislikes doubleing up\n"
	<<"5. players may use items if it is their turn\n"
	<<"6. items may use themselves if needed\n"
	<<"7. If you succesfully hit the opponent you get 5 gold\n"
	<<"8. If all self shots blank then you get an extra turn\n"
	<<"9. the winner loots the body\n";
	printSGR(1);
	std::cout<<"<------->\n\n";
	
	int menu = 0;
	int inp = 0;
	std::cout<<"Select a class:\n1. Sniper\n2. Shotgun\n3. Pistol\n4. Semi-Auto\n";
	std::vector<std::string> types = {"Sniper", "Shotgun", "Pistol", "Semi-Auto"};
	while (menu == 0){
		try{
			printSGR(1);
			std::cout<<"-> ";
			printSGR(0);
			std::string inpt;
			std::getline(std::cin,inpt);
			inp = std::stoi(inpt);
			int siz = std::to_string(inp).length()+1;
			std::cout<<std::string(10,' ');
			std::cout<<"\033[F";
			
			if (inp < 1 or inp > 4){
				inp = 0;
				menu = 0;
				for (int i = 0; i<siz+10; i++){
					lineback(i);
					std::cout << std::string(i,' ')<<"X";
					std::this_thread::sleep_for(std::chrono::milliseconds(10));
				}
				lineclear(siz+10);
				
			}else{
				std::cout<<"\x1b[2K";
				std::cout<<"\n\n";
				menu = 1;
			}
			
		}
		catch (...){
			inp = 0;
			menu = 0;
			std::cout<<"\033[F"<<"\x1b[2K";
			std::cout<<"XXX";
			std::this_thread::sleep_for(std::chrono::milliseconds(30));
			lineclear(3);
		}			
	}

	printSGR(1);
	printSGR(4);
	std::cout<<inp<<". "<<types[inp-1]<<"\n";
	printSGR(0);
	return inp;
}

int damagecalc(int character, std::vector<int> items, std::vector<int> lootsizes, std::vector<int> gamestate, std::vector<int> chambers, std::vector<int> status, std::vector<int> status2, std::vector<int> consumablelist){
	//{playerhealth, enemyhealth, rounds, chambercount, loop, turn, remaining, livecount, gothit, playergold, enemygold, damagebonus, loopbonus, extraturn, newround,solditems};
	srand(time(0));
	int damage = 1;
	int siz = items.size();
	
	std::vector<int> remainingchamberONES = {};
	std::vector<int> remainingchamberZEROS = {};
	int inventoryconsumables = 0;

	for (int i = gamestate[3]+1; i < gamestate[2]; i++){
		if (chambers[i] == 1){
			remainingchamberONES.emplace_back(i);
		}else{
			remainingchamberZEROS.emplace_back(i);
		}
	}
	
	
	for (int x: items){
		int isin = std::count(consumablelist.begin(), consumablelist.end(), x);
		if (isin > 0){
			inventoryconsumables++;
		}
	}
	
	
	if (character==1){
		int crit = rand()%2;
		if (crit == 1){
			damage = 2;
		}else{
			crit = rand()%2;
			if (crit == 1){
				damage = 4;
			}else{
				damage = 1;
			}
		}
		
	}
	if (character==2){
		damage = 3;
	}
	if (character==3){
		damage = 2;
	}
	if (character==4){
		damage = 1;
	}
	
	damage = damage+gamestate[11];
	
	//addition
	int banked = std::count(items.begin(), items.end(), 72);
	
	for (int i = 0; i<lootsizes[3]; i++){
		int amount = std::count(items.begin(), items.end(), i);
		for (int a = 0; a<amount; a++){
			
			if (i == 0){
				damage = damage+1;
			}
			else if (i == 1){
				if (gamestate[6] == 1){
					damage = damage + 2;
				}
			}
			else if (i == 2){
				if (gamestate[4] == 0){
					damage = damage + 2;
				}
			}
			else if (i == 6){
				int chance = 0;
				if (banked > 0){
					if ((4/(banked+1)) != 0){
						chance = rand()%(4/(banked+1));
					}
					else{
						chance = 0;
					}
				}
				else{
					chance = rand()%4;
				}
				if (chance != 0){
					damage = damage+2;
				}
			}
			else if (i == 9){
				if (gamestate[8] == 1){
					damage = damage + 2;
				}
			}
			else if (i == 10){
				if (gamestate[8] == 0){
					damage = damage + 1;
				}
			}
			else if (i == 14){
				damage = damage + gamestate[9]/10;
			}
			else if (i == 20){
				if (gamestate[0] <= 5){
					damage = damage + 1;
				}
			}
			else if (i == 21){
				if (gamestate[1] <= 5){
					damage = damage + 1;
				}
			}
			else if (i == 24){
				if (gamestate[0] <= 10){
					damage = damage + (10-gamestate[0]);
				}
			}
			else if (i == 38){
				damage = damage +1;
			}
			else if (i == 39){
				int count40 = std::count(items.begin(), items.end(), 40);
				if (count40 > 0 and gamestate[5] == 0){
					damage = damage + remainingchamberZEROS.size();
				}
				else{
					damage = damage + remainingchamberONES.size();
				}
			}
			else if (i == 40){
				int count39 = std::count(items.begin(), items.end(), 39);
				if (count39 > 0 and gamestate[5] == 0){
					damage = damage * (remainingchamberONES.size()+1);
				}
				else{
					damage = damage + remainingchamberZEROS.size();
				}
			}
			else if (i == 41){
				if (gamestate[5] == 0 and gamestate[13] == 1){
					damage = damage +5;
				}
			}
			else if (i == 53){
				siz = status2.size();
				int dtemptotal = 0;
				for (int i = 0; i<siz; i++){
					dtemptotal = dtemptotal + status2[i];
				}
				damage = damage +dtemptotal;
			}
			else if (i == 60){
				damage = damage+3;
			}
			else if (i == 65){
				damage = damage+(gamestate[0]/2);
			}
			else if (i == 65){
				if (gamestate[5] == 0){
					damage = damage+gamestate[15];
				}
				
			}
			else if (i == 70){
				damage = damage+gamestate[15];
			}
			else if (i == 77){
				damage = damage+inventoryconsumables;
			}
		}
	}
	
	//multiplication
	for (int i = 0; i<lootsizes[3]; i++){
		int amount = std::count(items.begin(), items.end(), i);
		for (int a = 0; a<amount; a++){
			
			if (i == 3){
				int chance = 0;
				if (banked > 0){
					if ((2/(banked+1)) != 0){
						chance = rand()%(2/(banked+1));
					}
					else{
						chance = 0;
					}
				}
				else{
					chance = rand()%2;
				}
				if (chance == 0){
					damage = damage*2;
				}
			}
			else if (i == 68){
				int chance = 0;
				if (banked > 0){
					if ((5/(banked+1)) != 0){
						chance = rand()%(5/(banked+1));
					}
					else{
						chance = 0;
					}
				}
				else{
					chance = rand()%5;
				}
				if (chance == 0){
					damage = damage*0;
				}
			}
			
			
		}
	}
	
	
	//status changes
	for (int i = 0; i<lootsizes[3]; i++){
		int amount = std::count(items.begin(), items.end(), i);
		for (int a = 0; a<amount; a++){
			
			if (status[1] > 0){
				damage = damage/2;
			}
		}
	}
	
	
	
	
	
	return damage;
}

std::tuple<int, std::vector<int>, std::vector<int>> livecalc(int character, std::vector<int> items, std::vector<int> eitems, std::vector<int> lootsizes, std::vector<int> gamestate, std::vector<int> playerstatus, std::vector<int> enemystatus){
	//{playerhealth, enemyhealth, rounds, chambercount, loop, turn, remaining, livecount, gothit, playergold, enemygold, damagebonus, loopbonus, extraturn, newround};
	srand(time(0));
	int lives = 1;
	
	if (character==1){
		lives = 1;
	}
	if (character==2){
		lives = 1;
	}
	if (character==3){
		lives = 1;
	}
	if (character==4){
		lives = 3;
	}
	int siz = items.size();
	int esiz = eitems.size();
	
	int banked = std::count(items.begin(), items.end(), 72);
	
	//addition
	std::vector<int> clonedplayeritems = {};
	std::vector<int> clonedenemyitems = {};
	for (int i = 0; i<lootsizes[3]; i++){
		int amount = std::count(items.begin(), items.end(), i);
		for (int a = 0; a<amount; a++){
			
			if (i == 4){
				lives = lives +1;
			}
			else if (i == 38){
				lives = lives +1;
			}
			else if (i == 71){
				int choice = 0;
				if (banked > 0){
					if ((10/(banked+1)) != 0){
						choice = rand()%(10/(banked+1));
					}
					else{
						choice = 0;
					}
				}
				else{
					choice = rand()%10;
				}
				if (choice == 0){
					choice = rand()%siz;
					clonedplayeritems.emplace_back(items[choice]);
				}				
			}
		}
	}
	
	//enemy addition
	for (int i = 0; i<lootsizes[3]; i++){
		int amount = std::count(eitems.begin(), eitems.end(), i);
		for (int a = 0; a<amount; a++){
			
			if (i == 4){
				lives = lives +1;
			}
			else if (i == 38){
				lives = lives +1;
			}
			else if (i == 71){
				int choice = 0;
				if (banked > 0){
					if ((10/(banked+1)) != 0){
						choice = rand()%(10/(banked+1));
					}
					else{
						choice = 0;
					}
				}
				else{
					choice = rand()%10;
				}
				if (choice == 0){
					choice = rand()%esiz;
					clonedenemyitems.emplace_back(eitems[choice]);
				}				
			}
		}
	}
	
	//multiplication
	for (int i = 0; i<lootsizes[3]; i++){
		int amount = std::count(items.begin(), items.end(), i);
		for (int a = 0; a<amount; a++){
			
			if (i == 0){
					int z = 0;
			}
		}
	}
	
	//enemy multiplication
	for (int i = 0; i<lootsizes[3]; i++){
		int amount = std::count(eitems.begin(), eitems.end(), i);
		for (int a = 0; a<amount; a++){
			
			if (i == 0){
				int z = 0;
			}
		}
	}
	
	
	for (int x: clonedplayeritems){
		items.emplace_back(x);
	}
	for (int x: clonedenemyitems){
		eitems.emplace_back(x);
	}
	
	
	
	return {lives, items, eitems};
	
}

int roundscalc(int character, std::vector<int> items, std::vector<int> eitems, std::vector<int> lootsizes, std::vector<int> gamestate, std::vector<int> playerstatus, std::vector<int> enemystatus){
	srand(time(0));
	int rounds = 1;
	
	if (character==1){
		rounds = 4;
	}
	if (character==2){
		rounds = 2;
	}
	if (character==3){
		rounds = 6;
	}
	if (character==4){
		rounds = 21;
	}
	int siz = items.size();
	std::sort(items.begin(), items.end());
	int esiz = eitems.size();
	std::sort(eitems.begin(), eitems.end());
	
	int banked = std::count(items.begin(), items.end(), 72);
	
	//addition
	for (int i = 0; i<lootsizes[3]; i++){
		int amount = std::count(items.begin(), items.end(), i);
		for (int a = 0; a<amount; a++){
			
			if (i == 7){
				rounds = rounds +2;
			}
			else if (i == 8){
				if (rounds !=1){
					rounds = rounds -1;
				}
			}
			else if (i == 22){
				if (rounds !=1 and gamestate[1] <= 5){
					rounds = rounds -1;
				}
			}
		}
	}
	
	//enemy addition
	for (int i = 0; i<lootsizes[3]; i++){
		int amount = std::count(eitems.begin(), eitems.end(), i);
		for (int a = 0; a<amount; a++){
			
			if (i == 7){
				rounds = rounds +2;
			}
			else if (i == 8){
				rounds = rounds -1;
			}
		}
	}
	
	//multiplication
	for (int i = 0; i<lootsizes[3]; i++){
		int amount = std::count(items.begin(), items.end(), i);
		for (int a = 0; a<amount; a++){
			
			if (i == 61){
				rounds = rounds*2;
			}
		}
	}
	
	//enemy multiplication
	for (int i = 0; i<lootsizes[3]; i++){
		int amount = std::count(eitems.begin(), eitems.end(), i);
		for (int a = 0; a<amount; a++){
			
			if (i == 61){
				rounds = rounds*2;
			}
		}
	}
	return rounds;
	
}

int loopcalc(int character, std::vector<int> items, std::vector<int> lootsizes, std::vector<int> gamestate, std::vector<int> playerstatus, std::vector<int> enemystatus){
	//{playerhealth, enemyhealth, rounds, chambercount, loop, turn, remaining, livecount, gothit, playergold, enemygold, damagebonus, loopbonus, extraturn, newround};
	srand(time(0));
	int loop = 1;
	
	if (character==1){
		loop = 1;
	}
	if (character==2){
		loop = 1;
	}
	if (character==3){
		loop = 1;
	}
	if (character==4){
		loop = 3;
	}
	
	int banked = std::count(items.begin(), items.end(), 72);
	
	std::vector<int> itemtruthtable = {};
	for (int i = 0; i<lootsizes[3]; i++){
		if (std::count(items.begin(), items.end(), i) > 0){
			itemtruthtable.emplace_back(1);
		}else{
			itemtruthtable.emplace_back(0);
		}
	}
	
	loop = loop + gamestate[12];
	
	int siz = items.size();

	
	//addition
	for (int i = 0; i<lootsizes[3]; i++){
		int amount = std::count(items.begin(), items.end(), i);
		for (int a = 0; a<amount; a++){
			
			if (i == 11){
				if (gamestate[8] == 0){
					loop = loop + 1;
				}
			}
		}
	}
	
	//multiplication
	for (int i = 0; i<lootsizes[3]; i++){
		int amount = std::count(items.begin(), items.end(), i);
		for (int a = 0; a<amount; a++){
			
			if (i == 63){
				if (gamestate[3] == 0){
					loop = loop*2;
				}
			}
		}
	}
	
	
	//status changes
	siz = playerstatus.size();
	for (int i = 0; i<siz;i++){
		
		if (i == 2){
			if (gamestate[5] == 0 and playerstatus[2] > 0){
				int parrand = rand()%(playerstatus[2]);
				if (loop > 0 and parrand > 1){
					loop = loop-1;
				}
			}
			else if (enemystatus[2] > 0){
				int parrand = rand()%(enemystatus[2]);
				if (loop > 0 and parrand > 1){
					loop = loop-1;
					if (itemtruthtable[59] == 1){
						gamestate[1] = gamestate[1] - 1;
					}
				}
			}
		}
	}	
	
	gamestate[12] = 0;
	return loop;
	
}


double shopcalc(int character, std::vector<int> items, std::vector<int> lootsizes, std::vector<int> gamestate){
	double shopdiscount = 1;
	
	int banked = std::count(items.begin(), items.end(), 72);
	
	for (int i = 0; i<lootsizes[3]; i++){
		int amount = std::count(items.begin(), items.end(), i);
		for (int a = 0; a<amount; a++){
			
			if (i == 42){
				shopdiscount = shopdiscount + 0.25;
			}
			if (i == 69){
				shopdiscount = shopdiscount/2;
			}
		}
	}
	
	return shopdiscount;
}

double sellcalc(int character, std::vector<int> items, std::vector<int> lootsizes, std::vector<int> gamestate){
	double sellcost = 1;
	
	int banked = std::count(items.begin(), items.end(), 72);
	
	for (int i = 0; i<lootsizes[3]; i++){
		int amount = std::count(items.begin(), items.end(), i);
		for (int a = 0; a<amount; a++){
			
			if (i == 43){
				sellcost = (sellcost*1.25);
			}
			else if (i == 69){
				sellcost = sellcost/2;
			}
		}
	}
	
	return sellcost;
}

std::tuple<int,int> lootamountcalc(int character, std::vector<int> items, std::vector<int> lootsizes, std::vector<int> gamestate, std::vector<int> playerstatus, std::vector<int> enemystatus){
	int lootamount = 1;
	int lootrarity = 4;
	
	int banked = std::count(items.begin(), items.end(), 72);
	
	
	for (int i = 0; i<lootsizes[3]; i++){
		int amount = std::count(items.begin(), items.end(), i);
		for (int a = 0; a<amount; a++){
			
			if (i == 75){
				lootamount = lootamount +1;
			}
			if (i == 76){
				if (lootrarity > 1){
					lootrarity = lootrarity -1;
				}
			}
		}
	}
	
	return {lootamount, lootrarity};
}


std::tuple<std::vector<int>, std::vector<int>, std::vector<int>, int> gamestateconsumablecalc(int character, int item, std::vector<int> items, std::vector<int> lootsizes, std::vector<int> gamestate, std::vector<int> chambers, std::vector<int> playerstatus, std::vector<int> enemystatus){
	//{playerhealth, enemyhealth, rounds, chambercount, loop, turn, remaining, livecount, gothit, playergold, enemygold, damagebonus, loopbonus, extraturn, newround};
	srand(time(0));
	int i = item;
	int siz = 0;
	
	int aciditems = std::count(items.begin(), items.end(), 74);
	for (int i = 0; i<aciditems; i++){
		enemystatus[0] = enemystatus[0] + 3;
	}
	
	int banked = std::count(items.begin(), items.end(), 72);
	
	if (i == 26){
		if (gamestate[2] != 1 and gamestate[3] != gamestate[2]){
			gamestate[3] = gamestate[3] + 1;
		}
	}
	else if (i == 27){
		gamestate[11] = gamestate[11] + 5;
	}
	else if (i == 28){
		gamestate[0] = gamestate[0] + 2;
	}
	else if (i == 29){
		int temp = gamestate[0];
		gamestate[0] = gamestate[1];
		gamestate[1] = temp;
	}
	else if (i == 30){
		if (gamestate[9] >= 1){
			gamestate[9] = gamestate[9]-1;
			gamestate[11] = gamestate[11]+2;
		}
	}
	else if (i == 31){
		gamestate[9] = gamestate[9]+4;
		gamestate[11] = gamestate[11]-1;
	}
	else if (i == 32){
		gamestate[8] = 1;
	}
	else if (i == 33){
		gamestate[1] = gamestate[1] - 1;
	}
	else if (i == 34){
		if (gamestate[9] >= 1){
			gamestate[7] = gamestate[7] - 1;
		}
	}
	else if (i == 35){
		gamestate[12] = (gamestate[4]+gamestate[12])*2;
	}
	else if (i == 45){
		gamestate[13] = 1;
	}
	else if (i == 47){
		if (gamestate[3] != 0){
			gamestate[3] = gamestate[3] - 1;
		}
		else{
			printSGR(0);
			printSGR(1);
			printSGR(6);
			printSGR(33);
			std::cout<<"\nTHE CHAMBER IS UNLOCKED\n";
			printSGR(0);
			gamestate[12] = gamestate[2];
		}
	}
	else if (i == 52){
		siz = enemystatus.size();
		for (int i = 0; i<siz; i++){
			enemystatus[i] = enemystatus[i]*2;
		}
	}
	else if (i == 79){
		gamestate[1] = gamestate[1] - gamestate[9];
		gamestate[9] = 0;
	}
	else if (i == 80){
		std::vector<int>charas={};
		for (int x = 1; x<5;x++){
			if (x!=character){
				charas.emplace_back(x);
			}
		}
		character = charas[rand()%4];
	}
	else if (i == 83){
		gamestate[16] = 1;
	}
	else if (i == 84){
		gamestate[17] = 1;
	}
	else if (i == 85){
		gamestate[17] = 2;
	}
	
	return {playerstatus, enemystatus, gamestate, character};
}

std::tuple<std::vector<int>, std::vector<int>, std::vector<int>> chamberstateconsumablecalc(int character, int item, std::vector<int> items, std::vector<int> lootsizes, std::vector<int> gamestate, std::vector<int> chambers, std::vector<int> playerstatus, std::vector<int> enemystatus){
	srand(time(0));
	std::vector<int> remainingchamberONES = {};
	std::vector<int> remainingchamberZEROS = {};

	int banked = std::count(items.begin(), items.end(), 72);

	for (int i = gamestate[3]; i < gamestate[2]; i++){
		if (chambers[i] == 1){
			remainingchamberONES.emplace_back(i);
		}else{
			remainingchamberZEROS.emplace_back(i);
		}
	}
	
	std::vector<int> itemtruthtable = {};
	for (int i = 0; i<lootsizes[3]; i++){
		if (std::count(items.begin(), items.end(), i) > 0){
			itemtruthtable.emplace_back(1);
		}else{
			itemtruthtable.emplace_back(0);
		}
	}
	
	
	if (item == 5){
		
		if (remainingchamberZEROS.size() != 0){			
			
			if (itemtruthtable[58] == 1){
				enemystatus[1] = enemystatus[1]+1;
			}
			
			int choice = rand()%remainingchamberZEROS.size();
			remainingchamberONES.emplace_back(remainingchamberZEROS[choice]);
			remainingchamberZEROS.erase(remainingchamberZEROS.begin()+choice);

			for (int i = gamestate[3]; i < gamestate[2]; i++){
				int amount = std::count(remainingchamberONES.begin(), remainingchamberONES.end(), i);
				if (amount == 1){
					chambers[i] = 1;
				}else {
					chambers[i] = 0;
				}
			}
		}
	}
	
	else if (item == 46){
		
		if (remainingchamberONES.size() != 0){
			int choice = rand()%remainingchamberONES.size();
			remainingchamberZEROS.emplace_back(remainingchamberONES[choice]);
			remainingchamberONES.erase(remainingchamberONES.begin()+choice);

			for (int i = gamestate[3]; i < gamestate[2]; i++){
				int amount = std::count(remainingchamberONES.begin(), remainingchamberONES.end(), i);
				if (amount == 1){
					chambers[i] = 1;
				}else {
					chambers[i] = 0;
				}
			}
		}
	}
	
	else if (item == 62){
		//<3
		for (int i = 0; i<3; i++){
			if (remainingchamberZEROS.size() != 0){			
				
				if (itemtruthtable[58] == 1){
					enemystatus[1] = enemystatus[1]+1;
				}
				
				int choice = rand()%remainingchamberZEROS.size();
				remainingchamberONES.emplace_back(remainingchamberZEROS[choice]);
				remainingchamberZEROS.erase(remainingchamberZEROS.begin()+choice);

				for (int i = gamestate[3]; i < gamestate[2]; i++){
					int amount = std::count(remainingchamberONES.begin(), remainingchamberONES.end(), i);
					if (amount == 1){
						chambers[i] = 1;
					}else {
						chambers[i] = 0;
					}
				}
			}
		}
	}
	

	return {playerstatus, enemystatus, chambers};
}


std::tuple<std::vector<int>, std::vector<int>, std::vector<int>> hitevent(int character, std::vector<int> items, std::vector<int> lootsizes, std::vector<int> gamestate, int damage, std::vector<int> playerstatus, std::vector<int> enemystatus){
	//{playerhealth, enemyhealth, rounds, chambercount, loop, turn, remaining, livecount, gothit, playergold, enemygold, damagebonus, loopbonus, extraturn, newround};
	//std::vector<int> gams = {};
	//for (int x: gamestate){
	//	gams.emplace_back(x);
	//}
	//gamestate = gams;
	
	int banked = std::count(items.begin(), items.end(), 72);
	
	//std::cout<<"-XXX- "<<gamestate[9]<<" -XXX- \n";
	int thbolt = 0;
	for (int i = 0; i<lootsizes[3]; i++){
		int amount = std::count(items.begin(), items.end(), i);
		for (int a = 0; a<amount; a++){
			
			if (i == 12){
				if (gamestate[5] == 0 and gamestate[8] == 1){
					gamestate[9] = gamestate[9] +5;
				}
			}else if (i == 13){
				if (gamestate[5] == 1 and gamestate[8] == 1){
					gamestate[9] = gamestate[9] +5;
				}
			}
			else if (i == 23){
				if ((gamestate[5] == 0 and gamestate[8] == 1 and damage >= 3) or (gamestate[5] == 1 and gamestate[8] == 0 and damage >= 3)){
					gamestate[1] = gamestate[1] -1;
				}
			}
			else if (i == 39){
				if (gamestate[5] == 0 and thbolt == 0){
					int count40 = std::count(items.begin(), items.end(), 40);
					if (count40 > 0){
						printSGR(0);
						printSGR(1);
						printSGR(31);
						std::cout<<"THERMAL ";
						printSGR(0);
						printSGR(1);
						printSGR(6);
						printSGR(33);
						std::cout<<"BOLT\n";
						printSGR(0);
						thbolt = 1;
					}
				}
			}
			else if (i == 50){
				if (gamestate[5] == 0){
					if (gamestate[8] == 1){
						playerstatus[0] = playerstatus[0] + 2;
					}
					else{
						enemystatus[0] = enemystatus[0] + 2;
					}
				}
			}
			else if (i == 51){
				if (gamestate[5] == 0){
					if (gamestate[8] == 1){
						playerstatus[1] = playerstatus[1] + 1;
					}
					else{
						enemystatus[1] = enemystatus[1] + 1;
					}
				
				}
			}
			else if (i == 55){
				if (gamestate[5] == 0){
					if (gamestate[8] == 1){
						playerstatus[2] = playerstatus[2] + 1;
					}
					else{
						enemystatus[2] = enemystatus[2] + 1;
					}
				}
			}
			else if (i == 73){
				if ((gamestate[5] == 0 and gamestate[8] == 1 and damage >= 3) or (gamestate[5] == 1 and gamestate[8] == 0 and damage >= 3)){
					enemystatus[1] = enemystatus[1] + 1;
				}
			}
			
		}
	}
	

	return {playerstatus, enemystatus, gamestate};
}


std::tuple<std::vector<int>, std::vector<int>, std::vector<int>> blankevent(int character, std::vector<int> items, std::vector<int> lootsizes, std::vector<int> gamestate, std::vector<int> playerstatus, std::vector<int> enemystatus){
	//{playerhealth, enemyhealth, rounds, chambercount, loop, turn, remaining, livecount, gothit, playergold, enemygold, damagebonus, loopbonus, extraturn, newround};
	//std::vector<int> gams = {};
	//for (int x: gamestate){
	//	gams.emplace_back(x);
	//}
	//gamestate = gams;
	srand(time(0));
	
	int banked = std::count(items.begin(), items.end(), 72);
	
	for (int i = 0; i<lootsizes[3]; i++){
		int amount = std::count(items.begin(), items.end(), i);
		for (int a = 0; a<amount; a++){
			
			if (i == 25){
				int temp = 0;
				if (banked > 0){
					if ((4/(banked+1)) != 0){
						temp = rand()%(4/(banked+1));
					}
					else{
						temp = 0;
					}
				}
				else{
					temp = rand()%4;
				}
				if (temp == 0){
					gamestate[1] = gamestate[1] - 1;
				}
			}
			else if (i == 36){
				if (gamestate[5] == 1 and gamestate[13] == 1){
					gamestate[9] = gamestate[9] + 1;
				}
			}
			else if (i == 44){
				int temp = 0;
				if (banked > 0){
					if ((10/(banked+1)) != 0){
						temp = rand()%(10/(banked+1));
					}
					else{
						temp = 0;
					}
				}
				else{
					temp = rand()%10;
				}
				if (temp == 0){
					gamestate[0] = gamestate[0] + 1;
				}
			}
			else if (i == 54){
				if (gamestate[5] == 1 and gamestate[13] == 1){
					enemystatus[2] = enemystatus[2] + 1;
				}
			}
			else if (i == 64){
				if (gamestate[5] == 0){
					gamestate[12] = gamestate[12] + 1;
				}
			}
			else if (i == 67){
				int temp = 0;
				if (banked > 0){
					if ((2/(banked+1)) != 0){
						temp = rand()%(2/(banked+1));
					}
					else{
						temp = 0;
					}
				}
				else{
					temp = rand()%2;
				}
				if (temp == 0){
					printSGR(0);
					printSGR(31);
					std::cout<<"EXPLOSIVE BLANK - 1 damage\n";
					printSGR(0);
					if (gamestate[5] == 0){
						if (gamestate[13] == 0){
							gamestate[1] = gamestate[1] - 1;
							enemystatus[1] = enemystatus[1]+1;
						}
						else {
							gamestate[0] = gamestate[0] - 1;
							playerstatus[1] = playerstatus[1]+1;
						}
					}
					else {
						if (gamestate[13] == 0){
							gamestate[0] = gamestate[0] - 1;
							playerstatus[1] = playerstatus[1]+1;
						}
						else {
							gamestate[1] = gamestate[1] - 1;
							enemystatus[1] = enemystatus[1]+1;
						}
					}
				}
			}
			
		}
	}
	
	
	return {playerstatus, enemystatus, gamestate};
}

std::tuple<std::vector<int>, std::vector<int>, std::vector<int>> chamberevent(int character, std::vector<int> items, std::vector<int> lootsizes, std::vector<int> gamestate, std::vector<int> chambers, std::vector<int> playerstatus, std::vector<int> enemystatus){
	srand(time(0));
	std::vector<int> remainingchamberONES = {};
	std::vector<int> remainingchamberZEROS = {};

	int banked = std::count(items.begin(), items.end(), 72);

	for (int i = gamestate[3]+1; i < gamestate[2]; i++){
		if (chambers[i] == 1){
			remainingchamberONES.emplace_back(i);
		}else{
			remainingchamberZEROS.emplace_back(i);
		}
	}
	
	std::vector<int> itemtruthtable = {};
	for (int i = 0; i<lootsizes[3]; i++){
		if (std::count(items.begin(), items.end(), i) > 0){
			itemtruthtable.emplace_back(1);
		}else{
			itemtruthtable.emplace_back(0);
		}
	}
	
	
	
	for (int i = 0; i<lootsizes[3]; i++){
		int amount = std::count(items.begin(), items.end(), i);
		for (int a = 0; a<amount; a++){
			
			if (i == 15){
				if(gamestate[5] == 1 and gamestate[8] == 0 and gamestate[13] == 0){
					if (remainingchamberZEROS.size() != 0){
						
						if (itemtruthtable[58] == 1){
							enemystatus[1] = enemystatus[1] +1;
						}
						
						int choice = rand()%remainingchamberZEROS.size();
						remainingchamberONES.emplace_back(remainingchamberZEROS[choice]);
						remainingchamberZEROS.erase(remainingchamberZEROS.begin()+choice);
						printSGR(0);
						printSGR(31);
						std::cout<<"LIVE ADDED\n";
						printSGR(0);
						for (int i = gamestate[3]; i < gamestate[2]; i++){
							int amount = std::count(remainingchamberONES.begin(), remainingchamberONES.end(), i);
							if (amount == 1){
								chambers[i] = 1;
							}else {
								chambers[i] = 0;
							}
						}
					}
				}
			}	
			else if (i == 37){
				if(gamestate[5] == 0 and gamestate[13] == 1){
					if (remainingchamberZEROS.size() != 0){
						
						if (itemtruthtable[58] == 1){
							enemystatus[1] = enemystatus[1] +1;
						}
						
						int choice = rand()%remainingchamberZEROS.size();
						remainingchamberONES.emplace_back(remainingchamberZEROS[choice]);
						remainingchamberZEROS.erase(remainingchamberZEROS.begin()+choice);
						printSGR(0);
						printSGR(31);
						std::cout<<"LIVE ADDED\n";
						printSGR(0);
						for (int i = gamestate[3]; i < gamestate[2]; i++){
							int amount = std::count(remainingchamberONES.begin(), remainingchamberONES.end(), i);
							if (amount == 1){
								chambers[i] = 1;
							}else {
								chambers[i] = 0;
							}
						}
					}
				}
			}
		}
	}
	return {playerstatus, enemystatus, chambers};

}

std::tuple<std::vector<int>, std::vector<int>, std::vector<int>,std::vector<int>> coinevent(int character, std::vector<int> items, std::vector<int> lootsizes, std::vector<int> gamestate, std::vector<int> playerstatus, std::vector<int> enemystatus, std::vector<int> chambers){
	//{playerhealth, enemyhealth, rounds, chambercount, loop, turn, remaining, livecount, gothit, playergold, enemygold, damagebonus, loopbonus, extraturn, newround};
	//std::vector<int> gams = {};
	//for (int x: gamestate){
	//	gams.emplace_back(x);
	//}
	//gamestate = gams;
	
	srand(time(0));
	std::vector<int> remainingchamberONES = {};
	std::vector<int> remainingchamberZEROS = {};

	int banked = std::count(items.begin(), items.end(), 72);

	for (int i = gamestate[3]+1; i < gamestate[2]; i++){
		if (chambers[i] == 1){
			remainingchamberONES.emplace_back(i);
		}else{
			remainingchamberZEROS.emplace_back(i);
		}
	}
	
	for (int i = 0; i<lootsizes[3]; i++){
		int amount = std::count(items.begin(), items.end(), i);
		for (int a = 0; a<amount; a++){
			
			if (i == 16){
				if (gamestate[5] == 0){
					gamestate[9] = gamestate[9] +1;
				}
			}
			else if (i == 17){
				if (gamestate[5] == 1){
					gamestate[9] = gamestate[9] +1;
				}
			}
			else if (i == 18){
				if (gamestate[5] == 0 and gamestate[6] >= 2 and gamestate[3] != gamestate[2]){
					gamestate[3] = gamestate[3] +1;
					
				}
			}
			else if (i == 19){
				if (gamestate[5] == 1 and gamestate[6] >= 2 and gamestate[3] != gamestate[2]){
					gamestate[3] = gamestate[3] +1;
				}
			}
			else if (i == 48){
				if (gamestate[5] == 0 and gamestate[13] != 1){
					gamestate[11] = gamestate[11] +2;
				}
			}
			else if (i == 49){
				if (gamestate[5] == 1 and gamestate[13] != 1){
					gamestate[12] = gamestate[12] +1;
				}
			}
			else if (i == 57){
				if (gamestate[5] == 1){
					enemystatus[0] = enemystatus[0] +1;
				}
			}
			else if (i == 78){
				if (gamestate[5] == 0){
					gamestate[0] = gamestate[0] +1;
				}
			}
			else if (i == 81){
				for (int i = gamestate[3]; i < gamestate[2]; i++){
					int amount = std::count(remainingchamberONES.begin(), remainingchamberONES.end(), i);
					if (amount == 1){
						chambers[i] = 0;
					}else {
						chambers[i] = 1;
					}
				}
			}
			
			
		}
	}
	
	
	
	
	return {playerstatus, enemystatus, gamestate,chambers};
}


std::tuple<std::vector<int>, std::vector<int>, std::vector<int>> statusupdate(int character, std::vector<int> items, std::vector<int> lootsizes, std::vector<int> gamestate, std::vector<int> chambers, std::vector<int> playerstatus, std::vector<int> enemystatus){
	//{playerhealth, enemyhealth, rounds, chambercount, loop, turn, remaining, livecount, gothit, playergold, enemygold, damagebonus, loopbonus, extraturn, newround};
	
	//status list construction
	// { poison lvl, burn lvl, paralysis lvl }
	
	// status types - poison, burn
	
	int banked = std::count(items.begin(), items.end(), 72);
	
	for (int i = 0; i<lootsizes[3]; i++){
		int amount = std::count(items.begin(), items.end(), i);
		for (int a = 0; a<amount; a++){
			
			if (i == 56){
				if ((playerstatus[1] > 0) or (enemystatus[1] > 0)){
					printSGR(0);
					printSGR(31);
					std::cout<<"\nburning ";
					printSGR(0);
					printSGR(6);
					printSGR(33);
					std::cout<<"gold ";
					printSGR(0);
					gamestate[9] = gamestate[9] +1;
				}
			}
			else if (i == 66){
				if (enemystatus[0] > 0){
					int stealrand = 0;
					if (banked > 0){
						if ((10/(banked+1)) != 0){
							stealrand = rand()%(10/(banked+1));
						}
						else{
							stealrand = 0;
						}
					}
					else{
						stealrand = rand()%10;
					}
					if (stealrand == 0){
						gamestate[0] = gamestate[0]+1;
					}
				}
			}
		}
	}
	//player update
	int siz = playerstatus.size();
	for (int i = 0; i<siz; i++){
		
		if (i == 0 and (playerstatus[0] > 0)){
			gamestate[0] = gamestate[0] - playerstatus[0]; // poison
			printSGR(0);
			printSGR(32);
			std::cout<<"\nplayer poisoned for "<<playerstatus[0]<<"\n";
			printSGR(0);
			playerstatus[0] = playerstatus[0]-1;
			
		}
		
		else if (i == 1 and (playerstatus[1] > 0)){
			gamestate[0] = gamestate[0] - (playerstatus[1]/2); // burn
			printSGR(0);
			printSGR(31);
			std::cout<<"\nplayer burnt for "<<(playerstatus[1]/2)<<"\n";
			printSGR(0);
		}
		else if (i == 2 and (playerstatus[2] > 0)){
			int parrand = rand()%(playerstatus[2]);
			if (parrand != 0){
				playerstatus[2] = playerstatus[2]-1;
			}
		}
		
	}
	
	//enemy update
	siz = enemystatus.size();
	for (int i = 0; i<siz; i++){
		
		if (i == 0 and (enemystatus[0] > 0)){
			gamestate[1] = gamestate[1] - enemystatus[0]; // poison
			printSGR(0);
			printSGR(32);
			std::cout<<"\nenemy poisoned for "<<enemystatus[0]<<"\n";
			printSGR(0);
			enemystatus[0] = enemystatus[0]-1;
		}
		
		else if (i == 1 and (enemystatus[1] > 0)){
			gamestate[1] = gamestate[1] - (enemystatus[1]/2); // burn
			printSGR(0);
			printSGR(31);
			std::cout<<"\nenemy burnt for "<<(enemystatus[1]/2)<<"\n";
			printSGR(0);
		}
		else if (i == 2 and (enemystatus[2] > 0)){
			int parrand = rand()%(enemystatus[2]);
			if (parrand != 0){
				enemystatus[2] = enemystatus[2]-1;
			}
		}
		
	}
	
	
	
	
	return {playerstatus, enemystatus, gamestate};
	
}


std::vector<int> visibcalc(int character, std::vector<int> items, std::vector<int>eitems, std::vector<int>lootsizes, std::vector<int>gamestate, std::vector<int>playerstatus, std::vector<int>enemystatus, std::vector<int> chambers){
	//{ on/off full visib, percentage chance for visible bullet}
	std::vector<int> visibchamber = {gamestate[16],0};
	
	int banked = std::count(items.begin(), items.end(), 72);
	
	
	for (int i = 0; i<lootsizes[3]; i++){
		int amount = std::count(items.begin(), items.end(), i);
		for (int a = 0; a<amount; a++){
			
			if (i == 82){
				visibchamber[1] = visibchamber[1] + 1;
			}
		}
	}
			
	
	
	
	return visibchamber;
}


int menu2(int character){
	int siz = 0;
	//game loop
	
	
	//loot tables
	
	
	std::vector<std::vector<int>> loot_table;
	
	std::vector<int> common_loot = {};
	for (int i = 0; i<50; i++){
		common_loot.emplace_back(i);
	}
	
	std::vector<int> rare_loot = {};
	for (int i = 50; i<75; i++){
		rare_loot.emplace_back(i);
	}
	
	std::vector<int> epic_loot = {};
	for (int i = 75; i<100; i++){
		epic_loot.emplace_back(i);
	}
	
	std::vector<int> wacky_loot = {};
	for (int i = 100; i<105; i++){
		wacky_loot.emplace_back(i);
	}
	
	
	int csiz = common_loot.size();
	int rsiz = rare_loot.size();
	int esiz = epic_loot.size();
	int wsiz = wacky_loot.size();
	std::vector<int> lootsizes = {csiz,csiz+rsiz,csiz+rsiz+esiz,csiz+rsiz+esiz+wsiz};
	
	
	std::vector<int> consumablelist = {5,26,27,28,29,30,31,32,33,34,35,45,46,47,52,62,79,80,83,84,85};
	std::vector<int> commonconsumable = {0};
	std::vector<int> rareconsumable = {0};
	std::vector<int> epicconsumable = {0};
	std::vector<int> wackyconsumable = {0};
	

	for (int x: consumablelist){
		if (x < lootsizes[0]){
			commonconsumable.emplace_back(x);
		}
		else if (x < lootsizes[1]){
			rareconsumable.emplace_back(x);
		}
		else if (x < lootsizes[2]){
			epicconsumable.emplace_back(x);
		}
		else if (x < lootsizes[3]){
			wackyconsumable.emplace_back(x);
		}
	}	
	
	std::vector<std::string> lootnames = {
	"0 - calibre"," | +1 damage to your shots",
	"1 - danger"," | +2 damage if the live round is in the final shot",
	"2 - gamble"," | +2 damage if the live round is in the first shot",
	"3 - crit"," | 50% chance to do x2 damage",
	"4 - live round"," | add a live round to the chamber",
	"5 + pocket round"," | add a live round to the remaining chamber - consumable",
	"6 - semi crit"," | 75% chance to do +2 damage",
	"7 - size up"," | +2 chamber size",
	"8 - size down"," | -1 chamber size",
	"9 - revenge"," | +2 damage if someone took damage last turn",
	"10 - odds"," | +1 damage if no one took damage last turn",
	"11 - evens"," | +1 bullet fired if no one took damage last turn",
	"12 - pain gain"," | +5 gold when you hit yourself",
	"13 - gimmee"," | +5 gold when the enemy hits itself",
	"14 - money gun"," | +1 damage for every 10 gold",
	"15 - hyped"," | add a live round to the chamber if the enemy attacks you",
	"16 - heads up"," | +1 gold when the coin lands heads",
	"17 -  tails up"," | +1 gold when the coin lands tails",
	"18 - heads down"," | skip a round in the chamber when the coin lands heads",
	"19 - tails down"," | skip a round in the chamber when the coin lands tails",
	"20 - adrenaline"," | +1 damage when you have 5 or less health",
	"21 - tactical"," | +1 damage when the enemy has 5 or less health",
	"22 - quick draw"," | -1 round in the chamber when the enemy has less than 5 hp",
	"23 - sting"," | 1 damage to the enemy if you take 3 or more damage in a single shot",
	"24 - rage"," | +1 damage for every point of missing health below 10",
	"25 - bad blank"," | 25% chance a blank deals 1 damage to the enemy",
	"26 + next"," | skip a round in the chamber - consumable",
	"27 + buff"," | if the next shot is successful +5 damage - consumable",
	"28 + fixup"," | +2 health - consumable",
	"29 + switch sides"," | swap player health and enemy health - consumable",
	"30 + money shot"," | spend 1 gold to increase next shot damage by 2 - consumable",
	"31 + patience"," | gain 4 gold but the next shot does -1 damage - consumable",
	"32 + pass turn"," | you will be considered hit this turn - consumable",
	"33 + pocket knife"," | deal 1 damage to the enemy - consumable",
	"34 + tooth shot"," | spend 1 gold and confuse the enemy - consumable",
	"35 + double barrel"," | fire twice as many bullets next shot - consumable",
	"36 - toll"," | gain 1 gold when the enemy blanks itself",
	"37 - stakes"," | add a live round to the chamber when you blank yourself",
	"38 - strength"," | +1 damage and +1 live round",
	"39 - heated rounds"," | +1 damage per live round in the chamber",
	"40 - cooled rounds"," | +1 damage per blank in the chamber",
	"41 - example"," | +5 damage on next shot when you blank yourself",
	"42 - discount","| 25% discount in the shop",
	"43 - dollar store"," | sell items for 25% more",
	"44 - good blank"," | 10% chance a blank heals you by 1",
	"45 + aggressor"," | you get an extra turn - consumable",
	"46 + clean chamber"," | remove a live round from the chamber - consumable",
	"47 + again"," | go back a round in the chamber - consumable",
	"48 - hot coin"," | when the coin flips heads you do +2 damage",
	"49 - cold coin"," | when the coin flips tails the enemy shoots twice",
	"50 - poison rounds"," | live rounds apply 2 poison when fired",
	"51 - burning rounds"," | live rounds apply 1 burn when fired",
	"52 + big stacks"," | double all status effects on the enemy - consumable",
	"53 - magic round"," | +1 damage for each stack of status effects applied to enemy",
	"54 - analysis paralysis"," | when the enemy blanks itself, it gets 1 stack of paralysis",
	"55 - electric round"," | live rounds apply 1 paralysis when fired",
	"56 - burning gold"," | gain +1 gold when someone take burn damage",
	"57 - acid coin"," | apply 1 poison when the coin lands tails",
	"58 - burning chamber"," | when a round is added to the chamber, burn the enemy",
	"59 - high voltage"," | the enemy takes 1 damage when paralysed",
	"60 - high calibre"," | +3 damage to your shots",
	"61 - heavy metal"," | x2 chamber size",
	"62 + recalibrate"," | add 3 live rounds to the chamber - consumable",
	"63 - guns blazing"," | if it is the first round in the chamber, shoot twice as many rounds",
	"64 - blank slate"," | if you blank, you shoot an additional shot next turn",
	"65 - beating heart"," | deal extra damage equal to half your health",
	"66 - lifesteal"," | 10% chance to gain 1hp when the enemy takes poison damage",
	"67 - explosives"," | 50% chance blanks deal 1 damage and burn",
	"68 - cushions"," | 20% chance a live round does no damage",
	"69 - coin pile"," | everything is 100% more expensive",
	"70 - jettison"," | +1 damage per item sold this game",
	"71 - cloning device"," | 5% chance to clone an item when the chamber is reloaded",
	"72 - banked curve"," | doubles item probabilities",
	"73 - hot blood"," | burn the enemy for 1 when you take 3 or more damage",
	"74 - acidic items"," | poison the enemy for 3 when you use a consumable",
	"75 - more"," | recieve an extra loot drop",
	"76 - power loot"," | increase loot rarity",
	"77 - usage"," | +1 damage for each consumable in your inventory",
	"78 - healthy coin"," | when the coin lands heads you heal 1",
	"79 + 'bury YOU with my money'"," | Spend all coins, deal damage equal to money spent - consumable",
	"80 + shuffle"," | change weapon to a random gun - consumable",
	"81 - twisted fate"," | if you land on heads, change live rounds to blank, change blank rounds to live",
	"82 - suspicion"," | show 50% of all live rounds, rounded down ",
	"83 + awareness"," | show entire chamber - consumable",
	"84 + powerful demeanor"," | force enemy to shoot itself on its next turn - consumable",
	"85 + bluff"," | force enemy to shoot you on its next turn - consumable",
	"","",
	"","",
	"","",
	"","",
	"","",
	"","",
	"","",
	"","",
	"","",
	"","",
	"","",
	"","",
	"","",
	"","",
	"","",
	"","",
	"","",
	"","",
	"",""
	};	
	
	
	//loot tables
	
	int ante = 1;
	int rounds = 6;
	int damage = 1;
	int lives = 1;
	int charloops = 1;
	int playerhealth = 10;
	int playergold = 0;
	int enemygold = 0;
	double shopdiscount = 1;
	int shophp = 2;
	int solditems = 0;
	
	
	//                                               player and enemy starting items
	std::vector<int> items = {};

	std::vector<int> eitems ={};
	
	
	if (character==1){
		rounds = 4;
		damage = 2;
		lives = 1;
		charloops = 1;
		
		std::cout<<"\nGun rules.\n"
		<<"1. 50% chance to do 2 damage, 25% 1 damage, 25% 4 damage\n"
		<<"2. 4 rounds in the chamber\n"
		<<"3. 1 live round in the chamber\n\n";
	}
	else if (character==2){
		rounds = 2;
		damage = 3;
		lives = 1;
		charloops = 1;
		
		std::cout<<"\nGun rules.\n"
		<<"1. 3 damage\n"
		<<"2. 2 rounds in the chamber\n"
		<<"3. 1 live round in the chamber\n\n";
	}
	else if (character==3){
		rounds = 6;
		damage = 2;
		lives = 1;
		charloops = 1;
		
		std::cout<<"\nGun rules.\n"
		<<"1. 2 damage\n"
		<<"2. 6 rounds in the chamber\n"
		<<"3. 1 live round in the chamber\n\n";
	}
	else if (character==4){
		rounds = 21;
		damage = 1;
		lives = 3;
		charloops = 3;
		
		std::cout<<"\nGun rules.\n"
		<<"1. 1 damage\n"
		<<"2. 21 rounds in the chamber\n"
		<<"3. fires 3 rounds at once\n"
		<<"4. 3 live rounds in the chamber\n\n";
	}
	
	
	// start of main game loop
	while (playerhealth>0){  //                         --- here
		
		
		
	// give the enemy items
	std::vector<int> eitems ={};
	
	int enemloot = 0;
	if (ante<2){
		enemloot = 0;
	}
	else{
		enemloot = (ante-2)/2+rand()%2;
	}
	
	
	int rarity = 0;
	int lootnum = 0;
	int lootrarity = 5;
	
	for (int i = 0; i<enemloot; i++){
		rarity = rand()%lootrarity;
		//rarity = 1;
		if (rarity != 0){
			lootnum = rand()%common_loot.size();
			lootnum = common_loot[lootnum];
		}else{
			rarity = rand()%lootrarity;
			if (rarity != 0){
				lootnum = rand()%rare_loot.size();
				lootnum = rare_loot[lootnum];
			}else{
				rarity = rand()%lootrarity;
				if (rarity != 0){
					lootnum = rand()%epic_loot.size();
					lootnum = epic_loot[lootnum];
				}else{
					lootnum = rand()%wacky_loot.size();
					lootnum = wacky_loot[lootnum];
				}
			}
		}
		eitems.emplace_back(lootnum);
	}
	
	
	
	std::string title = "\n[-ENEMY-ITEMS-]\n";
	siz = title.size();
	printSGR(6);
	printSGR(31);
	for (int i = 0; i<siz; i++){
		std::cout<<title[i];
		std::this_thread::sleep_for(std::chrono::milliseconds(20));
	}
	printSGR(0);
		
	siz = eitems.size();
	for (int i = 0; i < siz; i++){
		if (eitems[i] <lootsizes[0]){
			printSGR(37);
		}else if (eitems[i] <lootsizes[1]){
			printSGR(31);
		}else if (eitems[i] <lootsizes[2]){
			printSGR(1);
			printSGR(33);
		}else if (eitems[i] <lootsizes[3]){
			printSGR(1);
			printSGR(6);
			printSGR(35);
		}
		std::cout<<"[ ITEM "<<i<<". |["<<lootnames[eitems[i]*2]<<lootnames[eitems[i]*2+1]<<" ]\n";
		printSGR(0);
	}
	printSGR(6);
	printSGR(31);
	std::cout<<"[---]\n";
	printSGR(0);
	
	
	
	
	int enemyhealth = ante;
	std::vector<int> enemystatus = {0,0,0};
	std::vector<int> playerstatus = {0,0,0};
	
	//mod append extra status types to lists
	
	
	printSGR(1);
	std::cout<<"PLAYER HEALTH: "<<playerhealth<<" | ENEMY HEALTH: "<<enemyhealth<<"\n";
	printSGR(6);
	std::cout<<"ANTE: "<<ante<<"| Enter to start new round: ";
	std::string temp;
	std::getline(std::cin,temp);
	std::cout<<"\033[F"<<"\x1b[2K";
	printSGR(0);
	
	srand(time(0));
	std::vector<int> chambers;
	int randnum = rand()%rounds;
	std::vector<int> livechambers = {};
	
	int extraturn = 0;
	int chambercount = 0;
	int turn = 0;
	int attack = 0;
	int enemymults = 0;
	int coinmults = 0;
	int gothit = 0;	
	int loop = 0;
	int remaining = 0;
	int livecount = 0;
	int damagebonus = 0;
	int loopbonus = 0;
	int newround = 0;
	int fullvisib = 0;
	int forcenemy = -1;
	std::vector<int> gamestate = {playerhealth, enemyhealth, rounds, chambercount, loop, turn, remaining, livecount, gothit, playergold, enemygold, damagebonus, loopbonus, extraturn, newround, solditems,fullvisib,forcenemy};
	
	std::tie(lives,items,eitems) = livecalc(character,items,eitems,lootsizes,gamestate,playerstatus,enemystatus);
				
	rounds = roundscalc(character,items,eitems,lootsizes,gamestate,playerstatus,enemystatus);
				
	
	if (lives > rounds){
		lives = rounds;
	}
	std::vector<int> chamberchoices = {};
	for (int i = 0; i<rounds;i++){
		chamberchoices.emplace_back(i);
	}

	for (int i = 0; i < lives; i++){
		siz = chamberchoices.size();
		randnum = rand()%siz;
		randnum = chamberchoices[randnum];
		livechambers.emplace_back(randnum);
		auto choice = std::find(chamberchoices.begin(), chamberchoices.end(),randnum);
		chamberchoices.erase(choice);
	}
	
	
	for (int i = 0; i <rounds; i++){
		chambers.emplace_back(0);
	}
	for (int x: livechambers){
		chambers[x] = 1;
	}
	
	std::vector<int> visibchamber = {0,0};
	visibchamber = visibcalc(character,items,eitems,lootsizes,gamestate,playerstatus,enemystatus,chambers);
	//visibchamber = {on/off , percent};
	
	remaining = (chambers.size()-chambercount);
	for (int i = 0; i < remaining; i++){
		if (visibchamber[0] == 1){
			if (chambers[remaining-i-1+chambercount] == 1){
				printSGR(0);
				printSGR(31);
				std::cout<<"[] ";
				printSGR(0);
			}else{
				printSGR(0);
				std::cout<<"[] ";
			}
		}
		else{
			if (visibchamber[1] > 0){
				if ( (rand()%(visibchamber[1]+1)) >0 and chambers[remaining-i-1+chambercount] == 1){
					printSGR(0);
					printSGR(31);
					std::cout<<"[] ";
					printSGR(0);
				}else{
					printSGR(0);
					std::cout<<"[] ";
				}
			}
			else{
				printSGR(0);
				std::cout<<"[] ";
			}
		}
	}
		
	std::cout<<"    Rounds in the chamber: "<<rounds<<"\n"; 
	
	livecount = lives;
	int damagebonuscount = 0;
	while (playerhealth > 0 and enemyhealth>0){
		if (damagebonuscount == 1){
			damagebonus = 0;
			damagebonuscount = 0;
		}
			
		if (damagebonus > 0){
			damagebonuscount = 1;
		}
		
		newround = 0;
		if (extraturn == 0){
			std::cout<<"Flipping the coin...\n";
			
			randnum = rand()%2;
			if (randnum == turn){
				coinmults++;
			}else{
				coinmults=0;
			}
			int randnumcoin = rand()%(1+coinmults);	
			if (randnumcoin !=0){
				randnum = rand()%2;	
			}
			
			
			turn = randnum;
			std::this_thread::sleep_for(std::chrono::milliseconds(1000));
			printSGR(1);
			if (turn==0){
				std::cout<<"HEADS\n\n";
				
				
			}else{
				std::cout<<"TAILS\n\n";
				
				
			}
			
			
			gamestate = {playerhealth, enemyhealth, rounds, chambercount, loop, turn, remaining, livecount, gothit, playergold, enemygold, damagebonus, loopbonus, extraturn, newround, solditems,fullvisib,forcenemy};
			std::tie(playerstatus, enemystatus, gamestate,chambers) = coinevent(character, items, lootsizes, gamestate,playerstatus,enemystatus,chambers);
						
			playerhealth = gamestate[0];
			enemyhealth = gamestate[1];
			rounds = gamestate[2];
			chambercount = gamestate[3];
			loop = gamestate[4];
			turn = gamestate[5];
			remaining = gamestate[6];
			livecount = gamestate[7];
			gothit = gamestate[8];
			playergold = gamestate[9];
			enemygold = gamestate[10];
			damagebonus = gamestate[11];
			loopbonus = gamestate[12];
			extraturn = gamestate[13];
			newround = gamestate[14];
			solditems = gamestate[15];
			fullvisib = gamestate[16];
			forcenemy = gamestate[17];
			
			printSGR(0);
			std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		}
		std::cout<<"it is ";
		printSGR(1);
		printSGR(6);
		if (turn==0){
			std::cout<<"YOUR";
		}else{
			printSGR(31);
			std::cout<<"THE ENEMIES";
		}
		printSGR(0);
		std::cout<<" turn\n";
		
		printSGR(1);
		std::cout<<"--------\n";
		
		printSGR(0);
		
		std::vector<int> visibchamber = {0,0};
		visibchamber = visibcalc(character,items,eitems,lootsizes,gamestate,playerstatus,enemystatus,chambers);
		
		remaining = (chambers.size()-chambercount);
		for (int i = 0; i < remaining; i++){
			if (visibchamber[0] == 1){
				if (chambers[remaining-i-1+chambercount] == 1){
					printSGR(0);
					printSGR(31);
					std::cout<<"[] ";
					printSGR(0);
				}else{
					printSGR(0);
					std::cout<<"[] ";
				}
			}
			else{
				if (visibchamber[1] > 0){
					if ( (rand()%(visibchamber[1]+1)) >0 and chambers[remaining-i-1+chambercount] == 1){
						printSGR(0);
						printSGR(31);
						std::cout<<"[] ";
						printSGR(0);
					}else{
						printSGR(0);
						std::cout<<"[] ";
					}
				}
				else{
					printSGR(0);
					std::cout<<"[] ";
				}
			}
		}
		
		
		std::cout<<"    Rounds in the chamber: "<<rounds<<"\n";
		
		std::cout<<"PLAYER HEALTH: "<<playerhealth<<" | ENEMY HEALTH: "<<enemyhealth<<"\n";
		int loop = charloops;
		gamestate = {playerhealth, enemyhealth, rounds, chambercount, loop, turn, remaining, livecount, gothit, playergold, enemygold, damagebonus, loopbonus, extraturn, newround, solditems,fullvisib,forcenemy};
		loop = loopcalc(character,items,lootsizes,gamestate,playerstatus,enemystatus);
		gamestate = {playerhealth, enemyhealth, rounds, chambercount, loop, turn, remaining, livecount, gothit, playergold, enemygold, damagebonus, loopbonus, extraturn, newround, solditems,fullvisib,forcenemy};
		
		int yourdamage = damagecalc(character,items,lootsizes,gamestate,chambers,playerstatus,enemystatus,consumablelist);
		int theirdamage = damagecalc(character,eitems,lootsizes,gamestate,chambers,enemystatus,playerstatus,consumablelist);
		int gothit = 0;	
		
		if (extraturn == 1){
			extraturn = 0;
		}
		else if (extraturn == 2){
			extraturn = 1;
		}
		
		
		if (turn == 0){
			std::vector<int> inputs = {1,2,3};
			int inp;
			std::cout<<"1.shoot the enemy\n2.shoot yourself\n3.items\n";
			inp = selectmenu(inputs);
			
			
			if (inp == 3){
				
				printSGR(0);	
				std::cout<<"<-->\n";
				
				inputs = {};
					
				siz = items.size();
				for (int i = 0; i < siz; i++){
					std::cout<<i<<". ";
					if (items[i] <lootsizes[0]){
						printSGR(37);
					}else if (items[i] <lootsizes[1]){
						printSGR(31);
					}else if (items[i] <lootsizes[2]){
						printSGR(1);
						printSGR(33);
					}else if (items[i] <lootsizes[3]){
						printSGR(1);
						printSGR(6);
						printSGR(35);
					}
					inputs.emplace_back(i);
					std::cout<<lootnames[items[i]*2]<<lootnames[items[i]*2+1];
					printSGR(0);
					std::cout<<"\n";
				}
				std::cout<<siz<<". EXIT";
				inputs.emplace_back(siz);
				std::cout<<"\n<-->\nSelect an item: \n";
				std::this_thread::sleep_for(std::chrono::milliseconds(1000));
				
				inp = selectmenu(inputs);
				
				if (inp != siz){
					
					int isin = 0;
					for (int x: consumablelist){
						if (items[inp] == x){
							isin = 1;
						}
					}
					
					
					if (isin == 1){
						
						
						gamestate = {playerhealth, enemyhealth, rounds, chambercount, loop, turn, remaining, livecount, gothit, playergold, enemygold, damagebonus, loopbonus, extraturn, newround, solditems,fullvisib,forcenemy};
						
						std::tie(playerstatus, enemystatus, gamestate, character) = gamestateconsumablecalc(character,items[inp],items,lootsizes,gamestate,chambers,playerstatus,enemystatus);
						
						playerhealth = gamestate[0];
						enemyhealth = gamestate[1];
						rounds = gamestate[2];
						chambercount = gamestate[3];
						loop = gamestate[4];
						turn = gamestate[5];
						remaining = gamestate[6];
						livecount = gamestate[7];
						gothit = gamestate[8];
						playergold = gamestate[9];
						enemygold = gamestate[10];
						damagebonus = gamestate[11];
						loopbonus = gamestate[12];
						extraturn = gamestate[13];
						newround = gamestate[14];
						solditems = gamestate[15];
						fullvisib = gamestate[16];
						forcenemy = gamestate[17];
						
						std::tie(playerstatus, enemystatus, chambers) = chamberstateconsumablecalc(character,items[inp],items,lootsizes,gamestate,chambers,playerstatus,enemystatus);
						
						std::cout<<"\nITEM USED: "<<lootnames[items[inp]*2]<<"\n";
						
						items.erase(items.begin()+inp);
					}else {
						std::cout<<"\nCANT USE: "<<lootnames[items[inp]*2]<<"\n";
					}
					std::this_thread::sleep_for(std::chrono::milliseconds(1000));
				}
				if (extraturn == 1){
					extraturn = 2;
				}
				else{
					extraturn = 1;
				}

				
			}else{
				fullvisib = 0;
				while (loop!=0 and chambercount != rounds){
				if (inp == 2){
					if (chambers[chambercount] == 0){
						extraturn = 1;
						std::cout<<"You blanked\n";
						gamestate = {playerhealth, enemyhealth, rounds, chambercount, loop, turn, remaining, livecount, gothit, playergold, enemygold, damagebonus, loopbonus, extraturn, newround, solditems,fullvisib,forcenemy};
						std::tie(playerstatus, enemystatus, gamestate) = blankevent(character, items, lootsizes, gamestate,playerstatus,enemystatus);
						std::tie(playerstatus, enemystatus, chambers) = chamberevent(character, items, lootsizes, gamestate, chambers,playerstatus,enemystatus);
						
						playerhealth = gamestate[0];
						enemyhealth = gamestate[1];
						rounds = gamestate[2];
						chambercount = gamestate[3];
						loop = gamestate[4];
						turn = gamestate[5];
						remaining = gamestate[6];
						livecount = gamestate[7];
						gothit = gamestate[8];
						playergold = gamestate[9];
						enemygold = gamestate[10];
						damagebonus = gamestate[11];
						loopbonus = gamestate[12];
						extraturn = gamestate[13];
						newround = gamestate[14];
						solditems = gamestate[15];
						fullvisib = gamestate[16];
						forcenemy = gamestate[17];
						
						std::this_thread::sleep_for(std::chrono::milliseconds(100));
					}else{
						playerhealth = playerhealth - yourdamage;
						gothit = 1;
						printSGR(1);
						printSGR(31);
						std::cout<<"You damage yourself for "<<yourdamage<<"\n";
						gamestate = {playerhealth, enemyhealth, rounds, chambercount, loop, turn, remaining, livecount, gothit, playergold, enemygold, damagebonus, loopbonus, extraturn, newround, solditems,fullvisib,forcenemy};
						std::tie(playerstatus, enemystatus, gamestate) = hitevent(character, items, lootsizes, gamestate, yourdamage,playerstatus,enemystatus);
						std::tie(playerstatus, enemystatus, chambers) = chamberevent(character, items, lootsizes, gamestate, chambers,playerstatus,enemystatus);

						playerhealth = gamestate[0];
						enemyhealth = gamestate[1];
						rounds = gamestate[2];
						chambercount = gamestate[3];
						loop = gamestate[4];
						turn = gamestate[5];
						remaining = gamestate[6];
						livecount = gamestate[7];
						gothit = gamestate[8];
						playergold = gamestate[9];
						enemygold = gamestate[10];
						damagebonus = gamestate[11];
						loopbonus = gamestate[12];
						extraturn = gamestate[13];
						newround = gamestate[14];
						solditems = gamestate[15];
						fullvisib = gamestate[16];
						forcenemy = gamestate[17];
						
						printSGR(0);
						std::this_thread::sleep_for(std::chrono::milliseconds(200));
						livecount--;
					}
				}else{
					if (chambers[chambercount] == 0){
						std::cout<<"You miss\n";
						gamestate = {playerhealth, enemyhealth, rounds, chambercount, loop, turn, remaining, livecount, gothit, playergold, enemygold, damagebonus, loopbonus, extraturn, newround, solditems,fullvisib,forcenemy};
						std::tie(playerstatus, enemystatus, gamestate) = blankevent(character, items, lootsizes, gamestate,playerstatus,enemystatus);
						std::tie(playerstatus, enemystatus, chambers) = chamberevent(character, items, lootsizes, gamestate, chambers,playerstatus,enemystatus);
						
						playerhealth = gamestate[0];
						enemyhealth = gamestate[1];
						rounds = gamestate[2];
						chambercount = gamestate[3];
						loop = gamestate[4];
						turn = gamestate[5];
						remaining = gamestate[6];
						livecount = gamestate[7];
						gothit = gamestate[8];
						playergold = gamestate[9];
						enemygold = gamestate[10];
						damagebonus = gamestate[11];
						loopbonus = gamestate[12];
						extraturn = gamestate[13];
						newround = gamestate[14];
						solditems = gamestate[15];
						fullvisib = gamestate[16];
						forcenemy = gamestate[17];
						
						std::this_thread::sleep_for(std::chrono::milliseconds(100));
					}else{
						enemyhealth = enemyhealth - yourdamage;
						printSGR(1);
						printSGR(32);
						std::cout<<"You damage the enemy for "<<yourdamage<<"\n";
						gamestate = {playerhealth, enemyhealth, rounds, chambercount, loop, turn, remaining, livecount, gothit, playergold, enemygold, damagebonus, loopbonus, extraturn, newround, solditems,fullvisib,forcenemy};
						std::tie(playerstatus, enemystatus, gamestate) = hitevent(character, items, lootsizes, gamestate, yourdamage,playerstatus,enemystatus);
						std::tie(playerstatus, enemystatus, chambers) = chamberevent(character, items, lootsizes, gamestate, chambers,playerstatus,enemystatus);
						
						playerhealth = gamestate[0];
						enemyhealth = gamestate[1];
						rounds = gamestate[2];
						chambercount = gamestate[3];
						loop = gamestate[4];
						turn = gamestate[5];
						remaining = gamestate[6];
						livecount = gamestate[7];
						gothit = gamestate[8];
						playergold = gamestate[9];
						enemygold = gamestate[10];
						damagebonus = gamestate[11];
						loopbonus = gamestate[12];
						extraturn = gamestate[13];
						newround = gamestate[14];
						solditems = gamestate[15];
						fullvisib = gamestate[16];
						forcenemy = gamestate[17];
						
						std::this_thread::sleep_for(std::chrono::milliseconds(200));
						printSGR(0);
						livecount--;
						playergold=playergold+5;
					}
				}
				loop--;
				chambercount++;
				
				}
			}			
		}else{
			std::cout<<"...";
			
			
			
			
			int loop = charloops;
			loop = loopcalc(character,eitems,lootsizes,gamestate,playerstatus,enemystatus);
			//calculate loops seperately for enemy
			
			
			if (gamestate[17] != -1){
				if (chambercount < rounds-loop){
					if (livecount != 0){
						
						
						randnum = rand()%(1+enemymults);
						
						if (randnum > 0){
							randnum = 1;
						}else{
							randnum = rand()%2;
						}
						
						
									
						if (randnum == attack){
							enemymults++;
						}else {
							enemymults = 0;
						}
						int randnum2 = rand()%(1+enemymults);	
						if (randnum2 !=0){
							randnum = rand()%2;	
						}
						attack = randnum;
						std::this_thread::sleep_for(std::chrono::milliseconds(2000));
					}else {
						attack = 0;
						std::this_thread::sleep_for(std::chrono::milliseconds(1500));
					}
				}else{
					if (livecount != 0){
						attack = 1;
						std::this_thread::sleep_for(std::chrono::milliseconds(2000));
					}else {
						attack = 0;
						std::this_thread::sleep_for(std::chrono::milliseconds(1500));
					}
				}
			}
			else{
				attack = gamestate[17];
				gamestate[17]=-1;
			}
			
			lineback(3);
						
			while (loop!=0  and chambercount != rounds){
			if (attack == 0){
				if (chambers[chambercount] == 0){
					extraturn = 1;
					printSGR(1);
					std::cout<<"The enemy blanks itself\n";
					gamestate = {playerhealth, enemyhealth, rounds, chambercount, loop, turn, remaining, livecount, gothit, playergold, enemygold, damagebonus, loopbonus, extraturn, newround, solditems,fullvisib,forcenemy};
					std::tie(playerstatus, enemystatus, gamestate) = blankevent(character, items, lootsizes, gamestate,playerstatus,enemystatus);
					std::tie(playerstatus, enemystatus, chambers) = chamberevent(character, items, lootsizes, gamestate, chambers,playerstatus,enemystatus);
					
					playerhealth = gamestate[0];
					enemyhealth = gamestate[1];
					rounds = gamestate[2];
					chambercount = gamestate[3];
					loop = gamestate[4];
					turn = gamestate[5];
					remaining = gamestate[6];
					livecount = gamestate[7];
					gothit = gamestate[8];
					playergold = gamestate[9];
					enemygold = gamestate[10];
					damagebonus = gamestate[11];
					loopbonus = gamestate[12];
					extraturn = gamestate[13];
					newround = gamestate[14];
					solditems = gamestate[15];
					fullvisib = gamestate[16];
					forcenemy = gamestate[17];
						
					std::this_thread::sleep_for(std::chrono::milliseconds(100));
					printSGR(0);
				}else{
					enemyhealth = enemyhealth - theirdamage;
					gothit = 1;
					printSGR(32);
					std::cout<<"The enemy damaged itself for "<<theirdamage<<"\n";
					gamestate = {playerhealth, enemyhealth, rounds, chambercount, loop, turn, remaining, livecount, gothit, playergold, enemygold, damagebonus, loopbonus, extraturn, newround, solditems,fullvisib,forcenemy};
					std::tie(playerstatus, enemystatus, gamestate) = hitevent(character, items, lootsizes, gamestate, theirdamage,playerstatus,enemystatus);
					std::tie(playerstatus, enemystatus, chambers) = chamberevent(character, items, lootsizes, gamestate, chambers,playerstatus,enemystatus);
					
					playerhealth = gamestate[0];
					enemyhealth = gamestate[1];
					rounds = gamestate[2];
					chambercount = gamestate[3];
					loop = gamestate[4];
					turn = gamestate[5];
					remaining = gamestate[6];
					livecount = gamestate[7];
					gothit = gamestate[8];
					playergold = gamestate[9];
					enemygold = gamestate[10];
					damagebonus = gamestate[11];
					loopbonus = gamestate[12];
					extraturn = gamestate[13];
					newround = gamestate[14];
					solditems = gamestate[15];
					fullvisib = gamestate[16];
					forcenemy = gamestate[17];
						
					std::this_thread::sleep_for(std::chrono::milliseconds(200));
					printSGR(0);
					livecount--;
				}
			}else{
				if (chambers[chambercount] == 0){
					std::cout<<"The enemy shoots a blank at you\n";
					gamestate = {playerhealth, enemyhealth, rounds, chambercount, loop, turn, remaining, livecount, gothit, playergold, enemygold, damagebonus, loopbonus, extraturn, newround, solditems,fullvisib,forcenemy};
					std::tie(playerstatus, enemystatus, gamestate) = blankevent(character, items, lootsizes, gamestate,playerstatus,enemystatus);
					std::tie(playerstatus, enemystatus, chambers) = chamberevent(character, items, lootsizes, gamestate, chambers,playerstatus,enemystatus);
					
					playerhealth = gamestate[0];
					enemyhealth = gamestate[1];
					rounds = gamestate[2];
					chambercount = gamestate[3];
					loop = gamestate[4];
					turn = gamestate[5];
					remaining = gamestate[6];
					livecount = gamestate[7];
					gothit = gamestate[8];
					playergold = gamestate[9];
					enemygold = gamestate[10];
					damagebonus = gamestate[11];
					loopbonus = gamestate[12];
					extraturn = gamestate[13];
					newround = gamestate[14];
					solditems = gamestate[15];
					fullvisib = gamestate[16];
					forcenemy = gamestate[17];
						
					std::this_thread::sleep_for(std::chrono::milliseconds(100));
				}else{
					playerhealth = playerhealth - theirdamage;
					printSGR(1);
					printSGR(31);
					std::cout<<"The enemy damages you for "<<theirdamage<<"\n";
					gamestate = {playerhealth, enemyhealth, rounds, chambercount, loop, turn, remaining, livecount, gothit, playergold, enemygold, damagebonus, loopbonus, extraturn, newround, solditems,fullvisib,forcenemy};
					std::tie(playerstatus, enemystatus, gamestate) = hitevent(character, items, lootsizes, gamestate, theirdamage,playerstatus,enemystatus);
					std::tie(playerstatus, enemystatus, chambers) = chamberevent(character, items, lootsizes, gamestate, chambers,playerstatus,enemystatus);
					
					playerhealth = gamestate[0];
					enemyhealth = gamestate[1];
					rounds = gamestate[2];
					chambercount = gamestate[3];
					loop = gamestate[4];
					turn = gamestate[5];
					remaining = gamestate[6];
					livecount = gamestate[7];
					gothit = gamestate[8];
					playergold = gamestate[9];
					enemygold = gamestate[10];
					damagebonus = gamestate[11];
					loopbonus = gamestate[12];
					extraturn = gamestate[13];
					newround = gamestate[14];
					solditems = gamestate[15];
					fullvisib = gamestate[16];
					forcenemy = gamestate[17];
						
					printSGR(0);
					std::this_thread::sleep_for(std::chrono::milliseconds(200));
					livecount--;
				}
					
			}
			loop--;
			chambercount++;
			
			}
		}
		
		
		gamestate = {playerhealth, enemyhealth, rounds, chambercount, loop, turn, remaining, livecount, gothit, playergold, enemygold, damagebonus, loopbonus, extraturn, newround, solditems,fullvisib,forcenemy};
		
		std::tie(playerstatus, enemystatus, gamestate) = statusupdate(character,items,lootsizes,gamestate,chambers,playerstatus,enemystatus);
		
		playerhealth = gamestate[0];
		enemyhealth = gamestate[1];
		rounds = gamestate[2];
		chambercount = gamestate[3];
		loop = gamestate[4];
		turn = gamestate[5];
		remaining = gamestate[6];
		livecount = gamestate[7];
		gothit = gamestate[8];
		playergold = gamestate[9];
		enemygold = gamestate[10];
		damagebonus = gamestate[11];
		loopbonus = gamestate[12];
		extraturn = gamestate[13];
		newround = gamestate[14];		
		solditems = gamestate[15];
		fullvisib = gamestate[16];
		
		
		
		if (gothit == 1){
			extraturn = 0;
		}else if(extraturn == 1){
			printSGR(1);
			std::cout<<"EXTRA TURN\n";
			
			printSGR(0);
		}
		if (turn == 0){
			if (extraturn == 1){
				std::this_thread::sleep_for(std::chrono::milliseconds(250));
			}else {
				std::this_thread::sleep_for(std::chrono::milliseconds(750));
			}
		}else {
			std::this_thread::sleep_for(std::chrono::milliseconds(750));
		}
		
		std::cout<<"\nPLAYER HEALTH: "<<playerhealth<<" | ENEMY HEALTH: "<<enemyhealth<<"\n\n";
		
		
		if (playerhealth > 0 and enemyhealth>0){
			if (chambercount >= rounds){
				printSGR(1);
				printSGR(6);
				printSGR(33);
				std::cout<<"\nNEWROUND\n";
				
				newround = 1;
				
				std::cout<<"Enter to continue: ";
				std::getline(std::cin,temp);
				std::cout<<"\033[F"<<"\x1b[2K";
				printSGR(0);
				
				chambercount = 0;
				
				chambers = {};
				std::vector<int> livechambers = {};
				
				gamestate = {playerhealth, enemyhealth, rounds, chambercount, loop, turn, remaining, livecount, gothit, playergold, enemygold, damagebonus, loopbonus, extraturn, newround, solditems,fullvisib,forcenemy};
				std::tie(lives,items,eitems) = livecalc(character,items,eitems,lootsizes,gamestate,playerstatus,enemystatus);
				
				rounds = roundscalc(character,items,eitems,lootsizes,gamestate,playerstatus,enemystatus);
				
				livecount = lives;
				std::vector<int> chamberchoices = {};
				for (int i = 0; i<rounds;i++){
					chamberchoices.emplace_back(i);
				}
				if (lives > rounds){
					lives = rounds;
				}
				for (int i = 0; i < lives; i++){
					siz = chamberchoices.size();
					randnum = rand()%siz;
					randnum = chamberchoices[randnum];
					livechambers.emplace_back(randnum);
					auto choice = std::find(chamberchoices.begin(), chamberchoices.end(),randnum);
					chamberchoices.erase(choice);
				}
				
				
				for (int i = 0; i <rounds; i++){
					chambers.emplace_back(0);
				}
				for (int x: livechambers){
					chambers[x] = 1;
				}
				std::this_thread::sleep_for(std::chrono::milliseconds(1500));
			}
		}
		
	}
	
	if (playerhealth>0){
		//acquire loot
		std::string title = "";
		
		
		title = "\n\n-LOOT-\n";
		printSGR(1);
		printSGR(6);
		printSGR(36);
		siz = title.size();
		for (int i = 0; i<siz; i++){
			std::cout<<title[i];
			std::this_thread::sleep_for(std::chrono::milliseconds(50));
		}
		printSGR(0);
		
		printSGR(1);
		printSGR(36);
		std::cout<<"|: [ ";
		printSGR(0);
		printSGR(1);
		
		int rarity = 0;
		int lootnum = 0;
		std::vector<int> loot = {};
		int lootamount = 1;
		int lootrarity = 5;
		
		std::tie(lootamount,lootrarity) = lootamountcalc(character,items,lootsizes,gamestate,playerstatus,enemystatus);
		
		
		for (int i = 0; i<lootamount; i++){
			
			rarity = rand()%lootrarity;
			//rarity = 1;
			if (rarity != 0){
				lootnum = rand()%commonconsumable.size();
				lootnum = commonconsumable[lootnum];
			}else{
				rarity = rand()%lootrarity;
				if (rarity != 0){
					lootnum = rand()%rareconsumable.size();
					lootnum = rareconsumable[lootnum];
				}else{
					rarity = rand()%lootrarity;
					if (rarity != 0){
						lootnum = rand()%epicconsumable.size();
						lootnum = epicconsumable[lootnum];
					}else{
						lootnum = rand()%wackyconsumable.size();
						lootnum = wackyconsumable[lootnum];
					}
				}
			}
			loot.emplace_back(lootnum);
			items.emplace_back(lootnum);
		}
		
		siz = loot.size()-1;	
		for (int i = 0; i < siz+1; i++){
			if (loot[i] <lootsizes[0]){
				printSGR(37);
			}else if (loot[i] <lootsizes[1]){
				printSGR(31);
			}else if (loot[i] <lootsizes[2]){
				printSGR(1);
				printSGR(33);
			}else if (loot[i] <lootsizes[3]){
				printSGR(1);
				printSGR(6);
				printSGR(35);
			}
			std::cout<<loot[i];
			printSGR(0);
			if (i == siz){
				std::cout<<" ";
			}else{
				std::cout<<", ";
			}
		}
		
		printSGR(1);
		printSGR(36);
		std::cout<<"] :|\n";
		printSGR(0);
		std::cout<<"\nEnter to continue: ";
		std::getline(std::cin,temp);
		std::cout<<"\033[F"<<"\x1b[2K<------->\n\n";
		
		
		
		
		title = "-INVENTORY-\n";
		
		printSGR(1);
		printSGR(6);
		printSGR(34);
		siz = title.size();
		for (int i = 0; i<siz; i++){
			std::cout<<title[i];
			std::this_thread::sleep_for(std::chrono::milliseconds(50));
		}
		printSGR(0);	
		std::cout<<"[---]\n";
		
		siz = items.size();
		for (int i = 0; i < siz; i++){
			if (items[i] <lootsizes[0]){
				printSGR(37);
			}else if (items[i] <lootsizes[1]){
				printSGR(31);
			}else if (items[i] <lootsizes[2]){
				printSGR(1);
				printSGR(33);
			}else if (items[i] <lootsizes[3]){
				printSGR(1);
				printSGR(6);
				printSGR(35);
			}
			std::cout<<"[ "<<lootnames[items[i]*2]<<lootnames[items[i]*2+1]<<" ]\n";
			printSGR(0);
		}
		std::cout<<"\n[---]\n\n";
		
		
		
		
		
		//shop 
		title = "\n\n-SHOP-\n";
		
		shopdiscount = shopcalc(character, items, lootsizes, gamestate);
		
		printSGR(1);
		printSGR(6);
		printSGR(34);
		siz = title.size();
		for (int i = 0; i<siz; i++){
			std::cout<<title[i];
			std::this_thread::sleep_for(std::chrono::milliseconds(50));
		}	
		std::vector<int> shopitems = {};
		shopitems.emplace_back(0);
		std::vector<int> shopcosts = {};
		shopcosts.emplace_back(10*(1/shopdiscount));
		
		for (int i = 0; i <5; i++){
			rarity = rand()%3;
			if (rarity != 0){
				lootnum = rand()%common_loot.size();
				lootnum = common_loot[lootnum];
			}else{
				rarity = rand()%4;
				if (rarity != 0){
					lootnum = rand()%rare_loot.size();
					lootnum = rare_loot[lootnum];
				}else{
					rarity = rand()%5;
					if (rarity != 0){
						lootnum = rand()%epic_loot.size();
						lootnum = epic_loot[lootnum];
					}else{
						lootnum = rand()%wacky_loot.size();
						lootnum = wacky_loot[lootnum];
					}
				}
			}
			shopitems.emplace_back(lootnum);
		}
		
		
		std::vector<int> inps = {0,1,2,3,4,5,6};
		printSGR(0);
		std::cout<<"\n0. "<<10*(1/shopdiscount)<<" G -> "<<shophp<<" health\n";
		siz = shopitems.size();
		int shopcost = 0;
		for (int i = 1; i<siz-1; i++){
			printSGR(0);
			std::cout<<i<<". ";
			
			if (shopitems[i] <lootsizes[0]){
				shopcost = 5*(1/shopdiscount);
				std::cout<<shopcost<<" G -> ";
				shopcosts.emplace_back(shopcost);
				printSGR(37);
			}else if (shopitems[i] <lootsizes[1]){
				shopcost = 10*(1/shopdiscount);
				std::cout<<shopcost<<" G -> ";
				shopcosts.emplace_back(shopcost);
				printSGR(31);
			}else if (shopitems[i] <lootsizes[2]){
				shopcost = 15*(1/shopdiscount);
				std::cout<<shopcost<<" G -> ";
				shopcosts.emplace_back(shopcost);
				printSGR(1);
				printSGR(33);
			}else if (shopitems[i] <lootsizes[3]){
				shopcost = 20*(1/shopdiscount);
				std::cout<<shopcost<<" G -> ";
				shopcosts.emplace_back(shopcost);
				printSGR(1);
				printSGR(6);
				printSGR(35);
			}
			std::cout<<lootnames[shopitems[i]*2]<<lootnames[shopitems[i]*2+1]<<"\n";
			printSGR(0);	
		}
		std::cout<<siz-1<<". EXIT\n";
		std::cout<<siz<<". SELL";
		
		printSGR(0);
		std::cout<<"\nGOLD: ";
		printSGR(34);
		std::cout<<playergold<<"\n";
		printSGR(0);
		int inp = 0;
		while (inp != 5){
			printSGR(6);
			std::cout<<"SELECT AN ITEM: \n";
			printSGR(0);
			inp = selectmenu(inps);
			std::cout<<"\033[F"<<"\x1b[2K";
			if (inp == 0){
				if (playergold >= shopcosts[0]){
					printSGR(31);
					playerhealth=playerhealth+shophp;
					std::cout<<"\nPURCHASED HP+"<<shophp<<" -> "<<playerhealth;
					std::this_thread::sleep_for(std::chrono::milliseconds(2000));
					playergold=playergold-shopcosts[0];
				}
			}else {
				if (inp != 5){
					if (inp == 6){
						
						std::vector<int> inputs = {};
						printSGR(0);	
						std::cout<<"[---]\n";
						
						siz = items.size();
						double sellcost = 1;
						
						sellcost = sellcalc(character, items, lootsizes, gamestate);
						sellcost = sellcost/shopdiscount;
						
						int sc = 0;
						for (int i = 0; i < siz; i++){
							if (items[i] <lootsizes[0]){
								sc = int(sellcost*2);
								printSGR(37);
							}else if (items[i] <lootsizes[1]){
								sc = int(sellcost*4);
								printSGR(31);
							}else if (items[i] <lootsizes[2]){
								sc = int(sellcost*8);
								printSGR(1);
								printSGR(33);
							}else if (items[i] <lootsizes[3]){
								sc = int(sellcost*15);
								printSGR(1);
								printSGR(6);
								printSGR(35);
							}
							inputs.emplace_back(i);
							std::cout<<"[ ITEM "<<i<<". |["<<lootnames[items[i]*2]<<lootnames[items[i]*2+1]<<" ] -> SELL FOR: "<<sc<<" G\n";
							printSGR(0);
						}
						std::cout<<"\n[---]\n\n";
						std::cout<<siz<<". BACK";
						inputs.emplace_back(siz);
						std::cout<<"\n<-->\nSelect an item: \n";
						std::this_thread::sleep_for(std::chrono::milliseconds(1000));
						int sellitem = 0;
						sellitem = selectmenu(inputs);
						
						if (sellitem != siz){
							int sc = 0;
							if (items[sellitem] <lootsizes[0]){
								sc = int(sellcost*2);
							}else if (items[sellitem] <lootsizes[1]){
								sc = int(sellcost*4);
							}else if (items[sellitem] <lootsizes[2]){
								sc = int(sellcost*8);
							}else if (items[sellitem] <lootsizes[3]){
								sc = int(sellcost*15);
							}
							playergold = playergold + int(sc);
							
							std::cout<<"ITEM: "<<lootnames[items[sellitem]*2];
							printSGR(6);
							printSGR(33);
							std::cout<<" SOLD ";
							printSGR(0);
							std::cout<<"for "<<sc<<" G\n";
							items.erase(items.begin()+sellitem);
							solditems = solditems + 1;
							
						}
						
						
						
						
						
					}else if (playergold >= shopcosts[inp] and inps[inp] != -1){
						printSGR(31);
						std::cout<<"\nPURCHASED "<<lootnames[shopitems[inp]*2];
						std::this_thread::sleep_for(std::chrono::milliseconds(1000));
						playergold=playergold-shopcosts[inp];
						items.emplace_back(shopitems[inp]);
						inps[inp] = -1;
						
						printSGR(0);	
						std::cout<<"\n|: [ ";
						
						siz = items.size();
						for (int i = 0; i < siz; i++){
							if (items[i] <lootsizes[0]){
								printSGR(37);
							}else if (items[i] <lootsizes[1]){
								printSGR(31);
							}else if (items[i] <lootsizes[2]){
								printSGR(1);
								printSGR(33);
							}else if (items[i] <lootsizes[3]){
								printSGR(1);
								printSGR(6);
								printSGR(35);
							}
							std::cout<<lootnames[items[i]*2];
							printSGR(0);
							if (i == siz-1){
								std::cout<<" ";
							}else{
								std::cout<<", ";
							}
						}
						std::cout<<"] :|\n";
						std::this_thread::sleep_for(std::chrono::milliseconds(1000));

					}
				}else {
					std::cout<<"\n";
				}
				
			}
			if (inp != 5){
			printSGR(0);
			std::cout<<"\n0. "<<10*(1/shopdiscount)<<" G -> "<<shophp<<" health\n";
			siz = shopitems.size();
			int shopcost = 0;
			for (int i = 1; i<siz-1; i++){
				printSGR(0);
				if (inps[i] == -1){
					printSGR(9);
				}
				std::cout<<i<<". ";

				if (shopitems[i] <lootsizes[0]){
					shopcost = 5*(1/shopdiscount);
					std::cout<<shopcost<<" G -> ";
					shopcosts.emplace_back(shopcost);
					printSGR(37);
				}else if (shopitems[i] <lootsizes[1]){
					shopcost = 10*(1/shopdiscount);
					std::cout<<shopcost<<" G -> ";
					shopcosts.emplace_back(shopcost);
					printSGR(31);
				}else if (shopitems[i] <lootsizes[2]){
					shopcost = 15*(1/shopdiscount);
					std::cout<<shopcost<<" G -> ";
					shopcosts.emplace_back(shopcost);
					printSGR(1);
					printSGR(33);
				}else if (shopitems[i] <lootsizes[3]){
					shopcost = 20*(1/shopdiscount);
					std::cout<<shopcost<<" G -> ";
					shopcosts.emplace_back(shopcost);
					printSGR(1);
					printSGR(6);
					printSGR(35);
				}
				std::cout<<lootnames[shopitems[i]*2]<<lootnames[shopitems[i]*2+1]<<"\n";
				printSGR(0);	
			}
			std::cout<<siz-1<<". EXIT\n";
			std::cout<<siz<<". SELL";
			printSGR(0);
			std::cout<<"\nGOLD: ";
			printSGR(34);
			std::cout<<playergold<<"\n";
			printSGR(0);
			
			}
		}
	}else {
		
		std::string title = "\nYOU DIED\n";
		printSGR(1);
		printSGR(31);
		siz = title.size();
		for (int i = 0; i<siz; i++){
			std::cout<<title[i];
			std::this_thread::sleep_for(std::chrono::milliseconds(200));
		}
		printSGR(0);
		
		
	}
	std::cout<<"\nEnter to continue: ";
	std::getline(std::cin,temp);
	std::cout<<"\033[F"<<"\x1b[2K<------->\n\n";

	//next ante
	
	
	
	
	ante++; //up the ante
	
	
	
	
	}//entire game
	screenreset();
	return 0;
}




int main(){
	//int menu = 0;
	while (true){
		int character = menu1(); //start screen
		menu2(character); //game?
	}

	
}
