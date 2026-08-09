#pragma once

#include "IObjectPool.hpp"
#include "../Utils/Delegate.hpp"
#include "../ChatPlexSDK.hpp"

#include <vector>
#include <exception>
#include <memory>
#include <mutex>

namespace CP_SDK::Pool {

    namespace _v
    {
        using namespace CP_SDK::Utils;
    }

    /// @brief Vector based object pool
    /// @tparam t_Type Pooled object type
    template<class t_Type>
    class MTObjectPool : public IObjectPool<t_Type>
    {
        CP_SDK_NO_DEF_CTORS(MTObjectPool);

        private:
            std::vector<t_Type> m_Vector;
            std::mutex          m_Mutex;
            _v::Func<t_Type>    m_CreateFunc;
            _v::Action<t_Type&> m_ActionOnGet;
            _v::Action<t_Type&> m_ActionOnRelease;
            _v::Action<t_Type&> m_ActionOnDestroy;
            int                 m_MaxSize;
            int                 m_CountAll = 0;
            bool                m_CollectionCheck;

        public:
            using Ptr = std::shared_ptr<MTObjectPool<t_Type>>;

            /// @brief Constructor
            MTObjectPool(   const typename IObjectPool<t_Type>::__this_is_private & __pvTag,
                            _v::CFuncRef<t_Type>    createFunc,
                            _v::CActionRef<t_Type&> actionOnGet,
                            _v::CActionRef<t_Type&> actionOnRelease,
                            _v::CActionRef<t_Type&> actionOnDestroy,
                            bool                    collectionCheck,
                            int                     defaultCapacity,
                            int                     maxSize)
                : IObjectPool<t_Type>::IObjectPool(CP_SDK_PRIV_TAG_FWD())
            {
                if (!createFunc.IsValid())
                    throw std::runtime_error("createFunc is null");

                if (maxSize <= 0)
                    throw std::runtime_error("Max Size must be greater than 0");
                if (defaultCapacity < 0 || defaultCapacity > maxSize)
                    throw std::runtime_error("Default capacity must be between 0 and maxSize");

                m_CreateFunc        = createFunc;
                m_MaxSize           = maxSize;
                m_ActionOnGet       = actionOnGet;
                m_ActionOnRelease   = actionOnRelease;
                m_ActionOnDestroy   = actionOnDestroy;
                m_CollectionCheck   = collectionCheck;

                m_Vector.reserve(maxSize);

                while (defaultCapacity-- > 0)
                {
                    m_Vector.push_back(m_CreateFunc());
                    m_CountAll++;
                }
            }
            /// @brief Destructor
            ~MTObjectPool()
            {
                try { Clear(); } catch (...) { }
            }

            /// @brief Constructor
            /// @return Created memeory pool
            static Ptr Make(_v::CFuncRef<t_Type>    createFunc,
                            _v::CActionRef<t_Type&> actionOnGet     = nullptr,
                            _v::CActionRef<t_Type&> actionOnRelease = nullptr,
                            _v::CActionRef<t_Type&> actionOnDestroy = nullptr,
                            bool                    collectionCheck = true,
                            int                     defaultCapacity = 10,
                            int                     maxSize         = 100)
            {
                return std::make_shared<MTObjectPool<t_Type>>(typename IObjectPool<t_Type>::__this_is_private{0},
                    createFunc, actionOnGet, actionOnRelease, actionOnDestroy, collectionCheck, defaultCapacity, maxSize
                );
            }

        public:
            /// @brief Active elements
            int CountActive()
            {
                std::lock_guard<std::mutex> l_Lock(m_Mutex);
                return m_CountAll - static_cast<int>(m_Vector.size());
            }
            /// @brief Released element
            int CountInactive() override
            {
                std::lock_guard<std::mutex> l_Lock(m_Mutex);
                return static_cast<int>(m_Vector.size());
            }

        public:
            /// @brief Simple get
            t_Type Get() override
            {
                t_Type l_Result{};
                bool l_Create = false;

                {
                    std::lock_guard<std::mutex> l_Lock(m_Mutex);

                    if (m_Vector.size() == 0)
                        l_Create = true;
                    else
                    {
                        l_Result = m_Vector.back();
                        m_Vector.pop_back();
                    }
                }

                if (l_Create)
                {
                    l_Result = m_CreateFunc();
                    std::lock_guard<std::mutex> l_Lock(m_Mutex);
                    m_CountAll++;
                }

                m_ActionOnGet(l_Result);
                return l_Result;
            }
            /// @brief Release an element
            /// @param p_Element Element to release
            void Release(t_Type& p_Element) override
            {
                m_ActionOnRelease(p_Element);

                bool l_Destroy = false;
                {
                    std::lock_guard<std::mutex> l_Lock(m_Mutex);

                    if (m_CollectionCheck && m_Vector.size() > 0 && std::find(m_Vector.begin(), m_Vector.end(), p_Element) != m_Vector.end())
                        throw std::runtime_error("Trying to release an object that has already been released to the pool.");

                    if (static_cast<int>(m_Vector.size()) < m_MaxSize)
                        m_Vector.push_back(const_cast<t_Type&>(p_Element));
                    else
                    {
                        m_CountAll--;
                        l_Destroy = true;
                    }
                }

                if (l_Destroy)
                    m_ActionOnDestroy(p_Element);
            }

        public:
            /// @brief Clear the object pool
            void Clear() override
            {
                std::vector<t_Type> l_ToDestroy;
                {
                    std::lock_guard<std::mutex> l_Lock(m_Mutex);
                    l_ToDestroy.swap(m_Vector);
                    m_CountAll -= static_cast<int>(l_ToDestroy.size());
                }

                for (auto& l_Current : l_ToDestroy)
                    m_ActionOnDestroy(l_Current);
            }

    };

}   ///< namespace CP_SDK::Pool
