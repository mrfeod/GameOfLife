#include <vector>
#include <memory>

class LifeCicleImpl;

class LifeCicle
{
public:
	using Univers = std::vector< std::vector<bool> >;

public:
	LifeCicle(size_t w = 10, size_t h = 10);
	LifeCicle(Univers&& genesis);
	~LifeCicle();

	bool Alive();
	size_t Step();
	const Univers& State();

	void NextStep();

private:
	std::unique_ptr<LifeCicleImpl> p_impl;
};
