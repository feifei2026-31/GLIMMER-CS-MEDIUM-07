#include <stdio.h>
#include <string.h>

/* 取指数字段：第 2~5 位（下标 1~4） */
static int get_exp(const char *s)
{
    return ((s[1] - '0') << 3) | ((s[2] - '0') << 2) |
           ((s[3] - '0') << 1) | (s[4] - '0');
}

/* 取尾数字段：第 6~8 位（下标 5~7），只有小数部分 */
static int get_man(const char *s)
{
    return ((s[5] - '0') << 2) | ((s[6] - '0') << 1) | (s[7] - '0');
}

int main(void)
{
    char a[32], b[32];

    /* 一次读两个 8 位字符串，支持多行输入直到文件结束 */
    while (scanf("%31s %31s", a, b) == 2) {
        if (strlen(a) != 8 || strlen(b) != 8)
            continue;

        int ea = get_exp(a), eb = get_exp(b);
        int ma = get_man(a) + 8;          /* +8：补上规格化隐含的整数位 1 */
        int mb = get_man(b) + 8;

        /* ---- 第一步：对阶 ---- */
        /* 小阶向大阶看齐。把小阶那个尾数左移，是为了不丢失有效位；
           右移会丢掉低位，左移不会。 */
        int e = (ea < eb) ? ea : eb;
        int total = (ma << (ea - e)) + (mb << (eb - e));

        /* ---- 第二步：尾数运算 ---- */
        /* 就是上面那个 +，已经完成。total 是 (1.xxx + 1.xxx) 放大后的整数，
           范围在 [16, 32)。 */

        /* ---- 第三步：规格化 ---- */
        /* 结果 >= 16 说明整数部分进位了（1x.xxx），右移一位、阶码加一。
           右移时丢弃的最低位就是「向零舍入」，因为都是非负数。 */
        while (total >= 16) {
            total >>= 1;
            e++;
        }
        while (total < 8) {               /* 双保险，本题的规格化输入用不到 */
            total <<= 1;
            e--;
        }

        /* ---- 拼出 8 位：符号 0 + 4 位阶码 + 3 位尾数 ---- */
        char out[9];
        out[0] = '0';
        for (int i = 0; i < 4; i++)
            out[1 + i] = (char)('0' + ((e >> (3 - i)) & 1));
        for (int i = 0; i < 3; i++)
            out[5 + i] = (char)('0' + ((total >> (2 - i)) & 1));
        out[8] = '\0';

        printf("%s\n", out);
    }
    return 0;
}
