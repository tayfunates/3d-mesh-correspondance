#ifndef _FIBHEAP_H
#define _FIBHEAP_H

//adapted (www.nist.gov/dads/HTML/fibonacciHeap.html) by ysf

//***************************************************************************
// The Fibonacci heap implementation contained in FIBHEAP.H and FIBHEAP.CPP
// is Copyright (c) 1996 by John Boyer
//
// Once this Fibonacci heap implementation (the software) has been published
// by Dr. Dobb's Journal, permission to use and distribute the software is
// granted provided that this copyright notice remains in the source and
// and the author (John Boyer) is acknowledged in works that use this program.
//
// Every effort has been made to ensure that this implementation is free of
// errors.  Nonetheless, the author (John Boyer) assumes no liability regarding
// your use of this software.
//
// The author would also be very glad to hear from anyone who uses the
// software or has any feedback about it.
// Email: jboyer@gulf.csc.uvic.ca
//***************************************************************************

#define OK      0
#define NOTOK   -1

//#include <iostream>
//using namespace std;
//#include <math.h>

class FibHeapNode
{
public:
	FibHeapNode *Left, *Right, *Parent, *Child;
	short Degree, Mark; //, NegInfinityFlag;

	int element; //this node represents mesh.verts[element]
	float key; //heap shaped on this value

	FibHeapNode(float ke, int ele)
		: Left(NULL), Right(NULL), Parent(NULL), Child(NULL), Degree(0), key(ke), element(ele), Mark(0) {}

	~FibHeapNode()
	{
		//destructor to reclaim memo
		delete Left;
		delete Right;
		delete Parent;
		delete Child;
	}
};



class FibHeap
{
public:
 /** Creates a new instance of FibHeap */
	FibHeap() : MinRoot(NULL), NumNodes(0), NumTrees(0), NumMarkedNodes(0) {};

//===========================================================================
// Insert() - O(1) actual; O(2) amortized
//
// I am using O(2) here to indicate that although Insert() is
// constant time, its amortized rating is more costly because some
// of the work of inserting is done by other operations such as
// ExtractMin(), which is where tree-balancing occurs.
//
// The child pointer is deliberately not set to NULL because Insert()
// is also used internally to help put whole trees onto the root list.
//===========================================================================
	void Insert(FibHeapNode* NewNode)
	{
		if (NewNode == NULL) return;

// If the heap is currently empty, then new node becomes singleton
// circular root list
 
     if (MinRoot == NULL)
	 MinRoot = NewNode->Left = NewNode->Right = NewNode;

     else
     {
// Pointers from NewNode set to insert between MinRoot and MinRoot->Right

         NewNode->Right = MinRoot->Right;
	 NewNode->Left = MinRoot;

// Set Pointers to NewNode  

	 NewNode->Left->Right = NewNode;
         NewNode->Right->Left = NewNode;

// The new node becomes new MinRoot if it is less than current MinRoot

         //if (*NewNode < *MinRoot)
		 if (NewNode->key < MinRoot->key)
	     MinRoot = NewNode;
     }

// We have one more node in the heap, and it is a tree on the root list

     NumNodes++;

     NumTrees++;
     NewNode->Parent = NULL;
	}
	
