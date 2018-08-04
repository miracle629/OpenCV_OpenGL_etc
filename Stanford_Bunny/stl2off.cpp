/*************************************************/
/*
	May , 9 , 2018
	Guodong Zhao
	ASCII格式的几何stl文件转换为off文件
	采用双向map映射数据结构
*/
/*************************************************/
#pragma warning(disable:4996)
#include<fstream>
#include<iostream>
#include <vector>
#include<map>
#include <windows.h>

using namespace std;

void main()
{
	double start = GetTickCount();//开始计时
	int str_i = 0, cnt = 0, str_j = 0;//读取字符串计数指针
	int four_zero = 0;
	int face_number = 0;//面数量
	int point_length = 0;
	int vfs_length = 0;
	int tmp_length = 0;//建立map过程中临时的不重复点向量集合数量
	int p_count = 0;//map中对应各点的int值
	double x = 0, y = 0, z = 0;
	ifstream f_in("bun_zipper.stl");//输入文件
	ofstream f_out;//输出文件
	f_out.open("bun_zipper.off", ios::trunc);
	char a[100];//临时行记录
	char str[100];//临时字符串转数字记录
	vector<vector<double>> vfs;//重复记录的点向量的合集
	vector<double> vs;//临时单个点向量
	map<vector<double>, int> point_name;//映射1，点向量为key，int数为value
	map<int, vector<double>> point_name2;//映射2，int数为key，点向量为value
	do
	{
		str_i = 0;
		cnt = 0;
		//字符串读入，并将字母与空格剔除
		f_in.getline(a,100,'\n');
		while (a[str_i] != '\0')
		{
			if (!islower((int)a[str_i]) && !isupper((int)a[str_i]) && a[str_i] != ' ')
				break;
			cnt++;
			str_i++;
		}

		while (a[cnt] != '\0')
		{
			str[str_j] = a[cnt];
			cnt++;
			str_j++;
		}
		str[str_j] = '\0';
		str_j = 0;
		//数据读入向量并建立映射
		if (sscanf(str, "%lf%lf%lf", &x, &y, &z) == 3)
		{
			//压入vs
			vs.push_back(x);
			vs.push_back(y);
			vs.push_back(z);

			if (four_zero != 0)//剔除面皮法向量
			{
				vfs.push_back(vs);//读入有冗余点集
				tmp_length = point_name.size();
				if (tmp_length == 0)
				{
					point_name[vs] = p_count;//建立正向映射
					point_name2[p_count] = vs;//建立反向映射
					p_count++;
				}
				else
				{
					if (point_name.count(vs) == 0)//搜索是否冗余，正向映射搜索便捷
					{
						point_name[vs] = p_count;
						point_name2[p_count] = vs;
						p_count++;
					}
				}
			}
			four_zero++;
			face_number++;
			four_zero = four_zero % 4;//每加4归为0，剔除面片法向量的索引
			vs.erase(vs.begin(), vs.end());//擦除
		}
	} while (!f_in.eof());
	double  end = GetTickCount();
	point_length = point_name.size();//不重复的点向量数量
	vfs_length = vfs.size();//冗余记录的点向量集合数量
	face_number = face_number / 4;//1法向量+3点向量=1个面，除以4得到面数量
	f_out << "OFF" << endl;
	f_out << point_length << " " << face_number << " " << 0 << endl;
	for (int j = 0;j < point_length;j++)
	{
		vs=point_name2[j];//反向映射以int类key值为索引输出所有点
		f_out <<" "<< vs[0] << " " << vs[1] << " " << vs[2] << endl;
		vs.erase(vs.begin(), vs.end());
	}
	for (int j = 0;j < face_number;j++)
	{
		
		int l = point_name[vfs[3*j]];//正向映射查找点的索引号
		int m = point_name[vfs[3 * j + 1]];
		int n = point_name[vfs[3 * j + 2]];
		f_out << "3" << " " << l << " " << m << " " << n << endl;
	}
	cout << "GetTickCount:" << end - start << "ms" <<endl;//输出时间
	f_out.close();//关闭文件
	cout << "OK\n" << endl;
	system("pause");
}
