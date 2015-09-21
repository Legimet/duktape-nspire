#ifndef __FLOOD_FILL_H__
#define __FLOOD_FILL_H__ 1

 /***************************************
 * Flood fill : non recursive algorithm *
 *                                      *
 ***************************************/

  // later look at : http://www.codeproject.com/Articles/6017/QuickFill-An-efficient-flood-fill-algorithm

	typedef struct ffnode_ { void* next; int x, y; } ffnode_t;
  typedef struct queue_ { ffnode_t* head; ffnode_t* last; } queue_t;

  bool _isEmpty(queue_t* queue) {
  	return queue->head == NULL;
  }
  
  void _add(queue_t* queue, ffnode_t* node) {
  	if ( queue->head == NULL ) { queue->head = node; }
  	if ( queue->last != NULL ) { queue->last->next = node; }
  	queue->last = node;
  }
  
  ffnode_t* _remove(queue_t* queue) {
  	if ( queue->head == NULL ) { return NULL; }
  	ffnode_t* rem = queue->head;
  	queue->head = queue->head->next;
  	return rem;
  }


  queue_t* new_queue() {
	    queue_t *queue = (queue_t*)malloc(sizeof(queue_t));
	    queue->head = NULL;
	    queue->last = NULL;
	    return queue;
	}
	
	ffnode_t* new_ffnode(int x, int y) {
	    ffnode_t *node = (ffnode_t*)malloc(sizeof(ffnode_t));
	    node->x = x; node->y = y;
	    node->next = NULL;
	    return node;
	}
	
	// beware w/ memeroy hole
	void flood_fill(uint16_t* image, int imageWidth, int imageHeight, int startx, int starty, uint16_t color) {
	    ffnode_t *node = NULL;
	    ffnode_t *w = NULL;
	    ffnode_t *e = NULL;
	    ffnode_t *n = NULL;
	    ffnode_t *s = NULL;
	
	    queue_t* q = new_queue();
	
	    node = new_ffnode(startx, starty);
	    _add(q,node);
	    
	    while ( !_isEmpty(q) ) {
	    	node = _remove(q);
	    	 if ( node->x >= 0 && node->x < imageWidth && node->y >= 0 && node->y < imageHeight ) {
	        if (image[ (node->y*imageWidth)+node->x ] != color) {
	
	            image[ (node->y*imageWidth)+node->x ] = color;

	            e=new_ffnode( node->x+1, node->y );
	            _add(q,e);
	            
							w=new_ffnode( node->x-1, node->y );
							_add(q,w);
							
							s=new_ffnode( node->x, node->y+1 );
							_add(q,s);
							
							n=new_ffnode( node->x, node->y-1 );
              _add(q,n);
	        }
	      }
	      //node = NULL;
	      free(node);
	    }
	    //q = NULL;
	    free(q);
	}

#endif