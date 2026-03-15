#pragma once

template<class T>	// 템플릿을 사용하겠다 : 상속받는 클래스의 종류를 알기 위해서
class Singleton
{
protected:	// 상속받은 클래스 또는 friend class만 접근할 수 있게 하겠다 : 싱글턴 디자인 패턴 클래스 (한개의 객체만 허용하는 클래스) 이므로 함부로 생성하는것을 막겠다.
	Singleton() {}
	Singleton(const Singleton& other) {}
	virtual ~Singleton() {}	// 상속받는 클래스의 소멸자가 호출된 이후에 호출하겠다는 의미 : 항상 생성된 순서의 역순으로 소멸을 시켜주어야 한다.

public:
	static T* GetInstance()	// 이 클래스의 객체(인스턴스)를 반환한다
	{
		return instance;
	}
	
	static void Create() // 새롭게 객체를 생성한다
	{
		if (instance == nullptr)	// 객체가 없을 경우에만
		{
			instance = new T();	// 새로 생성한다 : 여기에서 T 를 부르기 위해서 이 클래스를 상속받는 클래스는 [friend Singleton] 를 선언해야한다
			instance->Initialize();
		}
	}

	static void Delete() // 생성된 객체를 소멸시킨다
	{
		if (instance != nullptr)	// 객체가 있을 경우에만
		{
			instance->Destroy();
			delete instance;		// 파괴한다
		}
	}

public:
	virtual void Initialize() {}
	virtual void Destroy() {}

	virtual void Update() {}	// 클래스의 정보를 업데이트 하는 함수 : 이 클래스를 상속받는 클래스가 업데이트가 필수적이면 순수가상함수로 대체해도 좋다.
	virtual void Render() {}
	// 이외에 또 중복될만한 내용들 (함수, 변수 등등) 을 추가하여도 좋다

protected:
	static T* instance;	// 클래스의 객체 : 단 하나만 있어야 하므로 static으로 선언한다

};

template <class T>
T* Singleton<T>::instance = nullptr;	// 클래스 외부에서 객체를 꼭 초기화한다 : 클래스 내부의 static 변수를 사용하기전 초기화 하지 않으면 컴파일 에러 발생
