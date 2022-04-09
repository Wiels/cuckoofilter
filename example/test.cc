#include "cuckoofilter.h"

#include <assert.h>
#include <math.h>

#include <iostream>
#include <vector>

using cuckoofilter::CuckooFilter;
using namespace cuckoofilter;

int main(int argc, char **argv) {
  size_t total_items = 2048;

  // Create a cuckoo filter where each item is of type size_t and
  // use 12 bits for each item:
  //    CuckooFilter<size_t, 12> filter(total_items);
  // To enable semi-sorting, define the storage of cuckoo filter to be
  // PackedTable, accepting keys of size_t type and making 13 bits
  // for each key:
  //   CuckooFilter<size_t, 13, cuckoofilter::PackedTable> filter(total_items);
  CuckooFilter<size_t, 8> filter(total_items);

  // Insert items to this cuckoo filter
  size_t num_inserted = 0;
  for (size_t i = 0; i < 1000; i++, num_inserted++) {
    filter.Add(i);
    //if (filter.Add(i) != cuckoofilter::Ok) {
     // break;
    //}
  }

  // Check if previously inserted items are in the filter, expected
  // true for all items
  for (size_t i = 0; i < 1000; i++) {
    assert(filter.Contain(i) == cuckoofilter::Ok);
  }

  // Check non-existing items, a few false positives expected
  size_t total_queries = 0;
  size_t false_queries = 0;
  //for (size_t i = total_items; i < 2 * total_items; i++) {
    for (size_t i =2048; i < 2*2048; i++) {
    if (filter.Contain(i) == cuckoofilter::Ok) {
      false_queries++;
    }
    total_queries++;
  }

  // Output the measured false positive rate
  std::cout << "false positive rate is "
            << 100.0 * false_queries / total_queries << "%\n";

  filter.Add(388877);
  filter.Add(252150);
  filter.Add(91233);
  filter.Add(729238);
  filter.Add(818960);
  filter.Add(431594);
  filter.Add(611038);
  //assert(filter.Add(1500)!=cuckoofilter::NotEnoughSpace);
  filter.Add(549794);
  //filter.Add(341872);
  assert(filter.Add(639516)!=cuckoofilter::NotEnoughSpace);
  //filter.Add(1500);
  //assert(filter.Add(1500)!=cuckoofilter::NotSupported);
  /*filter.Add(1500);
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
