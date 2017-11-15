#include<cstdlib>
#include<iostream>
#include<fstream>
#include<string>
#include<cmath>
#include<ctime>

using namespace std;

void ef(char* n) { int j,k=-1; for(;n[++k];); for(;--k>=0;) if(n[k]=='\\') break; for(j=0;!!(n[j++]=n[++k]);); }

string addp(string a,string b)
{ 
	int i,j,d; string c="";
	if(a.size()>=b.size()) { for(c=a,i=b.size()-1,j=c.size()-1;i>=0;c[j--]=b[i--]); for(;j>=0;c[j--]='0'); b=c; }
	else { for(c=b,i=a.size()-1,j=c.size()-1;i>=0;c[j--]=a[i--]); for(;j>=0;c[j--]='0'); a=c; }
	for(d=0,i=c.size()-1;i>=0;j=a[i]+b[i]-96+d,c[i--]=j%10+48,d=j/10);
	if(d) c="1"+c;
	for(;c[0]=='0'&&c.size()>1;c.erase(i-1)) for(i=1;i<c.size();c[i-1]=c[i++]);
	return c;
}

int main(int mn,char* nm[])
{ 
	int i,j,k,l,s,t,row,*col,max,sum,N,K; char C,c,*p,*d,*b; string aa,ss; float q,Q,Q_,x,x_;
	cout<<"Генератор символов по заданному вектору N вероятностей появления символов\\nили матрице NxN условных вероятностей\n\n"; ef(nm[0]);
	if(mn!=5) cerr<<nm[0]<<" ver.txt simv.cb shift N\n",
	cout<<"\nver.txt - текстовый файо с вероятностями пояаления символов\n",
	cout<<" либо сумма построчно равна 1, либо одно значение в строчке вне диапазона (0..1),\n",
	cout<<"simv.cb - результирующий текст,\n",
	cout<<"shift - добавка к кодам символов для переноса в текстовую часть (изначально коды отсчитываются с 00h,\n",
	cout<<"N - размер ситезируемого файла.\n",exit(1);
	ifstream in(nm[1],ios::binary); if(!in) cerr<<"Файл \""<<nm[1]<<"\" не открыт!\n",exit(2);
	in.seekg(0,ios::end);s=in.tellg();in.seekg(0,ios::beg);
	p=new char[s+2]; in.read(p,s); in.close(); p[s]=0x0A; p[s+1]=0;
	for(k=i=0;p[k]=p[i];i++) k+=((p[i]|2)==0x2E||(p[i]|7)=='7'||(p[i]|1)=='9'||p[i]==9||p[i]==0x20||p[i]==0x0A);
	for(i=-1;p[++i]+=23*!(p[i]-0x09);); // замена '\t' на пробел
	for(c=0x20,k=i=0;p[k]=p[i];c=p[i++]) if(!(c^p[i])&(c==0x20)); else p[k++]=p[i]; // двойных пробелов, а также пробелов в конце строки
	for(i=-1;p[++i];) if(p[i+1]==0x0A&&p[i]==0x20||p[i+1]==0x20&&p[i]==0x0A) p[i]=p[i+1]=0x0A; // замена пробелов в начале и конце строки на 0x0A
	for(c=0x0A,k=i=0;p[k]=p[i];c=p[i++]) if(!(c^p[i])&(c==0x0A)); else p[k++]=p[i]; s=k+1; // удаление двойных символов 0x0A, а также символа 0x0A вначале
	for(row=i=0;p[i];row+=(p[i++]==0x0A)); // подсчёт числа строк
	col=new int[row]; for(i=-1;++i<row;col[i]=0); // массив для количества солбцов в каждой строке
	for(k=i=0;p[i];col[k]+=(p[i]==0x20),k+=(p[i++]==0x0A)); // подсчёт пробелов в каждой строке
	for(k=i=-1;++i<row;k&=!(col[0]^col[i])); // проверка одинаковости количества пробелов в строках
	if(!k) cerr<<"Ошибка! Разное количество параметров в строках\n",exit(3);
	if(!col[0]) { for(i=-1;++i<s-2;) if(p[i]==0x0A) p[i]=0x20; col[0]=row-1,row=1; } // столбец в строку
	if(row-1) if(row!=col[0]+1) cerr<<"Ошибка! Матрица не квадратная\n",exit(4);
	float *f=new float[row*(col[0]+1)];
	float **F=new float*[row];
	float *P=new float[row];
	for(i=-1;++i<row;) F[i]=&f[(col[0]+1)*i];
	for(i=-1;++i<s;) if(p[i]==0x0A) p[i]=0x20;
	for(f[0]=atof(p),k=i=0;++i<s;) if(p[i]==0x20) f[++k]=atof(&p[i]);
	for(i=-1;++i<row;cout<<endl) for(k=-1;++k<=col[i];) cout<<F[i][k]<<'\t'; // вывод вероятностей
	d=new char[s*2];
	b=new char[s*2];
	for(c=j=i=-1;p[++i];d[++j]=p[i],c=p[i]) if((p[i]|2)==0x2E&&(c!=0x30)) d[++j]=0x30; d[++j]=0; // добавление нуля перед десятичной запятой (точкой) при его отсутствии
	for(max=k=0,i=-1;p[++i];) if(p[i]==0x20) { if(max<k) max=k; k=0; } else k++; // определение максимального размера строки
	for(j=i=-1;d[++i];) if((d[i]|2)==0x2E); else d[++j]=d[i]; d[++j]=0; // устранение десятичной запятой (точки)
	for(--max,l=i=-1,k=0;d[++i];b[++l]=d[i],k*=!!j) // добавление нулей справа
	{	
		j=(d[i]!=32); k+=j;
		if(!j) for(;k++<max;b[++l]=48);
	}
	b[++l]=0;
	int* g=new int[row*(col[0]+1)];int** G=new int*[row];for(i=-1;++i<row;) G[i]=&g[(col[0]+1)*i];
	for(s=k=i=-1;++i<row;)
	{
		for(ss="0",t=1,j=-1;++j<=col[i];k++,ss=addp(ss,aa),G[i][j]=atoi((char*)&aa[0]))
		for(aa="",l=0;l++<max;aa+=b[++k]);
		for(l=-1;ss[++l];); t&=(l==max); // проверка длины результирующей строки
		for(t&=(ss[0]=='1'),l=0;ss[++l];) t&=(ss[l]=='0');// проверка наличия 1 и любого количества нулей
		s&=t;
		if(!t) cerr<<"сумма вероятностей не равна 1 в строке: "<<i<<endl;
		}
		sum=atoi((char*)&ss[0]);
		if(!s) exit(1); //выход по ошибке сумме вероятностей
		for(K=2;K<sum;K*=2);
		N=atoi(nm[4]); if(N<=0) cerr<<" Ошибка количество N="<<N<<" <= 0!"<<endl,exit(5);
		C=atoi(nm[3]); if(C<0 || C>255) cerr<<" Ошибка смещение вне диапазона 0...255!"<<endl,exit(6);
		for(i=-1;++i<row;) for(j=0;++j<=col[0];) G[i][j]+=G[i][j-1];
		srand(time(NULL)),rand();
		ofstream ou(nm[2],ios::binary);
		if(row==1) // row x 1
		{
			for(i=0;i<N;) for(t=rand()%K,j=0;j<=col[0];j++) if(t<G[0][j]) ou.put(C+j),j=col[0],i++;
			for(q=i=0;i<=col[0];i++) if(F[0][i]>1e-9) q+=-F[0][i]*log(F[0][i])/log(2.);
			x=1-q/(log(float(col[0]+1.))/log(2.));
			cout<<"Энтропия и избыточность для рассчитанных безусловных вероятностей:\n"
			<<"H(A)="<<q<<"\nх(A)="<<x
			<<"\nВозможность сжатия синтезированного текста без учёта связи между соседними символами:"
			<<"\nn/n0="<<1/(1-x)<<endl;
			}
			else // row x row
			{ 
				int rw,idx; float mx,sm,w,ml; rw=row-1;
				float *R=new float[row]; float *B=new float[rw];
				float *u=new float[rw*rw]; float **U=new float*[rw]; for(i=-1;++i<rw;) U[i]=&u[rw*i];
				float *e=new float[rw*rw]; float **E=new float*[rw]; for(i=-1;++i<rw;) E[i]=&e[rw*i];
				for(i=0;i<rw;B[i]=F[rw][i],i++) for(j=0;j<rw;j++) U[j][i]=(i==j)+F[rw][j]-F[i][j];
				for(i=0;i<rw;i++) for(j=0;j<rw;j++) E[i][j]=(i==j); // единичная матрица
				for(k=0;k<rw;k++) // поиск обратной матрицы (rw x rw)
				{ 
					for(mx=U[k][k],idx=k,i=k+1;i<rw;i++) if(fabs(U[i][k])>fabs(mx)) mx=U[idx=i][k]; // поиск максимума
					if(idx>k) // если есть максимум
					for(i=0;i<rw;i++)
					w=U[k][i],U[k][i]=U[idx][i],U[idx][i]=w,w=E[k][i],E[k][i]=E[idx][i],E[idx][i]=w;
					for(i=k+1;i<rw;i++) // обнуление умножением с вычитанием
					if(U[k][k]) for(ml=U[i][k]/U[k][k],j=0;j<rw;j++) U[i][j]-=ml*U[k][j],E[i][j]-=ml*E[k][j];
					else cerr<<"Ошибка 1 при нахождении обратной матрицы U["<<k<<"]["<<k<<"]\n",exit(7);
					}
					for(k=rw-1;k>=0;k--) // приведение матрицы к диагональной
					for(i=k-1;i>=0;i--)
					if(U[k][k]) for(ml=U[i][k]/U[k][k],j=0;j<rw;j++) E[i][j]-=ml*E[k][j];
					else cerr<<"Ошибка 2 при нахождении обратной матрицы U["<<k<<"]["<<k<<"]\n",exit(8);
					for(k=0;k<rw;k++) for(i=0;i<rw;i++) E[k][i]/=U[k][k]; // приведение матрицы к единичной (обратная)
					for(sm=i=0;i<rw;sm+=(P[i]=R[i++])) for(R[i]=j=0;j<rw;j++) R[i]+=E[i][j]*B[j]; // умножение матрицы на вектор
					P[rw]=R[rw]=1-sm; // расчёт последней вероятности
					for(i=0;i<=rw;i++) cout<<"P("<<i<<")="<<P[i]<<endl; // вывод рассчитанных вероятностей
					for(i=1;i<=rw;i++) R[i]+=R[i-1]; // последний порог = 1 - не нужен
					for(t=rand(),i=-1;++i<rw;) if(t<RAND_MAX*R[i]) ou.put(C+(k=i)),i=row;
					if(i<=row) k=rw,ou.put(C+k);
					for(i=1;i<N;) for(t=rand()%K,j=0;j<=row;j++) if(t<G[k][j]) ou.put(C+(k=j)),j=row,i++;
					for(Q=i=0;i<=row;i++) if(P[i]>1e-9) Q+=-P[i]*log(P[i])/log(2.);
					ou.close(); x=1-Q/(log(float(row))/log(2.));
					for(Q_=i=0;i<row;Q_+=-P[i++]*q) for(q=j=0;j<row;j++) if(F[i][j]) q+=F[i][j]*log(F[i][j])/log(2.);
					x_=1-Q_/(log(float(row))/log(2.));
					cout<<"Энтропия и избыточность для рассчитанных безусловных вероятностей:\nH(A)="<<Q<<"\nх(A)="<<x
					<<"\nВозможность сжатия синтезированного текста без учёта связи между соседними символами:"
					<<"\nn/n0="<<1/(1-x)<<"\nЭнтропия и избыточность для рассчитанных условных вероятностей:"
					<<"\nH(A|A^)="<<Q_<<"\nх(A|A^)="<<x_
					<<"\nВозможность сжатия синтезированного текста с учётом связи между соседними символами:"
					<<"\nn_/n0="<<1/(1-x_)<<endl;
					delete [] R; delete [] B; delete [] U; delete [] E; delete [] u; delete [] e;
					}
					delete [] P;delete [] G; delete [] g;delete [] b;delete [] d;delete [] f;delete [] F;delete [] col;delete [] p;
					return 0;
}
