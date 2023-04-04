#include <stdio.h>
#include <openssl/sha.h>

int main()
{
  unsigned char input[] = "Hello, world!";
  size_t input_len = sizeof(input) - 1;
  unsigned char hash[SHA256_DIGEST_LENGTH];
  SHA256(input, input_len, hash);

  printf("SHA256 Hash: ");
  for (int i = 0; i < SHA256_DIGEST_LENGTH; i++)
  {
    printf("%02x", hash[i]);
  }
  printf("\n");

  return 0;
}