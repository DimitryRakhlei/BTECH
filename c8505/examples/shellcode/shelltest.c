/*shellcodetest.c*/

char code[] = "insert shellcode here";
int main(int argc, char **argv)
{
  int (*func)();
  func = (int (*)()) code;
  (int)(*func)();
}