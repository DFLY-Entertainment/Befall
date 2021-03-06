#ifndef MYSTRUCTS_H
#define MYSTRUCTS_H
#include "MyEnums.h"

#include "cocos2d.h"
#include <set>
#include <vector>
#include <map>

struct MyPointStruct
{
	int x;
	int y;
	
};
bool operator < (const MyPointStruct & thisOne, const MyPointStruct & another);
bool operator == (const MyPointStruct & thisOne, const MyPointStruct & another);
bool operator != (const MyPointStruct & thisOne, const MyPointStruct & another);

struct InfluenceStruct
{
	std::string type;
	std::string target;
	int value;
};


//use this in vector
struct PathNodeStruct
{
	MyPointStruct point;
	int indexParent;
	int stepLeft;
};
bool operator == (const PathNodeStruct & thisOne, const PathNodeStruct & another);
bool operator != (const PathNodeStruct & thisOne, const PathNodeStruct & another);

/*
bool operator < (const PathNodeStruct & thisOne, const PathNodeStruct & another)
{
	return (thisOne.stepLeft < another.stepLeft);
}
*/

//use it in vector
struct TechTreeNodeStruct
{
	TechEnum techName;
	std::vector<int> indexParents;
	bool isUnclocked;
};

/*
bool operator < (const TechTreeNodeStruct & thisOne, const TechTreeNodeStruct & another)
{
	return (thisOne.techName < another.techName);
}
*/

struct UnitPropertyStruct
{
	int numHitPoint;
	int numDefence;
	int numAttack;
	int numRangeAttack;
	int numRangeMove;
	int numPopulation;
	UnitPropertyStruct operator += (const UnitPropertyStruct & another)
	{
		this->numHitPoint += another.numHitPoint;
		this->numDefence += another.numDefence;
		this->numAttack += another.numAttack;
		this->numRangeAttack += another.numRangeAttack;
		this->numRangeMove+= another.numRangeMove;
		this->numPopulation += another.numPopulation;
		return (*this);
	}
};

UnitPropertyStruct operator + (const UnitPropertyStruct & thisOne, const UnitPropertyStruct & another);


struct ResourcesStruct
{
	int numFixedResource;
	int numRandomResource;
	int numProductivity;
	int numResearchLevel;
	ResourcesStruct operator += (const ResourcesStruct & another)
	{
		this->numFixedResource += another.numFixedResource;
		this->numRandomResource += another.numRandomResource;
		//this->numProductivity += another.numProductivity;
		//this->numResearchLevel += another.numResearchLevel;
		return (*this);
	}
	ResourcesStruct operator -= (const ResourcesStruct & another)
	{
		this->numFixedResource -= another.numFixedResource;
		this->numRandomResource -= another.numRandomResource;
		//this->numProductivity -= another.numProductivity;
		//this->numResearchLevel -= another.numResearchLevel;
		return (*this);
	}
	bool operator >= (const ResourcesStruct & another)
	{
		return ( (this->numFixedResource >= another.numFixedResource) && (this->numRandomResource >= another.numRandomResource)/* && (this->numProductivity >= another.numProductivity) && (this->numResearchLevel >= another.numResearchLevel)*/ );
	}
};

struct Unit
{
	UnitEnum type;
	UnitPropertyStruct property;
	UnitStateEnum state;
	cocos2d::Sprite * sprite;
};

#endif // !MYSTRUCTS_H
