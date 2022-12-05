// Author: Hyunjae Kim
// Date: 04/11/2022

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
// Library for BFS&DFS
#include <deque>
//priority queue for A* search
#include <queue> 


using namespace std;

struct NodeState {
  // Left Bank: Chicken, Wolf, Boat
	int LB[3]; 
  // Right Bank: Chicken, Wolf, Boat
	int RB[3]; 

  // bool operator to compare the NodeState structure
  bool operator==(const NodeState& state) const{
    return ((LB[0] == state.LB[0]) && (LB[1] == state.LB[1]) && (LB[2] == state.LB[2])
            &&(RB[0] == state.RB[0]) && (RB[1] == state.RB[1]) && (RB[2] == state.RB[2]));
  }
  
};

struct Node{
  Node* parent;
  NodeState state;
  int depth;
  int priority; 
};

// Compare function for priority_queue
// priority = f(n) of A* search, the smaller value of f(n) has higher priority
struct compare{
  // The less amount of wolves in the right bank has higher priority
  // The smaller one will be the top of the priority queue
 bool operator()(const Node* p1, const Node* p2)const{
    return p1->priority > p2->priority;
  }
};

// Global Variables

// Counters for tracking nodes
int NodeCounter = 0;
int DepthCounter = 0;

// Initial State & Goal State
NodeState Init;
NodeState GS;

// movement of animals => {chicken, wolf}
vector<pair<int, int> > moves = {{2,0},{1,0},{1,1},{0,1},{0,2}};


// NodeState Checking Functions
int checkVisited(NodeState state, vector<Node*> visited){
  for(int i = 0;i < visited.size(); i++){
    if(state == visited[i]->state){
      return 1;
    }
  }
  return 0;
}
int checkFrontier(NodeState state, deque<Node*> frontier){
  for(int i = 0;i <  frontier.size(); i++){
    if(state == frontier[i]->state){
      return 1;
    }
  }
  return 0;
}

// condition1: N(chicken) >= N(wolf) for each bank
// condition2: wolf can be moved if N(chicken) = 0 on the same bank
// condition3: N(chicken) of each bank >=0
// condition4: N(wolf) of each bank >= 0
int checkValid(NodeState state){
  if((state.LB[0] >= state.LB[1] || state.LB[0] == 0) && (state.RB[0] >= state.RB[1] || state.RB[0] == 0)){
    if((state.LB[0]>=0 && state.LB[1] >= 0) && (state.RB[0] >= 0 && state.RB[1] >= 0)){
      return 1;
    }
  }
  return 0;
}

// Try every possible moves, and then collect the valid movements for each state
void MoveAnimals(Node* node, vector<NodeState>& bag){
  for(int i = 0; i < 5;i++){
    NodeState state = node->state;
    //Move from Right Bank to Left Bank
    //Movement is possible if there is at least one each animal
    if(state.RB[2] == 1 && (state.RB[0] >= 1 || state.RB[1] >=1)){ 
      state.RB[0] -= moves[i].first;
      state.RB[1] -= moves[i].second;
      state.RB[2] -= 1;
      state.LB[0] += moves[i].first;
      state.LB[1] += moves[i].second;
      state.LB[2] += 1; 
    }
    //Move from Left Bank to Right Bank
    //Movement is possible if there is at least one each animal
    else if(state.LB[2] == 1 && ((state.LB[0] >= 1 || state.LB[1] >=1))){ 
      state.RB[0] += moves[i].first;
      state.RB[1] += moves[i].second;
      state.RB[2] += 1;
      state.LB[0] -= moves[i].first;
      state.LB[1] -= moves[i].second;
      state.LB[2] -= 1;
    }
    // Check the movement is valid, and then collect the valid movement.
    if(checkValid(state) == 1){
      bag.push_back(state);
    } 
  }
}

