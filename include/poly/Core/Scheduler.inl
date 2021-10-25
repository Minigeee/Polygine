namespace poly
{

#ifndef DOXYGEN_SKIP
namespace priv
{


///////////////////////////////////////////////////////////
template <typename T>
struct arg
{
    typedef typename std::conditional<
        std::is_lvalue_reference<T>::value,
        typename std::conditional<
            std::is_const<typename std::remove_reference<T>::type>::value,
            typename std::decay<T>::type,
            typename T
        >::type,
        typename std::decay<T>::type
    >::type type;
};

template <typename T>
using arg_t = typename arg<T>::type;


///////////////////////////////////////////////////////////
template<int ...>
struct seq { };

template<int N, int ...S>
struct gens : gens<N - 1, N - 1, S...> { };

template<int ...S>
struct gens<0, S...> {
    typedef seq<S...> type;
};


///////////////////////////////////////////////////////////
template <typename Sig>
class TaskState;


///////////////////////////////////////////////////////////
template <typename Ret, typename... Args>
class TaskState<Ret(Args...)> : public TaskStateResultType<Ret>
{
public:
    template <typename Func, typename... PassArgs>
    TaskState(Func&& func, PassArgs&&... args) :
        m_function      (std::forward<Func>(func)),
        m_args          (std::forward<PassArgs>(args)...)
    {
        m_refCount = 2;
    }

    void operator()() override
    {
        invoke(typename gens<sizeof...(Args)>::type());

        // Delete self if ref count is 0
        if (--m_refCount == 0)
            delete this;
    }

private:
    template<int ...S>
    void invoke(seq<S...>)
    {
        m_result = m_function(std::forward<arg_t<Args>>(std::get<S>(m_args))...);
    }

public:
    std::function<Ret(Args...)> m_function;
    std::tuple<arg_t<Args>...> m_args;
};


///////////////////////////////////////////////////////////
template <typename... Args>
class TaskState<void(Args...)> : public TaskStateResultType<void>
{
public:
    template <typename Func, typename... PassArgs>
    TaskState(Func&& func, PassArgs&&... args) :
        m_function      (std::forward<Func>(func)),
        m_args          (std::forward<PassArgs>(args)...)
    {
        m_refCount = 2;
    }

    void operator()() override
    {
        invoke(typename gens<sizeof...(Args)>::type());

        // Delete self if ref count is 0
        if (--m_refCount == 0)
            delete this;
    }

private:
    template<int ...S>
    void invoke(seq<S...>)
    {
        m_function(std::get<S>(m_args)...);
    }

public:
    std::function<void(Args...)> m_function;
    std::tuple<arg_t<Args>...> m_args;
};


///////////////////////////////////////////////////////////
template <typename Ret>
class TaskState<Ret()> : public TaskStateResultType<Ret>
{
public:
    template <typename Func>
    TaskState(Func&& func) :
        m_function      (std::forward<Func>(func))
    {
        m_refCount = 2;
    }

    void operator()() override
    {
        m_result = m_function();

        // Delete self if ref count is 0
        if (--m_refCount == 0)
            delete this;
    }

public:
    std::function<Ret()> m_function;
};


///////////////////////////////////////////////////////////
template <typename Ret, typename T, typename... Args>
class TaskState<Ret(T::*)(Args...)> : public TaskState<Ret(T*, Args...)>
{
public:
    template <typename Func, typename... PassArgs>
    TaskState(Func&& func, PassArgs&&... args) :
        TaskState<Ret(T*, Args...)>(std::forward<Func>(func), std::forward<PassArgs>(args)...)
    { }
};

///////////////////////////////////////////////////////////
template <typename Ret, typename... Args>
class TaskState<std::function<Ret(Args...)>> : public TaskState<Ret(Args...)>
{
public:
    template <typename Func, typename... PassArgs>
    TaskState(Func&& func, PassArgs&&... args) :
        TaskState<Ret(Args...)>(std::forward<Func>(func), std::forward<PassArgs>(args)...)
    { }
};


///////////////////////////////////////////////////////////
template <>
class TaskState<void()> : public TaskStateResultType<void>
{
public:
    template <typename Func>
    TaskState(Func&& func) :
        m_function      (std::forward<Func>(func))
    {
        m_refCount = 2;
    }

    void operator()() override
    {
        m_function();

        // Delete self if ref count is 0
        if (--m_refCount == 0)
            delete this;
    }

public:
    std::function<void()> m_function;
};


///////////////////////////////////////////////////////////
template <typename Ret>
inline Ret& TaskStateResultType<Ret>::getResult()
{
    return m_result;
}


}
#endif


///////////////////////////////////////////////////////////
template <typename Ret>
inline TaskBase<Ret>::TaskBase() :
    m_state     (0)
{

}


///////////////////////////////////////////////////////////
template <typename Ret>
inline TaskBase<Ret>::TaskBase(TaskBase<Ret>&& other) :
    m_state     (other.m_state)
{
    other.m_state = 0;
}


///////////////////////////////////////////////////////////
template <typename Ret>
inline TaskBase<Ret>& TaskBase<Ret>::operator=(TaskBase<Ret>&& other)
{
    if (&other != this)
    {
        // Delete last state
        if (m_state && --m_state->m_refCount == 0)
            delete m_state;

        m_state = other.m_state;
        other.m_state = 0;
    }

    return *this;
}


///////////////////////////////////////////////////////////
template <typename Ret>
inline TaskBase<Ret>::~TaskBase()
{
    if (m_state && --m_state->m_refCount == 0)
        delete m_state;

    m_state = 0;
}


///////////////////////////////////////////////////////////
template <typename Ret>
inline bool TaskBase<Ret>::isFinished() const
{
    // Task is finished when the ref count is 1
    return m_state && m_state->m_refCount == 1;
}


///////////////////////////////////////////////////////////
template <typename Ret>
inline Ret& Task<Ret>::getResult()
{
    return m_state->getResult();
}


///////////////////////////////////////////////////////////
template <typename F, typename... Args, typename Ret>
inline Task<Ret> Scheduler::addTask(F&& func, Args&&... args)
{
	return addTask(High, std::forward<F>(func), std::forward<Args>(args)...);
}


///////////////////////////////////////////////////////////
template <typename F, typename... Args, typename Ret>
inline Task<Ret> Scheduler::addTask(Scheduler::Priority priority, F&& func, Args&&... args)
{
    // Create new task state
    using Ft = typename std::conditional<std::is_member_function_pointer<F>::value, typename F, typename std::remove_reference<F>::type>::type;
    priv::TaskState<Ft>* state = new priv::TaskState<Ft>(std::forward<F>(func), std::forward<Args>(args)...);

    // Add to queue
    {
        std::unique_lock<std::mutex> lock(s_instance.m_mutex);
        s_instance.m_queue[priority].push(state);
    }

    // Notify any threads that are ready
    s_instance.m_scv.notify_one();

    // Return task object
    Task<Ret> task;
    task.m_state = state;
    return task;
}


}