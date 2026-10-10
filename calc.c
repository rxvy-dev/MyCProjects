#include <stdio.h>
#include <string.h>

void art(void) {
  puts("  ____    _    _     ____");
  puts(" / ___|  / \  | |   / ___|");
  puts("| |     / _ \ | |  | |   ");
  puts("| |___ / ___ \| |__| |___");
  puts("  \____/_/   \_\_____\____|");
}
int main() {
    char input[128];
    art();
    printf("would u like to x or +? (say add, times, divide): ");
    fgets(input, sizeof(input), stdin);
    if (strstr(input, "add")) {
        int first;
        first = printf("first number:");
        scanf("%d", &first);
        int second;
        second = printf("second number:");
        scanf("%d", &second);
        int result;
        result = first + second;
        printf("Answer: %d\n", result);
    }
    else if (strstr(input, "times")) {
        int one;
        one = printf("First number:");
        scanf("%d", &one);
        int two;
        two = printf("second number:");
        scanf("%d", &two);
        int answer;
        answer = one * two;
        printf("Answer: %d\n", answer);
    }
    else if (strstr(input, "divide")) {
        int divideone;
        divideone = printf("First number:");
        scanf("%d", &divideone);
        int dividetwo;
        dividetwo = printf("Number 2:");
        scanf("%d", &dividetwo);
        int divideanswer;
        divideanswer = divideone / dividetwo;
        printf("Answer: %d\n", divideanswer);
    }
}
