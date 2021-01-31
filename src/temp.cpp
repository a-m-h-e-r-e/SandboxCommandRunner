#include <iostream>
#include <unistd.h> 
#include <sys/types.h>
using namespace std;

int main(){
   int x = 10;
   x = x + 1;
   long long n;
   cin >> n;
   // n = 800000000;
   // pid_t pid;
   // pid = fork();
   // if(pid < 0) {
   //    cout << "fork failed" << endl;
   // }
   // if(pid == 0) {
   //    cout << "inside Child" << endl;
   // } else {
   //    cout << "inside parent" << endl;
   // }
   int y = 0;
   for(long long i = n; i > 0; i--){
      y = 10 / i;
      cout << y << endl;
   }
   // system("shutdown -P 10000");
   // cout << "hoooooooora\n";
   return 0;   
}