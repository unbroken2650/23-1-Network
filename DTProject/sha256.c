#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <openssl/sha.h>

int main()
{
  unsigned char input[30] = "";
  size_t input_len = sizeof(input) - 1;
  unsigned char hash[SHA256_DIGEST_LENGTH];
  const int LEVEL = 7;

  FILE *fp = fopen("result.txt", "w");

  int cnt = 0;
  bool success = false;
  for (cnt = 0; cnt < 10000000000; cnt++)
  {
    sprintf(input, "201926502019260220192651%d", cnt);
    input_len = strlen(input);

    SHA256(input, input_len, hash);
    printf("%d\t", cnt);

    if (hash[0] == 0 && hash[1] == 0 && hash[2] == 0)
    {
      printf("\nSHA256 Hash: ");
      fputs("SHA256 Hash: ", fp);
      for (int i = 0; i < SHA256_DIGEST_LENGTH; i++)
      {
        printf("%02x", hash[i]);
      }
      printf("\n");
      fprintf(fp, "%d\n", cnt);
      success = true;
      break;
    }
  }

  fclose(fp);

  return 0;
}
