//----------------------------------------------------------------------------
//                  CS 215 - Project 2 - MiniMud
//----------------------------------------------------------------------------
// Author:  David Pippen
// Date:  3/10/19
// Description:  Its a game
// Assistance: I received help from the following: Isabella Bowling and Aaron Bussell
//-----------------------------------------------------------
#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include "gameutils.h"

using namespace std;
//----------------------------------------------------------------------------
//							printLogo
//----------------------------------------------------------------------------
// Prints the logo.
//----------------------------------------------------------------------------
void printLogo()
{
	cout << "+---------------------------+\n";
	cout << "$     Walking Simulator     $\n";
	cout << "$      BY: David Pippen     $\n";
	cout << "+---------------------------+\n";
}//end printLogo

//----------------------------------------------------------------------------
//							getInputFile
//----------------------------------------------------------------------------
// Gets the name of the file of the game.
//----------------------------------------------------------------------------
string getInputFile()
{
	string name;
	cout << "Enter the name of the game file(no .txt): ";
	cin >> name;
	name += ".txt";
	return name;
}//end getInputFile

//----------------------------------------------------------------------------
//							initItems
//----------------------------------------------------------------------------
// takes an array and sets all of the descriptions to ""
//----------------------------------------------------------------------------
void initItems(item itemList[])
{
	for (int i = 0; i < MAX_ITEMS; ++i)
	{
		itemList[i].shortDesc = "";
		itemList[i].longDesc = "";
	}
}//end initItems

//----------------------------------------------------------------------------
//							initRooms
//----------------------------------------------------------------------------
// array of rroms that, sets descriptions to null, rooms have no exits and
// calls initItes for room Items
//----------------------------------------------------------------------------
void initRooms(room rooms[])
{
	for (int i = 0; i < MAX_ROOMS; ++i)
	{
		//sets descritpin of the room to ""
		rooms[i].shortDesc = "";
		rooms[i].longDesc = "";

		//makes no exit from the room
		rooms[i].north = NO_EXIT;
		rooms[i].east = NO_EXIT;
		rooms[i].south = NO_EXIT;
		rooms[i].west = NO_EXIT;

		//room inventory is initialized
		initItems(rooms[i].items);
		rooms[i].numItems = 0;
	}
}//end initRooms

//----------------------------------------------------------------------------
//							initWinData
//----------------------------------------------------------------------------
// initializes the win data
//----------------------------------------------------------------------------
void initWinData(winDataStruct &win)
{
	win.winMessage = "";
	win.winRoom = NO_EXIT;
	initItems(win.winItem);
	win.numWinItems = 0;
}//end initWinData


//----------------------------------------------------------------------------
//							addItem
//----------------------------------------------------------------------------
// adds an Item to the given Item array
//----------------------------------------------------------------------------
void addItem(item added, item itemList[], int &numberItems)
{
	string error;
	//if there are to many items in your inventory and cant pick up
	if (numberItems >= MAX_ITEMS)
	{
		error = "addItem: maximum number of items exceeded!";
		gameAbort(error);
	}
	else
	{
		//adds the item and increases the number of items
		itemList[numberItems] = added;
		numberItems += 1;
	}
}// addItem

//----------------------------------------------------------------------------
//							addItem
//----------------------------------------------------------------------------
// makes item from decriptions and then calls add item
//----------------------------------------------------------------------------
void addItem(string shortD, string longD, item items[], int &numItems)
{
	item newItem;
	//gives the new item the given descritions
	newItem.shortDesc = shortD;
	newItem.longDesc = longD;
	//calls the real additem with the new item
	addItem(newItem, items, numItems);
}

//----------------------------------------------------------------------------
//							makeExitList
//----------------------------------------------------------------------------
string makeExitList(room thisRoom) {
	string exitList = "You can go:";
	//see if you can go a direction
	if (thisRoom.north != NO_EXIT) exitList += " north,";
	if (thisRoom.south != NO_EXIT) exitList += " south,";
	if (thisRoom.east != NO_EXIT) exitList += " east,";
	if (thisRoom.west != NO_EXIT) exitList += " west.";
	exitList[exitList.length() - 1] = '.'; // change last comma to a period
	return exitList;
} // makeExitList()

//----------------------------------------------------------------------------
//							doLook
//----------------------------------------------------------------------------
// displays exits, items and room description in the room.
//----------------------------------------------------------------------------
void doLook(room roomIn, bool shortDiscY)
{
	if (shortDiscY)
		cout << roomIn.shortDesc << endl;
	else
		cout << roomIn.longDesc << endl;
	//if there are item then you notice the following.
	if (roomIn.numItems != 0)
		cout << "You notice the following: ";
	//if there are multiple items then print in a special way with commas followed with a period.
	if (roomIn.numItems == 1)
		cout << roomIn.items[0].shortDesc << endl;
	else if (roomIn.numItems >= 2)
	{
		for (int i = 0; i < roomIn.numItems - 1; ++i)
			cout << roomIn.items[i].shortDesc << ", ";
		cout << roomIn.items[(roomIn.numItems) - 1].shortDesc << ".\n";
	}
	//prints where you can go
	cout << makeExitList(roomIn) << endl;
}//doLook

