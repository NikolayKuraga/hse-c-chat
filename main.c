#include <stdio.h>
#include "server.h"
#include "client.h"
#include <iostream> //�� �������, ��� ���������� ����� ��� strcmp
using namespace std;

int main(int argc, char** argv)
{
    if (strcmp(argv[1], "server") == 0)
    {
        printf("SERVER\n");
    }
    else {
        printf("CLIENT\n");
    }

    return 0;
}
