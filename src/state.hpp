#pragma once

class State
{
public:
	virtual ~State();
	
	virtual State* update() = 0;

private:
};
