#include <fstream>
#include <iostream>
using namespace std;
void ef(char*& n) { int j=0,k=-1; for(;n[++k];); for(;--k+1;) if(n[k]=='\\') break; for(;n[++k];n[j++]=n[k]); n[j]=0;}
struct haff
{ unsigned s; // количество
int i; // индекс
int p; // ссылка
char c; // буква
char k; // цифровой код
int j; // направляющий индекс
void print() {cout<<"c="<<c<<" i="<<i<<" j="<<j<<" p="<<p<<" s="<<s<<" k="<<k<<endl;}
};
#define Q 0 // 1 - по вероятности, 0 - согласно ссылке на сортировку
int main(int mn,char* nm[])
{ int b[256],N,size,i,j,k,min,jmin1,jmin2,max,imax,Max=-2u/2,s; haff* h; unsigned char *d,*p,g,r,*v,**V;
for(ef(nm[0]),i=0;i<256;b[i++]=0); if(mn!=3) cerr<<nm[0]<<" in.ext out.arh\n",exit(1);
ifstream in(nm[1],ios::binary); if(!in) cerr<<"Файл \""<<nm[1]<<"\" не открыт!\n",exit(1);
in.seekg(0,ios::end);size=in.tellg();in.seekg(0,ios::beg);
p=new unsigned char[size]; in.read((char*)p,size);in.close();
for(i=0;i<size;i++) b[p[i]]++; // Подсчёт количества выпадения каждого из 256 символов
for(N=i=0;i<256;i++) N+=!!b[i]; // Подсчёт количества уникальных символов, содержащихся в исходном файле
cout<<"N="<<N<<endl; // вывод количества уникальных символов, содержащихся в исходном файле
h=new haff[2*N+1];if(!h) cerr<<"Error memory!\n",exit(1); // создание массива структур с учётом дополнительных узлов
// при построении схемы для определения 0 и 1 цифрового кода и ссылок на узлы сходящихся ветвей
for(j=0;j<2*N-1;j++) h[j].c=h[j].s=h[j].i=h[j].k=h[j].p=h[j].j=0; // Обнуление каждого элемента структуры
//для каждого элемента массива структур
for(j=i=0;i<256;i++) if(b[i]) h[j].s=b[i],h[j++].c=i; // запись в .s количества выпадений символа и в .c кода по ASCII самого символа
// для символов с ненулевым выпадением в элементы структуры, по порядку в первые элементы массива структур
unsigned char* S=new unsigned char[N];// массив под размеры кодовых последовательностей для каждого символа
for(j=-N;j<0;imax>=0?h[imax].i=h[imax].j=j++:j++) for(imax=-1,max=k=0;k<N;k++) if(!h[k].i) if(h[k].s>max) max=h[k].s,imax=k;
//Заполнение отрицательными индексами элементов первой части массива структур с учётом сортировки по убыванию количества
//выпадения символа в исходном файле (первый индекс самый минимальный отрицательный)
for(i=0;i<N-1;i++) // добавление ячеек
{ for(min=Max,jmin1=-1,j=0;j<N+i;j++) // от первой до последней добавленной ячейки
if(!h[j].p) if(min>=h[j].s) min=h[j].s,jmin1=j; // поиск первого символа с минимальным количеством выпадения
// среди неучтённых символов (неучтённый символ .p==0)
h[jmin1].p=N+i; // учёт символа записью ссылки на новый элемент из дополнительной области массива структур
for(min=Max,jmin2=-1,j=0;j<N+i;j++) // от первой до последней добавленной ячейки
if(!h[j].p) if(min>=h[j].s) min=h[j].s,jmin2=j; // поиск второго символа с минимальным количеством выпадения
// среди неучтённых символов
h[jmin2].p=N+i; // учёт символа записью ссылки на новый элемент из дополнительной области массива структур
if(Q) h[jmin1].k=0x30,h[jmin2].k=0x31; // установка цифрового кода (0 или 1) по вероятности
else
if(h[jmin1].j<h[jmin2].j) h[N+i].j=h[jmin1].j,h[jmin1].k=0x31,h[jmin2].k=0x30; // установка цифрового кода (0 или 1)
else h[N+i].j=h[jmin2].j,h[jmin1].k=0x30,h[jmin2].k=0x31; // согласно ссылке на сортировку
h[N+i].s=h[jmin1].s+h[jmin2].s; // определение количества выпадений для нового узлового суммарного символа
h[N+i].i=i+1; // присвоение новому символу очередного индекса (начиная с 1, ноль - признак отсутствия индексации)
}
ofstream ou(nm[2],ios::binary); if(!ou) cerr<<"Файл \""<<nm[2]<<"\" не создан!\n",exit(1);
for(ou.put(N-1),s=0,j=-1;++j<N;ou.put(h[j].c),ou.put(S[j]),s+=S[j]); // вывод в заголовок количества символов-1, и пар (код символа и размер кодовой последовательности для этого символа)
for(S[k=j]=0;h[k].p;k=h[k].p,++S[j]); // подсчёт числа кодовых бит для каждой кодовой последовательности
d=new unsigned char[s];v=new unsigned char[s]; V=new unsigned char*[N]; // в массиве d в обратном порядке, в v и V - в прямом
for(i=j=-1;++j<N;k++,i--) // запись кодовых последовательностей в обратном порядке
for(S[k=j]=0;d[++i]=h[k].k,h[k].p;k=h[k].p) S[j]++;
for(j=-1,k=0;++j<N;) V[j]=&v[k],k+=S[j];
for(g=8,s=r=k=0,j=-1;++j<N;k+=S[j])
for(i=S[j];i--;r<<=1)
{
r|=d[k+i]&1,v[s++]=d[k+i];
if(--g); else ou.put(r),g=8,r=0; // запись кодовых последовательностей в заголовок
}
//ou.close();exit(0);
for(;--g;r<<=1); ou.put(r); // запись в заголовок последнего байта, содержащего часть кодовой последовательности
for(j=-1,k=0;++j<N;) V[j]=&v[k],k+=S[j];
for(j=-1;++j<N;cout<<endl) for(cout<<'\''<<h[j].c<<"\' ",i=-1;++i<S[j];) cout<<V[j][i];
for(i=0;i<256;b[i++]=-1); for(i=0;i<N;b[(unsigned char)h[i].c]=i++); // запись в b[] индексов согласно массиву структур для определения индекса по считанному байту
for(g=k=j=0;j<size;j++) // формирование байтов из закодированного сообщения
for(i=0;i<S[b[p[j]]];g|=(V[b[p[j]]][i++]&1)<<(7-k%8),++k%8?1:(ou.put(g),g=0));
if(k%8) ou.put(g); // последний байт, если количество нулей и единиц сжатого текста не кратно 8
cout<<"Кодированная часть файла в битах: "<<k<<endl;
g=k%8,ou.put(g); // количество нулей и единиц сжатого текста
ou.close(); delete [] V;delete [] v;delete [] d;delete [] S;delete [] p;delete [] h; return 0;
}
