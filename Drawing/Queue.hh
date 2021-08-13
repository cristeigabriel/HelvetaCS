#pragma once

#include "Geometry.hh"
#include <vector>
#include <memory>
#include <functional>
#include <shared_mutex>

struct Queue_t
{
	Queue_t() = default;
	Queue_t(std::shared_mutex *mutPtr);
	inline ~Queue_t(){};

	void Push(std::shared_ptr<Base_t> &&pRenderable);

	virtual void Run(const std::function<void(Queue_t *)> &runFn);

	std::vector<std::shared_ptr<Base_t>> m_vecRenderables = {};

private:
	std::shared_mutex *m_mutPtr = nullptr;
};