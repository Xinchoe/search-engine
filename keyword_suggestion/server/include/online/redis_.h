#ifndef __SINGLETON_H__
#define __SINGLETON_H__
#include <sw/redis++/redis++.h>
namespace keyword_suggestion
{
template<typename T>
class Redis
{
public:
    /* template<typename ...Args> */
    static T* getInstance()
    {
        if(nullptr==_pInstance)
        {
            _con.db=0;
            _con.host="127.0.0.1";
            _con.port=6379;
            _con_pool.size=3;
            _pInstance=new T(_con,_con_pool);
            _ar;//为了在模板参数推导时创建ar对象
        }
        return _pInstance;
    }
private:
    class AutoRelease
    {
    public:
        AutoRelease(){}
        ~AutoRelease()
        {
            if(_pInstance)
            {
                delete _pInstance;
            }
        }
    };
private:
    Redis(){
    }
    ~Redis() {}
private:
    static T* _pInstance;
    static AutoRelease _ar;
    static sw::redis::ConnectionOptions _con;
    static sw::redis::ConnectionPoolOptions _con_pool;
};
template<typename T>
T * Redis<T>::_pInstance=nullptr;

template<typename T>
typename Redis<T>::AutoRelease Redis<T>::_ar;

template<typename T>
sw::redis::ConnectionOptions Redis<T>::_con;
template<typename T>
sw::redis::ConnectionPoolOptions Redis<T>::_con_pool;
}//end of namespace keyword_suggestion
#endif

