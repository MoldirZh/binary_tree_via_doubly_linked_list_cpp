
#include "set.h"


void print( std::ostream& out, const treenode* n, size_t indent )
{

   for( size_t i = 0; i != indent; ++ i )
      out << "|  ";
   if(n)
   {
      out << ( n -> val ) << "\n";
      print( out, n -> left, indent + 1 );
      print( out, n -> right, indent + 1 );
   }
   else
      out << "#\n";
}


// both the upperbound and the lowerbound are strict,
// we use pointers, so that they can be absent.

void checkbetween( const treenode* n,
                   const int* lowerbound, const int* upperbound )
{
   while(n)
   {
      if( lowerbound && *lowerbound >= n -> val )
      {
         std::cout << "value " << ( n -> val );
         std::cout << " is not above lower bound " << *lowerbound << "\n";
         std::abort( );
      }

      if( upperbound && n -> val >= *upperbound )
      {
         std::cout << "value " << ( n -> val );
         std::cout << "is not below upperbound " << *upperbound << "\n";
         std::abort( );
      }

      checkbetween( n -> left, lowerbound, &( n -> val ));
      lowerbound = & ( n -> val );
      n = n -> right;
   }
}


void deallocate( treenode* n )
{
   while(n)
   {
      deallocate( n -> left );
      treenode* bye = n;
      n = n -> right;
      delete bye;
   }
}


void writecopy( treenode** to, treenode* from )
{
   while( from )
   {
      *to = new treenode{ from -> val };

      writecopy( & (*to) -> left, from -> left );
      to = & (*to) -> right;
      from = from -> right;
   }
   *to = nullptr;
}


void set::checksorted( ) const
{
   ::checkbetween( tr, nullptr, nullptr );
}


std::ostream& set::print( size_t indent, std::ostream& out ) const
{
   ::print( out, tr, indent );
   return out;
}

size_t log_base2( size_t s )
{
    size_t count = 0;
    if (s == 0)
        return 0;
    else {
        while (s >= 2) {
            s = s / 2;
            count++;
        }
        return count;
    }
}

const treenode* find( const treenode* n, int i )
{
    if (n == nullptr) // empty tree or i is not found
        return n;
    else if (n->val == i) // i is the root
        return n;
    else if (n->val > i) // i is on the left
        return find(n->left, i);
    else // i is on the right
        return find(n->right, i);
    return nullptr;
}

treenode** find( treenode** n, int i )
{
    if (*n == nullptr) // empty tree or i is not found
        return n;
    else if ((*n)->val == i) // i is found
        return n;
    else if ((*n)->val > i) // i is on the left
        return find(&(*n)->left, i);
    else if ((*n)->val < i) // i is on the right
        return find(&(*n)->right, i);
    return n;
}

bool set::insert( int i )
{
    if (tr == nullptr) { // if tree is empty
        tr = new treenode(i);
        return true;
    }
    treenode **a = find(&tr, i);
    if (*a == nullptr) { // if i is not in the tree
        *a = new treenode(i);
        return true;
    }
    else // if i is already in the tree
        return false;
}

bool set::contains( int i ) const
{
    treenode const *a = find( tr, i );
    if (a != nullptr) // if i is found
        return true;
    else // if i is not found
        return false;
}

treenode* removerightmost( treenode** from )
{
    if ((*from)->right == nullptr) {
        return *from; // return rightmost value
    } else {
        return removerightmost(&(*from)->right);
    }
}

bool set::remove( int i )
{
    if (tr == nullptr) // if tree is empty
        return false;

    treenode **a = find(&tr, i);

    if (*a == nullptr) // i not found
        return false;
    else {
        // no children
        if ((*a)->left == nullptr && (*a)->right == nullptr) {
            delete *a;
            *a = nullptr;
        }
        // only one child
        else if ((*a)->left != nullptr && (*a)->right == nullptr) { // node has a left child
            treenode *temp = *a;
            *a = (*a)->left;
            temp->left = nullptr;
            temp->right = nullptr;
            delete temp;
            temp = nullptr;
        }
        else if ((*a)->right != nullptr && (*a)->left == nullptr) { // node has a right child
            treenode *temp = *a;
            *a = (*a)->right;
            temp->left = nullptr;
            temp->right = nullptr;
            delete temp;
            temp = nullptr;
        }
        // two children
        else {
            treenode *leftsubtree = (*a)->left; // move to left subtree
            treenode *leftmax = removerightmost(&leftsubtree); // get rightmost value of left subtree
            int temp = leftmax->val; // temporary var that will store the rightmost value
            remove(temp); // remove the duplicate of the rightmost value
            (*a)->val = temp; // copy temp into the prev. root's place
        }
        return true;
    }
}

size_t size( const treenode* n )
{
    // Returns the total size of tree.
    if (n == nullptr) {
        return 0;
    } else {
        int count = 1;
        if (n->left != nullptr) { // for all left nodes
            count += size (n->left);
        }
        if (n->right != nullptr) { // for all right nodes
            count += size (n->right);
        }
        return count;
    }
}

void set::clear()
{
    deallocate( tr );
    tr = nullptr;
}

bool set::isempty()
{
    if (tr == nullptr)
        return true;
    else
        return false;
}

size_t height( const treenode* n )
{
    // Returns the height of the tree starting at n.
    if (n == nullptr) return 0;
    else {
        size_t leftnode = height(n->left);
        size_t rightnode = height(n->right);

        if (leftnode >= rightnode)
            return 1 + leftnode;
        else
            return 1 + rightnode;
    }
}
