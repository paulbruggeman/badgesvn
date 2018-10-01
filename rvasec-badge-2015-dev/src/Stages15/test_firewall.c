#include <stdio.h>

#define FWBOARDEXTERN extern
#include "firewall_board.h"

void print_grid(const unsigned char some_grid[]) {
  int i, j;
  for (i = 0; i < 8; i++) {
    for (j = 0; j < 8; j++) {
      putchar(get_pos(some_grid, i, j) ? '+' : '-');
    }
    putchar('\n');
  }
  putchar('\n');
}

int main(const int argc, const char argv[]) {
  int i;
  srand(time(NULL));
  
  for (i=0; i<8; i++) {
    set_pos(offensive_tries, i, i);
    set_pos(offensive_tries, 7 - i, i);
  }

  print_grid(offensive_tries);
  clear_grid(offensive_tries);
  set_ship(ROUTER);
  print_grid(placed_items);
  set_ship(ROUTER);
  print_grid(placed_items);
  for (i=0; i < 1000; i++) {
      clear_grid(placed_items);
      while (set_ship(ROUTER));
      }
  print_grid(placed_items);

  
}