// only expand the node for valid moves. 
Node* expandNode(Node* node, vector<NodeState> bag, int i){
  Node* child  = new Node();
  child->parent = node;
  child->depth = node->depth +1;
  DepthCounter = child->depth;
  child->state = bag[i];
  
  return child;
}

Node* bfs(Node* node, NodeState goal, int& NodeCounter){
  deque<Node*> frontier;
  vector<Node*> visited;
  vector<NodeState> bag;
  

  frontier.push_back(node);
  visited.push_back(node);

  while(1){
    if(frontier.empty()){
      return NULL;
    }
    node = frontier.front();
    frontier.pop_front();
  
    if(node->state == goal){
      return node;
    }
    
    MoveAnimals(node, bag);
    // Iterate for each valid movements
    for(int i = 0; i < bag.size(); i++){
      NodeCounter++;
      Node* child = expandNode(node, bag, i);
      if(child->state == goal){
        return child;
      }
      if(checkVisited(child->state, visited)==0){
        frontier.push_back(child);
        visited.push_back(child);
      }
    }
    bag.clear();
  } 
}

Node* dfs(Node* node, NodeState goal, int& NodeCounter){
  deque<Node*> frontier;
  vector<Node*> visited;
  vector<NodeState> bag;
  Node* child;

  frontier.push_back(node);
  visited.push_back(node);

  while(1){
    if(frontier.empty()){
      return NULL;
    }
    // stack operation of deque
    node = frontier.back();
    frontier.pop_back();

    if(node->state == goal){
      return node;
    }
    
    MoveAnimals(node, bag);
    // Iterate for each valid movements
    for(int i = 0; i < bag.size(); i++){
       NodeCounter++;
      child = expandNode(node, bag, i);
      if(child->state == goal){
        return child;
      }
      if(checkVisited(child->state, visited)==0){
        frontier.push_back(child);
        visited.push_back(child);
      }
    }
    bag.clear();
  }
}


Node* iddfs(Node* node, NodeState goal, int& NodeCounter){
  deque<Node*> frontier;
  vector<Node*> visited;
  vector<NodeState> bag;
  // Root Setting
  Node* start = node;
  Node* child;
  // Number Count
  int n = 0;
  // Depth Limit
  int capacity = 1; 
  // Max Depth for preventing infinite loop
  // Inifinite loop can occurr if there is no solution in the graph.
  int maxDepth = 1000;
  while(capacity != maxDepth){
    frontier.push_back(start);
    visited.push_back(start);
    // If solution found, Exit the loop
    while(n != capacity){
      if(frontier.empty()){
        return NULL;
      }
      // stack operation of deque
      node = frontier.back();
      frontier.pop_back();
      
      if(node->state == goal){
        return node;
      }
      
      
      MoveAnimals(node, bag);
      // Iterate for each valid movements
      for(int i = 0; i < bag.size(); i++){
         NodeCounter++;
        child = expandNode(node, bag, i);
        if(child->state == goal){
          return child;
        }
        if(checkVisited(child->state, visited)==0){
          frontier.push_back(child);
          visited.push_back(child);
        }
      } 
      bag.clear();
      n++;
    }
    // If solution not found, Restart the DFS.
    n = 0;
    //  Increase the Depth Limit
    capacity++;
    // reset frontier and visited list
    frontier.clear();
    visited.clear();
  }
  // If there is no solution in the graph, return failure
  return NULL;
}

// use priority element of node
Node* astar(Node* node, NodeState goal, int& NodeCounter){
  priority_queue<Node*, vector<Node*>, compare> frontier;
  vector<Node*> visited;
  vector<NodeState> bag;
  Node* child;
  // g(n) = 0
  // h(n) = N(wolf) in the right bank
  // f(n) = h(n)+g(n)
  node->priority = node->state.RB[1];
  frontier.push(node);
  visited.push_back(node);

  while(1){
    if(frontier.empty()){
      return NULL;
    }
    node = frontier.top();
    frontier.pop();
    
    if(node->state == goal){
      return node;
    }
    
    MoveAnimals(node, bag);
    // Iterate for each valid movements
    for(int i = 0; i < bag.size(); i++){
      NodeCounter++;
      child = expandNode(node, bag, i);
      // g(n) = 0
      // h(n) = N(wolf) in the right bank
      // f(n) = h(n)+g(n)
      child->priority =  child->state.RB[1];
      if(child->state == goal){
        return child;
      }
      if(checkVisited(child->state, visited)==0){
        frontier.push(child);
        visited.push_back(child);
      }
    }
    bag.clear();
  } 
}

