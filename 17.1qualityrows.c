/* 
 * LOGICA DI OTTIMIZZAZIONE: GLOBAL BINARY SEARCH + 2D PREFIX SUMS
 * --------------------------------------------------------------
 * Invece di calcolare la mediana per ogni rettangolo (locale), cerchiamo 
 * direttamente il valore della mediana minima possibile (globale).
 *
 * 1. BINARY SEARCH SULLA RISPOSTA:
 *    Il valore della mediana "k" è monotono: se esiste un rettangolo con 
 *    mediana <= k, allora ne esiste uno anche per ogni valore > k. 
 *    Questo permette di cercare k in O(log(R*C)).
 *
 * 2. TRASFORMAZIONE -1 / +1:
 *    Data una soglia 'quality', trasformiamo la matrice:
 *    - Se q[i][j] <= quality  =>  -1
 *    - Se q[i][j] >  quality  =>  +1
 *    Proprietà: Un rettangolo ha mediana <= quality se e solo se la 
 *    somma dei suoi elementi trasformati è <= 0, <= because =0 is literally that median <0 still feasable
 *
 * 3. SOMME PREFISSE 2D:
 *    Per verificare se esiste un rettangolo con somma <= 0 in O(1), 
 *    usiamo una matrice di somme prefisse. Questo rende il controllo 
 *    indipendente dalle dimensioni H e W del rettangolo.
 *
 * COMPLESSITÀ TOTALE: O(R * C * log(R * C))*/
#define MAX_ROWS 3001
#define MAX_COLS 3001

typedef int board[MAX_ROWS][MAX_COLS];

// Macro-idea:
// per ogni quality provo a vedere se esiste almeno un rettangolo buono
// poi con la binary search globale cerco il primo quality valido

#define MAX_ROWS 3001
#define MAX_COLS 3001

typedef int board[MAX_ROWS][MAX_COLS];

int can_make_quality(int quality, int r, int c, int h, int w, board q) {
  static int zero_one[MAX_ROWS][MAX_COLS];
  static int sum[MAX_ROWS + 1][MAX_COLS + 1];
  int i, j;
  int top_row, left_col, bottom_row, right_col;
  int total;

  // Fase 1: costruisco la matrice -1/+1
  for (i = 0; i < r; i++)
    for (j = 0; j < c; j++)
      if (q[i][j] <= quality)
        zero_one[i][j] = -1;
      else
        zero_one[i][j] = 1;

  // Fase 2: prefissi 2D per sommare un rettangolo in O(1)
  for (i = 0; i <= c; i++)
    sum[0][i] = 0;
  for (i = 0; i <= r; i++)
    sum[i][0] = 0;
  for (i = 1; i <= r; i++)
    for (j = 1; j <= c; j++)
      sum[i][j] = zero_one[i - 1][j - 1] + sum[i - 1][j] +
                  sum[i][j - 1] - sum[i - 1][j - 1];

  // Fase 3: provo tutti i rettangoli h*w e controllo la somma
  for (top_row = 1; top_row <= r - h + 1; top_row++)
    for (left_col = 1; left_col <= c - w + 1; left_col++) {
      bottom_row = top_row + h - 1;
      right_col = left_col + w - 1;
      total = sum[bottom_row][right_col] - sum[top_row - 1][right_col] -
              sum[bottom_row][left_col - 1] + sum[top_row - 1][left_col - 1];
      if (total <= 0)
        return 1;
    }
  return 0;
}

// Binary search per trovare il minimo quality
// cerco il primo k per cui esiste almeno un rettangolo con mediana <= k
int rectangle(int r, int c, int h, int w, board q) {
  int low, high, mid;
  low = 0;
  high = r * c + 1;
  while (high - low > 1) {
    mid = (low + high) / 2;
    if (can_make_quality(mid, r, c, h, w, q))
      high = mid;  // esiste rettangolo, prova piu' basso
    else
      low = mid;   // non esiste, vai piu' alto
  }
  return high;
}

