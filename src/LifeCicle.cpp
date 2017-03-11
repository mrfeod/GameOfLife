#include "LifeCicle.hpp"

#include <stdlib.h>
#include <assert.h>
#include <list>

namespace
{
	enum Side
	{
		TOP   = 2,
		BOT   = 4,
		LEFT  = 8,
		RIGHT = 16
	};

	const std::list<uint8_t> g_neighborhoods
	{
		Side::TOP,
		Side::BOT,
		Side::LEFT,
		Side::RIGHT,
		Side::TOP | Side::LEFT,
		Side::TOP | Side::RIGHT,
		Side::BOT | Side::LEFT,
		Side::BOT | Side::RIGHT
	};

	const uint8_t NEIGHBORS_MAX = 4;
}

class LifeCicleImpl
{
public:
	LifeCicleImpl(size_t w, size_t h)
	{
		assert(w > 0 && h > 0);
		m_univers.resize(h);
		m_buffer.resize(h);
		for (size_t line = 0; line < m_univers.size(); ++line)
		{
			m_univers[line].resize(w);
			m_buffer[line].resize(w);
			for (auto& cell : m_univers[line])
				cell = rand() % 8 == 0;
		}

		init();
	}

	LifeCicleImpl(LifeCicle::Univers&& genesis)
	{
		assert(genesis.size() > 0);
		m_univers = std::move(genesis);
		m_buffer.resize(m_univers.size());
		for (auto& line : m_buffer)
			line.resize(m_univers[0].size());

		init();
	}

	~LifeCicleImpl() = default;

	bool Alive() { return m_alive; }
	size_t Step() { return m_step;  }
	const LifeCicle::Univers& State() { return m_univers; }

	void NextStep()
	{
		m_alive = false;

		m_univers.swap(m_buffer);
		for (size_t i = 0; i < m_buffer.size(); ++i)
		{
			for (size_t j = 0; j < m_buffer[0].size(); ++j)
			{
				auto cell = m_buffer[i][j];
				switch (Neighbors(i, j))
				{
				case 2:  break;
				case 3:  cell = true; break;
				default: cell = false;
				}
				if (!m_alive && cell)
					m_alive = true;
				m_univers[i][j] = cell;
			}
		}
		++m_step;
	}

private:
	void init()
	{
		m_hMax = m_univers.size() - 1;
		m_wMax = m_univers[0].size() - 1;
		NextStep();
	}

	bool Neighbor(size_t i, size_t j, uint8_t type)
	{
		if (!!(type & Side::TOP))
			i = i == 0 ? m_hMax : i - 1;
		if (!!(type & Side::BOT))
			i = i == m_hMax ? 0 : i + 1;
		if (!!(type & Side::LEFT))
			j = j == 0 ? m_wMax : j - 1;
		if (!!(type & Side::RIGHT))
			j = j == m_wMax ? 0 : j + 1;

		return m_buffer[i][j];
	}

	uint8_t Neighbors(size_t i, size_t j)
	{
		uint8_t neighbors = 0;
		for (auto type : g_neighborhoods)
		{
			if (Neighbor(i, j, type))
				++neighbors;
			if (neighbors == NEIGHBORS_MAX)
				break;
		}
		return neighbors;
	}

private:
	bool m_alive = false;

	size_t m_step = -1;
	size_t m_wMax = 0;
	size_t m_hMax = 0;

	LifeCicle::Univers m_univers;
	LifeCicle::Univers m_buffer;
};

LifeCicle::LifeCicle(size_t w/* = 10*/, size_t h/* = 10*/)
	: p_impl(std::make_unique<LifeCicleImpl>(w,h))
{
}

LifeCicle::LifeCicle(Univers&& genesis)
	: p_impl(std::make_unique<LifeCicleImpl>(std::move(genesis)))
{
}

LifeCicle::~LifeCicle()
{
}

bool LifeCicle::Alive()
{
	return p_impl->Alive();
};

size_t LifeCicle::Step()
{
	return p_impl->Step();
};

const LifeCicle::Univers& LifeCicle::State()
{
	return p_impl->State();
};

void LifeCicle::NextStep()
{
	p_impl->NextStep();
}