//----------------------------------------------------------------------------
//							doLook
//----------------------------------------------------------------------------
// calls other doLook with false
//----------------------------------------------------------------------------
void doLook(room roomIn)
{
	//cals do look and prints the long description
	doLook(roomIn, false);
}//doLook

//----------------------------------------------------------------------------
//							loadData
//----------------------------------------------------------------------------
void loadData(room rooms[], int & numRooms, winDataStruct & winData) {
	// variable declarations
	ifstream f;				// input file handle
	string tag;				// first word extracted from each line
	string filename;		// name of input file
	string data;			// data read from file using cleanGetline()
	int roomNdx = -1;		// index of the "current room" being read/populated
							// this should always be rumRooms-1 

	// initialize the rooms and winData
	initRooms(rooms);
	initWinData(winData);

	// get input file name and open the input file; abort on failure
	filename = getInputFile();
	f.open(filename);
	if (f.fail())
		gameAbort("Unable to open " + filename);
	// read line-by-line and store data in the game data structures
	f >> tag;
	while (tag != "END:") {
		if (tag == "ROOM:") {
			numRooms++;		// starting a new room
			roomNdx++;		// increment every time numRooms is incremented
			cleanGetline(f, rooms[roomNdx].shortDesc); // short desc on remainder of ROOM: line
			cleanGetline(f, rooms[roomNdx].longDesc);  // long desc on next line by itself (no tag)
		}
		else if (tag == "ITEM:") {
			string shortDesc, longDesc;
			cleanGetline(f, shortDesc); //get shortDesc
			cleanGetline(f, longDesc);  //get longDesc
			addItem(shortDesc, longDesc, rooms[roomNdx].items, rooms[roomNdx].numItems); //add the item with given short & long desc
		}
		else if (tag == "WIN_ITEM:") {
			cleanGetline(f, data);
			addItem(data, "", winData.winItem, winData.numWinItems); //adds an item to the win condition item list
		}
		//direction reads for all 4 directions
		else if (tag == "NORTH:")
		{
			f >> rooms[roomNdx].north;
		}
		else if (tag == "SOUTH:")
		{
			f >> rooms[roomNdx].south;
		}
		else if (tag == "EAST:")
			f >> rooms[roomNdx].east;
		else if (tag == "WEST:")
			f >> rooms[roomNdx].west;
		else if (tag == "WIN_ROOM:")
			f >> winData.winRoom; //room to drop and to win in.
		else if (tag == "REMARK:")
		{
			cleanGetline(f, data);		// data is junk, throw it away
		}
		else if (tag == "WIN_TEXT:")
		{
			cleanGetline(f, data); 
			winData.winMessage = data; //gets the win message that prints when the game is won. 
		}
		else
			gameAbort("Unknown tag in " + filename + ": " + tag);//error and ends the game because of errors

		f >> tag;
		// check for read failure and read the next tag
		if (f.fail())
			gameAbort("Failure while reading input file " + filename);
	}
	f.close();
} // loadData()




//----------------------------------------------------------------------------
//							doInventory
//----------------------------------------------------------------------------
// Given: the player's inventory (partial array of items)
// This is the game's response to the "inv"/"i" (inventory) command.
// Prints a message telling the player what he/she is holding.
//----------------------------------------------------------------------------
void doInventory(item item[], int numItems) {
	if (numItems == 0)
		cout << "You don't have anything.\n";
	else {
		cout << "You are holding: ";
		for (int i = 0; i < (numItems - 1); i++)		// for all but last item
			cout << item[i].shortDesc << ", ";			// print short desc with comma
		cout << item[numItems - 1].shortDesc << ".\n";  // for last item, use period and newline
	}
} // doInventory()

//----------------------------------------------------------------------------
//							goNorth
//----------------------------------------------------------------------------
// Given:    partial array of rooms
// Modifies: index of current room
// This is the game's response to the player entering the "north"/"n" command.
// When the north exit exists, changes the current room index and performs
// a "look" command with a short room description
void goNorth(int & currentRoom, room rooms[]) {
	if (rooms[currentRoom].north == NO_EXIT)
		cout << "You can't go north.\n";
	else {
		currentRoom = rooms[currentRoom].north;
		doLook(rooms[currentRoom], true); // true = show short room desc.
	}
} // goNorth()

