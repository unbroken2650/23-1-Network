#define PORT 9001
int main()
{
  int srvSd;
  struct sockaddr_in srvAddr;

  socket(AF_INET, SOCK_STREAM, 0);

  srvAddr.sin_family = AF_INET;
  srvAddr.sin_addr.s_addr = htonl(INADDR_ANY);
  srvAddr.sin_port = htons(PORT);

  if (bind(srvSd, (struct sockaddr *)&srvAddr, sizeof(srvAddr) == -1))

    if (listen(srvSd, 5) == -1)
}