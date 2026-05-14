#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void set_a_switch(int door, int switch_positions[],
                  int door_for_switch[], int n) {
  int i, result;
  int low = 0, high = n-1, mid;

  // Metto a 0 tutti gli switch che non ho ancora associato a una porta.
  for (i = 0; i < n; i++)
    if (door_for_switch[i] == -1)
      switch_positions[i] = 0;

  // se la porta interessata si apre nice
  result = tryCombination(switch_positions);
  //se non si apre metto tutti a 1 (cosi si apre)
  if (result != door) {
    for (i = 0; i < n; i++)
      if (door_for_switch[i] == -1)
        switch_positions[i] = 1;
  }

  //è di sicuro aperta-->xor half until i have the one
  while (low != high) {
    mid = (low + high) / 2;
    for (i = low; i <= mid; i++)
      if (door_for_switch[i] == -1)
        switch_positions[i] ^= 1;

    // Se la porta resta nello stesso stato, il candidato giusto sta a sinistra;
    // altrimenti sta a destra.
    result = tryCombination(switch_positions);
    if (result != door) {
      high = mid;
      for (i = low; i <= mid; i++)
        if (door_for_switch[i] == -1)
          switch_positions[i] ^= 1;
    }
    else
      low = mid + 1;
  }

  // Ho trovato lo switch che controlla 'door'.
  door_for_switch[low] = door;

  // rimetto apposto
  switch_positions[low] ^= 1;
}

void exploreCave(int n) {
  int switch_positions[n], door_for_switch[n];
  int i;

  // All'inizio nessuno switch e' assegnato a una porta.
  for (i = 0; i < n; i++)
    door_for_switch[i] = -1;

  // Risolvo una porta alla volta.
  for (i = 0; i < n; i++)
    set_a_switch(i, switch_positions, door_for_switch, n);

  // Consegno al judge la configurazione finale e la mappa switch->porta.
  answer(switch_positions, door_for_switch);
}

int main(){ //placeholder
  return 0;
}