//----------------------------------------------------------------------------
//							goSouth
//----------------------------------------------------------------------------
// Prints the logo.
//----------------------------------------------------------------------------
void goSouth(int & currentRoom, room rooms[]) {
	if (rooms[currentRoom].south == NO_EXIT)
		cout << "You can't go south.\n";//no exit
	else {
		currentRoom = rooms[currentRoom].south;
		doLook(rooms[currentRoom], true); // true = show short room desc.
	}
} // goSouth()

//----------------------------------------------------------------------------
//							goEast
//----------------------------------------------------------------------------
// Prints the logo.
//----------------------------------------------------------------------------
void goEast(int & currentRoom, room rooms[]) {
	if (rooms[currentRoom].east == NO_EXIT)
		cout << "You can't go east.\n";//no exit
	else {
		currentRoom = rooms[currentRoom].east;
		doLook(rooms[currentRoom], true); // true = show short room desc.
	}
} // goeast()

//----------------------------------------------------------------------------
//							goWest
//----------------------------------------------------------------------------
// Prints the logo.
//----------------------------------------------------------------------------
void goWest(int & currentRoom, room rooms[]) {
	if (rooms[currentRoom].west == NO_EXIT)
		cout << "You can't go west.\n";//no exit
	else {
		currentRoom = rooms[currentRoom].west;
		doLook(rooms[currentRoom], true); // true = show short room desc.
	}
} // gowest()


//----------------------------------------------------------------------------
//							removeItem
//----------------------------------------------------------------------------
// remozes an Item
//----------------------------------------------------------------------------
item removeItem(item items[], int index, int &numItems)
{
	string error;
	item removed;
	if (index >= numItems)
	{
		error = "removeItem: invalid index = " + index;
		gameAbort(error);//error in index of removing item
	}
	else
	{
		removed = items[index];//makes an item equal to the item at the place being removed
		for (int i = index; i < numItems - 1; ++i)
		{
			items[i] = items[i + 1];//moves all items one to the left
		}
		items[numItems-1].longDesc = "";//makes the last item which is a copy of the now moved one equal to nothing
		items[numItems-1].shortDesc = "";
		numItems --;
		return removed;//returned the removed item
	}
}// removeItems



//----------------------------------------------------------------------------
//							getCmd
//----------------------------------------------------------------------------
// gets comand from the user
//----------------------------------------------------------------------------
string getCmd(string &remainder)
{
	string comand;
	cout << "===> ";
	if (cin.peek() == '\n')
		cin.ignore();
	getline(cin, comand);
	comand = split(comand, remainder);
	return comand;
}//getCmd



//----------------------------------------------------------------------------
//							startUp
//----------------------------------------------------------------------------
// starts up the game
//----------------------------------------------------------------------------
void startUp(room rooms[], item items[], winDataStruct &win, int &numRooms)
{
	printLogo();//prints logo
	loadData(rooms, numRooms, win);//loads all the data
	initItems(items);//initializes the players inventory
	cout << " You fall asleep and dream of a place far away...\n";//prints out the beginning line of the game
	doLook(rooms[START_ROOM]);//prints the starting room description
}//startUp

//----------------------------------------------------------------------------
//							findItem
//----------------------------------------------------------------------------
// finds an Item
//----------------------------------------------------------------------------
int findItem(string itemName, item items[], int numItems)
{
	for (int i = 0; i < numItems; i++)
	{
		if (items[i].shortDesc == itemName)
			return i;//searches the list of items to see if the items name is the same as given. Then returns index of the item if its found
	}
	return NOT_FOUND;//return the not found if the item is not found
}//findItem

//----------------------------------------------------------------------------
//							doExamine
//----------------------------------------------------------------------------
// search for item and disp the long discription
//----------------------------------------------------------------------------
void doExamine(item roomInv[], int numRoomItems, item playerInv[], int numPlayerItems, string itemName)
{
	int itemSearch1,itemSearch2;
	//tries to find the item in eather the room or in the player inventory
	itemSearch1 = findItem(itemName, roomInv, numRoomItems);
	itemSearch2 = findItem(itemName, playerInv, numPlayerItems);
	if (itemSearch1 >= 0)
		cout << roomInv[itemSearch1].longDesc << endl;//if in room print the long description
	else if (itemSearch2 >= 0)
		cout << playerInv[itemSearch2].longDesc << endl;//if in player inventory then print the long descrition
	else if (itemSearch1 < 0 && itemSearch2 < 0)
		cout << "you see no " << itemName << ".\n";//not found in any inventory
}//doExamine

