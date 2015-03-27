// Constant Definitions
#define BAUD_RATE 57600
#define END_DELAY 100

#include "double_list.h"

// Structure for each node
typedef struct point {
  // X & Y cordinates
  float x;
  float y;
  
  // Is this a waypoint
  boolean waypoint;
} point;

// Global variables
DoubleList <point> route;

void setup() {
  Serial.begin(BAUD_RATE);
}

void loop() {
  
  list_test();
  
  // Wait at end of test
  while (true) {
    delay(END_DELAY); 
  }
}

void list_test() {
  point newEle;
  
  DoubleNode<point> *temp;
  
  //fill_list();
  
  //print_path();

  
  // Add first on empty
  Serial.println("Add First on Empty:");
  
  newEle.x = 1;
  newEle.y = 1;
  newEle.waypoint = true;
  
  route.addFirst(newEle);

  print_path();

  // Add first on > 1
  Serial.println("Add First on > 1:");

  newEle.x = 2;
  newEle.y = 2;
  newEle.waypoint = false;

  route.addFirst(newEle);

  print_path();

  // Remove last on > 1
  Serial.println("Remove Last on > 1:");
  
  route.removeLast();
  
  print_path();
 
  // Remove last on Single
  Serial.println("Remove Last on Single:");
  
  route.removeLast();
  
  print_path();

  // Add last on empty
  Serial.println("Add Last on Empty:");
  
  newEle.x = 3;
  newEle.y = 3;
  newEle.waypoint = true;
  
  route.addLast(newEle);
  
  print_path();
 
  // Add last on > 1
  Serial.println("Add Last on > 1:");
  
  newEle.x = 4;
  newEle.y = 4;
  newEle.waypoint = false;
  
  route.addLast(newEle);

  print_path();

  // Remove first on > 1
  Serial.println("Remove First on > 1");
  
  route.removeFirst();
  
  print_path();
  
  // Remove first on single
  Serial.println("Remove First on Single");
  
  route.removeFirst();
  
  print_path();

  // Fill list with new data
  fill_list();
  
  // Add Before Center Item
  Serial.println("Add Before Last");
  
  newEle.x = 100;
  newEle.y = 100;
  newEle.waypoint = true;
  
  route.addBefore(route.last(), newEle);
  
  print_path();
  
  // Add After Center Item
  Serial.println("Add After First");
  
  newEle.x = 200;
  newEle.y = 200;
  newEle.waypoint = false;
  
  route.addAfter(route.first(), newEle);
  
  print_path();
  
  // Remove Center Item
  Serial.println("Remove Center");
  
  temp = route.first();
  
  route.remove(temp->next());
  
  print_path();
  
  // Remove Center Item
  Serial.println("Remove Center");
  
  temp = route.last();
  
  route.remove(temp->prev());
  
  print_path();
  
  // Clear List
  Serial.println("Clear List");
  
  route.clear();
  
  print_path();
  
}

void fill_list() {
  boolean fixed = true;
  point newEle;
  
  for (int i = 0; i < 10; i++) {
    newEle.x = i;
    newEle.y = i;
    newEle.waypoint = fixed;
    
    route.addLast(newEle);
   
    fixed = !fixed;
  }
}

//
// Serial debug
//

// Print path to debug
void print_path() {
  for (DoubleNode<point> * node = route.first(); node != NULL; node = node->next()) {
      Serial.print(1);Serial.print(") ");
      Serial.print("x: ");Serial.print(node->value.x);Serial.print(" ");
      Serial.print("y: ");Serial.print(node->value.y);Serial.print(" ");
      Serial.print("f: ");Serial.print(node->value.waypoint);Serial.println();
  }
  
  print_break();
}

// Print pointer
void print_pointers() {
  /*
  Serial.print("a: ");Serial.print(uint16_t(node), HEX);Serial.print(" ");
  Serial.print("p: ");Serial.print(uint16_t(node->Prev), HEX);Serial.print(" ");
  Serial.print("n: ");Serial.print(uint16_t(node->Next), HEX);Serial.println();
  
  print_break();
  */
}

// Print break to debug
void print_break() {
  Serial.println("==============================");
}
