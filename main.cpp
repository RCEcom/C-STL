#include <iostream>
#include "CArr.h"

class CTest
{
private:
	int m_i;
public:

};
int main(void)
{
	CArr<int> carr;
	//1~10
	for (int i = 0; i < 11; i++)
	{
		carr.push_back(i + 1);
	}
	CArr<int>::iterator myiter = carr.begin();

	//¦���� ����
	for (; myiter != carr.end();)	
	{
		if (*myiter % 2 == 0)
		{
			myiter = carr.erase(myiter);
		}
		else
		{
			++myiter;
		}
	}
	for (int i = 0; i < carr.size(); i++)
	{
		std::cout << carr[i] << std::endl;
	}
	//++, --
	//==
	//!=
	//*myiter ������ �����ϱ� 
	return 0;
}