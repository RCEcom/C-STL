#pragma once
#include <assert.h> 
template<typename T>
class CArr
{
private:
	T* m_pInt;
	int  m_iCount;
	int  m_iMaxCount;

public:
	//������ �����ϴ� �Լ�
	void push_back(const T& _Data);
	//ĭ�� �缳�� �� �� �ִ� �Լ�
	void resize(int _iDataCount);
	//���۷�����
	T& operator[](int idx); //�迭 and ���ڴ� index
	T* data() { return m_pInt; }
	int size() { return m_iCount; }
	int capacity() { return m_iMaxCount; }
	class iterator;
	iterator begin();
	iterator end();	
	iterator erase(const iterator& _iter);

	void clear() { m_iCount = 0; }
public:
	CArr();
	~CArr();
	class iterator
	{
	private:
		CArr* m_pArr; //iterator �� ����ų �����͸� �����ϴ� �����迭 �ּ�
		T* m_pData;   //�����͵��� ���� �ּ�
		int m_iIdx;   //����Ű�� �������� index
	public:
		T& operator * ()
		{
			//iter �ּҰ� vector���� �迭�� �˰� �ִ� �ּҶ� �ٸ��ٸ�(error) => ���� Ȯ������ �ּҰ� �޸��� ���
			//iter�� end iterator�� ���
			if (m_pArr->m_pInt != m_pData || m_iIdx == -1)
			{
				assert(nullptr);
			}
			return m_pData[m_iIdx];
		}

		//����
		iterator& operator ++()
		{ 
			//1 iterator �� ������ �����͸� ����Ű�� �ִ� ���
			//2 end iterator�� �� ���
			//3 iterator �� �˰��ִ� �ּҿ�, �����迭�� �˰� �ִ� �ּҰ� �޶��� ���(���� Ȯ������ �ּҰ� �޶��� ���)
			if (m_pArr->m_pInt != m_pData || m_iIdx == -1)
			{
				assert(nullptr);
			}
			//1 iterator �� ������ �����͸� ����Ű�� �ִ� ���
			if (m_iIdx == m_pArr->size() - 1)
			{
				m_iIdx = -1;
			}
			else
			{
				++m_iIdx;
			}
			return *this;
		}
		iterator& operator --()
		{
			--m_iIdx;
			return *this;
		}

		//����
		iterator operator++(int)
		{
			//fake : ���� ������ ��ó�� �ӿ���, ���� ���۷����Ͱ� ���� �ǵ���.
			iterator copy_iter = *this;
			++(*this); //���� �ε��� �ٶ󺸵��� ��.
			return copy_iter;
		}
		iterator operator--(int)
		{
			//fake : ���� ������ ��ó�� �ӿ���, ���� ���۷����Ͱ� ���� �ǵ���.
			iterator copy_iter = *this;
			--(*this); //���� �ε��� �ٶ󺸵��� ��.
			return copy_iter;
		}


		//�񱳿����� ==, !=
		bool operator == (const iterator& _otheriter)
		{
			if (m_pData == _otheriter.m_pData && m_iIdx == _otheriter.m_iIdx)
			{
				return true;
			}
			else
			{
				return false;
			}
		}

		bool operator != (const iterator& _otheriter)
		{
			if (m_pData == _otheriter.m_pData && m_iIdx == _otheriter.m_iIdx)
			{
				return false;
			}
			else
			{
				return true;
			}
		}

	public:
		iterator() : m_pArr(nullptr), m_pData(nullptr), m_iIdx(-1)
		{

		}
		iterator(CArr* _pArr, T* _pData, int _Iidx) : m_pData(_pData), m_pArr(_pArr), m_iIdx(_Iidx)
		{

		}
		~iterator()
		{

		}
		friend class CArr;
	};
};

template<typename T>
CArr<T>::CArr() : m_pInt(nullptr), m_iCount(0), m_iMaxCount(2)
{
	m_pInt = new T[2]; //C : malloc(8);
}
template<typename T>
CArr<T>::~CArr()
{
	delete[] m_pInt; //int�� ������������ ���� ��

}

template<typename T>
void CArr<T>::push_back(const T& _Data)
{
	//�� ������ �Ҵ��� ������ �� ��
	if (this->m_iMaxCount <= this->m_iCount)
	{
		resize(m_iMaxCount * 2);
	}
	//������ �߰�
	this->m_pInt[this->m_iCount++] = _Data;
}

template<typename T>
void CArr<T>::resize(int _iDataCount)
{
	if (m_iMaxCount >= _iDataCount)
	{
		assert(nullptr); //���α׷� ����
	}

	//1. �������� ��ų ������ŭ �����Ҵ� �Ѵ�.
	T* pNew = new T[_iDataCount];
	//2. ���� ������ �ִ� �����͵��� ���� �Ҵ��� �������� �����Ѵ�.
	for (int i = 0; i < m_iCount; i++)
	{
		pNew[i] = this->m_pInt[i];
	}

	//3. ���� ���� ����
	delete[] this->m_pInt;

	//4. �迭�� ���ο� ������ ����Ű���� ��
	m_pInt = pNew;

	//5. MaxCount ������ ����
	m_iMaxCount = _iDataCount;
}

template<typename T>  
T& CArr<T>::operator[](int idx)
{
	return this->m_pInt[idx];
}

template<typename T>
typename CArr<T>::iterator CArr<T>::begin()
{
	if (m_iCount == 0)
	{
		return iterator(this, m_pInt, -1);
	}
	return iterator(this, m_pInt, 0);
}	

template<typename T>
typename CArr<T>::iterator CArr<T>::end()
{
	return iterator(this, m_pInt, -1);
}	

template<typename T>
typename CArr<T>::iterator CArr<T>::erase(const iterator& _iter)
{
	if (this != _iter.m_pArr || end() == _iter || m_iCount <= _iter.m_iIdx)
	{
		assert(nullptr);
	}
	//iterator �� ����Ű�� �����͸�  �迭 ������ �����Ѵ�.
	int iLoopCount = m_iCount - (_iter.m_iIdx + 1); // +1 �� ���� index�� 0���� ������.

	for (int i = 0; i < iLoopCount; i++)
	{
		m_pInt[_iter.m_iIdx + i] = m_pInt[_iter.m_iIdx + i + 1];
	}
	//ī��Ʈ ����(���� �����ϱ�)
	--m_iCount;
	return iterator(this, m_pInt, _iter.m_iIdx);
}