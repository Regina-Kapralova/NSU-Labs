#include <ctype.h>
#include <unistd.h>

int main() {
    char buffer[100];
    int n;
    while ((n = read(0, buffer, 100)) > 0) {
        for (int i = 0; i < n; i++)
            if (islower(buffer[i]))
                buffer[i] = toupper(buffer[i]);
        write(0, buffer, n);
    }
    return 0;
}
