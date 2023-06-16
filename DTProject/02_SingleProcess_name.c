#define _CRT_SECURE_NO_WARNINGS
#include <openssl/sha.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

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
    clock_t search_start = clock();
    int clock_cnt = 1;

    if (LEVEL == 7) {
        while (true) {
            sprintf(input, "박채수이시현한수호%d", cnt);
            input_len = strlen(input);

            SHA256(input, input_len, hash);

            if (hash[0] == 0 && hash[1] == 0 && hash[2] == 0) {
                printf("\nSHA256 Hash: ");
                fputs("Nonce: ", fp);
                for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
                    printf("%02x", hash[i]);
                }
                printf("\n");
                fprintf(fp, "%d\n", cnt);
                fputs("SHA256 Hash: ", fp);
                for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
                    fprintf(fp, "%02x", hash[i]);
                }
                fputs("\n", fp);
                success = true;
                break;
            }
            cnt++;
            clock_t search_mid = clock();
            if (((double)(search_mid - search_start) / CLOCKS_PER_SEC) > clock_cnt) {
                printf("%ds \n", clock_cnt);
                clock_cnt++;
            }
        }
    } else if (LEVEL == 8) {
        while (true) {
            sprintf(input, "박채수이시현한수호%d", cnt);
            input_len = strlen(input);

            SHA256(input, input_len, hash);

            if (hash[0] == 0 && hash[1] == 0 && hash[2] == 0 && hash[3] == 0) {
                printf("\nSHA256 Hash: ");
                fputs("Nonce: ", fp);
                for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
                    printf("%02x", hash[i]);
                }
                printf("\n");
                fprintf(fp, "%d\n", cnt);
                fputs("SHA256 Hash: ", fp);
                for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
                    fprintf(fp, "%02x", hash[i]);
                }
                fputs("\n", fp);
                success = true;
                break;
            }
            cnt++;
            clock_t search_mid = clock();
            if (((double)(search_mid - search_start) / CLOCKS_PER_SEC) > clock_cnt) {
                printf("%ds \n", clock_cnt);
                clock_cnt++;
            }
        }
    } else {
        printf("Level should be 7 or 8\n");
        exit(1);
    }

    clock_t search_end = clock();
    fprintf(fp, "Elapsed Time : %lfs\n", (double)(search_end - search_start) / CLOCKS_PER_SEC);
    fclose(fp);

    return 0;
}
