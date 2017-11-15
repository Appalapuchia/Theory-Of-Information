#include<fstream>
#include<iostream>
using namespace std;
 
int main()
{
//	int a=0,b=0,c=0,d=0,;
	int i, size;
	char* ch;
	int c[256] = {0};

    ifstream in("a.txt",ios::binary);
    in.seekg(0,ios::end);size=in.tellg();in.seekg(0,ios::beg);
    cout << "Size = " << size << endl;
    ch = new char [size+1];
    ch [size] = 0;
    in.read (ch,size);
    
    for (int i=0;i<size;i++)
c[int(ch[i])]++;
cout<<"Symbol\tNumber\tQuantity"<<endl;
for (i=0;i<256;i++)
if (c[i]) cout<<char(i)<< "\t" << c[i] << "\t" << c[i]/float(size)<<endl;
 
    in.close();
    delete [] ch; 
    return 0;
}
