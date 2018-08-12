#include <thread>
#include <vector>
#include <unordered_map>
#include <stdint.h>
#include <iostream>
#include "tsl/hopscotch_map.h"
#include "tsl/hopscotch_hash.h"

long*  makeAndFill(int row, int col)
{
  char *storage = new (std::nothrow) char[ row * col * sizeof(long)]; 
  long *data = (long*) storage;
  for(int i = 0; i < row*col; ++i )
  {
    data[i] = (long) (rand() % 100000000);
    //data[i] = (long) (rand() % 1000);
    //std::cout << data[i] << ",";
  };  
  //std::cout << std::endl;
  //return storage;
  return data;
};

void thread_func(long* data, tsl::hopscotch_map<long, bool>& lookup, int col, int row)
{
	for(int i=0; i < row * col; ++i)
	{
		lookup.insert({data[i], true});
	};
	return;
};

#define TNUM 8
#define ROW 100000000
//#define ROW 10
int main()
{
  tsl::hopscotch_map<long, bool> lookup(120000000);;
  //tsl::hopscotch_map<long, bool> lookup(1000);
	std::vector<long*> data;
	int col = 1;
	int measure_col = 0;
	for(int i = 0; i < TNUM; ++i)
	{
		long* t1 = makeAndFill(ROW, col+measure_col);
		data.push_back(t1);
	}
	std::thread threads[TNUM];
	long t1 = time(NULL);
	for(int i=0; i< TNUM; ++i)
	{
		threads[i] = std::thread(&thread_func, data[i], std::ref(lookup), col, ROW);
	};  
	for(int i=0; i< TNUM; ++i)
	{
		threads[i].join();
	};  
	long t2 = time(NULL);
	std::cout << "elapse = " << (t2-t1) << std::endl;
	//std::cout << TNUM << "," << ROW << std::endl;
	std::cout << "lookup size=" << lookup.size() << std::endl;
	/*for(auto e = lookup.begin(); e != lookup.end(); ++e)
	{
		std::cout << e->first << ":" << e->second << ",";
	}
	std::cout << std::endl;*/
}
