int cur_floor;
typedef struct node{
    int floor;
    struct node* next;
}

void push(struct node** head_ref, int new_data)
{
    struct node* new_node = (struct node*) malloc(sizeof(struct node));

    new_node->data  = new_data;
  
    new_node->next = (*head_ref);
  
    (*head_ref)    = new_node;
}

void append(struct node** head_ref, int new_data)
{
    struct node* new_node = (struct node*) malloc(sizeof(struct node));
 
    struct node *last = *head_ref;
  
    new_node->data  = new_data;
 
    new_node->next = NULL;
 
    if (*head_ref == NULL)
    {
       *head_ref = new_node;
       return;
    }  
      
    while (last->next != NULL)
        last = last->next;
  
    last->next = new_node;
    return;    
}

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

void queue_man(int next_floor){
		
		if((count(node) != 0) && (next_floor != cur_floor)){
			if(cur_floor == 1) {
					if(next_floor == next_floor){
						push(&node,2);
					}
					if((next_floor == 3) && (nex_floor != 2))  {
						push(&node, next_floor);
					}
					
						
			}
			if(cur_floor == 2 ) {
				push(&node,next_floor);
			}
			if(cur_floor == 3) {
					if(next_floor == 2){
						push(&node,next_floor);
					}
					if((next_floor == 1) && (nex_floor != 2))  {
						push(&node, next_floor);
					}		
			}
				
		}
		else{
			if(cur_floor != 1){
				append(&node,1);
			}
			else {
				sleep(2);
			}
		}
}



















