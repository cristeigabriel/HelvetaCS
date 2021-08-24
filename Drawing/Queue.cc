#include "Queue.hh"
#undef NDEBUG
#include <assert.h>

Queue_t::Queue_t(std::shared_mutex *mutPtr)
{
	this->m_mutPtr = mutPtr;
}

void Queue_t::Push(std::shared_ptr<Base_t> &&pRenderable)
{
	this->m_vecRenderables.emplace_back(std::move(pRenderable));
}

void Queue_t::Run(const std::function<void(Queue_t *)> &runFn)
{
	static bool exists = this->m_mutPtr != nullptr;
	if (exists)
		this->m_mutPtr->lock();

	//	Assuming we previously had contents in the vector, clear them,
	//	we want a new frame.
	this->m_vecRenderables.clear();
	runFn(this);

	if (exists)
		this->m_mutPtr->unlock();
}