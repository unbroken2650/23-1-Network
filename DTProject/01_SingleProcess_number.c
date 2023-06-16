#define _CRT_SECURE_NO_WARNINGS
#include <openssl/sha.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {
    // 서버 시작
    if (argc != 3) {
        printf("Usage: %s [port] [level] \n", argv[0]);
        exit(1);
    }

    unsigned char input[30] = "";
    size_t input_len = sizeof(input) - 1;
    unsigned char hash[SHA256_DIGEST_LENGTH];
    const int LEVEL = atoi(argv[2]);

    FILE *fp = fopen("result_01.txt", "w");

    int cnt = 0;
    bool success = false;

    if (LEVEL == 7) {
        while (true) {
            sprintf(input, "201926502021170320212013%d", cnt);
            input_len = strlen(input);

            SHA256(input, input_len, hash);
            printf("%d\t", cnt);

            if (hash[0] == 0 && hash[1] == 0 && hash[2] == 0 && hash[3] < 0x10) {
                printf("\nSHA256 Hash: ");
                fputs("SHA256 Hash: ", fp);
                for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
                    printf("%02x", hash[i]);
                }
                printf("\n");
                fprintf(fp, "%d\n", cnt);
                success = true;
                break;
            }
            cnt++;
        }
    } else if (LEVEL == 8) {
        while (true) {
            sprintf(input, "201926502021170320212013%d", cnt);
            input_len = strlen(input);

            SHA256(input, input_len, hash);
            printf("%d\t", cnt);

            if (hash[0] == 0 && hash[1] == 0 && hash[2] == 0 && hash[3] == 0) {
                printf("\nSHA256 Hash: ");
                fputs("SHA256 Hash: ", fp);
                for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
                    printf("%02x", hash[i]);
                }
                printf("\n");
                fprintf(fp, "%d\n", cnt);
                success = true;
                break;
            }
            cnt++;
        }
    } else {
        printf("Level should be 7 or 8\n");
        exit(1);
    }

    fclose(fp);

    return 0;
}
