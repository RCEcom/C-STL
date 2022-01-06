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
	//데이터 삽입하는 함수
	void push_back(const T& _Data);
	//칸을 재설정 할 수 있는 함수
	void resize(int _iDataCount);
	//오퍼레이터
	T& operator[](int idx); //배열 and 인자는 index
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
		CArr* m_pArr; //iterator 가 가리킬 데이터를 관리하는 가변배열 주소
		T* m_pData;   //데이터들의 시작 주소
		int m_iIdx;   //가리키는 데이터의 index
	public:
		T& operator * ()
		{
			//iter 주소가 vector가변 배열이 알고 있는 주소랑 다르다면(error) => 공간 확장으로 주소가 달리진 경우
			//iter가 end iterator일 경우
			if (m_pArr->m_pInt != m_pData || m_iIdx == -1)
			{
				assert(nullptr);
			}
			return m_pData[m_iIdx];
		}

		//전위
		iterator& operator ++()
		{ 
			//1 iterator 가 마지막 데이터를 가리키고 있는 경우
			//2 end iterator가 될 경우
			//3 iterator 가 알고있는 주소와, 가변배열이 알고 있는 주소가 달라진 경우(공간 확장으로 주소가 달라진 경우)
			if (m_pArr->m_pInt != m_pData || m_iIdx == -1)
			{
				assert(nullptr);
			}
			//1 iterator 가 마지막 데이터를 가리키고 있는 경우
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

		//후위
		iterator operator++(int)
		{
			//fake : 후위 연산인 것처럼 속여서, 여기 오퍼레이터가 실행 되도록.
			iterator copy_iter = *this;
			++(*this); //다음 인덱스 바라보도록 함.
			return copy_iter;
		}
		iterator operator--(int)
		{
			//fake : 후위 연산인 것처럼 속여서, 여기 오퍼레이터가 실행 되도록.
			iterator copy_iter = *this;
			--(*this); //다음 인덱스 바라보도록 함.
			return copy_iter;
		}


		//비교연산자 ==, !=
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
	delete[] m_pInt; //int가 여러묶음으로 있을 떄

}

template<typename T>
void CArr<T>::push_back(const T& _Data)
{
	//힙 영역에 할당한 공간이 다 참
	if (this->m_iMaxCount <= this->m_iCount)
	{
		resize(m_iMaxCount * 2);
	}
	//데이터 추가
	this->m_pInt[this->m_iCount++] = _Data;
}

template<typename T>
void CArr<T>::resize(int _iDataCount)
{
	if (m_iMaxCount >= _iDataCount)
	{
		assert(nullptr); //프로그램 종료
	}

	//1. 리사이즈 시킬 개수만큼 동적할당 한다.
	T* pNew = new T[_iDataCount];
	//2. 기존 공간에 있던 데이터들을 새로 할당한 공간으로 복사한다.
	for (int i = 0; i < m_iCount; i++)
	{
		pNew[i] = this->m_pInt[i];
	}

	//3. 기존 공간 해제
	delete[] this->m_pInt;

	//4. 배열이 새로운 공간을 가르키도록 함
	m_pInt = pNew;

	//5. MaxCount 변경점 적용
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
	//iterator 가 가르키는 데이터를  배열 내에서 제거한다.
	int iLoopCount = m_iCount - (_iter.m_iIdx + 1); // +1 한 이유 index는 0부터 시작함.

	for (int i = 0; i < iLoopCount; i++)
	{
		m_pInt[_iter.m_iIdx + i] = m_pInt[_iter.m_iIdx + i + 1];
	}
	//카운트 감소(제거 했으니까)
	--m_iCount;
	return iterator(this, m_pInt, _iter.m_iIdx);
}