	//===========================================================================
// Union() - O(1) actual; O(1) amortized
//===========================================================================
void Union(FibHeap *OtherHeap)
{
FibHeapNode *Min1, *Min2, *Next1, *Next2;

     if (OtherHeap == NULL || OtherHeap->MinRoot == NULL) return;

// We join the two circular lists by cutting each list between its
// min node and the node after the min.  This code just pulls those
// nodes into temporary variables so we don't get lost as changes
// are made.

     Min1 = MinRoot;
     Min2 = OtherHeap->MinRoot;
     Next1 = Min1->Right;
     Next2 = Min2->Right;

// To join the two circles, we join the minimum nodes to the next
// nodes on the opposite chains.  Conceptually, it looks like the way
// two bubbles join to form one larger bubble.  They meet at one point
// of contact, then expand out to make the bigger circle.
 
     Min1->Right = Next2;
     Next2->Left = Min1;
     Min2->Right = Next1;
     Next1->Left = Min2;

// Choose the new minimum for the heap
 
     //if (*Min2 < *Min1)
	 if (Min2->key < Min1->key)
         MinRoot = Min2;

// Set the amortized analysis statistics and size of the new heap
                   
     NumNodes += OtherHeap->NumNodes;
     NumMarkedNodes += OtherHeap->NumMarkedNodes;
     NumTrees += OtherHeap->NumTrees;

// Complete the union by setting the other heap to emptiness
// then destroying it

     OtherHeap->MinRoot  = NULL;
     OtherHeap->NumNodes =
     OtherHeap->NumTrees =
     OtherHeap->NumMarkedNodes = 0;

     delete OtherHeap;
}

//===========================================================================
// Minimum - O(1) actual; O(1) amortized
//===========================================================================
FibHeapNode* Minimum()
{
     return MinRoot;
}

//===========================================================================
// ExtractMin() - O(n) worst-case actual; O(lg n) amortized
//===========================================================================
	FibHeapNode* ExtractMin()
{
FibHeapNode *Result;
FibHeap *ChildHeap = NULL;

// Remove minimum node and set MinRoot to next node

     if ((Result = Minimum()) == NULL)
          return NULL;

     MinRoot = Result->Right;
     Result->Right->Left = Result->Left;
     Result->Left->Right = Result->Right;
     Result->Left = Result->Right = NULL;

     NumNodes --;
     if (Result->Mark)
     {
	 NumMarkedNodes --;
         Result->Mark = 0;
     }
     Result->Degree = 0;

// Attach child list of Minimum node to the root list of the heap
// If there is no child list, then do no work

     if (Result->Child == NULL)
     {
	 if (MinRoot == Result)
	     MinRoot = NULL;
     }

// If MinRoot==Result then there was only one root tree, so the
// root list is simply the child list of that node (which is
// NULL if this is the last node in the list)

     else if (MinRoot == Result)
         MinRoot = Result->Child;

// If MinRoot is different, then the child list is pushed into a
// new temporary heap, which is then merged by Union() onto the
// root list of this heap.

     else 
     {
         ChildHeap = new FibHeap();
         ChildHeap->MinRoot = Result->Child;
     }

// Complete the disassociation of the Result node from the heap

     if (Result->Child != NULL)
	 Result->Child->Parent = NULL;
     Result->Child = Result->Parent = NULL;

// If there was a child list, then we now merge it with the
//	rest of the root list

     if (ChildHeap)
         Union(ChildHeap);

// Consolidate heap to find new minimum and do reorganize work

     if (MinRoot != NULL)
         _Consolidate();

// Return the minimum node, which is now disassociated with the heap
// It has Left, Right, Parent, Child, Mark and Degree cleared.

     return Result;
}

//===========================================================================
// ExtractNodeForDeletion() - O(1) worst-case actual; just returns min (root) to
// delete it; not maintaining the fib-heap property; use for memo dealloc only
//===========================================================================
	FibHeapNode* ExtractNodeForDeletion()
{
FibHeapNode *Result;
FibHeap *ChildHeap = NULL;

// Remove minimum node and set MinRoot to next node

     if ((Result = Minimum()) == NULL)
          return NULL;

     MinRoot = Result->Right;
     Result->Right->Left = Result->Left;
     Result->Left->Right = Result->Right;
     Result->Left = Result->Right = NULL;

     NumNodes --;
     if (Result->Mark)
     {
	 NumMarkedNodes --;
         Result->Mark = 0;
     }
     Result->Degree = 0;

// Attach child list of Minimum node to the root list of the heap
// If there is no child list, then do no work

     if (Result->Child == NULL)
     {
	 if (MinRoot == Result)
	     MinRoot = NULL;
     }

// If MinRoot==Result then there was only one root tree, so the
// root list is simply the child list of that node (which is
// NULL if this is the last node in the list)

     else if (MinRoot == Result)
         MinRoot = Result->Child;

// If MinRoot is different, then the child list is pushed into a
// new temporary heap, which is then merged by Union() onto the
// root list of this heap.

     else 
     {
         ChildHeap = new FibHeap();
         ChildHeap->MinRoot = Result->Child;
     }

// Complete the disassociation of the Result node from the heap

     if (Result->Child != NULL)
	 Result->Child->Parent = NULL;
     Result->Child = Result->Parent = NULL;

// If there was a child list, then we now merge it with the
//	rest of the root list

     if (ChildHeap)
         Union(ChildHeap); //O(1) anyway, besides mandatory for correct memo deallocation

//only difference from ExtractMin() is skipping this costly _Consolidate() which maintains fib-heap property
	 // Consolidate heap to find new minimum and do reorganize work
//     if (MinRoot != NULL) _Consolidate();

// Return the minimum node, which is now disassociated with the heap
// It has Left, Right, Parent, Child, Mark and Degree cleared.

     return Result;
}

//===========================================================================
// DecreaseKey() - O(lg n) actual; O(1) amortized
//
// The O(lg n) actual cost stems from the fact that the depth, and
// therefore the number of ancestor parents, is bounded by O(lg n).
//===========================================================================
int DecreaseKey(FibHeapNode *theNode, float NewKey)
{
FibHeapNode *theParent;

     if (theNode==NULL || //*theNode < NewKey)
							theNode->key < NewKey)
         return NOTOK;

////////     *theNode = NewKey;
	theNode->key = NewKey;

	 theParent = theNode->Parent;
     if (theParent != NULL && //*theNode < *theParent)
								theNode->key < theParent->key)
     {
         _Cut(theNode, theParent);
         _CascadingCut(theParent);
     }

     //if (*theNode < *MinRoot)
	 if (theNode->key < MinRoot->key)
         MinRoot = theNode;

     return OK;
}

//===========================================================================
// Delete() - O(lg n) amortized; ExtractMin() dominates
//
// Notice that if we don't own the heap nodes, then we clear the
// NegInfinityFlag on the deleted node.  Presumably, the programmer
// will be reusing the node.
//===========================================================================
int  Delete(FibHeapNode *theNode)
{
/*FibHeapNode Temp;
int Result;

     if (theNode == NULL) return NOTOK;

     Temp.NegInfinityFlag = 1;
     Result = DecreaseKey(theNode, Temp);

     if (Result == OK)
         if (ExtractMin() == NULL)
             Result = NOTOK;

     if (Result == OK)
     {
         if (GetHeapOwnership())
	      delete theNode;
	 else theNode->NegInfinityFlag = 0;
     }
         
     return Result;*/
}

/*char* getInfo()
	{
		printf("\n\n\nfibheap info:");
		char result[1000];
		char tmp[400] = "null";
		if (MinRoot != NULL)
			sprintf(tmp, "%f, %d, %d", MinRoot->key, MinRoot->element, MinRoot->Degree);
		sprintf(result, "numNodes= %d, numTrees= %d, MinRoot= %s", NumNodes, NumTrees, tmp);

		if(MinRoot != NULL){
			sprintf(result, "%s\n\n", result);
			FibHeapNode* cur = MinRoot;
			do {
				sprintf(tmp, "%f, %d, %d", cur->key, cur->element, cur->Degree);
				sprintf(result, "%s%s--\t", result, tmp);
				cur=cur->Right;
			}while(cur!=MinRoot);
		}

		char* res = new char[1000];
		int c;
		for (c = 0; c < 1000; c++)
			if (result[c] == '\0')
				break;
			else
				res[c] = result[c];
		res[c] = '\0';
		return res;
	}*/

private:
	FibHeapNode *MinRoot;
    long NumNodes, NumTrees, NumMarkedNodes;
	
void _Exchange(FibHeapNode*& N1, FibHeapNode*& N2)
{
FibHeapNode *Temp;

     Temp = N1;
     N1 = N2;
     N2 = Temp;
}

// Consolidate()
//
// Internal function that reorganizes heap as part of an ExtractMin().
// We must find new minimum in heap, which could be anywhere along the
// root list.  The search could be O(n) since all nodes could be on
// the root list.  So, we reorganize the tree into more of a binomial forest
// structure, and then find the new minimum on the consolidated O(lg n) sized
// root list, and in the process set each Parent pointer to NULL, and count
// the number of resulting subtrees.
//
// Note that after a list of n inserts, there will be n nodes on the root
// list, so the first ExtractMin() will be O(n) regardless of whether or
// not we consolidate.  However, the consolidation causes subsequent
// ExtractMin() operations to be O(lg n).  Furthermore, the extra cost of
// the first ExtractMin() is covered by the higher amortized cost of
// Insert(), which is the real governing factor in how costly the first
// ExtractMin() will be.
//===========================================================================
void _Consolidate()
{
FibHeapNode *x, *y, *w;
FibHeapNode *A[1+8*sizeof(long)]; // 1+lg(n)
int  I=0, Dn = 1+8*sizeof(long);
short d;

// Initialize the consolidation detection array

     for (I=0; I < Dn; I++)
          A[I] = NULL;

// We need to loop through all elements on root list.
// When a collision of degree is found, the two trees
// are consolidated in favor of the one with the lesser
// element key value.  We first need to break the circle
// so that we can have a stopping condition (we can't go
// around until we reach the tree we started with
// because all root trees are subject to becoming a
// child during the consolidation).

     MinRoot->Left->Right = NULL;
     MinRoot->Left = NULL;
     w = MinRoot;

     do {
//cout << "Top of Consolidate's loop\n";
//Print(w);

	x = w;
        d = x->Degree;
        w = w->Right;

// We need another loop here because the consolidated result
// may collide with another large tree on the root list.

        while (A[d] != NULL)
        {
             y = A[d];
	     //if (*y < *x)
			 if (y->key < x->key)
				_Exchange(x, y);
             if (w == y) w = y->Right;
			_Link(y, x);
             A[d] = NULL;
             d++;
//cout << "After a round of Linking\n";
//Print(x);
	}
	A[d] = x;

     } while (w != NULL);

// Now we rebuild the root list, find the new minimum,
// set all root list nodes' parent pointers to NULL and
// count the number of subtrees.

     MinRoot = NULL;
     NumTrees = 0;
     for (I = 0; I < Dn; I++)
          if (A[I] != NULL)
              _AddToRootList(A[I]);
}
	
//===========================================================================
// The node y is removed from the root list and becomes a subtree of node x.
//===========================================================================

void _Link(FibHeapNode *y, FibHeapNode *x)
{
// Remove node y from root list

     if (y->Right != NULL)
	 y->Right->Left = y->Left;
     if (y->Left != NULL)
         y->Left->Right = y->Right;
     NumTrees--;

// Make node y a singleton circular list with a parent of x

     y->Left = y->Right = y;
     y->Parent = x;

// If node x has no children, then list y is its new child list

     if (x->Child == NULL)
	 x->Child = y;

// Otherwise, node y must be added to node x's child list

     else
     {
         y->Left = x->Child;
         y->Right = x->Child->Right;
         x->Child->Right = y;
         y->Right->Left = y;
     }

// Increase the degree of node x because it's now a bigger tree

     x->Degree ++;

// Node y has just been made a child, so clear its mark

     if (y->Mark) NumMarkedNodes--;
     y->Mark = 0;
}

//===========================================================================
//===========================================================================
void _AddToRootList(FibHeapNode *x)
{
     if (x->Mark) NumMarkedNodes --;
     x->Mark = 0;

     NumNodes--;
     Insert(x);
}

//===========================================================================
// Remove node x from the child list of its parent node y
//===========================================================================
void _Cut(FibHeapNode *x, FibHeapNode *y)
{
     if (y->Child == x)
         y->Child = x->Right;
     if (y->Child == x)
	 y->Child = NULL;

     y->Degree --;

     x->Left->Right = x->Right;
     x->Right->Left = x->Left;

     _AddToRootList(x);
}

//===========================================================================
// Cuts each node in parent list, putting successive ancestor nodes on the
// root list until we either arrive at the root list or until we find an
// ancestor that is unmarked.  When a mark is set (which only happens during
// a cascading cut), it means that one child subtree has been lost; if a
// second subtree is lost later during another cascading cut, then we move
// the node to the root list so that it can be re-balanced on the next
// consolidate. 
//===========================================================================
void _CascadingCut(FibHeapNode *y)
{
FibHeapNode *z = y->Parent;

     while (z != NULL)
     {
         if (y->Mark == 0)
         {
             y->Mark = 1;
             NumMarkedNodes++;
             z = NULL;
         }
         else
         {
             _Cut(y, z);
             y = z;
	     z = y->Parent;
         }
     }
}

};

#endif
