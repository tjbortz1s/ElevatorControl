

//current floor position as an int value.
int cur_floor;
//node structure appears to hold the data and the poitner to the next node
typedef struct node{
    int floor;
    struct node* next;
}

//creates a node and attaches it to the headnode?
//the head node is the most recently added node
void push(struct node** head_ref, int new_data)
{
  //allocates a new node
  struct node* new_node = (struct node*) malloc(sizeof(struct node));
  //sets the data of the new node
  new_node->data  = new_data;
  //sets the new nodes next value to the reference of the passed node?
  new_node->next = (*head_ref);
  //sets the head reference to the new node
  (*head_ref)    = new_node;
}

//append, or create a node at the "end" of the linked list
//rather than creating a node at the beginning
void append(struct node** head_ref, int new_data)
{
  //alloate a new node
  struct node* new_node = (struct node*) malloc(sizeof(struct node));
  //creates a last node pointer that points to the headNode
  struct node *last = *head_ref;
  //assigns the data of the new node
  new_node->data  = new_data;
  //sets the pointer of the new node to a null "no-node value"
  new_node->next = NULL;

  //if the passed value is a null value, or no head reference exists
  //then set the new node to the head node reference
  if (*head_ref == NULL)
  {
     *head_ref = new_node;
     return;
  }

  //run down the nodes until a null value is reached
  while (last->next != NULL)
      last = last->next;
  //after which, assign the next value to the new node
  last->next = new_node;
  return;
}

//returns a count of the number of existing nodes
int count(node *head)
{
  node *cursor = head;
  int c = 0;
  while(cursor != NULL)
  {
      c++;
      cursor = cursor->next;
  }
  return c;
}

struct node* nex_floor = NULL;

int main(char argc, char** argv) {
	while(true) {
		if(nex_floor > cur_floor) {
			//motor up
		}
		if(nex_floor < cur_floor) {
			//motor down
		}
	}
}
