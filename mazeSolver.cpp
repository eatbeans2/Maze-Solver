//Beau Horenberger
//February 13th, 2018

//Maze solving program

/*This program will perform two operations.
 *First, it will open a file and read its data into a character array.
 *Then, it will traverse the map by reading the adjacent tiles into a stack.
 */

#include <iostream>
#include <fstream>

using namespace std;

//Linked list node. Will hold locations to explore in a stack.
struct node{
  node* next;
  int* location;
};

//Convenient name convention
typedef node* nodeptr;

//Start location for the linked list
nodeptr start = NULL;

void loadMap(char** & map, int* & dims);

void addToStack(int* location);

void popStack(int*& curLocation);

void deleteStack();

void loadMap(char** & map, int* & dims){
  //Loads the file into our fstream
  fstream mapFile;
  mapFile.open("newMap.txt");
  
  //This will hold the first line of code
  char* firstLine = new char[50];
  mapFile.getline(firstLine, 50);
  //The following initializes the dimensions based on the first line of the file
  int placeTracker = 0;
  for(int i = 0; i < strlen(firstLine)+1; i++)
    {
      //cout << strlen(firstLine) << " " << i << endl;
      if(firstLine[i]==' ' || firstLine[i]=='\0')
	{
	  char* newNum = new char[10];
	  if(placeTracker == 0)
	    {
	      memcpy(newNum, firstLine, i);
	      dims[0]=stoi(newNum);
	    }
	  else
	    {
	      memcpy(newNum, firstLine+placeTracker, strlen(firstLine)-i);
	      dims[1]=stoi(newNum);
	    }
	  delete[] newNum;
	  placeTracker = i;
	}
    }

  //Initializes the map
  map = new char*[dims[0]];
  for(int i = 0; i < dims[0]; i++)
    {
      //Plus 1 for the terminating characters
      map[i] = new char[dims[1]+1];
    }

  //Now we read the file to the map array
  for(int i = 0; i < dims[0]; i++)
    {
      mapFile.getline(map[i], dims[1]+1);
      //cout << map[i] << endl;
    }

  mapFile.close();
  delete[] firstLine;

}

//Adds new members to the start of the linked list
void addToStack(int* location)
{
  nodeptr n = new node;
  n->location = new int[2];
  n->location[0] = location[0];
  n->location[1] = location[1];  
  n->next = start;
  start = n;
}

//Returns data of the top member of the list, deletes it
void popStack(int*& curLocation){
  if(start != NULL)
    {
      // cout << "node deleted" << endl;
      nodeptr curr = start;
      curLocation[0] = curr->location[0];
      curLocation[1] = curr->location[1];
      start = curr->next;
      delete curr;
    }
}

void deleteStack(){
  nodeptr cur;
  while(start != NULL)
    {
      cur = start;
      start=start->next;
      delete cur;
    }
}

int main(){

  //Holds the file data
  char** map;
  int* dims = new int[2];

  int* curLocation = new int[2];

  //Loads the data from the file
  loadMap(map, dims);

  //Copy to track our progress
  char** progMap = new char*[dims[0]];
  for(int i = 0; i < dims[0]; i++)
    {
      progMap[i] = new char[dims[1]+1];
      for(int j = 0; j < dims[1]+1; j++)
	{
	  //Initializes location at the start char
	  if(map[i][j] == 'S')
	    {
	      progMap[i][j] = 'C';
	      curLocation[0] = i;
	      curLocation[1] = j;
	    }
	  else
	    progMap[i][j] = map[i][j];
	}
      //cout << progMap[i] << endl;
    }

  //Prints the map
  for(int i = 0; i < dims[0]; i++)
    {
      cout << progMap[i] << endl;
    }
  cout << endl;

  //Loop to simulate progression
  int isFound = 0;
  //Used to add surrounding spaces to stack
  int* nextVal;

  while(isFound == 0)
    {
      nextVal = new int[2];

      //Determines whether each direction should be added to stack based on its contents
      if(curLocation[0] > 0 && (progMap[curLocation[0]-1][curLocation[1]] != 'X' && progMap[curLocation[0]-1][curLocation[1]] != '0' && progMap[curLocation[0]-1][curLocation[1]] != '#'))
	{
	  //Marks the location as "in the stack"
	  progMap[curLocation[0]-1][curLocation[1]]= 'X';
	  nextVal[0] = curLocation[0]-1;
	  nextVal[1] = curLocation[1];
	  addToStack(nextVal);
	}
      //Could DEFINITELY automate this better...
      if(curLocation[0] < dims[1]-1 && (progMap[curLocation[0]+1][curLocation[1]] != 'X' && progMap[curLocation[0]+1][curLocation[1]] != '0' && progMap[curLocation[0]+1][curLocation[1]] != '#'))
	{
	  progMap[curLocation[0]+1][curLocation[1]]= 'X';
	  nextVal[0] = curLocation[0]+1;
	  nextVal[1] = curLocation[1];
	  addToStack(nextVal);
	}
      //Maybe make a function for it? Lots of vars to pass around...
      if(curLocation[1] > 0 && (progMap[curLocation[0]][curLocation[1]-1] != 'X' && progMap[curLocation[0]][curLocation[1]-1] != '0' && progMap[curLocation[0]][curLocation[1]-1] != '#'))
	{
	  progMap[curLocation[0]][curLocation[1]-1]= 'X';
	  nextVal[0] = curLocation[0];
	  nextVal[1] = curLocation[1]-1;
	  addToStack(nextVal);
	}
      if(curLocation[1] < dims[0]-1 && (progMap[curLocation[0]][curLocation[1]+1] != 'X' && progMap[curLocation[0]][curLocation[1]+1] != '0' && progMap[curLocation[0]][curLocation[1]+1] != '#'))
	{
	  progMap[curLocation[0]][curLocation[1]+1]= 'X';
	  nextVal[0] = curLocation[0];
	  nextVal[1] = curLocation[1]+1;
	  addToStack(nextVal);
	}

      //cout << curLocation[0] << " " << curLocation[1] << endl;

      //Now we update the map with our current location
      progMap[curLocation[0]][curLocation[1]] = 'C';
     
      //Update our location and draw map if we're done
      progMap[curLocation[0]][curLocation[1]] = '0';

      popStack(curLocation);

      delete[] nextVal;

      if(map[curLocation[0]][curLocation[1]] == 'G')
	{
	  for(int i = 0; i < dims[0]; i++)
	    {
	      cout << progMap[i] << endl;
	    }
	  isFound = 1;
	}
    }

  //Deleting all our remaining dynamic memory
  for(int i = 0; i < dims[0]; i++)
    {
  delete[] map[i];
  delete[] progMap[i];
    }
  delete[] map;
  delete[] progMap;
  delete[] dims;
  delete[] curLocation;

  deleteStack();
}
