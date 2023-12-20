int cnt;

void nqueens(char *a, int n, int pos)
{
    /* b[i] = j means the queen in i-th row is in column j.  */
    char b[pos + 1];
    int i, j;

    for (int k = 0; k < pos; k++) {
        b[k] = a[k];
    }
    for (i = 0; i < n; i++) {
        for (j = 0; j < pos; j++) {
            if (b[j] == i || b[j] == i + pos - j || i == b[j] + pos - j) {
                break;
            }
        }

        if (j < pos) {
            continue;
        }
        if (pos == n - 1) {
            /* Found a solution.  Could output it here.  */
            ++cnt;
        } else {
            b[pos] = i;
            nqueens(b, n, pos + 1);
        }
    }
}

int main()
{
    int n = 8;
    if (n < 1 || n > 127) {
        return -1;
    }

    cnt = 0;
    nqueens("", n, 0);
    return cnt;
}