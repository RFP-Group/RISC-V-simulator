#define N 20

int main()
{
    int a = 0, b = 1;
    for (int i = 0; i < N; ++i) {
        int tmp = b;
        b += a;
        a = tmp;
    }
    return b;
}