// Print the Output on both screen and the output file. 
void printSteps(Node* solution, ofstream& File, int& counter){
  // initialize the global variable again.
  counter = 0;
  // Bag for printing the output from start state to goal state
  vector<NodeState> bag;
  // Put the solution path to the bag
  while(solution != NULL){
    bag.push_back(solution->state);
    solution = solution->parent;
    counter++;
  }
//  Print the solution path to both screen and output file.
// Order: Start state ~ Goal state
  cout << "Start State" << endl;
  File << "Start State" << endl;
  for(int i = bag.size()-1;i >= 0; i--){
    cout << endl;
    cout <<"Left Bank: " << bag[i].LB[0] << ","<<bag[i].LB[1] << "," << bag[i].LB[2] << endl;
    cout << "Right Bank: " << bag[i].RB[0] << ","<< bag[i].RB[1] << ","<< bag[i].RB[2] << endl;
    cout << endl;

    File << endl;
    File << "Left Bank: " << bag[i].LB[0] << ","<<bag[i].LB[1] << "," << bag[i].LB[2] << endl;
    File << "Right Bank: " << bag[i].RB[0] << ","<< bag[i].RB[1] << ","<< bag[i].RB[2] << endl;
    File << endl;
   
  }
  cout << "Goal State" << endl;
  File << "Goal State" << endl;

  cout << "Depth(Number of Nodes in Solution): " << counter << ", " <<"Node Expanded: " <<  NodeCounter << endl;
  File << "Depth(Number of Nodes in Solution): " << counter << ", " <<"Node Expanded: " <<  NodeCounter << endl;

  bag.clear();
}

void clearAll(Node* solution){
  while(solution != NULL){
    Node* s = solution;
    solution = solution->parent;
    delete s;
  }
}
void InitNode(Node* &starting, NodeState Init){
  starting->parent = NULL;
  starting->depth = 0;
  for(int i = 0; i < 3; i++){
    starting->state.LB[i] = Init.LB[i];
    starting->state.RB[i] = Init.RB[i];
  }
}

int main(int argc, char* argv[]){
  string c = argv[3]; // mode command line
  fstream File(argv[1], ios::in);
  char comma;
  File >> Init.LB[0] >> comma >> Init.LB[1] >>comma >> Init.LB[2];
  File >> Init.RB[0] >> comma >> Init.RB[1] >> comma >> Init.RB[2];
  File.close();
  fstream Goal(argv[2], ios::in);
  Goal >> GS.LB[0] >> comma >> GS.LB[1] >> comma >> GS.LB[2];
  Goal >> GS.RB[0] >> comma >> GS.RB[1] >> comma >> GS.RB[2];
  Goal.close();
  // Initialize the Start node(state)
  Node* starting = new Node();
  InitNode(starting, Init);
  Node* solution;
//  output file initialization
  ofstream Output(argv[4], ios_base::app);

  // parsing the output depending on the mode.  
  if(c == "bfs"){
  solution = bfs(starting, GS, NodeCounter);
  }
  
  if(c == "dfs"){
  solution = dfs(starting, GS, NodeCounter);
  }
  if(c == "iddfs"){
  solution = iddfs(starting, GS, NodeCounter);
  }
  if(c == "astar"){
  solution = astar(starting, GS, NodeCounter);
  }
  printSteps(solution, Output, DepthCounter);
  // clear all of the data of solution path
  clearAll(solution);
  Output.close();

return 0;
}
