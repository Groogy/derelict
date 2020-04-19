#pragma once

class Earth;

class Energy
{
public:
	void update(const Earth& earth);
	float calculateChange(const Earth& earth) const;

	float getValue() const;

private:
	float myValue = 10;
};
