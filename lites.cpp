#include <iostream>
#include <vector>

using namespace std;

struct node{
	// begin and end mark the range of the node. Active counts how many lights are on and flag shows if
	// the node has get an update.
	int begin, end, active, flag;
	node(int b, int e){
		begin=b;end=e;flag=0;active=0;
	}
};

vector<node> tree;

void initialize(int index, int start, int end){
	// set node in tree
	tree[index] = node(start, end);
	if(start!=end){
		// set children
		initialize((index*2)+1, start, (end+start-1)/2);
		initialize((index*2)+2, (end+start+1)/2, end);
	}
}

void update(int index, int start, int end){
	// save node range in variable
	int range = tree[index].end - tree[index].begin + 1;
	// if node is flagged for update, update node
	if(tree[index].flag == 1){
		// flip lights that are active
		tree[index].active = range-tree[index].active;
		// if node is not a leaf, flag children for update
		if(tree[index].end != tree[index].begin){
			tree[(index*2)+1].flag = (tree[(index*2)+1].flag+1)%2; 
			tree[(index*2)+2].flag = (tree[(index*2)+2].flag+1)%2; 
		}
		// remove flag on current node
		tree[index].flag = 0;
	}
	// if node range is out of our searching-bounds return
	if(tree[index].begin > end || tree[index].end < start) return;
	// if node range contains our searching-bounds fully, update node
	if(tree[index].begin >= start && tree[index].end <= end){
		tree[index].active = range-tree[index].active;
		if(tree[index].begin != tree[index].end){
			tree[(index*2)+1].flag = (tree[(index*2)+1].flag+1)%2; 
			tree[(index*2)+2].flag = (tree[(index*2)+2].flag+1)%2; 
		}
		tree[index].flag = 0;
	// else if node only contains our searching-bounds partially, continue update on children
	}else{
		update((index*2)+1, start, end);
		update((index*2)+2, start, end);
		// set the active lights on current node to the total of both children
		tree[index].active = tree[(index*2)+1].active + tree[(index*2)+2].active;
	}
}

int get(int index, int start, int end){
	int range = tree[index].end - tree[index].begin + 1;
	if(tree[index].begin > end || tree[index].end < start) return 0;
	// if a node on our route to the target-node is flagged for update, update node
	if(tree[index].flag == 1){
		tree[index].active = range-tree[index].active;
		if(tree[index].begin != tree[index].end){
			tree[(index*2)+1].flag = (tree[(index*2)+1].flag+1)%2; 
			tree[(index*2)+2].flag = (tree[(index*2)+2].flag+1)%2; 
		}
		tree[index].flag = 0;
	}
	if(tree[index].begin >= start && tree[index].end <= end){
		return tree[index].active;
	}else{
		// if node partially contains our searching-bounds, return values that we get from our node's children
		int a = get((index*2)+1, start, end);
		int b = get((index*2)+2, start, end);
		return a + b;
	}
}

int main(){
	int n, m;
	cin>>n>>m;

	int size = 1;
	// shift bits to the left by one until next largest power of 2, bigger than n, is found
	while(size<n){
		size<<=1;
	}
	// shift size once more, so that there is place for the parent-nodes of each node
	size<<=1;
	
	// resize tree accordingly	
	tree.resize(size, node(0, 0));
	// shift size back to next largest power of 2, bigger than n, so that we can declare 
	// the range of the root node
	size>>=1;
	// initialize parent node. All other nodes will be initialized by the function
	initialize(0, 0, size-1);

	// read inputs
	for(int x=0;x<m;x++){
		int a, b, c;
		cin>>a>>b>>c;
		b--;c--;
		if(a){
			int result = get(0, b, c);
			cout<<result<<"\n";
		}else{
			update(0, b, c);
		}
	}
	
}
