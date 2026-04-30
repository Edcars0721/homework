#include <stdio.h>
int main() {
    char input[100];  // 用来存你输入的内容
    printf("Hello, World!\n");
    printf("请输入内容：");
    scanf("%s", input);  // 读取键盘输入
    printf("你输入的是：%s\n", input);  // 把输入的内容显示出来
    return 0;
}