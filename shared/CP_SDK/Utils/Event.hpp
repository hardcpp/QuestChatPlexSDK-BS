#pragma once

#include "Delegate.hpp"

#include <mutex>
#include <vector>

namespace CP_SDK::Utils {

    /// Forward declaration
    template <class t_FnType> class EventImpl;

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Event class
    template <class t_Ret, class... t_Args> class EventImpl<t_Ret(t_Args...)>
    {
        /// Copy constructor
        EventImpl(const EventImpl&) = delete;
        /// Move constructor
        EventImpl(EventImpl&&) = delete;

        public:
            /// Constructor
            EventImpl()
            {

            }

            /// Add a delegate
            /// @p_Delegate: Delegate to add
            EventImpl& operator+=(const Delegate<void(t_Args...)> & p_Delegate)
            {
                std::lock_guard l_Guard(m_Mutex);

                auto l_It = std::find_if(m_Delegates.begin(), m_Delegates.end(), [&p_Delegate](const Delegate<void(t_Args...)> & p_Item) -> bool {
                    return p_Item.EqualTo(&p_Delegate);
                });

                if (l_It == m_Delegates.end())
                    m_Delegates.push_back(p_Delegate);

                return *this;
            }
            /// Remove a delegate
            /// @p_Delegate: Delegate to remove
            EventImpl& operator-=(const Delegate<void(t_Args...)> &  p_Delegate)
            {
                std::lock_guard l_Guard(m_Mutex);

                auto l_It = std::find_if(m_Delegates.begin(), m_Delegates.end(), [&p_Delegate](const Delegate<void(t_Args...)> & p_Item) -> bool {
                    return p_Item.EqualTo(&p_Delegate);
                });

                if (l_It != m_Delegates.end())
                    m_Delegates.erase(l_It);

                return *this;
            }

            /// @brief Clear
            void Clear()
            {
                m_Mutex.lock();
                m_Delegates.clear();
                m_Mutex.unlock();
            }

            /// Invoke
            /// @p_Args...: Arguments
            void operator()(t_Args... p_Args)
            {
                Invoke(std::forward<t_Args>(p_Args)...);
            }
            /// Invoke
            /// @p_Args...: Arguments
            void Invoke(t_Args... p_Args)
            {
                m_Mutex.lock();
                std::vector<Delegate<void(t_Args...)>> l_Copy = m_Delegates;
                m_Mutex.unlock();

                for (const auto & l_Delegate : l_Copy)
                    l_Delegate.Invoke(std::forward<t_Args>(p_Args)...);
            }

        private:
            std::mutex                                 m_Mutex;        ///< Lock mutex
            std::vector<Delegate<void(t_Args...)>>     m_Delegates;    ///< Delegates

    };

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Event class helper
    template <class... t_Args>
    class Event : public EventImpl<void(t_Args...)>
    {
        using EventImpl<void(t_Args...)>::EventImpl;
    };

}   ///< namespace CP_SDK::Utils
