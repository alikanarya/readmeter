#include <QCoreApplication>
#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    printf("Hello World");
    cout << endl;

    return a.exec();
}
