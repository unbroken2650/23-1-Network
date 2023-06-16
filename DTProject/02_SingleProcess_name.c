#define _CRT_SECURE_NO_WARNINGS
#include <openssl/sha.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    unsigned char input[30] = "";
    size_t input_len = sizeof(input) - 1;
    unsigned char hash[SHA256_DIGEST_LENGTH];
    const int LEVEL = 7;

    FILE *fp = fopen("result_02.txt", "w");

    int cnt = 0;
    bool success = false;
    while (true) {
        sprintf(input, "박채수이시현한수호%d", cnt);
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

    fclose(fp);

    return 0;
}