//----------------------------------------------------------------------------
//							doTake
//----------------------------------------------------------------------------
// take an item from the room and add it to player inventory
//----------------------------------------------------------------------------
void doTake(string itemName, item roomInv[], int &numRoomItem, item playerInv[], int &numPlayItem)
{
	int index;
	//finds the item
	index = findItem(itemName, roomInv, numRoomItem);
	//you cant pick up the item because hands are full
	if (index >= 0 && numPlayItem == MAX_ITEMS)
		cout << " Your hands are full, you can't take that.\n";
	else if (index >= 0 && numPlayItem < MAX_ITEMS)
	{
		//item able to be picked up so you add it to player inv and remove it from room inv.
		item removed = removeItem(roomInv, index, numRoomItem);
		addItem(removed, playerInv, numPlayItem);
		cout << "You take the " << itemName << ".\n";
	}
	else
	{
		//looks to see if you already have the item or there is no such item
		index = findItem(itemName, playerInv, numPlayItem);
		if (index == NOT_FOUND)
			cout << "you see no " << itemName << ".\n";
		else
			cout << "you already have " << itemName << ".\n";
	}
}//doTake

//----------------------------------------------------------------------------
//							doDrop
//----------------------------------------------------------------------------
// take an item from the room and add it to player inventory
//----------------------------------------------------------------------------
void doDrop(string itemName, item roomInv[], int &numRoomItem, item playerInv[], int &numPlayItem)
{
	int index;
	index = findItem(itemName, playerInv, numPlayItem); //find item
	if (index >= 0 && numRoomItem == MAX_ITEMS)
		//cant drop item because room is full
		cout << " The room is full of junk; you cant drop that.\n";
	else if (index >= 0 && numRoomItem < MAX_ITEMS)
	{
		//remove it from the player inv and add it to the room inv
		addItem(removeItem(playerInv, index, numPlayItem), roomInv, numRoomItem);
		cout << "You drop the " << itemName << ".\n";
	}
	else
	{
		index = findItem(itemName, roomInv, numRoomItem);
		if (index == NOT_FOUND)
			//not found in either inv
			cout << "you have no " << itemName << ".\n";
		else
			//found in the room and not in your inventory
			cout << "you dont have the " << itemName << ".\n";
	}
}//doDrop

//----------------------------------------------------------------------------
//							gameWon
//----------------------------------------------------------------------------
bool gameWon(room rooms[], winDataStruct win,int itemsInRoom)
{
	bool winner = true;
	for (int i = 0; i < win.numWinItems; ++i)
	{
		if (findItem(win.winItem[i].shortDesc, rooms[win.winRoom].items, itemsInRoom) == NOT_FOUND)//looks to see if the game was won.
			winner = false;//if no return false
	}
	return winner;
}//gameWon



//----------------------------------------------------------------------------
//							main
//----------------------------------------------------------------------------
int main() {
	// declare data structures
	room rooms[MAX_ROOMS];
	item itemsP[MAX_ITEMS];
	int roomIn = START_ROOM;
	winDataStruct win;
	int numRooms = 0;
	int numItems = 0;

	// declare other variables
	string cmd;					// command entered by user (first word of input)
	string remainder;			// remainder of command entered by user
	bool   gameWasWon = false;	// check with gameWon() as needed

	// start up: load game data from file and initialize player inventory
	startUp(rooms, itemsP, win, numRooms);
	// get first command input
	cmd = getCmd(remainder);

	// repeat until "exit" entered or the game is won
	while (cmd != "exit" && !gameWasWon) {//while game is not won and the cmand is not exit
		if (cmd == "help")
			printHelp();
		// comands to go the 4 directions
		else if (cmd == "s" || cmd == "south")
			goSouth(roomIn, rooms);
		else if (cmd == "n" || cmd == "north")
			goNorth(roomIn, rooms);
		else if (cmd == "w" || cmd == "west")
			goWest(roomIn, rooms);
		else if (cmd == "e" || cmd == "east")
			goEast(roomIn, rooms);
		//look
		else if (cmd == "look" || cmd == "l")
			doLook(rooms[roomIn]);
		//examine
		else if (cmd == "x" || cmd == "exa")
		{
			doExamine(rooms[roomIn].items, rooms[roomIn].numItems, itemsP, numItems, remainder);
		}
		//inventory
		else if (cmd == "i" || cmd == "inv")
		{
			doInventory(itemsP, numItems);
		}
		//take the item
		else if (cmd == "t" || cmd == "take")
			doTake(remainder, rooms[roomIn].items, rooms[roomIn].numItems, itemsP, numItems);
		//drop the item
		else if (cmd == "d" || cmd == "drop")
		{
			doDrop(remainder, rooms[roomIn].items, rooms[roomIn].numItems, itemsP, numItems);
			gameWasWon = gameWon(rooms, win, rooms[win.winRoom].numItems);
		}
		else // the user entered an unrecognized command
			cout << "What??\n";

		// check if game was won and print win message or get next user input
		if (gameWasWon)
			cout << win.winMessage << endl;
		else
			cmd = getCmd(remainder);
	}
	system("pause");
	return 0;
}