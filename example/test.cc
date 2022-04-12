#include "cuckoofilter.h"

#include <assert.h>
#include <math.h>

#include <iostream>
#include <vector>

using cuckoofilter::CuckooFilter;
using namespace cuckoofilter;

int main(int argc, char **argv) {
  size_t total_items = 256*4;



  // Create a cuckoo filter where each item is of type size_t and
  // use 12 bits for each item:
  //    CuckooFilter<size_t, 12> filter(total_items);
  // To enable semi-sorting, define the storage of cuckoo filter to be
  // PackedTable, accepting keys of size_t type and making 13 bits
  // for each key:
  //   CuckooFilter<size_t, 13, cuckoofilter::PackedTable> filter(total_items);
  CuckooFilter<size_t, 8> filter(total_items);
  //filter.Add(123);
  //assert(filter.Add(1)==cuckoofilter::Ok);
  //assert(filter.Contain(1)==cuckoofilter::Ok);
  //filter.Add(9576);
  

  
  // Insert items to this cuckoo filter
  size_t num_inserted = 0;
  for (size_t i = 0; i < total_items*0.5; i++, num_inserted++) {
    //filter.Add(i);
    if (filter.Add(i) != cuckoofilter::Ok) {
      std::cout<<"Problem at: "<<i<<std::endl;
     // break;
    }
  }
  std::cout<<"Number of inserted elements is: "<<num_inserted<<std::endl;
  // Check if previously inserted items are in the filter, expected
  // true for all items
  for (size_t i = 0; i < total_items*0.5; i++) {
    assert(filter.Contain(i) == cuckoofilter::Ok);
  }

  // Check non-existing items, a few false positives expected
  size_t total_queries = 0;
  size_t false_queries = 0;
  //for (size_t i = total_items; i < 2 * total_items; i++) {
    for (size_t i =total_items; i < 2*total_items; i++) {
    if (filter.Contain(i) == cuckoofilter::Ok) {
      false_queries++;
    }
    total_queries++;
  }

  // Output the measured false positive rate
  std::cout << "false positive rate is "
            << 100.0 * false_queries / total_queries << "%\n";
/*
  filter.Add(13002);
  filter.Add(559833);
  filter.Add(933071);
  filter.Add(831160);
  filter.Add(123412);
  filter.Add(682432);
  filter.Add(663905);
  //assert(filter.Add(1500)!=cuckoofilter::NotEnoughSpace);
  filter.Add(19340);
  //filter.Add(341872);
  assert(filter.Add(720750)==cuckoofilter::Ok);
  //filter.Add(1500);
  //assert(filter.Add(1500)!=cuckoofilter::NotSupported);
  assert(filter.Add(99999)==cuckoofilter::Ok);
  
  filter.Add(1500);
  filter.Add(1500);
  filter.Add(1500);
  
  filter.Delete(1500);
  filter.Delete(1500);
  filter.Delete(1500);
  filter.Delete(1500);
  filter.Delete(1500);
  filter.Delete(1500);
  filter.Delete(1500);
  filter.Delete(1500);
  filter.Delete(1500);
  */
  //assert(filter.Delete(1500)==cuckoofilter::NotFound);
  //assert(filter.Add(1500)!=cuckoofilter::NotEnoughSpace);
  //assert(filter.Contain(1500) == cuckoofilter::Ok);

  return 0;
}
