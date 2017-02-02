#pragma once
#include <string>
#include <iostream>


class Level
{

private:
	std::string label;
	int xpForNext;
	int levelNumber;

public:
	Level();
	Level(std::string pLabel, int pXp, int pId);
	~Level();
	friend std::ostream& operator<< (std::ostream& stream, const Level &pLevel);
	std::string getLabel();
	int getLevelXpForNext();
	int getIndex();
	bool isNull();

	void show();

};

